/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        UIMain.c
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date
 *
 *    HISTORY:
 *
 *
 *******************************************************************/

#include "defines.h"

#include <windows.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>

#include "Main.h"
#include "UIMain.h"
#include "global.h"
#include "process.h"
#include "IODiag.h"
#include "IODiag_Funcs.h"
#include "PnlError_Funcs.h"
#include "zol.h"
#include "devicedat.h"
#include "toolbox.h"
#include "KWP2000Diag_Funcs.h"
#include "NetworkState_Funcs.h"
#include "SelectorPanel_Funcs.h"
#ifdef USE_LABEL_PRINTER
  #include "LabelPrt09_Funcs.h"
#endif // USE_LABEL_PRINTER

#include "SvnIds.h"

int iPanel_m;

tagUIElements tagUIElements_m;

#define IVALID_PANEL_HANDLE -1
extern int iPanelMain_m;
extern int iPanelMenuMain_m;
extern int iPanelTests_m;


//@TESTER_GUI_GENERATED_GUI_VARS_START@
//Generated: 14:02 @ 20.03.2020
//Gui Variables
int iGui0007_ProdDataRead_m;
int iGui0003_CurrentIdle_m;
int iGui0001_CheckLed_m;
int iGui0007_CheckAdc_m;
int iGui0007_CheckNfc_m;
int iGui0007_ProdDataWrite_m;
BOOL bTesterGuiInitialized_m=FALSE;
//@TESTER_GUI_GENERATED_GUI_VARS_END@


#ifdef SIMULATE_BOX
extern BOOL bBtnStart;   // TRUE = Taste "Start" gedrückt
extern BOOL bBtnQuit;    // TRUE = Taste "Quittierung" gedrückt

extern BOOL bBtnEnter;   // TRUE = Taste "Enter" gedrückt
extern BOOL bBtnIo;    // TRUE = Taste "i.O." gedrückt
extern BOOL bBtnNio;    // TRUE = Taste "n.i.O" gedrückt
extern BOOL bBtnNext;    // TRUE = Taste ">>" gedrückt
extern BOOL bBtnPrev;    // TRUE = Taste "<<" gedrückt
#endif // SIMULATE_BOX


void vQuitProgram_g(void);


//  ------------------------------------------------------------------------------------------------
//    Funktion iInitUserInterface_g()
//  ------------------------------------------------------------------------------------------------
void vInitUserInterface_g(int iPanel)
{
  char caPanelTitel[300];

  iPanel_m=iPanel;

  bIniFileDevice_FillSelectorCtrl_g(iPanelMain_m,PNL_MAIN_RING_DEVICE,TRUE);

  if((iPanelMenuMain_m=LoadPanel(iPanelMain_m,"Main.uir",PNL_MENU))<0)
  {
    printf("LoadPanel() failed\n");
    return;
  }

  sprintf(caPanelTitel,"%s: %s",tagAppConfig_g.pcProjectName,tagAppConfig_g.pcApplicationName);
  SetPanelAttribute(iPanel_m,ATTR_TITLE,caPanelTitel);

  SetPanelAttribute(iPanelMenuMain_m,ATTR_TITLEBAR_VISIBLE,0);

#ifndef SIMULATE_BOX
  // Simulierte Bedienbox verstecken
  SetCtrlAttribute(iPanel_m,PNL_MAIN_SIM_BOX,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_ENTER,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_NEXT,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_PREV,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_IO,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_NIO,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_RES1,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_RES2,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_LED_FAIL,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_LED_PASS,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_SIM_BOX2,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_QUIT,ATTR_VISIBLE,0);
  SetCtrlAttribute(iPanel_m,PNL_MAIN_BTN_START,ATTR_VISIBLE,0);
#endif // !SIMULATE_BOX

  bLibTesterGuiMgr_Configure(iPanel_m,PNL_MAIN_GUI,TESTER_GUI_DO_ANIMATION,TRUE,TRUE);
  vLibTesterGuiMgr_SetAnimationTime(TESTER_GUI_ANIMATION_TIME);

} // vInitUserInterface_g()



void vUIDiscard_g(void)
{
  if(bTesterGuiInitialized_m)
  {
    vLibTesterGui_Discard_g();
    bTesterGuiInitialized_m=FALSE;
  }
}

//@TESTER_GUI_GENERATED_GENERAL_FUNCS_START@
//Generated: 14:02 @ 20.03.2020
/**
 * Init Function for LibTesterGuis
 */
void vLibTesterGui_Init_g(void)
{
  if(bTesterGuiInitialized_m)
  {
    vLibTesterGui_Discard_g();
  }

  iGui0007_ProdDataRead_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0007);
  tagUIElements_m.ptagGuiProdDataRead = ptagLibTesterGui_0007_Init(iPanelMain_m,"Produktionsdaten lesen");

  iGui0003_CurrentIdle_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0003);
  tagUIElements_m.ptagGuiCurrentIdle = ptagLibTesterGui_0003_Init(iPanelMain_m,"Stromaufnahme messen");

  if(tagAppStatus_g.tagTest.bLed)
  {
    iGui0001_CheckLed_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0001);
    tagUIElements_m.ptagGuiCheckLed = ptagLibTesterGui_0001_Init(iPanelMain_m,"LED prüfen");
  }
  else
  {
    tagUIElements_m.ptagGuiCheckLed = NULL;
  }

  if(tagAppStatus_g.tagTest.bAnInputs)
  {
    iGui0007_CheckAdc_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0007);
    tagUIElements_m.ptagGuiCheckAdc = ptagLibTesterGui_0007_Init(iPanelMain_m,"ADC-Wert prüfen");
  }
  else
  {
    tagUIElements_m.ptagGuiCheckAdc = NULL;
  }

  if(tagAppStatus_g.tagTest.bNfc)
  {
    iGui0007_CheckNfc_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0007);
    tagUIElements_m.ptagGuiCheckNfc = ptagLibTesterGui_0007_Init(iPanelMain_m,"NFC prüfen");
  }
  else
  {
    tagUIElements_m.ptagGuiCheckNfc = NULL;
  }

  iGui0007_ProdDataWrite_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0007);
  tagUIElements_m.ptagGuiProdDataWrite = ptagLibTesterGui_0007_Init(iPanelMain_m,"Produktionsdaten schreiben");

  bTesterGuiInitialized_m=TRUE;
  bLibTesterGuiMgr_DisplayAllGuis(TRUE);
} // vLibTesterGui_Init_g()

