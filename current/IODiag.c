// ----------------------------------------------------------------------------
//
// diagnose.c
//
// Projekt: I/O-Diagnosefunktionen
//
// Version 1.0
// 17.02.2010 /WeU
//
// ----------------------------------------------------------------------------

#include "defines.h"

#include <ansi_c.h>
#include <toolbox.h>
#include <userint.h>

#include "zol.h"
#include "IODiag.h"
#include "ibs.h"
#include "NumPanel_Funcs.h"


int iTopOutputByte_m=0;
int iTopInputByte_m=0;

int panelDiagnose_m=0;
BOOL bPanelVisible_m=FALSE;
BOOL bAutoEnabled_m=FALSE;

BOOL bTouchPanel_m;
BOOL bLabelsInitialized_m=FALSE;

int iNumInputBytes_m=0;
int iNumOutputBytes_m;
int iLastImputBtn_m;
int iLastOutputBtn_m;
//Anzahl Byte die pro Seite Platz haben
int iBytesPerPage_m;

/**
 * vInitDiagnoseLabels_m()
 *
 *
 *
 * @author uidc9013 (18.02.2010)
 */
void vInitDiagnoseLabels_m(void)
{
  //Anzahl Input-/Outputs-Byte bestimmen
  int iMaxControl=0;
  int iIndex;

  for(iIndex=0;apDigInpList_g[iIndex];iIndex++)
  {
    iMaxControl=apDigInpList_g[iIndex]->iControl;
  }
  iNumInputBytes_m=(iMaxControl/8)+1;

  for(iIndex=0;apDigOutList_g[iIndex];iIndex++)
  {
    iMaxControl=apDigOutList_g[iIndex]->iControl;
  }
  iNumOutputBytes_m=(iMaxControl/8)+1;

  // Blätterer für Inputs enablen/diablen
  if(iNumInputBytes_m>iBytesPerPage_m)
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_UP,ATTR_DIMMED,0);
  else
  {
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_UP,ATTR_VISIBLE,0);
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_DOWN,ATTR_VISIBLE,0);
  }

  // Blätterer für Outputs enablen/diablen
  if(iNumOutputBytes_m>iBytesPerPage_m)
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_UP,ATTR_DIMMED,0);
  else
  {
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_UP,ATTR_VISIBLE,0);
    SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_DOWN,ATTR_VISIBLE,0);
  }
} // vInitDiagnoseLabels_m()

/**
 * vUpdateDiagnoseLabels_m()
 *
 * @author uidc9239
 */
void vUpdateDiagnoseLabels_m(void)
{
  int iIndex;

  /**
   * digitale Inputs
   */
  // alle Label vorerst dimmen
  for(iIndex=PNL_DIAGIO_INP00; iIndex<iLastImputBtn_m+1; iIndex++)
  {
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_DIMMED,1);
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_LABEL_TEXT,"");
  }

  // alle aktiven Label setzten
  for(iIndex=0; apDigInpList_g[iIndex]; iIndex++)
  {
    if(apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8>iLastImputBtn_m)
      break;
    if(apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8<PNL_DIAGIO_INP00)
      continue;

    SetCtrlAttribute(panelDiagnose_m,apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8,ATTR_DIMMED,0);
    SetCtrlAttribute(panelDiagnose_m,apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8,ATTR_LABEL_TEXT,apDigInpList_g[iIndex]->pcTxt);
  }

  /**
   * digitale Outputs
   */
  // alle Label vorerst dimmen
  for(iIndex=PNL_DIAGIO_OUT00; iIndex<iLastOutputBtn_m+1; iIndex++)
  {
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_DIMMED,1);
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_LABEL_TEXT,"");
    SetCtrlVal(panelDiagnose_m,iIndex,0);
  }
  // alle aktiven Label setzten
  for(iIndex=0; apDigOutList_g[iIndex]; iIndex++)
  {
    if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8>iLastOutputBtn_m)
      break;
    if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8<PNL_DIAGIO_OUT00)
      continue;

    SetCtrlAttribute(panelDiagnose_m,apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8,ATTR_DIMMED,0);
    SetCtrlAttribute(panelDiagnose_m,apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8,ATTR_LABEL_TEXT,apDigOutList_g[iIndex]->pcTxt);
  }

  for(iIndex=0; apAInpList_g[iIndex]; iIndex++)
  {
    if(apAInpList_g[iIndex])
    {
      SetCtrlAttribute(panelDiagnose_m,apAInpList_g[iIndex]->iControl,ATTR_DIMMED,0);
      SetCtrlAttribute(panelDiagnose_m,apAInpList_g[iIndex]->iControl,ATTR_LABEL_TEXT,apAInpList_g[iIndex]->pcTxt);
    }
  }
  for(iIndex=0; apAOutList_g[iIndex]; iIndex++)
  {
    if(apAOutList_g[iIndex])
    {
      SetCtrlAttribute(panelDiagnose_m,apAOutList_g[iIndex]->iControl,ATTR_DIMMED,0);
      SetCtrlAttribute(panelDiagnose_m,apAOutList_g[iIndex]->iControl,ATTR_DIMMED,0);
      SetCtrlAttribute(panelDiagnose_m,apAOutList_g[iIndex]->iControl,ATTR_LABEL_TEXT,apAOutList_g[iIndex]->pcTxt);
    }
  }

  SetCtrlVal(panelDiagnose_m,PNL_DIAGIO_CHK_AUTO_ENABLE,(bAutoEnabled_m)?1:0);

  bPanelVisible_m=(DisplayPanel(panelDiagnose_m)>=0);
} // vUpdateDiagnoseLabels_m()


