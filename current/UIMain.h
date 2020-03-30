/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   Funktionskontrolle
 *
 *    FILE:        UIMain.h
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

#ifndef UIMAIN_H_INCLUDED
  #define UIMAIN_H_INCLUDED

  #include "defines.h"
  #include "devicedat.h"

  #include "LibTesterGuis.h"

// ===============================
// Konstanten für Bilddateien
// ===============================
// Bilder für Zeiger-Soll-Positionen
  #define POINTER_IMAGE_NEUTRAL  "Ptr_XXX%d.jpg"
  #define POINTER_IMAGE_FUEL     "Ptr_Fuel%d.jpg"
  #define POINTER_IMAGE_SPEED    "Ptr_Speed%d.jpg"
  #define POINTER_IMAGE_RPM      "Ptr_RPM%d.jpg"
  #define POINTER_IMAGE_BATT     "Ptr_Batt%d.jpg"

// Bilder für Ziffernblattübersicht der aktuellen Zeigerposition
  #define DIAL_IMAGE_NEUTRAL     "ZB_XXX%d.jpg"
  #define DIAL_IMAGE_FUEL        "ZB_Fuel%d.jpg"
  #define DIAL_IMAGE_SPEED       "ZB_Speed%d.jpg"
  #define DIAL_IMAGE_RPM         "ZB_RPM%d.jpg"
  #define DIAL_IMAGE_BATT        "ZB_Batt%d.jpg"

// Bilder für Anweisungen
  #define WORK_ORDER_NONE        "0.jpg"
  #define WORK_ORDER_AUTOMATIC   "Auto.jpg"
  #define WORK_ORDER_FINAL       "Final.jpg"
  #define WORK_ORDER_LCD         "LCD_%02d.jpg"
  #define WORK_ORDER_PTR         "Ptr_%02d.jpg"
  #define WORK_ORDER_EL          "PtrEL_%02d.jpg"
  #define WORK_ORDER_TT          "TT_%02d.jpg"
  #define WORK_ORDER_EASYLINK    "EasyLink.jpg"



// ===============================
// Datentypen
// ===============================
typedef struct
{
  BOOL bVisible;
  BOOL bOk;
} TagChecked;

typedef struct
{
  /**
   * Produkt-Daten
   */
  struct
  {
    BOOL bUpdated;
    char caDevNr[DEVICE_A2C_NR_LEN+1];
    char caDescription[DEVICE_DESCRIPTION_LEN+1];   // zugleich Dateiname für anzuzeigender Screen-Shot
    unsigned int uiSerialNr;
  } tagProdData;

  // Programm-Status
  struct
  {
    BOOL bUpdated;
    int  iStateMachineStep;
    char caUserText[500];
    //char caErrorText[500];
    //BOOL bHasError;
  } tagProgStatus;

  struct
  {
    BOOL bUpdated;
    char caErrorText[500+1];
    BOOL bHasError;
    int  iErrorCode;
    int  iStep;
  } tagProgError;

  struct
  {
    BOOL bUpdated;
    int  iWorkOrder;
  } tagWorkOrder;


  struct
  {
    BOOL bUpdated;
    BOOL bHasError;
    BOOL bNetworkNeeded;
  } tagNetworkError;

//@TESTER_GUI_GENERATED_STRUCT_VARS_START@
//Generated: 14:02 @ 20.03.2020
//Variables for Struct
TagGuiTest0007 *ptagGuiProdDataRead;
TagGuiTest0003 *ptagGuiCurrentIdle;
TagGuiTest0001 *ptagGuiCheckLed;
TagGuiTest0007 *ptagGuiCheckAdc;
TagGuiTest0007 *ptagGuiCheckNfc;
TagGuiTest0007 *ptagGuiProdDataWrite;
//@TESTER_GUI_GENERATED_STRUCT_VARS_END@

} tagUIElements;


/**
 * Identifikationen für anzuzeigende Anweisungen
 */