/**
 * Discard Function for LibTesterGuis
 */
void vLibTesterGui_Discard_g(void)
{
  vLibTesterGui_0007_Discard(tagUIElements_m.ptagGuiProdDataRead,FALSE);
  vLibTesterGui_0003_Discard(tagUIElements_m.ptagGuiCurrentIdle,FALSE);
  if(tagUIElements_m.ptagGuiCheckLed)
  {
    vLibTesterGui_0001_Discard(tagUIElements_m.ptagGuiCheckLed,FALSE);
  }
  if(tagUIElements_m.ptagGuiCheckAdc)
  {
    vLibTesterGui_0007_Discard(tagUIElements_m.ptagGuiCheckAdc,FALSE);
  }
  if(tagUIElements_m.ptagGuiCheckNfc)
  {
    vLibTesterGui_0007_Discard(tagUIElements_m.ptagGuiCheckNfc,FALSE);
  }
  vLibTesterGui_0007_Discard(tagUIElements_m.ptagGuiProdDataWrite,FALSE);
  bLibTesterGuiMgr_DeleteAllGuis();
  bTesterGuiInitialized_m=FALSE;
} // vLibTesterGui_Discard_g()

/**
 * Set all Panel State Function for LibTesterGuis
 */
int iUiAll_SetState_g(EPanelStatus eState)
{
  if(!bTesterGuiInitialized_m)
    return(0);
  return(iLibTesterGuiMgr_SetPanelStatus(eAll,eAll,eState,TRUE));
} // iUiAll_SetState_g()

/**
 * Set state of Panel iCurrentState to State eState
 *
 * @param iCurrentState Value returned by functions iUi____SetState_g.
 * @param eState State to which set the GUI to.
 *
 * @return int
 */
int iUiGuiX_SetState_g(int iCurrentState,EPanelStatus eState)
{
  if(!bTesterGuiInitialized_m)
    return(0);
  return(iLibTesterGuiMgr_SetPanelStatus((iCurrentState>>16),iCurrentState&0xff,eState,FALSE));
} // iUiGuiX_SetState_g()
//@TESTER_GUI_GENERATED_GENERAL_FUNCS_END@

/**
 * Visual Update Function for LibTesterGuis
 */
void vLibTesterGui_VisualUpdate_g(void)
{
//@TESTER_GUI_GENERATED_VISUAL_UPDATE_START@
//Generated: 14:02 @ 20.03.2020
  if(bTesterGuiInitialized_m)
  {
    vLibTesterGui_0007_VisualUpdate(iGui0007_ProdDataRead_m);
    vLibTesterGui_0003_VisualUpdate(iGui0003_CurrentIdle_m);
    if(tagUIElements_m.ptagGuiCheckLed)
    {
      vLibTesterGui_0001_VisualUpdate(iGui0001_CheckLed_m);
    }
    if(tagUIElements_m.ptagGuiCheckAdc)
    {
      vLibTesterGui_0007_VisualUpdate(iGui0007_CheckAdc_m);
    }
    if(tagUIElements_m.ptagGuiCheckNfc)
    {
      vLibTesterGui_0007_VisualUpdate(iGui0007_CheckNfc_m);
    }
    vLibTesterGui_0007_VisualUpdate(iGui0007_ProdDataWrite_m);
  }
//@TESTER_GUI_GENERATED_VISUAL_UPDATE_END@
} // vLibTesterGui_Discard_g()



void vUISetNetworkState_g(BOOL bAvailable, BOOL bNetworkNeeded)
{
  tagUIElements_m.tagNetworkError.bHasError=(!bAvailable);
  tagUIElements_m.tagNetworkError.bNetworkNeeded=bNetworkNeeded;
  tagUIElements_m.tagNetworkError.bUpdated=TRUE;
}


