/*******************************************************************
 *
 *    PROJECT:     FlecCluster
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        UDS.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author: WeU $
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date: 2013-03-13 10:27:14 +0100 (Mi, 13 Mrz 2013) $
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef UDS_H_INCLUDED
  #define UDS_H_INCLUDED

  #include <windows.h>
  #ifdef _CVI_
    #include <cvirte.h>
  #else // !_CVI_
    #include <stdlib.h>
    #include <stdio.h>
  #endif // !_CVI_


  #define ECU_CAN_ADDRESS 0x560   // 0x560 / 0x7C2

//#define UDS_RETURN_ANALOG_INPUT_CALCULATED       // Analog-Input berechnet zur�ckgeben

  #define UDS_RETURN_DUT_CURRENT_CALCULATED        // Strom berechnet zur�ckgeben

  #define UDS_RETURN_ALTERNATOR_INPUT_CALCULATED   // Alternator berechnet zur�ckgeben



typedef enum
{
  eUdsDiagModeExtended = 0x03,
  eUdsDiagModeStandard = 0x61,   // StandardDiagnosticMode
  eUdsDiagModeVmsEol   = 0x83,   // EndOfLineVehicleManufacturerMode
  eUdsDiagModeSssEol   = 0x63,   // EndOfLineSystemSupplierMode
  eUdsDiagModeEcuProg  = 0x85,   // ECUProgrammingMode
  eUdsDiagModeDevelop  = 0x86,   // DevelopementMode
  eUdsDiagModeCvTest   = 0xF0    // Manufacturing tests (CV) active
} EUdsDiagMode;

typedef enum
{
  eUdsSecurityAccessDefault = 0x0,    // AccessModeDefault
  eUdsSecurityAccessVmsEol = 0x1,    // AccessModeVehicleManufacturerSpecificEndOfLine
  eUdsSecurityAccessVmsProg = 0x3,    // AccessModeVehicleManufacturerSpecificProgramming
  eUdsSecurityAccessVmsDev = 0x5,    // AccessModeVehicleManufacturerSpecificDevelopment
  eUdsSecurityAccessSssEol = 0x63,   // AccessSystemSupplierSpecificEndOfLine
  eUdsSecurityAccessSssDev = 0x65    // AccessSystemSupplierSpecificDevelopment
} EUdsSecurityAccess;

typedef enum
{
  eIoCtrlModeReturnControlToEcu = 0x00,
  eIoCtrlModeResetToDefault = 0x01,
  eIoCtrlModeFreezeCurrentState = 0x02,
  eIoCtrlModeShortTermAdjust = 0x03
} EIoCtrlMode;


BOOL bUds_TraceEnable_g(BOOL bEnable);

  #define LOG_HEADER_UDS "  Time    <ID>  0  1  2  3  4  5  6  7"
//                       |999.99 Tx 18ff0100 bb 99 07 ff 01 ff ff ff   enable UDS
//                       |  1.58 Tx 0560 05 2e 01 56 00 64         write data ID 0x156
//                       |  0.02 Rx 07c2 03 6e 01 56 00 00 00 00




BOOL bUds_Init_g(void);
void vUds_Close_g(void);

BOOL bUds_IsInitialized_g(void);

void vUds_SetBaudrate_g(int iCanBus,
                        int iBaudrate);

void vNmea2k_EnableUds_g(BOOL bEnable);


// SID 0x81
BOOL *pbUds_StartCommunication_g(void);
// SID 0x82
BOOL *pbUds_StopCommunication_g(void);

// SID 0x3E
void vUds_SendTesterPresent_g(void);
void vUds_EnableTesterPresent_g(BOOL bEnable);

// SID 0x11
BOOL *pbUds_ResetDevice_g(void);

// SID 0x10
BOOL *pbUds_StartDiagSession_g(EUdsDiagMode eDiagMode);
// SID 0x20
BOOL *pbUds_StopDiagSession_g(void);

// SID 0x27#1
BOOL *pbUds_SecurityAccessReqSeed_g(EUdsSecurityAccess eSecurityAccess);
// SID 0x27#2
BOOL *pbUds_SecurityAccessSendKey_g(void);

// SID 0x2E
BOOL *pbUds_WriteDataByIdentifier_g(unsigned int uiIdentifier,
                                    unsigned char *pucData,
                                    int iBytes);

// SID 0x22
BOOL *pbUds_RequestDataByIdentifier_g(unsigned int uiIdentifier);

// SID 0x23
BOOL *pbUds_ReadMemoryByAddress_g(unsigned long ulAddress,
                                  int iBytes);

BOOL *pbUds_RequestSerialNumber_g(void);
BOOL *pbUds_RequestCalibration_g(void);
BOOL *pbUds_RequestDataString_g(ULONG ulAddress,
                                UINT uiBytes);

BOOL *pbUds_RequestProductInterlock_g(int iTool);

// Datenzugriff auf mit 0x23 empfangene Daten
//BOOL bUds_GetMemoryByAddress_g(unsigned long ulAddress,
//                               unsigned char *pucData,
//                               int           iBytes);
BOOL bUds_GetSerialNumber_g(unsigned int *puiSerialNumber);
BOOL bUds_GetCalibration_g(UCHAR *pucCalibration);
BOOL bUds_GetDataString_g(unsigned int uiAddress,
                          unsigned char *pucDataString,
                          unsigned int *puiSize);
//BOOL bUds_GetPointerOffsets_g(unsigned char *pucPointerOffsets);


BOOL bUds_GetProductInterlockData_g(unsigned int uiTool,
                                    unsigned int *puiVersionMajor,
                                    unsigned int *puiVersionMinor,
                                    unsigned int *puiRunCounter,
                                    unsigned int *puiYear,
                                    unsigned int *puiMonth,
                                    unsigned int *puiDay,
                                    unsigned int *puiHour,
                                    unsigned int *puiMinute,
                                    unsigned int *puiSecond);

// SID 0x3D
BOOL *pbUds_WriteMemoryByAddress_g(unsigned long ulAddress,
                                   unsigned char *pucData,
                                   int iBytes);
/**
 * Wrapper-Funktionen f�r SID 0x3D
 */
