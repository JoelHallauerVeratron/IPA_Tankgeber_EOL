/*******************************************************************
 *
 *    PROJECT:     LinkUp
 *
 *    EQUIPMENT:   Label Printer
 *
 *    FILE:        LabelPrt09.c
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

#include <userint.h>
#include <ansi_c.h>
#include "global.h"
#include "LabelPrt09.h"

#include "LibIntermecPrinter.h"


int iPanelLabelPrint09_m=-1;

/**
 * Öffnet Drucker-Panel
 *
 * @author uidc9013 (26.09.2012)
 *
 * @param panelParent
 */
void vLabelPrintPanel09_Open_g(int panelParent)
{
  int iPanel=0;

  if(iPanelLabelPrint09_m<0)
  {
    if((iPanel=LoadPanel(0,"LabelPrt09.uir",PNL_LPRT09))<0)
    {
      printf("LoadPanel(%s) failed\n","LabelPrt09.uir");
      return;
    }
  }

  DisplayPanel(iPanel);

  iPanelLabelPrint09_m=iPanel;
} // vLabelPrintPanel09_Open_g()


/**
 * Schliesst Drucker-Panel
 *
 * @author uidc9013 (26.09.2012)
 *
 * @param panelParent
 */
void vLabelPrintPanel09_Close_g(void)
{
  if(iPanelLabelPrint09_m<0)
    return;

  DiscardPanel(iPanelLabelPrint09_m);
  iPanelLabelPrint09_m=-1;
} // vLabelPrintPanel09_Close_g()


int CVICALLBACK TmrStatus09CB(int panel,int control,int event,
                              void *callbackData,int eventData1,int eventData2)
{
  if(event!=EVENT_TIMER_TICK)
    return(0);

  if(bIntermecPrinter_IsConnected_g())
  {
    // connected
    SetCtrlVal(panel,PNL_LPRT09_LED_CONNECTED,1);
    SetCtrlAttribute(panel,PNL_LPRT09_BTN_PRINT,ATTR_DIMMED,0);
  }
  else
  {
    // not connected
    SetCtrlVal(panel,PNL_LPRT09_LED_CONNECTED,0);
    SetCtrlAttribute(panel,PNL_LPRT09_BTN_PRINT,ATTR_DIMMED,1);
  }

  return(0);
}


int CVICALLBACK BtnPrint09CB(int panel,int control,int event,
                             void *callbackData,int eventData1,int eventData2)
{
  char   caDescription[30];
  char   caRhiNr[20];
  char   caSerialNr[15];
  char   caDate[20];
  char  *pcDate;

  if(event!=EVENT_COMMIT)
    return(0);

  if(!bIntermecPrinter_IsConnected_g())
  {
    MessagePopup("Fehler","Printer not connected\n");
    return(0);
  }

  GetCtrlVal(panel,PNL_LPRT09_TXT_DESCR,caDescription);
  GetCtrlVal(panel,PNL_LPRT09_TXT_RHI_NR,caRhiNr);
  GetCtrlVal(panel,PNL_LPRT09_TXT_DATE,caDate);
  GetCtrlVal(panel,PNL_LPRT09_TXT_SNR,caSerialNr);

  if(strchr(caDate,'%')!=NULL)
  {
    // Format für strftime angegeben --> Format auflösen
    pcDate=pcCurrentDateFormated_g(caDate);
  }
  else
  {
    // konstantes Datum angegeben
    pcDate=caDate;
  }

  if(!bIntermecPrinterPrintLabel_g(9,               // UINT   uiLabelLayout,    // Etiketten-Layout
                                   1,               // UINT   uiLabelCount
                                   caRhiNr,         // char  *pcRhiNrUnpacked,  // RHI A2C-Nummer "A2C1234567890"
                                   "",              // char  *pcRhiNrPacked,    // RHI A2C-Nummer "A2C1234567890"
                                   "",              // char  *pcCustomerNr,     // Kundennummer "17340110000"
                                   caDescription,   // char  *pcDescription,    // Artikelbezeichnung
                                   caSerialNr,      // char  *pcSerialNr,       // SerieNummer
                                   "",              // char  *pcHwVersion,      // Hardware Version
                                   "",              // char  *pcSwVersion,      // Software Version
                                   pcDate,          // char  *pcDate,           // Datum
                                   "",              // char  *pcFreeText0,
                                   "",              // char  *pcFreeText1,
                                   "",              // char  *pcFreeText2,
                                   "",              // char  *pcFreeText3,
                                   "",              // char  *pcFreeText4,
                                   "",              // char  *pcFreeText5,
                                   "",              // char  *pcFreeText6,
                                   "",              // char  *pcFreeText7,
                                   "",              // char  *pcFreeText8,
                                   "")              // char  *pcFreeText9)
    )
  {
    MessagePopup("Fehler","bIntermecPrinterPrintLabel_g() failed\n");
  }

  return(0);
}


int CVICALLBACK BtnClose09CB(int panel,int control,int event,
                             void *callbackData,int eventData1,int eventData2)
{
  switch(event)
  {
    case EVENT_COMMIT:
      vLabelPrintPanel09_Close_g();
      break;
  }
  return(0);
}


int CVICALLBACK NumLabel09CB(int panel,int control,int event,
                             void *callbackData,int eventData1,int eventData2)
{
  switch(event)
  {
    case EVENT_COMMIT:
      break;
  }
  return(0);
}
