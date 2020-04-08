/*******************************************************************
 *
 *    PROJECT:     Tankfüllstandsgeber EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        process.c
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      HaJ
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date
 *
 *    HISTORY:
 *
 *
 *******************************************************************/

//#define TRACE_DIG_INPUT_TEST

#include "defines.h"

#include <windows.h>
#include <stdio.h>
#include <ansi_c.h>
#include <userint.h>
#include <utility.h>
#include "toolbox.h"

#include "status.h"
#include "ibs_cfg.h"
#include "zol.h"
#include "ibs.h"
#include "UIMain.h"
#include "devicedat.h"
#include "global.h"
#include "Main.h"
#include "process.h"
#include "IODiag_Funcs.h"
#include "PnlError_Funcs.h"
#include "NetworkState_Funcs.h"
#include "SerialNumber.h"

#include "LibDutCom_Import.h"
#include "LibDutCom_Import.c"

extern int iPanelMain_m;
extern tagUIElements tagUIElements_m;

/**
 * Indexes of components from LibDutCom, according its Capabilities
 */
// analog inputs:
#define DEV_AI_ADC                             0   // 0=ADC [1]

// prod. Interlock
#define DEV_PROD_INTERLOCK_TOOL_EOL            0   // Tool 0=EOL


//Flag bNeedNetworkConnection_m wird gesetzt wenn zwingend eine Serververbindung bestehen muss.
//Es muss mindestens ein step vor dem Netzwerkzugriff gesetzt werden.
BOOL bNeedNetworkConnection_m=FALSE;

#ifdef SIMULATE_BOX
BOOL bBtnStart;   // TRUE = Taste "Start" gedrückt
BOOL bBtnQuit;    // TRUE = Taste "Quittierung" gedrückt

BOOL bBtnEnter;   // TRUE = Taste "Enter" gedrückt
BOOL bBtnNext;    // TRUE = Taste "+" gedrückt
BOOL bBtnPrev;    // TRUE = Taste "-" gedrückt
BOOL bBtnIo;      // TRUE = Taste "IO" gedrückt
BOOL bBtnNio;     // TRUE = Taste "NIO" gedrückt
#endif // !SIMULATE_BOX


#define TEXT_NOT_AUS             "(Not-Aus)"
#define TEXT_START               "(Start)"
#define TEXT_STOERUNG            "(Störung)"

#define TEXT_BM_ENTER            "[Wedelschalter]"

#define TEXT_DUT                 "Prüfling"

#define REFERENCE_LIQUID_LVL_FULL          75
#define REFERENCE_LIQUID_LVL_FULL_TOL      2
#define REFERENCE_LIQUID_LVL_FULL_MINMAX   10
#define REFERENCE_LIQUID_LVL_HALF          50
#define REFERENCE_LIQUID_LVL_HALF_TOL      2
#define REFERENCE_LIQUID_LVL_HALF_MINMAX   10
#define REFERENCE_LIQUID_LVL_EMPTY         25
#define REFERENCE_LIQUID_LVL_EMPTY_TOL     2
#define REFERENCE_LIQUID_LVL_EMPTY_MINMAX  10


//  ------------------------------------------------------------------------------------------------
//    Programmglobale Variablen
//  ------------------------------------------------------------------------------------------------


//  ------------------------------------------------------------------------------------------------
//    Modulglobale Variablen
//  ------------------------------------------------------------------------------------------------

UINT uiDevSNr_m;
BOOL bSNr_write_m;
BOOL bPrintLabel_m;

//  ------------------------------------------------------------------------------------------------
//    Funktions prototypen
//  ------------------------------------------------------------------------------------------------

void vResetTestOutputs_g(void);

BOOL bInGrundstellung_m(void);

void vWriteResultsToDisk_m(void);

void vLiquidLevelFill_m(void);
void vLiquidLevelEmpty_m(void);
void vLiquidLevelStop_m(void);

float fGetLiquidLevelDUT_m(void);