void vUpdateUserInterface_g(BOOL bForceRefresh)
{
  vLibTesterGui_VisualUpdate_g();

  if(tagUIElements_m.tagProdData.bUpdated)
  {
    SetCtrlVal(iPanel_m,PNL_MAIN_NUM_SERIAL,tagUIElements_m.tagProdData.uiSerialNr);
    tagUIElements_m.tagProdData.bUpdated=FALSE;
    SetCtrlVal(iPanel_m,PNL_MAIN_TXT_DESCR,tagUIElements_m.tagProdData.caDescription);
  }

  if(tagUIElements_m.tagProgError.bUpdated)
  {
    if(tagUIElements_m.tagProgError.bHasError)
    {
      vErrorPanel_SetErrorMessage_g(tagUIElements_m.tagProgError.caErrorText);
      vErrorPanel_SetErrorCode_g(tagUIElements_m.tagProgError.iErrorCode);
      vErrorPanel_SetErrorStep_g(tagUIElements_m.tagProgError.iStep);
#ifdef USE_SLIP_PRINTER
      vErrorPanel_EnableErrorSlip_g(TRUE);
#else // !USE_SLIP_PRINTER
      vErrorPanel_EnableErrorSlip_g(FALSE);
#endif // !USE_SLIP_PRINTER
      vErrorPanel_Show_g();
    }
    else
      vErrorPanel_Hide_g();
    tagUIElements_m.tagProgError.bUpdated=FALSE;
  }

  if(tagUIElements_m.tagNetworkError.bUpdated)
  {
    if(tagUIElements_m.tagNetworkError.bHasError)
      SetCtrlVal(iPanel_m,PNL_MAIN_LED_NET_STATE,FALSE);
    else
      SetCtrlVal(iPanel_m,PNL_MAIN_LED_NET_STATE,TRUE);

    if(tagUIElements_m.tagNetworkError.bHasError  && tagUIElements_m.tagNetworkError.bNetworkNeeded)
      vNetState_Show_g();
    else
      vNetState_Hide_g();
    tagUIElements_m.tagNetworkError.bUpdated=FALSE;
  }

  if(tagUIElements_m.tagProgStatus.bUpdated)
  {
    ResetTextBox(iPanel_m,PNL_MAIN_TXT_USER,tagUIElements_m.tagProgStatus.caUserText);
    SetCtrlVal(iPanel_m,PNL_MAIN_NUM_STEP,tagUIElements_m.tagProgStatus.iStateMachineStep);
    SetCtrlVal(iPanel_m,PNL_MAIN_LED_RUN,(tagAppStatus_g.iProcessStatus==STATUS_RUN));
    SetCtrlVal(iPanel_m,PNL_MAIN_LED_STOP,(tagAppStatus_g.iProcessStatus==STATUS_STOP));
    SetCtrlVal(iPanel_m,PNL_MAIN_LED_STOERUNG,(tagAppStatus_g.iProcessStatus==STATUS_ERROR));
    tagUIElements_m.tagProgStatus.bUpdated=FALSE;
  }

  DisableBreakOnLibraryErrors();

  vUpdateDiagnosePanel_g();
} // vUpdateUserInterface_g()


BOOL bUiSetDeviceSelection_g(char *pcA2cNr)
{
  char caDescription[200];

  if(bIniFileDevice_ReadData_g(pcA2cNr))
  {
    // Daten fehlerfrei geladen
    vUiSetDeviceDescription_g(tagCurrentDeviceTyp_g.caDescription);
    SetCtrlAttribute(iPanelMain_m,PNL_MAIN_TXT_DESCR,ATTR_TEXT_COLOR,VAL_BLACK);
    //vIniFileDevices_Dump_g(&tagCurrentDeviceTyp_g);
    return(TRUE);
  }

  sprintf(caDescription,"Fehler in Datei %s",tagAppConfig_g.caDutDatIniFileName);
  SetCtrlVal(iPanelMain_m,PNL_MAIN_TXT_DESCR,caDescription);
  SetCtrlAttribute(iPanelMain_m,PNL_MAIN_TXT_DESCR,ATTR_TEXT_COLOR,VAL_RED);
  vLibTesterGui_Discard_g();

  return(FALSE);
} // bUiSetDeviceSelection_g()


int CVICALLBACK SelectDeviceCB(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
//#define USE_SELECTOR_PANEL
  int iItemIndex;
  char caDeviceNr[DEVICE_A2C_NR_LEN+1];
  char caDescription[200];
#ifdef USE_SELECTOR_PANEL
  int iPosLeft;
  int iPosTop;
#endif // USE_SELECTOR_PANEL

  if(event!=EVENT_COMMIT)
    return(0);

#ifdef USE_SELECTOR_PANEL
  GetCtrlAttribute(panel,control,ATTR_LEFT,&iPosLeft);
  GetCtrlAttribute(panel,control,ATTR_TOP,&iPosTop);

  if(eSelectorPanel_Open_g(panel,PNL_MAIN_RING_DEVICE,TRUE,-1,-1)!=eEnter)
    return(1);
#endif // USE_SELECTOR_PANEL

  GetCtrlIndex(panel,control,&iItemIndex);
  GetLabelFromIndex(panel,control,iItemIndex,(char *)&caDeviceNr);

  if(strcmp(caDeviceNr,DEVICE_DAT_SELECT_PROMPT)==0)
  {
    // noch kein Gerät ausgewählt
    return(0);
  }

  if(bIniFileDevice_ReadData_g(caDeviceNr))
  {
    // Daten fehlerfrei geladen
    vUiSetDeviceDescription_g(tagCurrentDeviceTyp_g.caDescription);
    SetCtrlAttribute(panel,PNL_MAIN_TXT_DESCR,ATTR_TEXT_COLOR,VAL_BLACK);
    vIniFileDevices_Dump_g(&tagCurrentDeviceTyp_g);
  }
  else
  {
    sprintf(caDescription,"Fehler in Datei %s",tagAppConfig_g.caDutDatIniFileName);
    vUiSetDeviceDescription_g(caDescription);
    SetCtrlAttribute(panel,PNL_MAIN_TXT_DESCR,ATTR_TEXT_COLOR,VAL_RED);
    vLibTesterGui_Discard_g();
    return(0);
  }

  // Liste in Ring-Control neu aufbauen
  bIniFileDevice_FillSelectorCtrl_g(panel,PNL_MAIN_RING_DEVICE,FALSE);
  vLibTesterGui_Init_g();
  return(0);
}


