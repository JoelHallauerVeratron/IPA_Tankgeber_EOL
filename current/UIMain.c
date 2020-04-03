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
//Generated: 15:48 @ 01.04.2020
//Gui Variables
int iGui0001_WaitForLiquidLevelFull_m;
int iGui0004_CheckLiquidLevelFull_m;
int iGui0001_WaitForLiquidLevelHalf_m;
int iGui0004_CheckLiquidLevelHalf_m;
int iGui0001_WaitForLiquidLevelEmpty_m;
int iGui0004_CheckLiquidLevelEmpty_m;
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
//Generated: 15:48 @ 01.04.2020
/**
 * Init Function for LibTesterGuis
 */
void vLibTesterGui_Init_g(void)
{
  if(bTesterGuiInitialized_m)
  {
    vLibTesterGui_Discard_g();
  }

  iGui0001_WaitForLiquidLevelFull_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0001);
  tagUIElements_m.ptagGuiWaitForLiquidLevelFull = ptagLibTesterGui_0001_Init(iPanelMain_m,"Wasserstand auf 100%");

  iGui0004_CheckLiquidLevelFull_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0004);
  tagUIElements_m.ptagGuiCheckLiquidLevelFull = ptagLibTesterGui_0004_Init(iPanelMain_m,"Prüfe Sensor 100%");

  iGui0001_WaitForLiquidLevelHalf_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0001);
  tagUIElements_m.ptagGuiWaitForLiquidLevelHalf = ptagLibTesterGui_0001_Init(iPanelMain_m,"Wasserstand auf 100");

  iGui0004_CheckLiquidLevelHalf_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0004);
  tagUIElements_m.ptagGuiCheckLiquidLevelHalf = ptagLibTesterGui_0004_Init(iPanelMain_m,"Prüfe Sensor 50%");

  iGui0001_WaitForLiquidLevelEmpty_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0001);
  tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty = ptagLibTesterGui_0001_Init(iPanelMain_m,"Wasserstand auf 0%");

  iGui0004_CheckLiquidLevelEmpty_m = iLibTesterGuiMgr_AddGui(eLibTesterGui_0004);
  tagUIElements_m.ptagGuiCheckLiquidLevelEmpty = ptagLibTesterGui_0004_Init(iPanelMain_m,"Prüfe Sensor 0%");

  bTesterGuiInitialized_m=TRUE;
  bLibTesterGuiMgr_DisplayAllGuis(TRUE);
} // vLibTesterGui_Init_g()

/**
 * Discard Function for LibTesterGuis
 */
void vLibTesterGui_Discard_g(void)
{
  vLibTesterGui_0001_Discard(tagUIElements_m.ptagGuiWaitForLiquidLevelFull,FALSE);
  vLibTesterGui_0004_Discard(tagUIElements_m.ptagGuiCheckLiquidLevelFull,FALSE);
  vLibTesterGui_0001_Discard(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf,FALSE);
  vLibTesterGui_0004_Discard(tagUIElements_m.ptagGuiCheckLiquidLevelHalf,FALSE);
  vLibTesterGui_0001_Discard(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty,FALSE);
  vLibTesterGui_0004_Discard(tagUIElements_m.ptagGuiCheckLiquidLevelEmpty,FALSE);
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
//Generated: 15:48 @ 01.04.2020
  if(bTesterGuiInitialized_m)
  {
    vLibTesterGui_0001_VisualUpdate(iGui0001_WaitForLiquidLevelFull_m);
    vLibTesterGui_0004_VisualUpdate(iGui0004_CheckLiquidLevelFull_m);
    vLibTesterGui_0001_VisualUpdate(iGui0001_WaitForLiquidLevelHalf_m);
    vLibTesterGui_0004_VisualUpdate(iGui0004_CheckLiquidLevelHalf_m);
    vLibTesterGui_0001_VisualUpdate(iGui0001_WaitForLiquidLevelEmpty_m);
    vLibTesterGui_0004_VisualUpdate(iGui0004_CheckLiquidLevelEmpty_m);
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
//Generated: 15:48 @ 01.04.2020
/**************************************************************
 *
 *
 * Funktionen zu WaitForLiquidLevelFull
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiWaitForLiquidLevelFull_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0001,iGui0001_WaitForLiquidLevelFull_m,eState,TRUE));
} // iUiWaitForLiquidLevelFull_SetState_g()

/**
 * Set Work Order
 */
BOOL bUiWaitForLiquidLevelFull_SetWorkOrder_g(char *pcWorkOrder)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetWorkOrder)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetWorkOrder(iGui0001_WaitForLiquidLevelFull_m,pcWorkOrder));
  }
  printf("bUiWaitForLiquidLevelFull_SetWorkOrder_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetWorkOrder() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelFull_SetWorkOrder_g()

/**
 * Set Title
 */
void vUiWaitForLiquidLevelFull_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFvSetTitle(iGui0001_WaitForLiquidLevelFull_m,caBuffer);
    return;
  }
  printf("vUiWaitForLiquidLevelFull_SetTitle_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFvSetTitle() == NULL!\n");
} // vUiWaitForLiquidLevelFull_SetTitle_g()

