/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   0000
 *
 *    FILE:        LibDutComInterface.h
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
/*******************************************************************
 *
 * This file must not be modified in any project other than LibDutCom_0000!!!
 *
 *
 * Make modifications only in the root project LibDutCom_0000!!!
 *
 *******************************************************************/

#ifndef LIB_DUT_COM_INTERFACE_H_INCLUDED
  #define LIB_DUT_COM_INTERFACE_H_INCLUDED

#define DATA_STRING_LEN            500   // max. length for DataString
#define DUT_COM_ERROR_MESSAGE_LEN  500   // max. length for error message


#define DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX  5   // max. dig. Inputs für Dev.specific Tests
#define DUT_COM_DEV_SPEC_TEST_DIG_OUTPUT_MAX  5  // max. dig. Outputs für Dev.specific Tests


typedef struct
{
  struct
  {
    UINT uiPort;
    UINT uiBaudrate;
  }tagRs232;
  struct
  {
    UINT uiPort;
    UINT uiBaudrate;
    UINT uiCardSerialNumber;
    ULONG ulTxId;
    ULONG ulRxId;
  }tagKvaserCan;
}TagLibDutComParam;


typedef enum
{
  eDiagModeStandard    = 0x01,   // StandardDiagnosticMode
  eDiagModeProgSession = 0x02,   //
  eDiagModeExtended    = 0x03,   // ExtendedDiagnosticMode
  eDiagModeSssEol      = 0x63,   // EndOfLineSystemSupplierMode
  eDiagModeVmsEol      = 0x83,   // EndOfLineVehicleManufacturerMode
  eDiagModeEcuProg     = 0x85,   // ECUProgrammingMode
  eDiagModeDevelop     = 0x86,   // DevelopementMode
  eDiagModeCvTest      = 0xF0    // Manufacturing tests (CV) active
} EDiagMode;


typedef enum
{
  eSecurityAccessDefault = 0,   // AccessModeDefault
  eSecurityAccessVmsEol  = 1,   // AccessModeVehicleManufacturerSpecificEndOfLine
  eSecurityAccessVmsProg = 3,   // AccessModeVehicleManufacturerSpecificProgramming
  eSecurityAccessVmsDev  = 5,   // AccessModeVehicleManufacturerSpecificDevelopment
  eSecurityAccessSssEol  = 7,   // AccessSystemSupplierSpecificEndOfLine
  eSecurityAccessSssDev  = 9    // AccessSystemSupplierSpecificDevelopment
} ESecurityAccess;

typedef enum
{
  eRgb_None   = 0,
  eRgb_Red    = 1,
  eRgb_Green  = 2,
  //eRgb_???    = 3,
  eRgb_Blue   = 4,
  //eRgb_???    = 5,
  //eRgb_???    = 6,
  eRgb_White  = 7
} ERgbColors;

typedef enum
{
  eLcdPatternA = 0,
  eLcdPatternB = 1,
  eLcdPatternC = 2,
  eLcdPatternD = 3,
  eLcdPatternE = 4,
  eLcdPatternF = 5,
  eLcdPatternG = 6,
  eLcdPatternH = 7,
  eLcdPatternI = 8,
  eLcdPatternJ = 9,
  eLcdPatternAll = 99   // war 5
}ELcdPattern;


typedef enum
{
  eProdNrUnpacked    = 0,
  eProdNrPacked      = 1,
  eProdNrCustomer    = 2,
  eProdNrPcb         = 3,
  eProgDatePcb       = 4,
  eHardwareVersion   = 5,
  eSoftwareVersion   = 6,
  eDigInputConfig    = 7,
  eFreqOutputConfig  = 8,
  eCheckSumBm        = 9,   // CRC Boot-Manager
  eCheckSumFl        = 10,  // CRC Flash-Loader
  eCheckSumSw        = 11,  // CRC Software
  eCheckSumBhmi      = 12,  // CRC BHMI
  eBluetoothVersion  = 13,
  eDialScaleX        = 14,
  eDialScaleY        = 15,
  eGaugeType         = 16,
  eProductCode       = 17,

}EDataType;


typedef enum
{
  eDataFormat_Unspecified = 0,
  eDataFormat_Ascii       = 1,   // 0-terminierter String mit Text
  eDataFormat_Bcd         = 2,   // 0-terminierter String mit BCD. Bsp.: "00" entspricht Zahlwert 0
  eDataFormat_Binary      = 3    // binär-Wert mit Grösse von uiDevSpec_GetDataSize_m(): 1=>char, 2=word, 4=dword
}EDataFormat;



#endif // LIB_DUT_COM_INTERFACE_H_INCLUDED

