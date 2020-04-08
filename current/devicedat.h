/*******************************************************************
 *
 *    PROJECT:     Tankfüllstandsgeber EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        devicedat.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      HaJ
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef DEVICEDAT_H_INCLUDED
  #define DEVICEDAT_H_INCLUDED

  #include "defines.h"
  //#include "roi.h"
  #include <windows.h>

  #define DEVICE_DAT_SELECT_PROMPT "Typ auswählen"



  // Daten-Längen
  #define DEVICE_A2C_NR_LEN               20
  #define DEVICE_DESCRIPTION_LEN         100
  #define DEVICE_PROD_NR_CUSTOMER_LEN     20
  #define DEVICE_SW_VERSION_LEN           12
  #define DEVICE_LABEL_TEXT_LEN           50
  #define DEVICE_LABEL_DATE_FORMAT_LEN    20
  #define DEVICE_HW_VERSION_LEN           12
  //#define DEVICE_CAL_FILE_LEN            100
  #define DEVICE_DIG_INPUT_CONFIG_LEN     26

  #define DEVICE_NUM_ANALOG_INPUTS         2   // max. Anzahl analog Inputs
  // 4=ANI5 R-Pin8
  // 5=ANI6 R-Pin9

//  ------------------------------------------------------------------------------------------------
//    Typdefinition
//  ------------------------------------------------------------------------------------------------
typedef enum
{
  eDigInputConfig_InActive = 0,
  eDigInputConfig_LoActive = 'L',
  eDigInputConfig_HiActive = 'H'
}EDigInputConfig;

typedef enum
{
  eGaugeType_Tacho          =  1,
  eGaugeType_Speedo         =  2,
  eGaugeType_AppaWindAngle  =  3,
  eGaugeType_TrueWindAngle  =  4,
  eGaugeType_DepthBelowTD   =  5,
  eGaugeType_DepthBelowKeel =  6,
  eGaugeType_AppaWindSpeed  =  7,
  eGaugeType_TrueWindSpeed  =  8,
  eGaugeType_MagnHeading    =  9,
  eGaugeType_TrueHeading    = 10,
  eGaugeType_WindAngleMagn  = 11
}EGaugeType;

typedef enum
{
  eLabelType_OlGps    =6,   // B000351 GPS only
  eLabelType_OLBt     =7,   // B000352 BLE only
  eLabelType_OlGpsBt  =8,   // B000353 GPS+BLE
  eLabelType_LinkUp   =9,   //
}ELabelType;

typedef struct
{
  UINT uiNMEACode;
  ELabelType eLabelType;
  char caLabelText[DEVICE_LABEL_TEXT_LEN+1];
  char caLabelDateFormat[DEVICE_LABEL_DATE_FORMAT_LEN+1];
  char caSampleText[DEVICE_LABEL_TEXT_LEN+1];
}TagProdData;

typedef enum
{
  eWaterType_Fresh,
  eWaterType_Wasted,
  //eSignalType_Kline,
}EWaterType;

typedef struct
{
  char           caDescription[DEVICE_DESCRIPTION_LEN+1];
  char           caProdNrUnpacked[DEVICE_A2C_NR_LEN+1];
  UINT           uiLibDutComNr;
  char           caSwVersion[DEVICE_SW_VERSION_LEN+1];
  BOOL           bCheckSwVersion;

  float          fBoottime;
  float          fUbatt;   // [V]
  int            iIvccIdleMin;   // [mA]
  int            iIvccIdleMax;   // [mA]

  BOOL           bSinkMode;
  BOOL           bSourceMode;
  BOOL           bAnalogSignal;
  int            iStecker;
  EWaterType     eWaterType;
  TagProdData    tagProdData;
} TagDevice;




//  ------------------------------------------------------------------------------------------------
//    Gerätedaten
//  ------------------------------------------------------------------------------------------------
extern  TagDevice tagCurrentDeviceTyp_g;

//  ------------------------------------------------------------------------------------------------
//    Funktionsdeklaration
//  ------------------------------------------------------------------------------------------------
extern TagDevice *ptagGetDeviceFromMenuItem_g(int  iMenuItem);

extern BOOL bIniFileDevice_SetFile_g(char *pcFilePath);
extern BOOL bIniFileDevice_FillSelectorCtrl_g(int iPanel, int iRingControl, BOOL bWithPrompt);

extern BOOL bIniFileDevice_ReadDataValid_g(void);
extern BOOL bIniFileDevice_ReadData_g(char *pcDeviceNr);

extern void vIniFileDevices_Dump_g(TagDevice *ptagDeviceTyp);

#endif // DEVICEDAT_H_INCLUDED