/**
 * bDiagPanel_IsVisible_g()<p>
 * Ist das Diagnose-Panel geöffnet
 *
 * @author uidc9013 (18.02.2010)
 * @return TRUE wenn das Panel geöffnet ist<BR>
 *         FALSE wenn das Panel nicht geöffnet ist
 */
BOOL bDiagPanel_IsVisible_g(void)
{
  return(bPanelVisible_m);
} // bDiagPanel_IsVisible_g()


/**
 * bDiagPanel_AutoStartEnabled_g()<p>
 * Ist Automatikablauf von Diagnose-Panel aus zugelassen
 *
 * @author uidc9013 (18.02.2010)
 * @return TRUE wenn Automatikablauf starten darf<BR>
 *         FALSE wenn Automatikablauf nicht gestartet werden darf
 */
BOOL bDiagPanel_AutoStartEnabled_g(void)
{
  if(bPanelVisible_m)
    return(bAutoEnabled_m);
  else
    return(TRUE);
} // bDiagPanel_AutoStartEnabled_g()

/**
 * vAdaptDiagPanelForTouch_m()
 *
 * Blendet die 4. Byte-Block aus und zeigt nur noch 3 Output-/Input-Bytes an.
 *
 * @author uidc9239 (23.02.2010)
 */
void vAdaptDiagPanelForTouch_m(void)
{
  int iIndex;
  int iHeight;
  int iWidth;

  GetScreenSize(&iHeight, &iWidth);

  // UP-Buttons um einen Byte-Block nach oben setzen
  SetPanelAttribute(panelDiagnose_m,ATTR_HEIGHT,460);   // TODO: für Panel über die Decoration die neue Grösse berechnen
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_UP,ATTR_TOP,436);   // TODO: Position der Scroll-Tasten mit Hilfe der Decoration neu berechnen
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_UP,ATTR_TOP,436);   // TODO: Position der Scroll-Tasten mit Hilfe der Decoration neu berechnen
  SetPanelAttribute(panelDiagnose_m,ATTR_SIZABLE,1);
  SetPanelAttribute(panelDiagnose_m,ATTR_SCALE_CONTENTS_ON_RESIZE,1);

  //4 Byte-Block ausblenden
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_TXT_INP_BYTE_3,ATTR_VISIBLE,0);
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_TXT_OUT_BYTE_3,ATTR_VISIBLE,0);
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_DECORATION_13,ATTR_VISIBLE,0);
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_DECORATION_10,ATTR_VISIBLE,0);
  for(iIndex=PNL_DIAGIO_INP30;iIndex<PNL_DIAGIO_INP37+1;iIndex++)
  {
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_VISIBLE,0);
  }
  for(iIndex=PNL_DIAGIO_OUT30;iIndex<PNL_DIAGIO_OUT37+1;iIndex++)
  {
    SetCtrlAttribute(panelDiagnose_m,iIndex,ATTR_VISIBLE,0);
  }

  //Panelgrösse an der Bildschirmauflösung anpassen
  SetPanelAttribute(panelDiagnose_m,ATTR_HEIGHT,iHeight-50);
  SetPanelAttribute(panelDiagnose_m,ATTR_WIDTH,iWidth-50);
  SetPanelAttribute(panelDiagnose_m,ATTR_TOP,25);
  SetPanelAttribute(panelDiagnose_m,ATTR_LEFT,25);
} // vAdaptDiagPanelForTouch_m()


