/*******************************************************************
 *
 *    PROJECT:     Piaggio eBike
 *
 *    EQUIPMENT:   KWP2000 Diag
 *
 *    FILE:        KWP2000Diag.c
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      uidc9013
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        16.09.2011
 *
 *    HISTORY:
 *      -16.09.2011: initial implementation
 *      -
 *
 *******************************************************************/

#include "defines.h"

#include <windows.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include "global.h"

#include "LibDutCom_Import.h"

#include "KWP2000Diag.h"
#include "KWP2000Diag_Funcs.h"


int panelKwp2000_m=-1;        // Hauptbildschirm für Manual

BOOL bKWP2000DiagOpen_m=FALSE;


BOOL bSetPtrOffset_m;
BOOL bTracePaused_m;


void vInitKWP2000Panel_g(int panelParent)
{
  if(panelKwp2000_m<0)
  {
    if((panelKwp2000_m=LoadPanel(0,"KWP2000Diag.uir",PNLKWP2000))<0)
      return;
  }

  SetPanelAttribute(panelKwp2000_m,ATTR_SCALE_CONTENTS_ON_RESIZE,1);

  if(panelParent>0)
  {
    // Panel genau unterhalb von ProcessPanel platzieren
    int iLeft;
    int iHeight;
    int iTop;

    GetPanelAttribute(panelParent,ATTR_LEFT,&iLeft);
    GetPanelAttribute(panelParent,ATTR_HEIGHT,&iHeight);
    GetPanelAttribute(panelParent,ATTR_TOP,&iTop);
    iTop=iTop+iHeight+25;   // 25 für Titelbalken dazurechnen
    iTop=iTop-200;   // 200 pixel höher
    SetPanelPos(panelKwp2000_m,iTop,iLeft);
  }

  DisplayPanel(panelKwp2000_m);
  SetCtrlVal(panelKwp2000_m,PNLKWP2000_CHK_TRACE_ENABLE,1);
  bKWP2000DiagOpen_m=TRUE;
}


BOOL bKWP2000DiagIsOpen_g(void)
{
  return(bKWP2000DiagOpen_m);
} // bKWP2000DiagIsOpen_g()


int CVICALLBACK PanelKwp2000CB(int panel, int event, void *callbackData,
                               int eventData1, int eventData2)
{
  switch(event)
  {
    case EVENT_CLOSE:
      HidePanel(panel);
      bKWP2000DiagOpen_m=FALSE;
      break;
    case EVENT_KEYPRESS:
      if(eventData1==VAL_ESC_VKEY)
      {
        HidePanel(panel);
        bKWP2000DiagOpen_m=FALSE;
      }
      break;
  }
  return(0);
}


int CVICALLBACK TmrUpdateCB(int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
  if(event!=EVENT_TIMER_TICK)
    return(0);

  // Trace anzeigen
  if(iDutCom_GetOneTraceLine!=NULL)
  {
    if(!bTracePaused_m)
    {
      char caTraceLine[500];
      int iTraceLinesRemaining=0;
      int iTraceLinesRetrieved=0;

      do
      {
        if(iDutCom_GetOneTraceLine(caTraceLine,&iTraceLinesRemaining)==0)
          break;

        ++iTraceLinesRetrieved;
        strcat(caTraceLine,"\n");
        SetCtrlVal(panel,PNLKWP2000_TRACE,caTraceLine);
      } while((iTraceLinesRetrieved<5)
              && (iTraceLinesRemaining>0)
             );
      SetCtrlVal(panel,PNLKWP2000_NUM_TRACE_BUFFER,iTraceLinesRemaining);
    }
  }

  return(0);
}


int CVICALLBACK TraceClearCB(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
  if(event!=EVENT_COMMIT)
    return(0);

  if(vDutCom_TraceClear)
    vDutCom_TraceClear();

  if(pcDutCom_GetTraceHeader)
  {
    char caTraceHeader[100];
    strcpy(caTraceHeader,pcDutCom_GetTraceHeader());
    strcat(caTraceHeader,"\n");
    ResetTextBox(panel,PNLKWP2000_TRACE,caTraceHeader);
  }
  else
    ResetTextBox(panel,PNLKWP2000_TRACE,"");
  Cls();

  return(0);
}


int CVICALLBACK TraceCopyCB(int panel, int control, int event,
                            void *callbackData, int eventData1, int eventData2)
{
  char *pcText;

  switch(event)
  {
    case EVENT_COMMIT:
      pcText=(char*)malloc(1000000);
      GetCtrlVal(panelKwp2000_m,PNLKWP2000_TRACE,pcText);
      if(strlen(pcText)>100)
        ClipboardPutText(pcText);
      free(pcText);
      break;
  }
  return(0);
}


int CVICALLBACK TracePauseCB(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
  int iValue;

  if(event!=EVENT_COMMIT)
    return(0);

  GetCtrlVal(panel,control,&iValue);
  bTracePaused_m=(iValue==1);

  return(0);
}


void vCheckSetTraceEnabled_m(int iPanel)
{
  int iValue;

  GetCtrlVal(iPanel,PNLKWP2000_CHK_TRACE_ENABLE,&iValue);
  if(vDutCom_TraceEnable!=NULL)
  {
    vDutCom_TraceEnable(iValue==1);
  }
} // vCheckSetTraceEnabled_m()


int CVICALLBACK ChkTraceEnableCB(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
  if(event!=EVENT_COMMIT)
    return(0);

  vCheckSetTraceEnabled_m(panel);
  return(0);
}


int CVICALLBACK BtnLibDutComLoad(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
  int iButtonState;
  int iLibNr;
  char caInfo[100];

  if(event!=EVENT_COMMIT)
    return(0);

  GetCtrlVal(panel,control,&iButtonState);

  if(iButtonState==1)
  {
    // load
    GetCtrlVal(panel,PNLKWP2000_NUM_LIBDUTCOM,&iLibNr);
    if(bDutCom_Load(iLibNr,TRUE))
    {
      vDutCom_GetInfo(caInfo,NULL,NULL,NULL,NULL);
      SetCtrlVal(panel,PNLKWP2000_TXT_LIBDUTCOM_INFO,caInfo);
    }
    else
    {
      vDutCom_GetImportErrorText(caInfo,100);
    }
    SetCtrlVal(panel,PNLKWP2000_TXT_LIBDUTCOM_INFO,caInfo);
  }
  else
  {
    // unload
    vDutCom_Unload();
    SetCtrlVal(panel,PNLKWP2000_TXT_LIBDUTCOM_INFO," ");
  }

  return(0);
}