int CVICALLBACK BoxCB(int panel, int control, int event,
                      void *callbackData, int eventData1, int eventData2)
{
#ifdef SIMULATE_BOX
  if(event==EVENT_COMMIT)
  {
    switch(control)
    {
      case PNL_MAIN_BTN_QUIT:  bBtnQuit =TRUE; break;
      case PNL_MAIN_BTN_START: bBtnStart=TRUE; break;
      case PNL_MAIN_BTN_ENTER: bBtnEnter=TRUE; break;
      case PNL_MAIN_BTN_NEXT:  bBtnNext =TRUE; break;
      case PNL_MAIN_BTN_PREV:  bBtnPrev =TRUE; break;
      case PNL_MAIN_BTN_IO:    bBtnIo   =TRUE; break;
      case PNL_MAIN_BTN_NIO:   bBtnNio  =TRUE; break;
    }
  }
#endif // SIMULATE_BOX

  return(0);
} // BoxCB()


int CVICALLBACK SwitchCB(int panel,int control,int event,
                         void *callbackData,int eventData1,int eventData2)
{
  switch(event)
  {
    case EVENT_COMMIT:
      switch(control)
      {
        case PNL_TESTS_SW_TEST_SW_VERSION:
          GetCtrlVal(panel,control,(int*)&tagAppStatus_g.tagTest.bSwVersion);
          break;
        case PNL_TESTS_SW_TEST_AI:
          GetCtrlVal(panel,control,(int*)&tagAppStatus_g.tagTest.bAnInputs);
          break;
        case PNL_TESTS_SW_TEST_NFC:
          GetCtrlVal(panel,control,(int*)&tagAppStatus_g.tagTest.bNfc);
          break;
        case PNL_TESTS_SW_TEST_LED:
          GetCtrlVal(panel,control,(int*)&tagAppStatus_g.tagTest.bLed);
          break;
        case PNL_TESTS_BTN_OK:
          vLibTesterGui_Init_g();
          DiscardPanel(panel);
          iPanelTests_m=-1;
          break;
      }
      break;
  }
  return(0);
} // SwitchCB()


int CVICALLBACK TouchMainMenuCB(int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2)
{
  if(event!=EVENT_LEFT_CLICK)
    return(0);

  switch(control)
  {
    case PNL_MAIN_BTN_MENU:
      {
        int iTop;
        int iHeight;
        int iLeft;
        int iFrameThickness;

        GetCtrlAttribute(panel,control,ATTR_TOP,&iTop);
        GetCtrlAttribute(panel,control,ATTR_HEIGHT,&iHeight);
        GetCtrlAttribute(panel,control,ATTR_LEFT,&iLeft);
        GetPanelAttribute(iPanelMenuMain_m,ATTR_FRAME_THICKNESS,&iFrameThickness);
        SetPanelPos(iPanelMenuMain_m,
                    iTop+iHeight+iFrameThickness,
                    iLeft+iFrameThickness);

#ifdef IGNORE_FLIL
        SetCtrlAttribute(iPanelMenuMain_m,PNL_MENU_BTN_DIAG_IO,ATTR_DIMMED,1);
#endif // IGNORE_FLIL
#ifndef USE_LABEL_PRINTER
        SetCtrlAttribute(iPanelMenuMain_m,PNL_MENU_BTN_DIAG_PRT9,ATTR_DIMMED,1);
#endif // !USE_LABEL_PRINTER
        DisplayPanel(iPanelMenuMain_m);
        return(1);   // event "schlucken"
      }
    default:
      printf("unexpected control %d\n",control);
  }

  return(0);
} // TouchMainMenuCB()


void vInitPanelTests_m(void)
{
  if(iPanelTests_m<1)
    return;

  SetPanelAttribute(iPanelTests_m,ATTR_TITLEBAR_VISIBLE,0);
  SetPanelPos(iPanelTests_m,30,40);

  SetCtrlVal(iPanelTests_m,PNL_TESTS_SW_TEST_SW_VERSION,(int)tagAppStatus_g.tagTest.bSwVersion);
  SetCtrlVal(iPanelTests_m,PNL_TESTS_SW_TEST_AI,(int)tagAppStatus_g.tagTest.bAnInputs);
  SetCtrlVal(iPanelTests_m,PNL_TESTS_SW_TEST_NFC,(int)tagAppStatus_g.tagTest.bNfc);
  SetCtrlVal(iPanelTests_m,PNL_TESTS_SW_TEST_LED,(int)tagAppStatus_g.tagTest.bLed);
} // vInitPanelTests_m()



int CVICALLBACK TouchMenuCB(int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
  if(event!=EVENT_LEFT_CLICK)
    return(0);

  HidePanel(iPanelMenuMain_m);
  switch(control)
  {
    case PNL_MENU_BTN_TEST_FKT:
      if(iPanelTests_m>IVALID_PANEL_HANDLE)
        return(0);   // Panel schon geladen
      // TODO: Passwort-Abfrage?
      if((iPanelTests_m=LoadPanel(iPanelMain_m,"Main.uir",PNL_TESTS))<0)
        return(0);
      vInitPanelTests_m();
      DisplayPanel(iPanelTests_m);
      break;

    case PNL_MENU_BTN_DIAG_CAN:
      vInitKWP2000Panel_g(panel);
      break;
    case PNL_MENU_BTN_DIAG_IO:
      vInitDiagnosePanel_g(TRUE);  // TRUE => für Touch-Display
      break;

    case PNL_MENU_BTN_DIAG_PRT9:
#ifdef USE_LABEL_PRINTER
      vLabelPrintPanel09_Open_g(0);
#endif // USE_LABEL_PRINTER
      break;

    case PNL_MENU_BTN_ABOUT:
      vAbout_m();
      break;
    case PNL_MENU_BTN_EXIT:
      vQuitProgram_g();
      break;
  }
  //iPanelMenuMain_m=IVALID_PANEL_HANDLE;
  return(1);
} // TouchMenuCB()


