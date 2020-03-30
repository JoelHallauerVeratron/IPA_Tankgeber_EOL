/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        Main.c
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author$
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#include <windows.h>

#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <utility.h>

#include "defines.h"
#include "Main.h"
#include "Config.h"
#include "LibNetworking.h"
#include "UIMain.h"
#include "global.h"
#include "process.h"
#include "IODiag.h"
#include "IODiag_Funcs.h"
#include "ibs.h"
#include "zol.h"
#include "status.h"
#include "devicedat.h"
#include "KWP2000Diag_Funcs.h"
#include "SerialNumber.h"
#ifdef USE_NFC
#include "LibNfc.h"
#endif // USE_NFC

#ifdef USE_LABEL_PRINTER
  #include "LibIntermecPrinter.h"
#endif // USE_LABEL_PRINTER


int iPanelMain_m=INVALID_PANEL_HANDLE;

int iPanelMenuMain_m =INVALID_PANEL_HANDLE;
int iPanelTests_m    =INVALID_PANEL_HANDLE;

BOOL bProgramExit_m;

extern TagDevice tagCurrentDeviceTyp_g;

static HANDLE hMutex_m;

/**
 * SingleInstanceChecker
 *
 * Stellt sicher, dass dieses Programm in nur einer Instanz läuft.<BR>
 * Dazu wird ein benanntes Semaphore kreiert.<BR>
 * Wenn eine weitere Programminstanz das selbe Semaphore nochmals<BR>
 * kreieren möchte, wird die erkannt und diese Funktion gibt FALSE zurück
 *
 * @param pcProgramName
 *               Name des Programmes, am besten argv[0]
 *               Bei NULL Pointer wird der Name des Computers genommen
 *
 * @return TRUE wenn dies die erste Instanz ist<BR>
 *         FALSE wenn schon eine Instanz läuft
 */
BOOL bCheckForSingleInstance_m(char *pcProgramName)
{
  BOOL m_bWasOpened;

  if(pcProgramName==NULL)
    pcProgramName=getenv("HOSTNAME");

  hMutex_m = CreateMutex(NULL, FALSE, pcProgramName);
  if(!hMutex_m)
  {
    printf("failed to create mutex %s (error=%d)\n",pcProgramName,GetLastError());
    return(FALSE);
  }

  m_bWasOpened = (GetLastError() == ERROR_ALREADY_EXISTS);

  return(!m_bWasOpened);
} // bCheckForSingleInstance_m()



/**
 * Programm initialisieren
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bProgInit_m(void)
{
  char caVersion[10];

  // Initialisierungen
  if(!bAppConfig_Read_g("Config.ini"))
  {
    printf("bAppConfig_Read_g() failed\n");
    vAppConfig_Dump_g();
    return(FALSE);
  }
  //vAppConfig_Dump_g();

  sprintf(caVersion,"%d.%03d",PROGRAM_VERSION_MAJOR,PROGRAM_VERSION_MINOR);
  if(!bNetworking_ConnectWait(tagAppConfig_g.tagNetwork.caUser,
                              tagAppConfig_g.tagNetwork.caPwd,
                              tagAppConfig_g.tagNetwork.caMapDrive,
                              tagAppConfig_g.tagNetwork.caShareName,
                              tagAppConfig_g.pcCompanyName,
                              tagAppConfig_g.pcProjectName,
                              tagAppConfig_g.pcApplicationName,
                              caVersion))
  {
    printf("bNetworking_ConnectWait(\"%s\",\n"
           "                        \"%s\",\n"
           "                        \"%s\",\n"
           "                        \"%s\",\n"
           "                        \"%s\",\n"
           "                        \"%s\",\n"
           "                        \"%s\"\n) failed\n",
           tagAppConfig_g.tagNetwork.caUser,
           "*****",
           tagAppConfig_g.tagNetwork.caMapDrive,
           tagAppConfig_g.tagNetwork.caShareName,
           tagAppConfig_g.pcProjectName,
           tagAppConfig_g.pcApplicationName,
           caVersion);
    MessageBox(NULL,pcNetworking_GetLastErrorString(),"Fehler",MB_OK|MB_ICONWARNING);
    return(FALSE);
  }

  if(iSNr_Init_g(tagAppConfig_g.caPathSerialNumbers,eMonthly)!=SNR_ERROR_NO_ERROR)
  {
    printf("iSNr_Init_g(): failed %s\n",tagAppConfig_g.caPathSerialNumbers);
    return(FALSE);
  }

#ifdef USE_LABEL_PRINTER
  if(!bIntermecPrinter_Connect_g(tagAppConfig_g.tagLabelPrinter.caIpAddress))
  {
    printf("Fehler beim initialisieren des Etiketten-Druckers\n");
    return(FALSE);
  }
#endif // USE_LABEL_PRINTER

#ifdef USE_NFC
  if(!bNFC_Init_g(tagAppConfig_g.tagNfcReader.uiComPort,
                  tagAppConfig_g.tagNfcReader.uiBaudrate))
  {
    printf("Fehler beim initialisieren des NFC-Readers\n"
           "Fehler: \"%s\"",pcNFC_GetErrorMessage_g());
    return(FALSE);
  }
#endif // USE_NFC

  bIniFileDevice_SetFile_g(tagAppConfig_g.caDutDatIniFileName);

#ifdef USE_TOUCH_PANEL
  SetCtrlAttribute(iPanelMain_m,PNL_MAIN_BTN_MENU,ATTR_VISIBLE,1);
#else // !USE_TOUCH_PANEL
  SetCtrlAttribute(iPanelMain_m,PNL_MAIN_BTN_MENU,ATTR_VISIBLE,0);
  LoadMenuBar(iPanelMain_m,"Main.uir",MENU);
#endif // !USE_TOUCH_PANEL

#ifdef IGNORE_FLIL
  #ifndef USE_TOUCH_PANEL
  SetMenuBarAttribute(GetPanelMenuBar(iPanelMain_m),MENU_WINDOW_IO_DIAG,ATTR_DIMMED,1);
  #endif // !USE_TOUCH_PANEL
#endif // IGNORE_FLIL

  vInitUserInterface_g(iPanelMain_m);

#ifndef IGNORE_FLIL
  iIBS_HLI_Init_g();
#endif // !IGNORE_FLIL

  bProgramExit_m=FALSE;

  return(TRUE);
} // bProgInit_m()


/**
 * Programm aufräumen und Event-Handler beenden
 *
 * @param iRc    zurück zu gebender Code
 */
