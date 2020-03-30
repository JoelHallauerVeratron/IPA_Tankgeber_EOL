/*******************************************************************
 *
 *    PROJECT:     Libraries
 *
 *    EQUIPMENT:   IntermecPrinter
 *
 *    FILE:        IntermecPrinter.h
 *
 *    DESCRIPTION: Interface zu Intermec-Druckern über Telnet/TCP
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

#ifndef INTERMEC_PRINTER_H_INCLUDED
  #define INTERMEC_PRINTER_H_INCLUDED

  #include <windows.h>
  #include <ansi_c.h>

extern BOOL bPrinterLabelDefined_m;
extern BOOL bPrinterDataValid_m;


BOOL bIntermecPrinter_Connect_g(char *pcIpAddress);
void vIntermecPrinter_Disconnect_g(void);

void vIntermecPrinter_InvalidateData_g(void);

BOOL bIntermecPrinter_ShowMenu_g(int iParentPanel);
void vIntermecPrinter_HideMenu_g(void);

BOOL bIntermecPrinterPrintLabel_g(UINT   uiLabelLayout,    // Etiketten-Layout
                                  UINT   uiLabelCount,     // Anzahl zu druckende Etiketten
                                  char  *pcRhiNrUnpacked,  // RHI A2C-Nummer "A2C1234567890"
                                  char  *pcRhiNrPacked,    // RHI A2C-Nummer "A2C1234567890"
                                  char  *pcCustomerNr,     // Kundennummer "17340110000"
                                  char  *pcDescription,    // Artikelbezeichnung
                                  char  *pcSerialNr,       // SerieNummer
                                  char  *pcHwVersion,      // Hardware Version
                                  char  *pcSwVersion,      // Software Version
                                  char  *pcDate,           // Datum
                                  char  *pcFreeText0,
                                  char  *pcFreeText1,
                                  char  *pcFreeText2,
                                  char  *pcFreeText3,
                                  char  *pcFreeText4,
                                  char  *pcFreeText5,
                                  char  *pcFreeText6,
                                  char  *pcFreeText7,
                                  char  *pcFreeText8,
                                  char  *pcFreeText9);

BOOL bIntermecPrinter_IsConnected_g(void);
BOOL bIntermecPrinter_IsOnline_g(void);
BOOL bIntermecPrinter_AllOk_g(void);
BOOL bIntermecPrinter_PrintHeadOpen_g(void);
BOOL bIntermecPrinter_LabelToPick_g(void);
BOOL bIntermecPrinter_NoLabel_g(void);
BOOL bIntermecPrinter_NoColorBand_g(void);
BOOL bIntermecPrinter_HighTension_g(void);
BOOL bIntermecPrinter_IsFeeding_g(void);

#endif // INTERMEC_PRINTER_H_INCLUDED