/**
 * Set Fix Mode
 */
BOOL bUiWaitForLiquidLevelFull_SetFixMode_g(EFixMode eFixMode)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetFixMode)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetFixMode(iGui0001_WaitForLiquidLevelFull_m,eFixMode));
  }
  printf("bUiWaitForLiquidLevelFull_SetFixMode_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pFbSetFixMode() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelFull_SetFixMode_g()

/**
 * Insert Picture
 */
int iUiWaitForLiquidLevelFull_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfiInsertPicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfiInsertPicture(iGui0001_WaitForLiquidLevelFull_m,pcPath,ptagImagesize));
  }
  printf("iUiWaitForLiquidLevelFull_InsertPicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfiInsertPicture() == NULL!\n");
  return(-1);
} // iUiWaitForLiquidLevelFull_InsertPicture_g()

/**
 * Remove Picture
 */
BOOL bUiWaitForLiquidLevelFull_RemovePicture_g(int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbRemovePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbRemovePicture(iGui0001_WaitForLiquidLevelFull_m,iPictureID));
  }
  printf("bUiWaitForLiquidLevelFull_RemovePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbRemovePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelFull_RemovePicture_g()

/**
 * Replace Picture
 */
BOOL bUiWaitForLiquidLevelFull_ReplacePicture_g(const char *pcPath, int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelFull)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbReplacePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbReplacePicture(iGui0001_WaitForLiquidLevelFull_m,pcPath,iPictureID));
  }
  printf("bUiWaitForLiquidLevelFull_ReplacePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelFull->pfbReplacePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelFull_ReplacePicture_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckLiquidLevelFull
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckLiquidLevelFull_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0004,iGui0004_CheckLiquidLevelFull_m,eState,TRUE));
} // iUiCheckLiquidLevelFull_SetState_g()

/**
 * Set Title
 */
void vUiCheckLiquidLevelFull_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTitle(iGui0004_CheckLiquidLevelFull_m,caBuffer);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTitle() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetTitle_g()

/**
 * Set Min/Max
 */
void vUiCheckLiquidLevelFull_SetMinMax_g(double dIMin, double dIMax)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetMinMax)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetMinMax(iGui0004_CheckLiquidLevelFull_m,dIMin,dIMax);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetMinMax() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetMinMax_g()

/**
 * Set current
 */
void vUiCheckLiquidLevelFull_SetValue_g(double dValue)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetValue)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetValue(iGui0004_CheckLiquidLevelFull_m,dValue);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetCurrent() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetCurrent_g()

/**
 * Set Unit
 */
void vUiCheckLiquidLevelFull_SetUnit_g(char *pcUnit)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetUnit)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetUnit(iGui0004_CheckLiquidLevelFull_m,pcUnit);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetUnit() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetUnit_g()