void vEnableCtrls_g(BOOL bEnable)
{
  SetCtrlAttribute(iPanel_m,PNL_MAIN_RING_DEVICE,ATTR_DIMMED,(!bEnable));

  // Menüpunkt "Tests" enablen/disablen
  if(iPanelMenuMain_m>0)
    SetCtrlAttribute(iPanelMenuMain_m,PNL_MENU_BTN_TEST_FKT,ATTR_DIMMED,(!bEnable));

  if(iPanelTests_m>0)
  {
    //Menü Tests ist offen --> schliessen
    DiscardPanel(iPanelTests_m);
    iPanelTests_m=-1;
  }

} // vEnableCtrls_g()



/*=========================================

    XX    XX
     X     X                        X
     X     X                        X
    X X    XXXXX   XXXXX  XX  XX   XXXX
    X X    X    X X     X  X   X    X
   X   X   X    X X     X  X   X    X
   XXXXX   X    X X     X  X   X    X
   X   X   X    X X     X  X  XX    X  X
  XXX XXX XXXXXX   XXXXX    XX XX    XX

=========================================*/


void vAbout_m(void)
{
  char caPopup_Message[1000];
  char caComponentsIgnored[100];

  char caSvnRepoName[200];
  char caSvnRevision[50];
  char caSvnAuthor[50];

  strcpy(caSvnRepoName,pcSvnGetRepoName());
  strcpy(caSvnRevision,pcSvnGetRevision());
  strcpy(caSvnAuthor,  pcSvnGetAuthor());

  /**
   * folgende Optionen sind Applikations-spezifisch
   */
#ifdef IGNORE_IVCC_ERROR
  #define ANY_IGNORE
#endif
#ifdef IGNORE_FLIL
  #define ANY_IGNORE
#endif
#ifdef SIMULATE_BOX
  #define ANY_IGNORE
#endif
#ifndef USE_SLIP_PRINTER
  #define ANY_IGNORE
#endif

  caComponentsIgnored[0]=0;
#ifdef ANY_IGNORE
  sprintf(caComponentsIgnored,"Abweichung: %s%s%s%s\n",
  #ifdef IGNORE_IVCC_ERROR
          "Strom, "
  #else
          " "
  #endif
          ,
  #ifdef IGNORE_FLIL
          "E/A, "
  #else
          " "
  #endif
          ,
  #ifdef SIMULATE_BOX
          "Bedienbox, "
  #else
          " "
  #endif
          ,
  #ifndef USE_SLIP_PRINTER
          "Fehler-Drucker, "
  #else
          " "
  #endif
         );
#endif

  sprintf(caPopup_Message,
          "Projekt: %s"
          "\nStation: %s"
          "\n"
          "\nVersion %d.%03d %s"
          "\n"
          "\nBuild date: "
#if _CVI_<800
#include "Build_Date.h"
#else
          __DATE__
#endif
          "\n"
          "\nRepository: %s"
          "\n%s"
          "\n%s"
          "\n"
          "\nCVI Version: %3.2f"
          "\n"
          "\n%s",

          tagAppConfig_g.pcProjectName,
          tagAppConfig_g.pcApplicationName,

          PROGRAM_VERSION_MAJOR,
          PROGRAM_VERSION_MINOR,
#ifdef _CVI_DEBUG_
          "(Debug)",
#else // !_CVI_DEBUG_
          " ",
#endif // !_CVI_DEBUG_
          caSvnRepoName,
          caSvnRevision,
          caSvnAuthor,
          _CVI_/100.0,
          caComponentsIgnored);

  MessagePopup("About",caPopup_Message);
} // vAbout_m()


void vSetStateMachineStep_g(int iStep)
{
  tagUIElements_m.tagProgStatus.bUpdated=TRUE;
  tagUIElements_m.tagProgStatus.iStateMachineStep=iStep;
} // vSetStateMachineStep_g()

void vUiSetDeviceDescription_g(char *pcDescription)
{
  tagUIElements_m.tagProdData.bUpdated=TRUE;
  strcpy(tagUIElements_m.tagProdData.caDescription,pcDescription);
} // vUiSetDeviceDescription_g()