BOOL *pbUds_StepperMotorSetOffset_g(unsigned int uiOffset);

BOOL *pbUds_WriteSerialNumber_g(unsigned int uiSerialNumber);

BOOL *pbUds_WriteDialScaleValues_g(UINT uiXScaleValue,
                                   UINT uiYScaleValue);
BOOL *pbUds_WriteGaugeType_g(UINT uiGaugeType);
BOOL *pbUds_WriteCalibration_g(unsigned char *pucCalibration);
BOOL *pbUds_WriteDataString_g(ULONG ulAddress,
                              char *pcDataString,
                              UINT uiStrlen);

BOOL *pbUds_SetProductInterlock_g(int iTool,
                                  int iVersionMajor,
                                  int iVersionMinor,
                                  int iCounter);


void vUds_IoCtrlInfoDump_g(void);



// SID 0x2F
BOOL *pbUds_IoCtrlRequestDigitalInput_g(void);
BOOL *pbUds_IoCtrlRequestAnalogInputs_g(void);

BOOL *pbUds_IoCtrlRequestFrequencyInput_g(void);

BOOL *pbUds_IoCtrlRequestStepperMotorState_g(void);


BOOL bUds_IoCtrlGetDigitalInput_g(UCHAR ucInput,
                                  BOOL *bInputState);
BOOL bUds_IoCtrlGetAnalogInput_g(unsigned char ucInput,
                                 unsigned int *puiAnalogInput);

BOOL bUds_IoCtrlGetGpsSnr_g(UINT *puiGpsSnr);
BOOL bUds_IoCtrlGetStepperMotorState_g(unsigned int *puiAnalogInput);


BOOL *pbUds_IoCtrlSetTTBitmap_g(unsigned long ulBitmap);

//BOOL *pbUds_IoCtrlStepperMotorSetPosition_g(int iPosition,
//                                            int iPositionEasyLink);

//BOOL *pbUds_IoCtrlStepperMotor360SetPosition_g(int iPosition,
//                                               int iPositionEasyLink);

//BOOL *pbUds_IoCtrlSetBacklight_g(BOOL bPointer,
//                                 BOOL bDial,
//                                 BOOL bLcd,
//                                 BOOL bEasyLink);

BOOL *pbUds_IoCtrlSetEasyLink_g(BOOL bActivate);
BOOL *pbUds_IoCtrlSetDigOutput_g(unsigned long ulBitmap);

BOOL *pbUds_RequestLocalTime_g(void);
BOOL bUds_GetLocalTime_g(unsigned char *pucTime);



// SID 0x14
BOOL *pbUds_ClearDtcInformation_g(unsigned int iSpn,
                                  unsigned int iFmi);


/**
 * Wrapper-Funktionen f�r startRoutineByLocId SID 0x31 und 0x32
 */
//BOOL *pbUds_IoCtrlLcdSetPattern_g(unsigned int uiLcdPattern);




#endif // !UDS_H_INCLUDED