/**
 * vInitDiagnosePanel_g()
 *
 * IODiagPanel initialisieren
 *
 * @param bTouchPanel TRUE => 3Byte per Page (TouchDisplay)
 *                    FALSE => 4Byte per Page
 *
 * @author uidc9239 (23.02.2010)
 */
void vInitDiagnosePanel_g(BOOL bTouchPanel)
{
  if(panelDiagnose_m<1)
  {
    if((panelDiagnose_m=LoadPanel(0,"IODiag.uir",PNL_DIAGIO))<0)
      return;
  }
  bTouchPanel_m=bTouchPanel;
  if(bTouchPanel_m)
  {
    iLastImputBtn_m=PNL_DIAGIO_INP27;   //3Byte à 8Bit
    iLastOutputBtn_m=PNL_DIAGIO_OUT27;  //3Byte à 8Bit
    iBytesPerPage_m=3;
    vAdaptDiagPanelForTouch_m();
  }
  else
  {
    iLastImputBtn_m=PNL_DIAGIO_INP37;   //4Byte à 8Bit
    iLastOutputBtn_m=PNL_DIAGIO_OUT37;  //4Byte à 8Bit
    iBytesPerPage_m=4;
  }
  vUpdateDiagnoseLabels_m();

  if(!bLabelsInitialized_m)
  {
    vInitDiagnoseLabels_m();
    bLabelsInitialized_m=TRUE;
  }
} // vInitDiagnosePanel_m()


/**
 * vUpdateDiagnosePanel_g()
 *
 * Anzeige-Werte von Inputs und Outputs gemäss Hardwarezustand aktuallisieren
 *
 * @author uidc9013 (18.02.2010)
 */
void vUpdateDiagnosePanel_g(void)
{
  int iIndex;

  if(!bPanelVisible_m)
  {
    // Panel nicht sichtbar --> kein Update machen
    return;
  }

  /**
   * analoge Outputs
   */
  for(iIndex=0; apDigInpList_g[iIndex]; iIndex++)
  {
    if(apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8>iLastImputBtn_m)
      break;
    if(apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8<PNL_DIAGIO_INP00)
      continue;
    SetCtrlVal(panelDiagnose_m,apDigInpList_g[iIndex]->iControl+PNL_DIAGIO_INP00-iTopInputByte_m*8,(int) *apDigInpList_g[iIndex]->IBS_Node);
  }

  /**
   * analoge Inputs
   */
  for(iIndex=0; apDigOutList_g[iIndex]; iIndex++)
  {
    if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8>iLastOutputBtn_m)
      break;
    if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8<PNL_DIAGIO_OUT00)
      continue;

    SetCtrlVal(panelDiagnose_m,apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8,(int) *apDigOutList_g[iIndex]->IBS_Node);
  }

  for(iIndex=0; apAInpList_g[iIndex]; iIndex++)
  {
    if(apAInpList_g[iIndex])
    {
      SetCtrlVal(panelDiagnose_m,apAInpList_g[iIndex]->iControl,
                 apAInpList_g[iIndex]->fConv * *apAInpList_g[iIndex]->IBS_Node);
    }
  }

  for(iIndex=0; apAOutList_g[iIndex]; iIndex++)
  {
    if(apAOutList_g[iIndex])
    {
      SetCtrlVal(panelDiagnose_m,apAOutList_g[iIndex]->iControlAW,
                 (float) *apAOutList_g[iIndex]->IBS_Node/apAOutList_g[iIndex]->fConv);
    }
  }
} // vUpdateDiagnosePanel_g()