//  ------------------------------------------------------------------------------------------------
//    Hauptablauf
//  ------------------------------------------------------------------------------------------------
int iProcess_g(void)
{
  static EProcessState eState=process_init;
  static EProcessState eOldState;
  static EProcessState eErrorState=0;
  static EProcessState eReturnState;
  static EProcessState eReturnAfterNetworkErrorState;
  static double dEntryTime, dStatusTime;
  float fReferenzgeber;

  static char caPrevA2CNrUnpacked[DEVICE_A2C_NR_LEN+1];
  //char caA2CNrUnpacked[DEVICE_A2C_NR_LEN+1];
//  char caVersionSoftware[10];

  static char *pcTimeoutErrorText;
  BOOL bShortDetected=FALSE;

  // Hardware prüfen
  static int iHwTestIndex;
  int iMinValue=0;
  int iMaxValue=0;

  // analoge Inputs
  UINT uiAnalogInputValue;
  int  iSoll=0;
  char *pcUnit=NULL;

  float fAnalogInputValue;

  UCHAR ucaNmeaCode[2+1];

  if(eState!=eOldState)            // hat ein Statuswechsel stattgefunden?
  {
    dEntryTime=Timer();           // Eintrittszeit speichern
    eOldState=eState;             //
    vSetStateMachineStep_g(eState);
#if 0
  #if 0
    if((eState>=impression_test_wait_trip_pressed)
       &&(eState<99999)
      )
  #endif
    {
      printf("State=%5d\n",eState);
    }
#endif
  }
  dStatusTime=Timer()-dEntryTime;       // Schrittzeit ermitteln


  switch(eState)
  {
    case process_init:
      vUiClearErrorText_g();
#if 1
      // Konsolenfenster an rechten Bildschirmrand setzen
      SetStdioWindowPosition(59,695);
      SetStdioWindowSize(894,583);
      SetStdioWindowOptions(1000,  // maxNumLines
                            1,     // bringToFrontWhenModified
                            0);    // showLineNumbers
#endif

#ifdef USE_SLIP_PRINTER
      if(!bTM88_Init_g())
      {
        tagAppStatus_g.iErrorCode=ERR_INTERNAL_HW;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Initialisierung Drucker fehlgeschlagen! Drucker inaktiv.");
      }
  #if 0
      else
        vPrintErrorSlip("Test");
  #endif
#endif // USE_SLIP_PRINTER
      eState=warte_grundstellung;
      break;

    case warte_grundstellung:
      iUiAll_SetState_g(eIdle);

#ifndef IGNORE_FLIL
      if(!iReadDI_g(&eNotausOk))
        vUiSetUserText_g("Taste %s lösen",
                         TEXT_NOT_AUS);
      if(iReadDI_g(&eNotausOk))
#endif // !IGNORE_FLIL
        eState=warte_start;
      break;

    case warte_start:
      vUiSetUserText_g("Start-Taste drücken");
#ifdef SIMULATE_BOX
      if(bBtnStart)
      {
        bBtnStart=FALSE;
        eState=warte_pruefling;
#else // !SIMULATE_BOX
      if(!iReadDI_g(&eNotausOk))
        vUiSetUserText_g("Taste %s entriegeln und %s drücken",
                         TEXT_NOT_AUS,
                         TEXT_START);
      else if(!iReadDI_g(&eNotausOk))
        vUiSetUserText_g("Taste %s drücken",
                         TEXT_START);
      else if(iReadDI_g(&eNotausOk))
      {
#endif // !SIMULATE_BOX
        eState=warte_start_delayed;
      }
      break;


    case warte_start_delayed:
      vUiClearErrorText_g();
      if(!bIniFileDevice_ReadDataValid_g())
      {
        // noch kein Gerät ausgewählt
        eState=warte_auswahl1;
      }
      else
        eState=warte_start_delayed_dut_weg;
      break;


    case warte_start_delayed_dut_weg:
      if(bIniFileDevice_ReadDataValid_g())
        eState=warte_pruefling;
      else
        eState=warte_auswahl1;
      break;

    case warte_auswahl1:
      vUiSetUserText_g("zu fertigendes Produkt auswählen...");
      eState=warte_auswahl2;
      break;

    case warte_auswahl2:
      if(bIniFileDevice_ReadDataValid_g())
        eState=warte_pruefling;
      break;


    case warte_pruefling:
      if(!bIniFileDevice_ReadDataValid_g())
      {
        // Noch kein Gerät ausgewählt
        break;
      }

      if(bDiagPanel_AutoStartEnabled_g())
        vUiSetUserText_g("%s einlegen",TEXT_DUT);
      else
        vUiSetUserText_g("Kein Start, I/O-Diag ist geöffnet");
      vUiClearErrorText_g();
      tagAppStatus_g.iErrorCode=ERR_NO_ERROR;
      tagAppStatus_g.bPass=FALSE;
      tagAppStatus_g.bFail=FALSE;

      if(strcmp(tagCurrentDeviceTyp_g.caProdNrUnpacked,caPrevA2CNrUnpacked)!=0)
      {
        // Artikelnummer hat geändert
        strcpy(caPrevA2CNrUnpacked,tagCurrentDeviceTyp_g.caProdNrUnpacked);
      }

      eState=warte_pruefling_start;
      tagAppStatus_g.iProcessStatus=STATUS_RUN;
      memset((void*)&tagMesswerte_g,0,sizeof(TagMesswerte));
      break;


    case warte_pruefling_start:
      vUiSetUserText_g("%s einlegen und Ablauf mit Taste %s starten",
                       TEXT_DUT,
                       TEXT_BM_ENTER);
#ifdef SIMULATE_BOX
      if(bBtnEnter)
      {
        bBtnEnter=FALSE;
#else // !SIMULATE_BOX
      if(  (iReadDI_g(&eStart))
//         &&(bDiagPanel_AutoStartEnabled_g())
        )
      {
#endif // !SIMULATE_BOX
        eState=kontakt_ein;
      }

#ifdef SIMULATE_BOX
      if(bBtnQuit)
      {
        bBtnQuit=FALSE;
#else // !SIMULATE_BOX
      if(!iReadDI_g(&eNotausOk))
      {
#endif // !SIMULATE_BOX
        eState=spannen_reset;
      }
      break;


    case kontakt_ein:
      iUiAll_SetState_g(eIdle);
      vEnableCtrls_g(FALSE);

      //vUiSetUserText_g("Kontaktiere Gerät");
      //vSetDO_g(&aVentilKontakt,TRUE);
      vLibTesterGuiMgr_ProcessStart();
      eState=kontakt_ein_wait;
      break;
    case kontakt_ein_wait:
#ifndef IGNORE_FLIL_EAS
      //if(iReadDI_g(&eKontaktAS))
#endif // !IGNORE_FLIL_EAS
      {
        vSetDO_g(&aUbattDutt,TRUE);
        //vSetDO_g(&aK503Reserve,TRUE);
        //vSetDO_g(&aK506DutIllum,TRUE);
        eState=communication_init;
      }
      if(dStatusTime>2.0)
      {
        eErrorState=kontakt_aus;
        tagAppStatus_g.iErrorCode=ERR_INTERNAL_HW;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout Kontaktierung hochfahren");
        eState=kontakt_aus;
      }
      break;



    case communication_init:
      vUiSetUserText_g("Starte Kommunikation mit DUT");
      if(!vDutCom_GetInfo)
      {
        if(!bDutCom_Load(tagCurrentDeviceTyp_g.uiLibDutComNr,TRUE))
        {
          char caErrorMessage[200]={0};
          eErrorState=eState;
          vDutCom_GetImportErrorText(caErrorMessage,sizeof(caErrorMessage));
          tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
          vUiSetErrorText_g(tagAppStatus_g.iErrorCode,
                            eState,
                            "Fehler: \"%s\"",caErrorMessage);
          eState=process_end; //ignition_reset;
          break;
        }
        else
        {
          if(dStatusTime>1.0)
          {
            tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
            vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"bDutCom_Load() - Timeout");
            eErrorState=eState;
            eState=process_end; //ignition_reset;
          }
        }
      }

      //vDutComAllocMemory_m();
      vDutCom_TraceEnable(TRUE);
      vDutCom_TraceClear();
      eReturnState=req_prod_data_init;
      eState=device_start_wait;
      break;

    case test_start:
      vUiSetDeviceSerialNr_g(uiDevSNr_m);
      eState=test_init;
      break;

    case test_init:
      eState = mess_current_idle;
      break;

   /**
   * Stromaufnahme messen
   */
    case mess_current_idle:
      eState = mess_current_idle_init;
      break;
    case mess_current_idle_init:
      vSetDO_g(&aUbattDutt,TRUE);
      eState = mess_current_idle_wait;
      break;
    case mess_current_idle_wait:


      break;
    case mess_current_idle_eval:
      break;

  /**
   * Wasserstand auf 100%
   */
    case LiquidLevelToFull_start:
      iUiWaitForLiquidLevelFull_SetState_g(eRunning);
      eState = LiquidLevelToFull_fill;
      break;

    case LiquidLevelToFull_fill:
      fReferenzgeber =  fGetLiquidLevelReferenz();

      if((fReferenzgeber > (REFERENCE_LIQUID_LVL_FULL - REFERENCE_LIQUID_LVL_FULL_TOL))
        && (fReferenzgeber < (REFERENCE_LIQUID_LVL_FULL + REFERENCE_LIQUID_LVL_FULL_TOL)))
      {
        // Wasser bereits auf 100 %
        eState = LiquidLevelToFull_wait;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber < (REFERENCE_LIQUID_LVL_FULL + REFERENCE_LIQUID_LVL_FULL_TOL))
      {
        // Wasserstand zu Tief
        eState = LiquidLevelToFull_wait;
        vLiquidLevelFill_m();
        break;
      }

      break;

    case LiquidLevelToFull_wait:

      fReferenzgeber =  fGetLiquidLevelReferenz();

      if((fReferenzgeber > (REFERENCE_LIQUID_LVL_FULL - REFERENCE_LIQUID_LVL_FULL_TOL))
        && (fReferenzgeber < (REFERENCE_LIQUID_LVL_FULL + REFERENCE_LIQUID_LVL_FULL_TOL)))
      {
        // Wasser bereits auf 100 %
        eState = LiquidLevelToFull_end;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_FULL - REFERENCE_LIQUID_LVL_FULL_TOL)
      {
        // Wasserstand zu Hoch
        vLiquidLevelEmpty_m();
        break;
      }

      if(fReferenzgeber < REFERENCE_LIQUID_LVL_FULL + REFERENCE_LIQUID_LVL_FULL_TOL)
      {
        // Wasserstand zu Tief
        vLiquidLevelFill_m();
        break;
      }

      break;

    case LiquidLevelToFull_end:
      iUiWaitForLiquidLevelFull_SetState_g(ePass);
      eState = CheckLiquidLevelFull_start;
      break;


  /**
   * Prüfen des Wasserstandes an den Tankgebern
   */
    case CheckLiquidLevelFull_start:
      iUiCheckLiquidLevelFull_SetState_g(eRunning);
      vUiCheckLiquidLevelFull_SetMinMax_g(REFERENCE_LIQUID_LVL_FULL-REFERENCE_LIQUID_LVL_FULL_MINMAX,
                                           REFERENCE_LIQUID_LVL_FULL+REFERENCE_LIQUID_LVL_FULL_MINMAX);
      vUiCheckLiquidLevelFull_SetSoll_g(REFERENCE_LIQUID_LVL_FULL);
      vUiCheckLiquidLevelFull_SetTolerance_g(REFERENCE_LIQUID_LVL_FULL_TOL);
      vDutCom_AnalogInputsInit();
      break;

    case CheckLiquidLevelFull_start_wait:
      if(bDutCom_AnalogInputsInitialized())
        eState=CheckLiquidLevelFull_WaitForDut;
      if(dStatusTime>2.0)
      {
        // timeout
        tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout Init analog Inputs");
        iUiCheckLiquidLevelFull_SetState_g(eFail);
        eState=ignition_reset;
      }
      break;

    case CheckLiquidLevelFull_WaitForDut:
      if(iGetIvcc_g() > 45)
        eState = CheckLiquidLevelFull_check;
      break;

    case CheckLiquidLevelFull_check_req:
      vDutCom_AnalogInputsRequestSingle(2);
      eState = CheckLiquidLevelFull_check;
      break;

    case CheckLiquidLevelFull_check:

      if(tagCurrentDeviceTyp_g.bAnalogSignal)
      {
        // Analoger Tankfüllstandsgeber
        fAnalogInputValue = fGetLiquidLevelDUT_m()
        vUiCheckLiquidLevelFull_SetValue_g(fAnalogInputValue);
      }
      else
      {
        // Digitaler Tankfüllstandsgeber
        bDutCom_AnalogInputsReadSingle(&uiAnalogInputValue);
        vUiCheckLiquidLevelFull_SetValue_g(uiAnalogInputValue);
      }

      if(iReadDI_g(&eStart))
      {
        if(tagCurrentDeviceTyp_g.eWaterType == eWaterType_Wasted)
        {
          eState = CheckLiquidLevelFull_Check_Alarm;
        }
        else
        {
          eState = CheckLiquidLevelFull_end;
        }
      }

      break;

    case CheckLiquidLevelFull_Check_Alarm:

      if(iReadDI_g(&eAlarmDut))
      {
        eState = CheckLiquidLevelFull_end;
      }

      if(dStatusTime>2.0)
      {
        tagAppStatus_g.iErrorCode=ERR_ALARM_DUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"No Aralm on DUT at Full Liquid Level");
        iUiCheckLiquidLevelFull_SetState_g(eFail);
        eState=ignition_reset;
      }
      break;

    case CheckLiquidLevelFull_end:
      iUiCheckLiquidLevelFull_SetState_g(ePass);
      eState = LiquidLevelToHalf_start;
      break;

  /**
   * Wasserstand auf 50%
   */
    case LiquidLevelToHalf_start:
      iUiWaitForLiquidLevelHalf_SetState_g(eRunning);
      eState = LiquidLevelToHalf_fill;

      break;
    case LiquidLevelToHalf_fill:
      fReferenzgeber =  fGetLiquidLevelReferenz();

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_HALF - REFERENCE_LIQUID_LVL_HALF_TOL
         && fReferenzgeber < REFERENCE_LIQUID_LVL_HALF + REFERENCE_LIQUID_LVL_HALF_TOL)
      {
        // Wasser bereits auf 50 %
        eState = LiquidLevelToHalf_wait;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_HALF - REFERENCE_LIQUID_LVL_HALF_TOL)
      {
        // Wasserstand zu Tief
        eState = LiquidLevelToHalf_wait;
        vLiquidLevelEmpty_m();
        break;
      }
      break;

    case LiquidLevelToHalf_wait:

      fReferenzgeber =  fGetLiquidLevelReferenz();

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_HALF - REFERENCE_LIQUID_LVL_HALF_TOL
         && fReferenzgeber < REFERENCE_LIQUID_LVL_HALF + REFERENCE_LIQUID_LVL_HALF_TOL)
      {
        // Wasser auf 50 %
        eState = LiquidLevelToHalf_end;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_HALF - REFERENCE_LIQUID_LVL_HALF_TOL)
      {
        // Wasserstand zu Hoch
        vLiquidLevelEmpty_m();
        break;
      }

      if(fReferenzgeber < REFERENCE_LIQUID_LVL_HALF + REFERENCE_LIQUID_LVL_HALF_TOL)
      {
        // Wasserstand zu Tief
        vLiquidLevelFill_m();
        break;
      }
      break;

    case LiquidLevelToHalf_end:
      iUiWaitForLiquidLevelHalf_SetState_g(ePass);
      eState = CheckLiquidLevelFull_start;
      break;

  /**
  * Prüfen des Wasserstandes an den Tankgebern
   */
    case CheckLiquidLevelHalf_start:
      iUiCheckLiquidLevelHalf_SetState_g(eRunning);
      vUiCheckLiquidLevelHalf_SetMinMax_g(REFERENCE_LIQUID_LVL_HALF-REFERENCE_LIQUID_LVL_HALF_MINMAX,
                                           REFERENCE_LIQUID_LVL_HALF+REFERENCE_LIQUID_LVL_HALF_MINMAX);
      vUiCheckLiquidLevelHalf_SetSoll_g(REFERENCE_LIQUID_LVL_HALF);
      vUiCheckLiquidLevelHalf_SetTolerance_g(REFERENCE_LIQUID_LVL_HALF_TOL);
      vDutCom_AnalogInputsInit();
      eState = CheckLiquidLevelHalf_start_wait;
      break;

    case CheckLiquidLevelHalf_start_wait:
      if(bDutCom_AnalogInputsInitialized())
        eState=CheckLiquidLevelHalf_WaitForDut;
      if(dStatusTime>2.0)
      {
        // timeout
        tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout Init analog Inputs");
        iUiCheckLiquidLevelHalf_SetState_g(eFail);
        eState=ignition_reset;
      }
      break;


    case CheckLiquidLevelHalf_WaitForDut:
      if(iGetIvcc_g() > 45)
      eState = CheckLiquidLevelHalf_check_req;
      break;

    case CheckLiquidLevelHalf_check_req:
      vDutCom_AnalogInputsRequestSingle(2);
      eState = CheckLiquidLevelHalf_check;
      break;

    case CheckLiquidLevelHalf_check:
      if(tagCurrentDeviceTyp_g.bAnalogSignal)
      {
        // Analoger Tankfüllstandsgeber
        fAnalogInputValue = fGetLiquidLevelDUT_m()
        vUiCheckLiquidLevelHalf_SetValue_g(fAnalogInputValue);
      }
      else
      {
        // Digitaler Tankfüllstandsgeber
        bDutCom_AnalogInputsReadSingle(&uiAnalogInputValue);
        vUiCheckLiquidLevelHalf_SetValue_g(uiAnalogInputValue);
      }

      if(iReadDI_g(&eStart))
      {
        eState = CheckLiquidLevelHalf_end;
      }
      break;

    case CheckLiquidLevelHalf_end:
      iUiCheckLiquidLevelHalf_SetState_g(ePass);
      eState = LiquidLevelToEmpty_start;
      break;

  /**
   * Wasserstand auf 0%
   */
    case LiquidLevelToEmpty_start:
      iUiWaitForLiquidLevelEmpty_SetState_g(eRunning);
      eState = LiquidLevelToEmpty_fill;
      break;

    case LiquidLevelToEmpty_fill:
      fReferenzgeber =  fGetLiquidLevelReferenz();

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_EMPTY - REFERENCE_LIQUID_LVL_EMPTY_TOL
         && fReferenzgeber < REFERENCE_LIQUID_LVL_EMPTY + REFERENCE_LIQUID_LVL_EMPTY_TOL)
      {
        // Wasser bereits auf 50 %
        eState = LiquidLevelToEmpty_wait;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_EMPTY - REFERENCE_LIQUID_LVL_EMPTY_TOL)
      {
        // Wasserstand zu Tief
        eState = LiquidLevelToEmpty_wait;
        vLiquidLevelEmpty_m();
        break;
      }
      break;

    case LiquidLevelToEmpty_wait:
            fReferenzgeber =  fGetLiquidLevelReferenz();

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_EMPTY - REFERENCE_LIQUID_LVL_EMPTY_TOL
         && fReferenzgeber < REFERENCE_LIQUID_LVL_EMPTY + REFERENCE_LIQUID_LVL_EMPTY_TOL)
      {
        // Wasser auf 0 %
        eState = LiquidLevelToEmpty_end;
        vLiquidLevelStop_m();
        break;
      }

      if(fReferenzgeber > REFERENCE_LIQUID_LVL_EMPTY - REFERENCE_LIQUID_LVL_EMPTY_TOL)
      {
        // Wasserstand zu Hoch
        vLiquidLevelEmpty_m();
        break;
      }

      if(fReferenzgeber < REFERENCE_LIQUID_LVL_EMPTY + REFERENCE_LIQUID_LVL_EMPTY_TOL)
      {
        // Wasserstand zu Tief
        vLiquidLevelFill_m();
        break;
      }
      break;
    case LiquidLevelToEmpty_end:
      iUiWaitForLiquidLevelEmpty_SetState_g(ePass);
      eState = CheckLiquidLevelEmpty_start;
      break;

  /**
  * Prüfen des Wasserstandes an den Tankgebern
   */
    case CheckLiquidLevelEmpty_start:
      iUiCheckLiquidLevelEmpty_SetState_g(eRunning);
      vUiCheckLiquidLevelEmpty_SetMinMax_g(REFERENCE_LIQUID_LVL_EMPTY-REFERENCE_LIQUID_LVL_EMPTY_MINMAX,
                                           REFERENCE_LIQUID_LVL_EMPTY+REFERENCE_LIQUID_LVL_EMPTY_MINMAX);
      vUiCheckLiquidLevelEmpty_SetSoll_g(REFERENCE_LIQUID_LVL_EMPTY);
      vUiCheckLiquidLevelEmpty_SetTolerance_g(REFERENCE_LIQUID_LVL_EMPTY_TOL);
      vDutCom_AnalogInputsInit();
      eState = CheckLiquidLevelEmpty_start_wait;
      break;

    case CheckLiquidLevelEmpty_start_wait:
      if(bDutCom_AnalogInputsInitialized())
        eState=CheckLiquidLevelEmpty_WaitForDut;
      if(dStatusTime>2.0)
      {
        // timeout
        tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout Init analog Inputs");
        iUiCheckLiquidLevelEmpty_SetState_g(eFail);
        eState=ignition_reset;
      }
      break;

    case CheckLiquidLevelEmpty_WaitForDut:
      if(iGetIvcc_g() > 45)
        if(tagCurrentDeviceTyp_g.bAnalogSignal)
          eState = CheckLiquidLevelEmpty_check;
        else
          eState = CheckLiquidLevelEmpty_check_req
      break;

    case CheckLiquidLevelEmpty_check_req:
      vDutCom_AnalogInputsRequestSingle(2);
      eState = CheckLiquidLevelEmpty_check;
      break;

    case CheckLiquidLevelEmpty_check:
      if(tagCurrentDeviceTyp_g.bAnalogSignal)
      {
        // Analoger Tankfüllstandsgeber
        fAnalogInputValue = fGetLiquidLevelDUT_m()
        vUiCheckLiquidLevelEmpty_SetValue_g(fAnalogInputValue);
      }
      else
      {
        // Digitaler Tankfüllstandsgeber
        bDutCom_AnalogInputsReadSingle(&uiAnalogInputValue);
        vUiCheckLiquidLevelEmpty_SetValue_g(uiAnalogInputValue);
      }



      if(iReadDI_g(&eStart))
      {
        if(tagCurrentDeviceTyp_g.eWaterType == eWaterType_Fresh)
        {
          eState = CheckLiquidLevelEmpty_Check_Alarm;
        }
        else
        {
          eState = CheckLiquidLevelEmpty_end;
        }
      }
      break;

    case CheckLiquidLevelEmpty_Check_Alarm:
      if(iReadDI_g(&eAlarmDut))
      {
        eState = CheckLiquidLevelEmpty_end;
      }

      if(dStatusTime>2.0)
      {
        tagAppStatus_g.iErrorCode=ERR_ALARM_DUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"No Aralm on DUT at Empty Liquid Level");
        iUiCheckLiquidLevelEmpty_SetState_g(eFail);
        eState=ignition_reset;
      }
      break;

    case CheckLiquidLevelEmpty_end:
      iUiCheckLiquidLevelEmpty_SetState_g(ePass);
      eState = process_end;

      break;



    case process_end:
      eErrorState=process_end;
      tagAppStatus_g.bPass=(tagAppStatus_g.iErrorCode==ERR_NO_ERROR);
      tagAppStatus_g.bFail=(tagAppStatus_g.iErrorCode!=ERR_NO_ERROR);

      bNeedNetworkConnection_m=FALSE;

      if(tagAppStatus_g.iErrorCode!=ERR_NO_ERROR)
      {
        tagMesswerte_g.iErrorCode=tagAppStatus_g.iErrorCode;
        strcpy(tagMesswerte_g.caErrorText,pcGetErrorText_g());
      }
      else
        tagMesswerte_g.caErrorText[0]=0;
      vWriteResultsToDisk_m();

      if(tagAppStatus_g.iErrorCode==ERR_NO_ERROR)
      {
        // Gerät ist gut
        eState=ignition_reset;
      }
      else
        eState=fehler; //ignition_reset;
      break;


    case ignition_reset:
      if(tagAppStatus_g.iErrorCode!=ERR_NO_ERROR)
      {
        vUiSetUserText_g("Taste %s drücken",TEXT_BM_ENTER);
      }

      if(vDutCom_CommunicationStop)
      {
        vDutCom_CommunicationStop();
      }
      eState=ignition_reset2;
      break;
    case ignition_reset2:
      if(bDutCom_CommunicationStarted)
      {
        if(bDutCom_CommunicationStarted())
          break;
      }
      vDutCom_Unload();

      //vSetDO_g(&aK503Reserve,FALSE);
      //vSetDO_g(&aK506DutIllum,FALSE);
      bShortDetected=FALSE;
      eState=interface_reset;
    case interface_reset:
      if(dStatusTime>0.5)
      {
        eState=kontakt_aus;
      }
      break;

   case wait_pruefling_weg:
      vUiSetUserText_g("Teil entnehmen");
      vEnableCtrls_g(TRUE);
      tagAppStatus_g.iProcessStatus=STATUS_STOP;
      eState=warte_pruefling;
      break;


    case fehler:
      tagAppStatus_g.bFail=TRUE;
      tagAppStatus_g.bPass=FALSE;
      break;

    case device_restart:
      vUiSetUserText_g("Gerät neu starten");
      if(dStatusTime>1.0)
      {
        vDutCom_Reset();
        eState=device_start_wait;
      }
      break;

    case device_start_wait:
      if(dStatusTime>tagCurrentDeviceTyp_g.fBoottime)
        eState=login_diag_mode;
      break;


    case login_diag_mode:
      vDutCom_DiagModeStart();
      eState=login_diag_mode_wait;
      break;
    case login_diag_mode_wait:
      if(bDutCom_DiagModeStarted())
        eState=login_security;

      if(dStatusTime>3.0)
      {
        // timeout
        tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout diag mode start");
        eState=process_end; //ignition_reset;
      }
      break;

    case login_security:
      if(dStatusTime<0.5)
        break;
      vDutCom_SecurityAccess(eSecurityAccessSssEol);
      eState=login_security_wait;
      break;
    case login_security_wait:
      if(eDutCom_SecurityGetCurrentAccess()==eSecurityAccessSssEol)
        eState=eReturnState;

      if(dStatusTime>2.0)
      {
        // timeout
        tagAppStatus_g.iErrorCode=ERR_KOM_TIMEOUT;
        vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Timeout set security mode");
        eState=process_end; //ignition_reset;
      }
      break;

    case network_wait_for_reconnect:
      if(bNetState_IsAvailable_g())
      {
        vUISetNetworkState_g(TRUE,    // bAvailable
                             FALSE);  // bNeeded
        eState=network_wait_for_reconnect_wait;
        break;
      }
      break;
    //Wenn verbindung zum Server besteht kann trotzdem noch nicht zugegriffen werden (5s zusätzlich warten)
    case network_wait_for_reconnect_wait:
      if(dStatusTime>5.0)
      {
        eState=eReturnAfterNetworkErrorState;
      }
      break;

    default:
      printf("unerwarteter eState=%d\n",(int)eState);
      break;
  } // switch(eState)


  if((tagAppStatus_g.iProcessStatus!=STATUS_ERROR) && (tagAppStatus_g.iErrorCode!=ERR_NO_ERROR))
  {
    tagAppStatus_g.iProcessStatus=STATUS_ERROR;
  }