/**
 * Set Tolerance
 */
void vUiCheckLiquidLevelFull_SetTolerance_g(double dTolerance)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTolerance)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTolerance(iGui0004_CheckLiquidLevelFull_m,dTolerance);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetTolerance() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetUnit_g()

/**
 * Set Soll
 */
void vUiCheckLiquidLevelFull_SetSoll_g(double dSoll)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetSoll)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetSoll(iGui0004_CheckLiquidLevelFull_m,dSoll);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetSoll() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetCurrent_g()

/**
 * Set Precision
 */
void vUiCheckLiquidLevelFull_SetPrecision_g(int iPrecision)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelFull)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetPrecision)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetPrecision(iGui0004_CheckLiquidLevelFull_m,iPrecision);
    return;
  }
  printf("vUiCheckLiquidLevelFull_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelFull->pFvSetPrecision() == NULL!\n");
} // vUiCheckLiquidLevelFull_SetCurrent_g()
/**************************************************************
 *
 *
 * Funktionen zu WaitForLiquidLevelHalf
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiWaitForLiquidLevelHalf_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0001,iGui0001_WaitForLiquidLevelHalf_m,eState,TRUE));
} // iUiWaitForLiquidLevelHalf_SetState_g()

/**
 * Set Work Order
 */
BOOL bUiWaitForLiquidLevelHalf_SetWorkOrder_g(char *pcWorkOrder)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetWorkOrder)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetWorkOrder(iGui0001_WaitForLiquidLevelHalf_m,pcWorkOrder));
  }
  printf("bUiWaitForLiquidLevelHalf_SetWorkOrder_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetWorkOrder() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelHalf_SetWorkOrder_g()

/**
 * Set Title
 */
void vUiWaitForLiquidLevelHalf_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFvSetTitle(iGui0001_WaitForLiquidLevelHalf_m,caBuffer);
    return;
  }
  printf("vUiWaitForLiquidLevelHalf_SetTitle_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFvSetTitle() == NULL!\n");
} // vUiWaitForLiquidLevelHalf_SetTitle_g()

/**
 * Set Fix Mode
 */
BOOL bUiWaitForLiquidLevelHalf_SetFixMode_g(EFixMode eFixMode)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetFixMode)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetFixMode(iGui0001_WaitForLiquidLevelHalf_m,eFixMode));
  }
  printf("bUiWaitForLiquidLevelHalf_SetFixMode_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pFbSetFixMode() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelHalf_SetFixMode_g()

/**
 * Insert Picture
 */
int iUiWaitForLiquidLevelHalf_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfiInsertPicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfiInsertPicture(iGui0001_WaitForLiquidLevelHalf_m,pcPath,ptagImagesize));
  }
  printf("iUiWaitForLiquidLevelHalf_InsertPicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfiInsertPicture() == NULL!\n");
  return(-1);
} // iUiWaitForLiquidLevelHalf_InsertPicture_g()

/**
 * Remove Picture
 */
BOOL bUiWaitForLiquidLevelHalf_RemovePicture_g(int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbRemovePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbRemovePicture(iGui0001_WaitForLiquidLevelHalf_m,iPictureID));
  }
  printf("bUiWaitForLiquidLevelHalf_RemovePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbRemovePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelHalf_RemovePicture_g()

/**
 * Replace Picture
 */
BOOL bUiWaitForLiquidLevelHalf_ReplacePicture_g(const char *pcPath, int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbReplacePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbReplacePicture(iGui0001_WaitForLiquidLevelHalf_m,pcPath,iPictureID));
  }
  printf("bUiWaitForLiquidLevelHalf_ReplacePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelHalf->pfbReplacePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelHalf_ReplacePicture_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckLiquidLevelHalf
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckLiquidLevelHalf_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0004,iGui0004_CheckLiquidLevelHalf_m,eState,TRUE));
} // iUiCheckLiquidLevelHalf_SetState_g()

/**
 * Set Title
 */