int CVICALLBACK SwitchOutputCB(int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
  int iIndex, iValue;
  float fValue;

  switch(event)
  {
    case EVENT_COMMIT:
      // digitale Outputs schalten
      for(iIndex=0; apDigOutList_g[iIndex]; iIndex++)
      {
        if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8>iLastOutputBtn_m)
          break;
        if(apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8<PNL_DIAGIO_OUT00)
          continue;

        GetCtrlVal(panel,apDigOutList_g[iIndex]->iControl+PNL_DIAGIO_OUT00-iTopOutputByte_m*8,&iValue);
        *apDigOutList_g[iIndex]->IBS_Node=(unsigned short)iValue;
      }
      break;

    case EVENT_LEFT_CLICK:
      // analoge Outputs schalten
      if(control==PNL_DIAGIO_DAC1_SOLL)
      {
        if(eNumPanel_Open_g(panel,control,eShowLimits|eValueSelected)==eEnter)
        {
          GetCtrlVal(panel,control,&fValue);
          //vSetUbatt_g(fValue);
          //*aDAC1.IBS_Node = fValue*aDAC1.fConv;
          return(1);
        }
      }
      if(control==PNL_DIAGIO_DAC2_SOLL)
      {
        if(eNumPanel_Open_g(panel,control,eShowLimits|eValueSelected)==eEnter)
        {
          GetCtrlVal(panel,control,&fValue);
          //vSetIvcc_g(fValue);
          //*aDAC2.IBS_Node = fValue*aDAC2.fConv;
          return(1);
        }
      }
      break;
  }
  return(0);
}


int CVICALLBACK ResetAllCB(int panel, int control, int event,
                           void *callbackData, int eventData1, int eventData2)
{
  int iIndex;

  switch(event)
  {
    case EVENT_COMMIT:
      for(iIndex=0; apDigOutList_g[iIndex]; iIndex++)
      {
        *apDigOutList_g[iIndex]->IBS_Node=0;
      }
      break;
  }
  return(0);
}


int CVICALLBACK cbPanelDiag(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
  switch(event)
  {
    case EVENT_CLOSE:
      HidePanel(panel);
      bPanelVisible_m=FALSE;
      break;
    case EVENT_RIGHT_CLICK:
      if(!bTouchPanel_m)
        return(0);
      HidePanel(panel);
      bPanelVisible_m=FALSE;
      break;
  }
  return(0);
}


int CVICALLBACK AutoEnableCB(int panel, int control, int event,
                             void *callbackData, int eventData1, int eventData2)
{
  int iValue;

  switch(event)
  {
    case EVENT_COMMIT:
      GetCtrlVal(panel,control,&iValue);
      bAutoEnabled_m=(iValue==1);
      break;
  }
  return(0);
}


int CVICALLBACK InputPageFlipCB(int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2)
{
  int iByteNummer;
  int iControl;
  char caByteNummer[10];

  if(event!=EVENT_COMMIT)
    return(0);

  if(control==PNL_DIAGIO_BTN_INP_UP)    //UP
    ++iTopInputByte_m;
  if(control==PNL_DIAGIO_BTN_INP_DOWN)  //DOWN
    --iTopInputByte_m;

  iByteNummer=iTopInputByte_m;
  for(iControl=PNL_DIAGIO_TXT_INP_BYTE_0;
     iControl<PNL_DIAGIO_TXT_INP_BYTE_3+1;
     iControl++)
  {
    sprintf(caByteNummer,"%d",iByteNummer);
    SetCtrlVal(panelDiagnose_m,iControl,caByteNummer);
    iByteNummer++;
  }
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_DOWN,ATTR_DIMMED,iTopInputByte_m<1);
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_INP_UP,ATTR_DIMMED,(iTopInputByte_m>iNumInputBytes_m-(iBytesPerPage_m+1)));

  vUpdateDiagnoseLabels_m();

  return(0);
}


int CVICALLBACK OutputPageFlipCB(int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
  int iControl;
  int iByteNummer=0;
  char caByteNummer[10];

  if(event!=EVENT_COMMIT)
    return(0);

  if(control==PNL_DIAGIO_BTN_OUT_UP)    //UP
    ++iTopOutputByte_m;

  if(control==PNL_DIAGIO_BTN_OUT_DOWN)  //DOWN
    --iTopOutputByte_m;

  iByteNummer=iTopOutputByte_m;
  for(iControl=PNL_DIAGIO_TXT_OUT_BYTE_0;
     iControl<PNL_DIAGIO_TXT_OUT_BYTE_3+1;
     iControl++)
  {
    sprintf(caByteNummer,"%d",iByteNummer);
    SetCtrlVal(panelDiagnose_m,iControl,caByteNummer);
    iByteNummer++;
  }
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_DOWN,ATTR_DIMMED,iTopOutputByte_m<1);
  SetCtrlAttribute(panelDiagnose_m,PNL_DIAGIO_BTN_OUT_UP,ATTR_DIMMED,(iTopOutputByte_m>iNumOutputBytes_m-(iBytesPerPage_m+1)));

  vUpdateDiagnoseLabels_m();

  return(0);
}


