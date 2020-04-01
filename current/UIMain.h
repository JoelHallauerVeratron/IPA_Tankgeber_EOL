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
//Generated: 15:48 @ 01.04.2020
//Variables for Struct
TagGuiTest0001 *ptagGuiWaitForLiquidLevelFull;
TagGuiTest0004 *ptagGuiCheckLiquidLevelFull;
TagGuiTest0001 *ptagGuiWaitForLiquidLevelHalf;
TagGuiTest0004 *ptagGuiCheckLiquidLevelHalf;
TagGuiTest0001 *ptagGuiWaitForLiquidLevelEmpty;
TagGuiTest0004 *ptagGuiCheckLiquidLevelEmpty;
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
//Generated: 15:48 @ 01.04.2020
//General Functions
void vLibTesterGui_Init_g(void);
void vLibTesterGui_Discard_g(void);
void vLibTesterGui_VisualUpdate_g(void);
int iUiAll_SetState_g(EPanelStatus eState);
int iUiGuiX_SetState_g(int iCurrentState,EPanelStatus eState);

//Prototypes of WaitForLiquidLevelFull, GUI: 0001
int iUiWaitForLiquidLevelFull_SetState_g(EPanelStatus eState);
BOOL bUiWaitForLiquidLevelFull_SetWorkOrder_g(char *pcWorkOrder);
void vUiWaitForLiquidLevelFull_SetTitle_g(char *pcTitle, ...);
BOOL bUiWaitForLiquidLevelFull_SetFixMode_g(EFixMode eFixMode);
int iUiWaitForLiquidLevelFull_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize);
BOOL bUiWaitForLiquidLevelFull_RemovePicture_g(int iPictureID);
BOOL bUiWaitForLiquidLevelFull_ReplacePicture_g(const char *pcPath, int iPictureID);

//Prototypes of CheckLiquidLevelFull, GUI: 0004
int iUiCheckLiquidLevelFull_SetState_g(EPanelStatus eState);
void vUiCheckLiquidLevelFull_SetTitle_g(char *pcTitle, ...);
void vUiCheckLiquidLevelFull_SetMinMax_g(double dIMin, double dIMax);
void vUiCheckLiquidLevelFull_SetValue_g(double dValue);
void vUiCheckLiquidLevelFull_SetUnit_g(char *pcUnit);
void vUiCheckLiquidLevelFull_SetTolerance_g(double dTolerance);
void vUiCheckLiquidLevelFull_SetSoll_g(double dSoll);
void vUiCheckLiquidLevelFull_SetPrecision_g(int iPrecision);

//Prototypes of WaitForLiquidLevelHalf, GUI: 0001
int iUiWaitForLiquidLevelHalf_SetState_g(EPanelStatus eState);
BOOL bUiWaitForLiquidLevelHalf_SetWorkOrder_g(char *pcWorkOrder);
void vUiWaitForLiquidLevelHalf_SetTitle_g(char *pcTitle, ...);
BOOL bUiWaitForLiquidLevelHalf_SetFixMode_g(EFixMode eFixMode);
int iUiWaitForLiquidLevelHalf_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize);
BOOL bUiWaitForLiquidLevelHalf_RemovePicture_g(int iPictureID);
BOOL bUiWaitForLiquidLevelHalf_ReplacePicture_g(const char *pcPath, int iPictureID);

//Prototypes of CheckLiquidLevelHalf, GUI: 0004
int iUiCheckLiquidLevelHalf_SetState_g(EPanelStatus eState);
void vUiCheckLiquidLevelHalf_SetTitle_g(char *pcTitle, ...);
void vUiCheckLiquidLevelHalf_SetMinMax_g(double dIMin, double dIMax);
void vUiCheckLiquidLevelHalf_SetValue_g(double dValue);
void vUiCheckLiquidLevelHalf_SetUnit_g(char *pcUnit);
void vUiCheckLiquidLevelHalf_SetTolerance_g(double dTolerance);
void vUiCheckLiquidLevelHalf_SetSoll_g(double dSoll);
void vUiCheckLiquidLevelHalf_SetPrecision_g(int iPrecision);

//Prototypes of WaitForLiquidLevelEmpty, GUI: 0001
int iUiWaitForLiquidLevelEmpty_SetState_g(EPanelStatus eState);
BOOL bUiWaitForLiquidLevelEmpty_SetWorkOrder_g(char *pcWorkOrder);
void vUiWaitForLiquidLevelEmpty_SetTitle_g(char *pcTitle, ...);
BOOL bUiWaitForLiquidLevelEmpty_SetFixMode_g(EFixMode eFixMode);
int iUiWaitForLiquidLevelEmpty_InsertPicture_g(const char *pcPath, TagPictureSize *ptagImagesize);
BOOL bUiWaitForLiquidLevelEmpty_RemovePicture_g(int iPictureID);
BOOL bUiWaitForLiquidLevelEmpty_ReplacePicture_g(const char *pcPath, int iPictureID);

//Prototypes of CheckLiquidLevelEmpty, GUI: 0004
int iUiCheckLiquidLevelEmpty_SetState_g(EPanelStatus eState);
void vUiCheckLiquidLevelEmpty_SetTitle_g(char *pcTitle, ...);
void vUiCheckLiquidLevelEmpty_SetMinMax_g(double dIMin, double dIMax);
void vUiCheckLiquidLevelEmpty_SetValue_g(double dValue);
void vUiCheckLiquidLevelEmpty_SetUnit_g(char *pcUnit);
void vUiCheckLiquidLevelEmpty_SetTolerance_g(double dTolerance);
void vUiCheckLiquidLevelEmpty_SetSoll_g(double dSoll);
void vUiCheckLiquidLevelEmpty_SetPrecision_g(int iPrecision);

//@TESTER_GUI_GENERATED_PROTOTYPES_END@


#endif // UIMAIN_H_INCLUDED