#ifndef IGNORE_FLIL_EAS
  /**
   * Not-aus abhandeln
   */
  if(!iReadDI_g(&eNotausOk))
  {
    // Not-aus-Taste wurde gedrückt
    bShortDetected=FALSE;
    vResAllOutputs_g();
    vDutCom_Unload();
    tagAppStatus_g.iErrorCode=ERR_USER_ABORT;
    vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Taste %s ist gedrückt",
                      TEXT_NOT_AUS);

    eState=warte_grundstellung;
    vEnableCtrls_g(TRUE);
  }

  /**
   * Kurzschluss abhandeln
   */
  if((iGetIvcc_g()>500)
     && (!bShortDetected)
    )
  {
    bShortDetected=TRUE;   // nur einmal in diesen IF-Fall kommen
    eErrorState=eState;
    tagAppStatus_g.iErrorCode=ERR_I_MAX;
    vUiSetErrorText_g(tagAppStatus_g.iErrorCode,eState,"Fehler: Strom zu gross! Kurzschluss? (I=%dmA)",
                    iGetIvcc_g());
    bNeedNetworkConnection_m=TRUE;
    eState=process_end;
  }
#endif // !IGNORE_FLIL_EAS

  if(!bNetState_IsAvailable_g())
  {
    //Netzwerkverbindung über LED anzeigen
    if(eState!=network_wait_for_reconnect)
    {
      vUISetNetworkState_g(FALSE,   // bAvailable
                           FALSE);  // bNeeded
    }

    //Netzwerkverbindung zwingend notwendig
    if(bNeedNetworkConnection_m)
    {
      bNeedNetworkConnection_m=FALSE;
      vUISetNetworkState_g(FALSE,   // bAvailable
                           TRUE);   // bNeeded
      eReturnAfterNetworkErrorState=eState;
      eState=network_wait_for_reconnect;
    }
  }
  else
  {
    vUISetNetworkState_g(TRUE,    // bAvailable
                         FALSE);  // bNeeded
  }

  return(0);
} // iProcess_g()