/**
 * Tab-Order aller Input-Controls prüfen
 */
#if PNL_DIAGIO_INP00+1 != PNL_DIAGIO_INP01
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP01+1 != PNL_DIAGIO_INP02
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP02+1 != PNL_DIAGIO_INP03
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP03+1 != PNL_DIAGIO_INP04
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP04+1 != PNL_DIAGIO_INP05
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP05+1 != PNL_DIAGIO_INP06
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP06+1 != PNL_DIAGIO_INP07
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP07+1 != PNL_DIAGIO_INP10
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP10+1 != PNL_DIAGIO_INP11
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP11+1 != PNL_DIAGIO_INP12
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP12+1 != PNL_DIAGIO_INP13
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP13+1 != PNL_DIAGIO_INP14
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP14+1 != PNL_DIAGIO_INP15
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP15+1 != PNL_DIAGIO_INP16
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP16+1 != PNL_DIAGIO_INP17
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP17+1 != PNL_DIAGIO_INP20
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP20+1 != PNL_DIAGIO_INP21
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP21+1 != PNL_DIAGIO_INP22
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP22+1 != PNL_DIAGIO_INP23
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP23+1 != PNL_DIAGIO_INP24
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP24+1 != PNL_DIAGIO_INP25
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP25+1 != PNL_DIAGIO_INP26
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP26+1 != PNL_DIAGIO_INP27
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP27+1 != PNL_DIAGIO_INP30
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP30+1 != PNL_DIAGIO_INP31
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP31+1 != PNL_DIAGIO_INP32
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP32+1 != PNL_DIAGIO_INP33
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP33+1 != PNL_DIAGIO_INP34
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP34+1 != PNL_DIAGIO_INP35
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP35+1 != PNL_DIAGIO_INP36
  #error Tab-Order bei Input falsch
#endif
#if PNL_DIAGIO_INP36+1 != PNL_DIAGIO_INP37
  #error Tab-Order bei Input falsch
#endif


/**
 * Tab-Order aller Output-Controls prüfen
 */
#if PNL_DIAGIO_OUT00+1 != PNL_DIAGIO_OUT01
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT01+1 != PNL_DIAGIO_OUT02
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT02+1 != PNL_DIAGIO_OUT03
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT03+1 != PNL_DIAGIO_OUT04
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT04+1 != PNL_DIAGIO_OUT05
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT05+1 != PNL_DIAGIO_OUT06
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT06+1 != PNL_DIAGIO_OUT07
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT07+1 != PNL_DIAGIO_OUT10
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT10+1 != PNL_DIAGIO_OUT11
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT11+1 != PNL_DIAGIO_OUT12
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT12+1 != PNL_DIAGIO_OUT13
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT13+1 != PNL_DIAGIO_OUT14
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT14+1 != PNL_DIAGIO_OUT15
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT15+1 != PNL_DIAGIO_OUT16
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT16+1 != PNL_DIAGIO_OUT17
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT17+1 != PNL_DIAGIO_OUT20
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT20+1 != PNL_DIAGIO_OUT21
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT21+1 != PNL_DIAGIO_OUT22
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT22+1 != PNL_DIAGIO_OUT23
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT23+1 != PNL_DIAGIO_OUT24
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT24+1 != PNL_DIAGIO_OUT25
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT25+1 != PNL_DIAGIO_OUT26
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT26+1 != PNL_DIAGIO_OUT27
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT27+1 != PNL_DIAGIO_OUT30
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT30+1 != PNL_DIAGIO_OUT31
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT31+1 != PNL_DIAGIO_OUT32
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT32+1 != PNL_DIAGIO_OUT33
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT33+1 != PNL_DIAGIO_OUT34
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT34+1 != PNL_DIAGIO_OUT35
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT35+1 != PNL_DIAGIO_OUT36
  #error Tab-Order bei Output falsch
#endif
#if PNL_DIAGIO_OUT36+1 != PNL_DIAGIO_OUT37
  #error Tab-Order bei Output falsch
#endif