char *pcGetFilePathFromWorkOrder(EWorkOrder eWorkOrder,
                                 BOOL bShowDefaultIfMissing)
{
  char caImage[30];
  char *pcImage;
  static char caImagePath[300];

  tagUIElements_m.tagWorkOrder.iWorkOrder=eWorkOrder;
  switch(tagUIElements_m.tagWorkOrder.iWorkOrder)
  {
    case eWoLcd01:
    case eWoLcd02:
    case eWoLcd03:
      pcImage=WORK_ORDER_LCD;
      sprintf(caImage,pcImage,tagUIElements_m.tagWorkOrder.iWorkOrder);
      break;
    case eWoPtr01:   // Zeiger auf 0-Marke
    case eWoPtr02:
    case eWoPtr03:
    case eWoPtr04:
      pcImage=WORK_ORDER_PTR;
      sprintf(caImage,pcImage,tagUIElements_m.tagWorkOrder.iWorkOrder);
      break;
    case eWoEL01:
    case eWoEL02:
      pcImage=WORK_ORDER_EL;
      sprintf(caImage,pcImage,tagUIElements_m.tagWorkOrder.iWorkOrder);
      break;

    case eWoTt01:
    case eWoTt02:
    case eWoTt03:
    case eWoTt04:
      pcImage=WORK_ORDER_TT;
      sprintf(caImage,pcImage,tagUIElements_m.tagWorkOrder.iWorkOrder);
      break;

    case eWoEasyLink:
      pcImage=WORK_ORDER_EASYLINK;
      strcpy(caImage,pcImage);
      break;

    case eWoFinal:
      pcImage=WORK_ORDER_FINAL;
      strcpy(caImage,pcImage);
      break;

    default:
      printf("Unbekannte WorkOrder\n");
      // nichts zu speichern
      return "";
  }

  // Gerätespezifische Work-Order anzeigen
  sprintf(caImagePath,"%s\\%s\\%s",
          tagAppConfig_g.caPathWorkOrders,
          tagCurrentDeviceTyp_g.caProdNrUnpacked,
          caImage);
  //printf("1. work order = \"%s\"\n",caImagePath);
  if(bShowDefaultIfMissing)
  {
    if(!FileExists(caImagePath,NULL))
    {
      // nicht gefunden --> generelle Work-Order anzeigen
      sprintf(caImagePath,"%s\\%s",
              tagAppConfig_g.caPathWorkOrders,
              caImage);
      //printf("2. work order = \"%s\"\n",caImagePath);
      if(!FileExists(caImagePath,NULL))
      {
        printf("Workorder nicht gefunden / Default Image <%s> nicht gefunden!\n",caImagePath);
        sprintf(caImagePath,"%s\\%s",
                tagAppConfig_g.caPathWorkOrders,
                WORK_ORDER_NONE);
        //printf("3. work order = \"%s\"\n",caImagePath);
        return(caImagePath);
      }
    }
  }
  else
  {
    printf("Workorder nicht gefunden / Default Image <%s> nicht gefunden!\n",caImagePath);
    sprintf(caImagePath,"%s\\%s",
            tagAppConfig_g.caPathWorkOrders,
            WORK_ORDER_NONE);
    return(caImagePath);
  }

  return(caImagePath);
}

void vUiSetDeviceSerialNr_g(unsigned int uiSerialNr)
{
  tagUIElements_m.tagProdData.bUpdated=TRUE;
  tagUIElements_m.tagProdData.uiSerialNr=uiSerialNr;
} // vUiSetDeviceSerialNr_g()


void vUiSetUserText_g(char *pcUserText, ...)
{
  va_list args;
  char caBuffer[200];

  tagUIElements_m.tagProgStatus.bUpdated=TRUE;
  va_start(args,pcUserText);
  vsprintf(caBuffer,pcUserText,args);
  va_end(args);
  strcpy(tagUIElements_m.tagProgStatus.caUserText,caBuffer);
} // vUiSetUserText_g()

void vUiSetErrorText_g(int iErrorCode,
                       int iStep,
                       char *pcErrorText,...)
{
  va_list args;
  char caBuffer[500+1];

  if(pcErrorPanel_GetErrorMessage_g()!=NULL)
  {
    if((iErrorCode==uiErrorPanel_GetErrorCode_g())
       &&(!strncmp(pcErrorPanel_GetErrorMessage_g(),pcErrorText,min(50,strlen(pcErrorText))))
      )
    {
      // gleicher Fehler-Code und Fehler-Text nur einmal anzeigen
      return;
    }
  }

  va_start(args,pcErrorText);
  vsprintf(caBuffer,pcErrorText,args);
  strcpy(tagUIElements_m.tagProgError.caErrorText,caBuffer);
  tagUIElements_m.tagProgError.bHasError=TRUE;
  tagUIElements_m.tagProgError.iErrorCode=iErrorCode;
  tagUIElements_m.tagProgError.iStep=iStep;
  tagUIElements_m.tagProgError.bUpdated=TRUE;
} // vUiSetErrorText_g()

void vUiClearErrorText_g(void)
{
  tagUIElements_m.tagProgError.bUpdated=TRUE;
  tagUIElements_m.tagProgError.bHasError=FALSE;
} // vUiClearErrorText_g()

char *pcGetErrorText_g(void)
{
  return(tagUIElements_m.tagProgError.caErrorText);
} // pcGetErrorText_g()

char *pcGetUserText_g(void)
{
  return(tagUIElements_m.tagProgStatus.caUserText);
} // pcGetUserText_g()


//@TESTER_GUI_GENERATED_FUNCTIONS_START@
//Generated: 14:02 @ 20.03.2020
/**************************************************************
 *
 *
 * Funktionen zu ProdDataRead
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiProdDataRead_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0007,iGui0007_ProdDataRead_m,eState,TRUE));
} // iUiProdDataRead_SetState_g()

/**
 * Set Title
 */
void vUiProdDataRead_SetTitle_g(char *pcTitle,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiProdDataRead)
     && (tagUIElements_m.ptagGuiProdDataRead->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiProdDataRead->pFvSetTitle(iGui0007_ProdDataRead_m,caBuffer);
    return;
  }
  printf("vUiProdDataRead_SetTitle_g(): tagUIElements_m.ptagGuiProdDataRead->pFvSetTitle() == NULL!\n");
} // vUiProdDataRead_SetTitle_g()