void vLiquidLevelFill_m()
{
  vSetDO_g(&aVentilTankDruck, TRUE);
  vSetDO_g(&aVentilWasserAblassen, FALSE);
  vSetDO_g(&aVentilWasserhahn, TRUE);
}
void vLiquidLevelEmpty_m()
{
  vSetDO_g(&aVentilTankDruck, FALSE);
  vSetDO_g(&aVentilWasserAblassen, TRUE);
  vSetDO_g(&aVentilWasserhahn, TRUE);
}
void vLiquidLevelStop_m()
{
  vSetDO_g(&aVentilTankDruck, FALSE);
  vSetDO_g(&aVentilWasserAblassen, FALSE);
  vSetDO_g(&aVentilWasserhahn, FALSE);
}

float fGetLiquidLevelDUT_m()
{
  float fDUTLevel;

  switch(tagCurrentDeviceTyp_g.iStecker)
  {
    case 2:
      fDUTLevel = (fGetCH3 - fGetCH2)*10.0; // 100 Ohm / in mA
      break;

    case 4:
      if(tagCurrentDeviceTyp_g.bSourceMode)
      {
        fDUTLevel = (fGetCH4 - fGetCH6)*10.0; // 100 Ohm / in mA
      }
      if(tagCurrentDeviceTyp_g.bSinkMode)
      {
        fDUTLevel = (fGetCH2 - fGetCH4)*10.0; // 100 Ohm / in mA
      }
      break;
  }
  return fDUTLevel;
}