void vUiCheckLiquidLevelHalf_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTitle(iGui0004_CheckLiquidLevelHalf_m,caBuffer);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTitle() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetTitle_g()

/**
 * Set Min/Max
 */
void vUiCheckLiquidLevelHalf_SetMinMax_g(double dIMin, double dIMax)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetMinMax)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetMinMax(iGui0004_CheckLiquidLevelHalf_m,dIMin,dIMax);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetMinMax() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetMinMax_g()

/**
 * Set current
 */
void vUiCheckLiquidLevelHalf_SetValue_g(double dValue)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetValue)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetValue(iGui0004_CheckLiquidLevelHalf_m,dValue);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetCurrent() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetCurrent_g()

/**
 * Set Unit
 */
void vUiCheckLiquidLevelHalf_SetUnit_g(char *pcUnit)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetUnit)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetUnit(iGui0004_CheckLiquidLevelHalf_m,pcUnit);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetUnit() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetUnit_g()

/**
 * Set Tolerance
 */
void vUiCheckLiquidLevelHalf_SetTolerance_g(double dTolerance)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTolerance)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTolerance(iGui0004_CheckLiquidLevelHalf_m,dTolerance);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetTolerance() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetUnit_g()

/**
 * Set Soll
 */
void vUiCheckLiquidLevelHalf_SetSoll_g(double dSoll)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetSoll)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetSoll(iGui0004_CheckLiquidLevelHalf_m,dSoll);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetSoll() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetCurrent_g()

/**
 * Set Precision
 */
void vUiCheckLiquidLevelHalf_SetPrecision_g(int iPrecision)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelHalf)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetPrecision)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetPrecision(iGui0004_CheckLiquidLevelHalf_m,iPrecision);
    return;
  }
  printf("vUiCheckLiquidLevelHalf_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelHalf->pFvSetPrecision() == NULL!\n");
} // vUiCheckLiquidLevelHalf_SetCurrent_g()
/**************************************************************
 *
 *
 * Funktionen zu WaitForLiquidLevelEmpty
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiWaitForLiquidLevelEmpty_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0001,iGui0001_WaitForLiquidLevelEmpty_m,eState,TRUE));
} // iUiWaitForLiquidLevelEmpty_SetState_g()

/**
 * Set Work Order
 */
BOOL bUiWaitForLiquidLevelEmpty_SetWorkOrder_g(char *pcWorkOrder)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetWorkOrder)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetWorkOrder(iGui0001_WaitForLiquidLevelEmpty_m,pcWorkOrder));
  }
  printf("bUiWaitForLiquidLevelEmpty_SetWorkOrder_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetWorkOrder() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelEmpty_SetWorkOrder_g()

/**
 * Set Title
 */
void vUiWaitForLiquidLevelEmpty_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFvSetTitle(iGui0001_WaitForLiquidLevelEmpty_m,caBuffer);
    return;
  }
  printf("vUiWaitForLiquidLevelEmpty_SetTitle_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFvSetTitle() == NULL!\n");
} // vUiWaitForLiquidLevelEmpty_SetTitle_g()

/**
 * Set Fix Mode
 */
BOOL bUiWaitForLiquidLevelEmpty_SetFixMode_g(EFixMode eFixMode)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetFixMode)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetFixMode(iGui0001_WaitForLiquidLevelEmpty_m,eFixMode));
  }
  printf("bUiWaitForLiquidLevelEmpty_SetFixMode_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pFbSetFixMode() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelEmpty_SetFixMode_g()

/**
 * Insert Picture
 */
int iUiWaitForLiquidLevelEmpty_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfiInsertPicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfiInsertPicture(iGui0001_WaitForLiquidLevelEmpty_m,pcPath,ptagImagesize));
  }
  printf("iUiWaitForLiquidLevelEmpty_InsertPicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfiInsertPicture() == NULL!\n");
  return(-1);
} // iUiWaitForLiquidLevelEmpty_InsertPicture_g()

/**
 * Remove Picture
 */