void vProgEnd_m(int iRc)
{
  // Aufräumen

  if(!bNetworking_Disconnect(tagAppConfig_g.tagNetwork.caMapDrive,TRUE))
  {
    printf("bNetworkDisconnect_g() failed\n");
  }

#ifdef USE_NFC
  bNFC_Close_g();
#endif // USE_NFC

  vUIDiscard_g();

  HidePanel(iPanelMain_m);
  DiscardPanel(iPanelMain_m);

  QuitUserInterface(iRc);
} // vProgEnd_m()








int main(int argc, char *argv[])
{
  if(InitCVIRTE(0,argv,0)==0)
    exit(-1);  /* out of memory */

  if(!bCheckForSingleInstance_m(argv[0]))
  {
    MessagePopup("Fehler","Das Programm darf nur einmal gestartet werden");
    exit(-2);
  }

  if((iPanelMain_m=LoadPanel(0,"Main.uir",PNL_MAIN))<0)
    exit(-3);

  tagAppConfig_g.pcCompanyName="veratron AG";
  tagAppConfig_g.pcProjectName="LinkUp";
  tagAppConfig_g.pcApplicationName="EOL";
  if(!bProgInit_m())
  {
    Sleep(10000);
    exit(-4);
  }

  /* Initialwerte für auszuführende Operationen setzen */
  tagAppStatus_g.tagTest.bSwVersion  =TRUE;
  tagAppStatus_g.tagTest.bAnInputs   =TRUE;
  tagAppStatus_g.tagTest.bNfc        =TRUE;
  tagAppStatus_g.tagTest.bLed        =TRUE;

#if 0
  // Zum Testen andere Schalterstellungen setzen
  //tagAppStatus_g.tagTest.bSwVersion  =TRUE;
  //tagAppStatus_g.tagTest.bAnInputs   =TRUE;
  tagAppStatus_g.tagTest.bNfc        =FALSE;
  tagAppStatus_g.tagTest.bLed        =TRUE;
#endif

  #ifndef USE_NFC
  tagAppStatus_g.tagTest.bNfc        =FALSE;
  #endif // !USE_NFC

#ifndef IGNORE_FLIL
  iIBS_HLI_Init_g();
#endif // !IGNORE_FLIL
  bProgramExit_m=FALSE;

  //SetPanelPos(iPanelMain_m,25,5);
  DisplayPanel(iPanelMain_m);

  RunUserInterface();

  return(0);
} // main()


/**
 * Programm aufräumen und Event-Handler beenden
 *
 * @param iRc    zurück zu gebender Code
 */
void vQuitProgram_g(void)
{
  vResAllOutputs_g();
  vSetDO_g(&aSteuerungBereit,FALSE);

#ifdef USE_LABEL_PRINTER
  vIntermecPrinter_Disconnect_g();
#endif // USE_LABEL_PRINTER

  bProgramExit_m=TRUE;
  // QuitUserInterface() wird verzögert vom Scheduler aufgerufen, dass zum
  //Auschalten aller Outputs das Prozessabbild nochmals aktualisiert werden kann.

} // vQuitProgram_g()


int CVICALLBACK MainPanelCB(int panel, int event,
                            void *callbackData, int eventData1, int eventData2)
{
  switch(event)
  {
    case EVENT_CLOSE:
      vQuitProgram_g();
      break;
    case EVENT_LEFT_CLICK:
    case EVENT_RIGHT_CLICK:
      if(iPanelMenuMain_m>INVALID_PANEL_HANDLE)
      {
        HidePanel(iPanelMenuMain_m);
        //iPanelMenuMain_m=IVALID_PANEL_HANDLE;
      }
  }
  return(0);
}


int CVICALLBACK SchedulerCB(int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
  static unsigned int iDelay;
  static unsigned int iProgramExitDelay=0;

  if(event!=EVENT_TIMER_TICK)
    return(0);

#ifndef IGNORE_FLIL
  if(bIBS_HLI_Running_g())
    iIBS_HLI_Process_g();           // Interbus Prozessabbild updaten
#endif // !IGNORE_FLIL

  if(iDelay>5)
  {
    vUpdateUserInterface_g(FALSE);     // User Interface updaten
    iDelay=0;
  }

  iProcess_g();                   // Hauptablauf

  // IBS verzögert beenden, dass zum Schluss alle Outputs ausgeschaltet werden können
  if(bProgramExit_m)
    ++iProgramExitDelay;

  if(iProgramExitDelay>5)
  {
    iIBS_HLI_Exit_g();
    vProgEnd_m(0);
    //QuitUserInterface(0);
  }

  iDelay++;

  return(0);
}