#if 0
/**
 * Alle Speicher fuer Kommunikation mit LibDutCom allozieren
 *
 * @author uidc9013 (20.04.2012)
 */
void vDutComAllocMemory_m(void)
{
  if(pucTellTaleState_m!=NULL)
  {
    vDutComFreeMemory_m();
  }
  // jeweils 2 bytes mehr allozieren fall ...GetCount() 0 zurück gäbe
  pucTellTaleState_m=(UCHAR *)malloc(uiDutCom_TellTaleCtrlGetCount()+2);
  pucDigOutputState_m=(UCHAR *)malloc(uiDutCom_DigOutputsGetCount()+2);
} // vDutComAllocMemory_m()

void vDutComFreeMemory_m(void)
{
  if(pucTellTaleState_m!=NULL)
  {
    free(pucTellTaleState_m);
    pucTellTaleState_m=NULL;

    free(pucDigOutputState_m);
    pucDigOutputState_m=NULL;
  }
} // vDutComFreeMemory_m()
#endif


BOOL bInGrundstellung_m(void)
{
  BOOL bGrundstellung;

#ifdef IGNORE_FLIL_EAS
  bGrundstellung=TRUE;
#else // !IGNORE_FLIL_EAS
  //bGrundstellung=iReadDI_g(&eSchlittenEinlPos);
  bGrundstellung=TRUE;
#endif // !IGNORE_FLIL_EAS

  return(bGrundstellung);
}