/**
 * Set Text
 */
void vUiProdDataRead_SetText_g(char *pcText,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcText);
  vsprintf(caBuffer,pcText,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiProdDataRead)
     && (tagUIElements_m.ptagGuiProdDataRead->pFvSetText)
    )
  {
    tagUIElements_m.ptagGuiProdDataRead->pFvSetText(iGui0007_ProdDataRead_m,caBuffer);
    return;
  }
  printf("vUiProdDataRead_SetText_g(): tagUIElements_m.ptagGuiProdDataRead->pFvSetText() == NULL!\n");
} // vUiProdDataRead_SetText_g()

/**************************************************************
 *
 *
 * Funktionen zu CurrentIdle
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCurrentIdle_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0003,iGui0003_CurrentIdle_m,eState,TRUE));
} // iUiCurrentIdle_SetState_g()

/**
 * Set Title
 */
void vUiCurrentIdle_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCurrentIdle)
     && (tagUIElements_m.ptagGuiCurrentIdle->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCurrentIdle->pFvSetTitle(iGui0003_CurrentIdle_m,caBuffer);
    return;
  }
  printf("vUiCurrentIdle_SetTitle_g(): tagUIElements_m.ptagGuiCurrentIdle->pFvSetTitle() == NULL!\n");
} // vUiCurrentIdle_SetTitle_g()

/**
 * Set Min/Max
 */
void vUiCurrentIdle_SetMinMax_g(double dIMin, double dIMax)
{
  if(   (tagUIElements_m.ptagGuiCurrentIdle)
     && (tagUIElements_m.ptagGuiCurrentIdle->pFvSetMinMax)
    )
  {
    tagUIElements_m.ptagGuiCurrentIdle->pFvSetMinMax(iGui0003_CurrentIdle_m,dIMin,dIMax);
    return;
  }
  printf("vUiCurrentIdle_SetTitle_g(): tagUIElements_m.ptagGuiCurrentIdle->pFvSetMinMax() == NULL!\n");
} // vUiCurrentIdle_SetMinMax_g()

/**
 * Set current
 */
void vUiCurrentIdle_SetCurrent_g(double dICurrent)
{
  if(   (tagUIElements_m.ptagGuiCurrentIdle)
     && (tagUIElements_m.ptagGuiCurrentIdle->pFvSetCurrent)
    )
  {
    tagUIElements_m.ptagGuiCurrentIdle->pFvSetCurrent(iGui0003_CurrentIdle_m,dICurrent);
    return;
  }
  printf("vUiCurrentIdle_SetCurrent_g(): tagUIElements_m.ptagGuiCurrentIdle->pFvSetCurrent() == NULL!\n");
} // vUiCurrentIdle_SetCurrent_g()

/**
 * Set Unit
 */
void vUiCurrentIdle_SetUnit_g(char *pcUnit)
{
  if(   (tagUIElements_m.ptagGuiCurrentIdle)
     && (tagUIElements_m.ptagGuiCurrentIdle->pFvSetUnit)
    )
  {
    tagUIElements_m.ptagGuiCurrentIdle->pFvSetUnit(iGui0003_CurrentIdle_m,pcUnit);
    return;
  }
  printf("vUiCurrentIdle_SetUnit_g(): tagUIElements_m.ptagGuiCurrentIdle->pFvSetUnit() == NULL!\n");
} // vUiCurrentIdle_SetUnit_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckLed
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckLed_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0001,iGui0001_CheckLed_m,eState,TRUE));
} // iUiCheckLed_SetState_g()

/**
 * Set Work Order
 */
BOOL bUiCheckLed_SetWorkOrder_g(char *pcWorkOrder)
{
  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pFbSetWorkOrder)
    )
  {
    return(tagUIElements_m.ptagGuiCheckLed->pFbSetWorkOrder(iGui0001_CheckLed_m,pcWorkOrder));
  }
  printf("bUiCheckLed_SetWorkOrder_g(): tagUIElements_m.ptagGuiCheckLed->pFbSetWorkOrder() == NULL!\n");
  return(FALSE);
} // bUiCheckLed_SetWorkOrder_g()

/**
 * Set Title
 */
void vUiCheckLed_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckLed->pFvSetTitle(iGui0001_CheckLed_m,caBuffer);
    return;
  }
  printf("vUiCheckLed_SetTitle_g(): tagUIElements_m.ptagGuiCheckLed->pFvSetTitle() == NULL!\n");
} // vUiCheckLed_SetTitle_g()

/**
 * Set Fix Mode
 */
BOOL bUiCheckLed_SetFixMode_g(EFixMode eFixMode)
{
  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pFbSetFixMode)
    )
  {
    return(tagUIElements_m.ptagGuiCheckLed->pFbSetFixMode(iGui0001_CheckLed_m,eFixMode));
  }
  printf("bUiCheckLed_SetFixMode_g(): tagUIElements_m.ptagGuiCheckLed->pFbSetFixMode() == NULL!\n");
  return(FALSE);
} // bUiCheckLed_SetFixMode_g()

/**
 * Insert Picture
 */