typedef enum
{
  eWoNone        =  0,

  eWoLcd01       =  1,   // LCD-Muster prüfen
  eWoLcd02       =  2,
  eWoLcd03       =  3,

  eWoPtr01       = 20,   // Zeiger auf 0-Marke
  eWoPtr02       = 21,   // Zeiger auf obere Marke
  eWoPtr03       = 22,   // Zeiger auf 0-Marke
  eWoPtr04       = 23,   // Zeiger auf obere Marke

  eWoEL01        = 30,   // Zeiger auf 0-Marke
  eWoEL02        = 31,   // Zeiger auf obere Marke

  eWoTt01        = 40,
  eWoTt02        = 41,
  eWoTt03        = 42,
  eWoTt04        = 43,

  eWoEasyLink    = 50,

  eWoFinal       = 90
} EWorkOrder;


//  ------------------------------------------------------------------------------------------------
//    Funktionsdeklaration
//  ------------------------------------------------------------------------------------------------
void vUIDiscard_g(void);

void vEnableCtrls_g(BOOL bEnable);



void vAbout_m(void);

void vInitUserInterface_g(int iPanel);
void vUpdateUserInterface_g(BOOL bForceRefresh);

void vSetStateMachineStep_g(int iStep);

BOOL bUiSetDeviceSelection_g(char *pcA2cNr);

void vUiSetDeviceDescription_g(char *pcDescription);
void vUiSetDeviceSerialNr_g(unsigned int uiSerialNr);

void vUiSetUserText_g(char *pcUserText, ...);
void vUiSetErrorText_g(int iErrorCode,
                       int iStep,
                       char *pcErrorText,...);
void vUiClearErrorText_g(void);

char *pcGetErrorText_g(void);
char *pcGetUserText_g(void);

void vUISetNetworkState_g(BOOL bAvailable, BOOL bNetworkNeeded);


//@TESTER_GUI_GENERATED_PROTOTYPES_START@
//Generated: 14:02 @ 20.03.2020
//General Functions
void vLibTesterGui_Init_g(void);
void vLibTesterGui_Discard_g(void);
void vLibTesterGui_VisualUpdate_g(void);
int iUiAll_SetState_g(EPanelStatus eState);
int iUiGuiX_SetState_g(int iCurrentState,EPanelStatus eState);

//Prototypes of ProdDataRead, GUI: 0007
int iUiProdDataRead_SetState_g(EPanelStatus eState);
void vUiProdDataRead_SetTitle_g(char *pcTitle,...);
void vUiProdDataRead_SetText_g(char *pcText,...);

//Prototypes of CurrentIdle, GUI: 0003
int iUiCurrentIdle_SetState_g(EPanelStatus eState);
void vUiCurrentIdle_SetTitle_g(char *pcTitle, ...);
void vUiCurrentIdle_SetMinMax_g(double dIMin, double dIMax);
void vUiCurrentIdle_SetCurrent_g(double dICurrent);
void vUiCurrentIdle_SetUnit_g(char *pcUnit);

//Prototypes of CheckLed, GUI: 0001
int iUiCheckLed_SetState_g(EPanelStatus eState);
BOOL bUiCheckLed_SetWorkOrder_g(char *pcWorkOrder);
void vUiCheckLed_SetTitle_g(char *pcTitle, ...);
BOOL bUiCheckLed_SetFixMode_g(EFixMode eFixMode);
int iUiCheckLed_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize);
BOOL bUiCheckLed_RemovePicture_g(int iPictureID);
BOOL bUiCheckLed_ReplacePicture_g(const char *pcPath, int iPictureID);

//Prototypes of CheckAdc, GUI: 0007
int iUiCheckAdc_SetState_g(EPanelStatus eState);
void vUiCheckAdc_SetTitle_g(char *pcTitle,...);
void vUiCheckAdc_SetText_g(char *pcText,...);

//Prototypes of CheckNfc, GUI: 0007
int iUiCheckNfc_SetState_g(EPanelStatus eState);
void vUiCheckNfc_SetTitle_g(char *pcTitle,...);
void vUiCheckNfc_SetText_g(char *pcText,...);

//Prototypes of ProdDataWrite, GUI: 0007
int iUiProdDataWrite_SetState_g(EPanelStatus eState);
void vUiProdDataWrite_SetTitle_g(char *pcTitle,...);
void vUiProdDataWrite_SetText_g(char *pcText,...);

//@TESTER_GUI_GENERATED_PROTOTYPES_END@


#endif // UIMAIN_H_INCLUDED