void vWriteResultsToDisk_m(void)
{
  BOOL bWriteHeader=FALSE;
  char *pcFileName;
  char caFilePath[MAX_PATHNAME_LEN];
  FILE *fp;

  if(tagMesswerte_g.iErrorCode==ERR_NO_ERROR)
    pcFileName="Results_pass.csv";
  else
    pcFileName="Results_fail.csv";
  sprintf(caFilePath,"%s\\%s",tagAppConfig_g.caPathResults,pcFileName);

  if(!FileExists(caFilePath,0))
    bWriteHeader=TRUE;

  DisableBreakOnLibraryErrors();
  if((fp=fopen(caFilePath,"a"))==NULL)
  {
    printf("unable to open \"%s\"\n",caFilePath);
    return;
  }
  if(bWriteHeader)
  {
    fprintf(fp,"Resultate für %s %s;\n",
            tagAppConfig_g.pcProjectName,
            tagAppConfig_g.pcApplicationName);
    fprintf(fp,
            "Gerät;"              // Gerät
            "Serial;"             // Serialnumber
            "Datum;"              // Datum
            "Zeit;"               // Zeit
            "U-Batt [mV];"        // U-Batt
            "Iidle [mA];"         // Leerlaufstrom
            "NFC UID;"            // NFC UID
            "Error;"              // ErrorCode
            "Error-Text;"         // ErrorText
            "\n");
  }

  if(tagMesswerte_g.iErrorCode!=ERR_NO_ERROR)
  {
    // Aus dem Fehlertext alle nicht-druckbaren Zeichen durch Leerschläge ersetzen
    int iIndex;
    for(iIndex=0;
       iIndex<strlen(tagMesswerte_g.caErrorText);
       iIndex++)
    {
      if(!isprint(tagMesswerte_g.caErrorText[iIndex]))
        tagMesswerte_g.caErrorText[iIndex]=' ';
    }
  }

  fprintf(fp,
          "%s;"              // Gerät
          "%08d;"            // Serialnumber
          "%s;"              // Datum
          "%s;"              // Zeit
          "%d;"              // U-Batt
          "%d;"              // Leerlaufstrom
          "%s;"              // NFC UID
          "%d;"              // ErrorCode
          "%s\n",            // ErrorText

          tagCurrentDeviceTyp_g.caProdNrUnpacked,    // Gerät
          uiDevSNr_m,                                // Serialnumber
          cDateStr_g(),                              // Datum
          TimeStr(),                                 // Zeit
          tagMesswerte_g.iUbatt,                     // U-Batt
          tagMesswerte_g.iCurrentIdleMa,             // Leerlaufstrom
          tagMesswerte_g.caNfcUid,                   // NFC UID
          tagMesswerte_g.iErrorCode,                 // ErrorCode
          tagMesswerte_g.caErrorText                 // ErrorText
         );
  fclose(fp);
} // vWriteResultsToDisk_m()