int iUiCheckLed_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize)
{
  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pfiInsertPicture)
    )
  {
    return(tagUIElements_m.ptagGuiCheckLed->pfiInsertPicture(iGui0001_CheckLed_m,pcPath,ptagImagesize));
  }
  printf("iUiCheckLed_InsertPicture_g(): tagUIElements_m.ptagGuiCheckLed->pfiInsertPicture() == NULL!\n");
  return(-1);
} // iUiCheckLed_InsertPicture_g()

/**
 * Remove Picture
 */
BOOL bUiCheckLed_RemovePicture_g(int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pfbRemovePicture)
    )
  {
    return(tagUIElements_m.ptagGuiCheckLed->pfbRemovePicture(iGui0001_CheckLed_m,iPictureID));
  }
  printf("bUiCheckLed_RemovePicture_g(): tagUIElements_m.ptagGuiCheckLed->pfbRemovePicture() == NULL!\n");
  return(FALSE);
} // bUiCheckLed_RemovePicture_g()

/**
 * Replace Picture
 */
BOOL bUiCheckLed_ReplacePicture_g(const char *pcPath, int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiCheckLed)
     && (tagUIElements_m.ptagGuiCheckLed->pfbReplacePicture)
    )
  {
    return(tagUIElements_m.ptagGuiCheckLed->pfbReplacePicture(iGui0001_CheckLed_m,pcPath,iPictureID));
  }
  printf("bUiCheckLed_ReplacePicture_g(): tagUIElements_m.ptagGuiCheckLed->pfbReplacePicture() == NULL!\n");
  return(FALSE);
} // bUiCheckLed_ReplacePicture_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckAdc
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckAdc_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0007,iGui0007_CheckAdc_m,eState,TRUE));
} // iUiCheckAdc_SetState_g()

/**
 * Set Title
 */
void vUiCheckAdc_SetTitle_g(char *pcTitle,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckAdc)
     && (tagUIElements_m.ptagGuiCheckAdc->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckAdc->pFvSetTitle(iGui0007_CheckAdc_m,caBuffer);
    return;
  }
  printf("vUiCheckAdc_SetTitle_g(): tagUIElements_m.ptagGuiCheckAdc->pFvSetTitle() == NULL!\n");
} // vUiCheckAdc_SetTitle_g()

/**
 * Set Text
 */
void vUiCheckAdc_SetText_g(char *pcText,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcText);
  vsprintf(caBuffer,pcText,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckAdc)
     && (tagUIElements_m.ptagGuiCheckAdc->pFvSetText)
    )
  {
    tagUIElements_m.ptagGuiCheckAdc->pFvSetText(iGui0007_CheckAdc_m,caBuffer);
    return;
  }
  printf("vUiCheckAdc_SetText_g(): tagUIElements_m.ptagGuiCheckAdc->pFvSetText() == NULL!\n");
} // vUiCheckAdc_SetText_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckNfc
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckNfc_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0007,iGui0007_CheckNfc_m,eState,TRUE));
} // iUiCheckNfc_SetState_g()

/**
 * Set Title
 */
void vUiCheckNfc_SetTitle_g(char *pcTitle,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckNfc)
     && (tagUIElements_m.ptagGuiCheckNfc->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckNfc->pFvSetTitle(iGui0007_CheckNfc_m,caBuffer);
    return;
  }
  printf("vUiCheckNfc_SetTitle_g(): tagUIElements_m.ptagGuiCheckNfc->pFvSetTitle() == NULL!\n");
} // vUiCheckNfc_SetTitle_g()

/**
 * Set Text
 */
void vUiCheckNfc_SetText_g(char *pcText,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcText);
  vsprintf(caBuffer,pcText,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckNfc)
     && (tagUIElements_m.ptagGuiCheckNfc->pFvSetText)
    )
  {
    tagUIElements_m.ptagGuiCheckNfc->pFvSetText(iGui0007_CheckNfc_m,caBuffer);
    return;
  }
  printf("vUiCheckNfc_SetText_g(): tagUIElements_m.ptagGuiCheckNfc->pFvSetText() == NULL!\n");
} // vUiCheckNfc_SetText_g()

/**************************************************************
 *
 *
 * Funktionen zu ProdDataWrite
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiProdDataWrite_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0007,iGui0007_ProdDataWrite_m,eState,TRUE));
} // iUiProdDataWrite_SetState_g()

/**
 * Set Title
 */
void vUiProdDataWrite_SetTitle_g(char *pcTitle,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiProdDataWrite)
     && (tagUIElements_m.ptagGuiProdDataWrite->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiProdDataWrite->pFvSetTitle(iGui0007_ProdDataWrite_m,caBuffer);
    return;
  }
  printf("vUiProdDataWrite_SetTitle_g(): tagUIElements_m.ptagGuiProdDataWrite->pFvSetTitle() == NULL!\n");
} // vUiProdDataWrite_SetTitle_g()

/**
 * Set Text
 */
void vUiProdDataWrite_SetText_g(char *pcText,...)
{
  va_list vaArgs;
  char caBuffer[500];

  va_start(vaArgs,pcText);
  vsprintf(caBuffer,pcText,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiProdDataWrite)
     && (tagUIElements_m.ptagGuiProdDataWrite->pFvSetText)
    )
  {
    tagUIElements_m.ptagGuiProdDataWrite->pFvSetText(iGui0007_ProdDataWrite_m,caBuffer);
    return;
  }
  printf("vUiProdDataWrite_SetText_g(): tagUIElements_m.ptagGuiProdDataWrite->pFvSetText() == NULL!\n");
} // vUiProdDataWrite_SetText_g()

//@TESTER_GUI_GENERATED_FUNCTIONS_END@