BOOL bUiWaitForLiquidLevelEmpty_RemovePicture_g(int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbRemovePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbRemovePicture(iGui0001_WaitForLiquidLevelEmpty_m,iPictureID));
  }
  printf("bUiWaitForLiquidLevelEmpty_RemovePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbRemovePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelEmpty_RemovePicture_g()

/**
 * Replace Picture
 */
BOOL bUiWaitForLiquidLevelEmpty_ReplacePicture_g(const char *pcPath, int iPictureID)
{
  if(   (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbReplacePicture)
    )
  {
    return(tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbReplacePicture(iGui0001_WaitForLiquidLevelEmpty_m,pcPath,iPictureID));
  }
  printf("bUiWaitForLiquidLevelEmpty_ReplacePicture_g(): tagUIElements_m.ptagGuiWaitForLiquidLevelEmpty->pfbReplacePicture() == NULL!\n");
  return(FALSE);
} // bUiWaitForLiquidLevelEmpty_ReplacePicture_g()

/**************************************************************
 *
 *
 * Funktionen zu CheckLiquidLevelEmpty
 *
 *
 **************************************************************/

/**
 * Set State
 */
int iUiCheckLiquidLevelEmpty_SetState_g(EPanelStatus eState)
{
  return(iLibTesterGuiMgr_SetPanelStatus(eLibTesterGui_0004,iGui0004_CheckLiquidLevelEmpty_m,eState,TRUE));
} // iUiCheckLiquidLevelEmpty_SetState_g()

/**
 * Set Title
 */
void vUiCheckLiquidLevelEmpty_SetTitle_g(char *pcTitle, ...)
{
  char caBuffer[500];
  va_list vaArgs;

  va_start(vaArgs,pcTitle);
  vsprintf(caBuffer,pcTitle,vaArgs);
  va_end(vaArgs);

  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTitle)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTitle(iGui0004_CheckLiquidLevelEmpty_m,caBuffer);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTitle() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetTitle_g()

/**
 * Set Min/Max
 */
void vUiCheckLiquidLevelEmpty_SetMinMax_g(double dIMin, double dIMax)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetMinMax)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetMinMax(iGui0004_CheckLiquidLevelEmpty_m,dIMin,dIMax);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetTitle_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetMinMax() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetMinMax_g()

/**
 * Set current
 */
void vUiCheckLiquidLevelEmpty_SetValue_g(double dValue)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetValue)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetValue(iGui0004_CheckLiquidLevelEmpty_m,dValue);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetCurrent() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetCurrent_g()

/**
 * Set Unit
 */
void vUiCheckLiquidLevelEmpty_SetUnit_g(char *pcUnit)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetUnit)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetUnit(iGui0004_CheckLiquidLevelEmpty_m,pcUnit);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetUnit() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetUnit_g()

/**
 * Set Tolerance
 */
void vUiCheckLiquidLevelEmpty_SetTolerance_g(double dTolerance)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTolerance)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTolerance(iGui0004_CheckLiquidLevelEmpty_m,dTolerance);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetUnit_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetTolerance() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetUnit_g()

/**
 * Set Soll
 */
void vUiCheckLiquidLevelEmpty_SetSoll_g(double dSoll)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetSoll)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetSoll(iGui0004_CheckLiquidLevelEmpty_m,dSoll);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetSoll() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetCurrent_g()

/**
 * Set Precision
 */
void vUiCheckLiquidLevelEmpty_SetPrecision_g(int iPrecision)
{
  if(   (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty)
     && (tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetPrecision)
    )
  {
    tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetPrecision(iGui0004_CheckLiquidLevelEmpty_m,iPrecision);
    return;
  }
  printf("vUiCheckLiquidLevelEmpty_SetCurrent_g(): tagUIElements_m.ptagGuiCheckLiquidLevelEmpty->pFvSetPrecision() == NULL!\n");
} // vUiCheckLiquidLevelEmpty_SetCurrent_g()
//@TESTER_GUI_GENERATED_FUNCTIONS_END@
























