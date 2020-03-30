/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   XXXX: LibDutCom
 *
 *    FILE:        LibDutCom.h
 *
 *    DESCRIPTION: exported functions
 *
 *    AUTHOR:      uidc9013
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        15.12.2011
 *
 *    HISTORY:
 *      -15.12.2011: initial implementation
 *      -
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

#ifndef LIB_DUT_COM_H_INCLUDED
  #define LIB_DUT_COM_H_INCLUDED

  #include <windows.h>

  #include "LibDutCom_Interface.h"
  #include "SvnIds.h"



  #ifdef DLL_EXPORTS
    #define MYDLL_API __declspec(dllexport)
  #else
    #define MYDLL_API __declspec(dllimport)
  #endif

#ifndef _CVI_
  #undef WINAPI
  #define WINAPI
#endif // !_CVI_

/**
 * Lib Info
 */
MYDLL_API BOOL WINAPI       bDutCom_SetParam(TagLibDutComParam *ptagParam);

MYDLL_API void WINAPI       vDutCom_GetInfo(char *pcDutInfo,
                                            UINT *puiDutId,
                                            char *pcVersion,
                                            char *pcBuild,
                                            char *pcCapabilities);

MYDLL_API int  WINAPI       iDutCom_GetOneTraceLine(char *pcTraceLine,
                                                    int *piLinesRemaining);
MYDLL_API char WINAPI      *pcDutCom_GetTraceHeader(void);
MYDLL_API void WINAPI       vDutCom_TraceEnable(BOOL bEnable);
MYDLL_API void WINAPI       vDutCom_TraceClear(void);

MYDLL_API BOOL WINAPI       bDutCom_GetErrorMessage(char  *pcErrorMessage,
                                                    UINT   uiBufferSize);

/**
 *
 * Dut Management
 *
 */
/**
 * Communication
 */
MYDLL_API void WINAPI       vDutCom_CommunicationStart(void);
MYDLL_API void WINAPI       vDutCom_CommunicationStop(void);

MYDLL_API BOOL WINAPI       bDutCom_CommunicationStarted(void);
MYDLL_API BOOL WINAPI       bDutCom_CommunicationStopped(void);


/**
 * Diagnostic mode
 */
MYDLL_API void WINAPI       vDutCom_DiagModeStart(void);
MYDLL_API void WINAPI       vDutCom_DiagModeStop(void);

MYDLL_API BOOL WINAPI       bDutCom_DiagModeStarted(void);
MYDLL_API BOOL WINAPI       bDutCom_DiagModeStopped(void);

MYDLL_API void WINAPI       vDutCom_DiagModeSet(EDiagMode eDiagMode);
MYDLL_API BOOL WINAPI       bDutCom_DiagModeIsSet(void);

/**
 * Security access
 */
MYDLL_API void WINAPI       vDutCom_SecurityAccess(ESecurityAccess eSecurityAccess);
MYDLL_API ESecurityAccess WINAPI eDutCom_SecurityGetCurrentAccess(void);


MYDLL_API void WINAPI       vDutCom_Reset(void);

/**
 * Clock access
 */
MYDLL_API void WINAPI       vDutCom_ClockReset(void);
MYDLL_API void WINAPI       vDutCom_ClockSetSeconds(UINT uiSeconds);
MYDLL_API void WINAPI       vDutCom_ClockSetMinutes(UINT uiMinutes);
MYDLL_API void WINAPI       vDutCom_ClockSetHours(UINT uiHours);
MYDLL_API void WINAPI       vDutCom_ClockSetTime(UINT uiHours,
                                                 UINT uiMinutes,
                                                 UINT uiSeconds);
MYDLL_API BOOL WINAPI       bDutCom_ClockSetTimeDone(void);
MYDLL_API void WINAPI       vDutCom_ClockRequestTime(void);
MYDLL_API UINT WINAPI       uiDutCom_ClockGetSeconds(void);
MYDLL_API UINT WINAPI       uiDutCom_ClockGetMinutes(void);
MYDLL_API UINT WINAPI       uiDutCom_ClockGetHours(void);
MYDLL_API BOOL WINAPI       bDutCom_ClockGetTime(UINT *puiHours,
                                                 UINT *puiMinutes,
                                                 UINT *puiSeconds);


/**
 *
 * static data
 *
 */
/**
 * static data: byte wise access
 */
MYDLL_API void WINAPI       vDutCom_DataEepromByteWrite(UINT uiAddress,
                                                        UCHAR ucData);
MYDLL_API BOOL WINAPI       bDutCom_DataEepromByteWritten(void);

MYDLL_API void WINAPI       vDutCom_DataEepromByteRequest(UINT uiAddress);
MYDLL_API BOOL WINAPI       bDutCom_DataEepromByteRead(UCHAR *pucData);

/**
 * static data: bulk wise access
 */
MYDLL_API void WINAPI       vDutCom_DataEepromBulkWrite(UINT  uiAddress,
                                                        UCHAR *pucData,
                                                        UINT  uiBytes);
MYDLL_API BOOL WINAPI       bDutCom_DataEepromBulkWritten(void);

MYDLL_API void WINAPI       vDutCom_DataEepromBulkRequest(UINT uiAddress,
                                                          UINT uiBytes);
MYDLL_API BOOL WINAPI       bDutCom_DataEepromBulkRead(UINT  uiAddress,
                                                       UCHAR *pucData,
                                                       UINT  uiBytes);


/**
 * static data: serial number access
 */
MYDLL_API void WINAPI       vDutCom_DataSerialNumberWrite(UINT uiSerialNumber);
MYDLL_API BOOL WINAPI       bDutCom_DataSerialNumberWritten(void);

MYDLL_API void WINAPI       vDutCom_DataSerialNumberRequest(void);
MYDLL_API BOOL WINAPI       bDutCom_DataSerialNumberRead(UINT *puiSerialNumber);


/**
 * static data: product data access
 */
MYDLL_API void WINAPI       vDutCom_DataStringWrite(EDataType  eDataType,
                                                    char      *pcDataString);
MYDLL_API BOOL WINAPI       bDutCom_DataStringWritten(void);

MYDLL_API void WINAPI       vDutCom_DataStringRequest(EDataType eDataType);
MYDLL_API BOOL WINAPI       bDutCom_DataStringRead(char *pcDataString);
MYDLL_API void WINAPI       vDutCom_DataStringGetDataInfo(EDataType    eDataType,
                                                          EDataFormat  *peDataFormat,
                                                          UINT         *puiDataSizeBytes);


/**
 * static data: production interlock access
 */
MYDLL_API UINT WINAPI       uiDutCom_DataProductionInterlockGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_DataProductionInterlockGetText(UINT uiTool);
MYDLL_API void WINAPI       vDutCom_DataProductionInterlockWrite(UINT uiToolNumber,
                                                                 UINT uiToolVersionMajor,
                                                                 UINT uiToolVersionMinor,
                                                                 UINT uiRunCounter);
MYDLL_API BOOL WINAPI       bDutCom_DataProductionInterlockWritten(void);

MYDLL_API void WINAPI       vDutCom_DataProductionInterlockRequest(UINT uiToolNumber);
MYDLL_API BOOL WINAPI       bDutCom_DataProductionInterlockRead(UINT *puiVersionMajor,
                                                                UINT *puiVersionMinor,
                                                                UINT *puiRunCounter,
                                                                UINT *puiYear,
                                                                UINT *puiMonth,
                                                                UINT *puiDay,
                                                                UINT *puiHours,
                                                                UINT *puiMinutes,
                                                                UINT *puiSeconds);


/**
 * pointer info
 */
MYDLL_API UINT WINAPI       uiDutCom_PointerGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_PointerGetText(UINT uiPointer);

/**
 * pointer adjust
 */
MYDLL_API void WINAPI       vDutCom_PointerAdjustInit(void);
MYDLL_API BOOL WINAPI       bDutCom_PointerAdjustInitialized(void);

MYDLL_API void WINAPI       vDutCom_PointerAdjustSetOffset(UINT uiPointer,
                                                           UINT uiOffset);
MYDLL_API BOOL WINAPI       bDutCom_PointerAdjustOffsetIsSet(void);

MYDLL_API void WINAPI       vDutCom_PointerAdjustSetGain(UINT uiPointer,
                                                         UINT uiPosition,
                                                         float fGain);
MYDLL_API BOOL WINAPI       bDutCom_PointerAdjustGainIsSet(void);

MYDLL_API void WINAPI       vDutCom_PointerAdjustStoreOffset(UINT uiPointer);
MYDLL_API BOOL WINAPI       bDutCom_PointerAdjustOffsetIsStored(void);


/**
 * pointer control
 */
MYDLL_API void WINAPI       vDutCom_PointerControlInit(void);
MYDLL_API BOOL WINAPI       bDutCom_PointerControlInitialized(void);

MYDLL_API void WINAPI       vDutCom_PointerControlSetPositionAll(UINT *puiPosition);
MYDLL_API BOOL WINAPI       bDutCom_PointerControlPositionAllIsSet(void);

MYDLL_API void WINAPI       vDutCom_PointerControlSetPositionSingle(UINT uiPointer,
                                                                    UINT uiPosition);
MYDLL_API BOOL WINAPI       bDutCom_PointerControlPositionSingleIsSet(UINT uiPointer);


/**
 * tell-tale control
 */
MYDLL_API void WINAPI       vDutCom_TellTaleCtrlInit(void);
MYDLL_API BOOL WINAPI       bDutCom_TellTaleCtrlInitialized(void);

MYDLL_API UINT WINAPI       uiDutCom_TellTaleCtrlGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_TellTaleGetText(UINT uiTellTale);

MYDLL_API void WINAPI       vDutCom_TellTaleCtrlSetBitmap(UCHAR *pcaTellTaleState);
MYDLL_API BOOL WINAPI       bDutCom_TellTaleCtrlBitmapIsSet(void);

MYDLL_API void WINAPI       vDutCom_TellTaleCtrlSetTellTale(UINT uiTellTale,
                                                            BOOL bOn);
MYDLL_API BOOL WINAPI       bDutCom_TellTaleCtrlTellTaleIsSet(void);


/**
 * backlight control
 */
MYDLL_API void WINAPI       vDutCom_BackLightCtrlInit(void);
MYDLL_API BOOL WINAPI       bDutCom_BackLightCtrlInitialized(void);

MYDLL_API UINT WINAPI       uiDutCom_BackLightCtrlGetCount(void);
MYDLL_API UINT WINAPI       uiDutCom_BackLightCtrlGetColorCount(UINT uiBacklight);
MYDLL_API char WINAPI      *pcDutCom_BacklightGetText(UINT uiBacklight);

MYDLL_API void WINAPI       vDutCom_BackLightCtrlSet(UINT uiBackLightBitmap,
                                                     UINT uiColor);
MYDLL_API BOOL WINAPI       bDutCom_BackLightCtrlIsSet(void);


/**
 * lcd control
 */
MYDLL_API void WINAPI       vDutCom_LcdCtrlInit(void);
MYDLL_API BOOL WINAPI       bDutCom_LcdCtrlInitialized(void);

MYDLL_API UINT WINAPI       uiDutCom_LcdCtrlGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_LcdCtrlGetText(UINT uiLcd);
MYDLL_API UINT WINAPI       uiDutCom_LcdCtrlPatternGetCount(UINT uiLcd);

MYDLL_API void WINAPI       vDutCom_LcdCtrlSetPattern(UINT uiLcd,
                                                      ELcdPattern eLcdPattern);
MYDLL_API BOOL WINAPI       bDutCom_LcdCtrlPatternIsSet(void);


/**
 * read digital inputs
 */
MYDLL_API void WINAPI       vDutCom_DigInputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_DigInputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_DigInputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_DigInputsGetText(UINT uiInput);
MYDLL_API void WINAPI       vDutCom_DigInputsRequestAll(void);
MYDLL_API BOOL WINAPI       bDutCom_DigInputsReadAll(UCHAR *pucDigInputStates);
MYDLL_API void WINAPI       vDutCom_DigInputsRequestSingle(UINT uiDigInput);
MYDLL_API BOOL WINAPI       bDutCom_DigInputsReadSingle(BOOL *pbDigInputState);

/**
 * write digital outputs
 */
MYDLL_API void WINAPI       vDutCom_DigOutputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_DigOutputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_DigOutputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_DigOutputsGetText(UINT uiOutput);
MYDLL_API void WINAPI       vDutCom_DigOutputsWriteAll(UCHAR *pucDigOutputState);
MYDLL_API BOOL WINAPI       bDutCom_DigOutputsAllWritten(void);
MYDLL_API void WINAPI       vDutCom_DigOutputsWriteSingle(UINT uiDigOutput,
                                                          BOOL bDigOutputState);
MYDLL_API BOOL WINAPI       bDutCom_DigOutputsSingleWritten(void);


/**
 * read analog inputs
 */
MYDLL_API void WINAPI       vDutCom_AnalogInputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_AnalogInputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_AnalogInputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_AnalogInputsGetText(UINT uiInput);
MYDLL_API void WINAPI       vDutCom_AnalogInputsRequestAll(void);
MYDLL_API BOOL WINAPI       bDutCom_AnalogInputsReadAll(UINT *puiaAnalogInputValues);
MYDLL_API void WINAPI       vDutCom_AnalogInputsRequestSingle(UINT uiAnalogInput);
MYDLL_API BOOL WINAPI       bDutCom_AnalogInputsReadSingle(UINT *puiAnalogInputValue);

/**
 * write analog outputs
 */
MYDLL_API void WINAPI       vDutCom_AnalogOutputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_AnalogOutputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_AnalogOutputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_AnalogOutputsGetText(UINT uiOutput);
MYDLL_API void WINAPI       vDutCom_AnalogOutputsWriteAll(UINT *puiAnalogOutputValues);
MYDLL_API BOOL WINAPI       bDutCom_AnalogOutputsAllWritten(void);
MYDLL_API void WINAPI       vDutCom_AnalogOutputsWriteSingle(UINT uiAnalogOutput,
                                                             UINT uiAnalogOutputValue);
MYDLL_API BOOL WINAPI       bDutCom_AnalogOutputsSingleWritten(void);

/**
 * read frequency inputs
 */
MYDLL_API void WINAPI       vDutCom_FreqInputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_FreqInputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_FreqInputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_FreqInputsGetText(UINT uiInput);
MYDLL_API void WINAPI       vDutCom_FreqInputsRequestAll(void);
MYDLL_API BOOL WINAPI       bDutCom_FreqInputsReadAll(UINT *puiFreqInputValue);
MYDLL_API void WINAPI       vDutCom_FreqInputsRequestSingle(UINT uiFreqInput);
MYDLL_API BOOL WINAPI       bDutCom_FreqInputsReadSingle(UINT *puiFreqInputValue);

/**
 * set frequency outputs
 */
MYDLL_API void WINAPI       vDutCom_FreqOutputsInit(void);
MYDLL_API BOOL WINAPI       bDutCom_FreqOutputsInitialized(void);
MYDLL_API UINT WINAPI       uiDutCom_FreqOutputsGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_FreqOutputsGetText(UINT uiOutput);
MYDLL_API void WINAPI       vDutCom_FreqOutputsWriteAll(UINT *puiFrequency,
                                                        UINT *puiDutyCycle);
MYDLL_API BOOL WINAPI       bDutCom_FreqOutputsAllWritten(void);
MYDLL_API void WINAPI       vDutCom_FreqOutputsWriteSingle(UINT uiFreqOutput,
                                                           UINT uiFrequency,
                                                           UINT uiDutyCycle);
MYDLL_API BOOL WINAPI       bDutCom_FreqOutputsSingleWritten(void);

MYDLL_API UINT WINAPI       uiDutCom_DeviceSpecificTestGetCount(void);
MYDLL_API char WINAPI      *pcDutCom_DeviceSpecificTestGetText(UINT uiTestNumber);

MYDLL_API void WINAPI       vDutCom_DeviceSpecificTestAssignInput(UINT  uiTestNumber,
                                                                  UINT  uiInputNr,
                                                                  void *pvTagDigInp,
                                                                  char *pcDescription);
MYDLL_API void WINAPI       vDutCom_DeviceSpecificTestAssignOutput(UINT  uiTestNumber,
                                                                   UINT  uiOutputNr,
                                                                   void *pvTagDigOutp,
                                                                   char *pcDescription);

MYDLL_API void WINAPI       vDutCom_DeviceSpecificTestStart(UINT uiTestNumber);
MYDLL_API BOOL WINAPI       bDutCom_DeviceSpecificTestEnded(BOOL *pbResult);


#endif // LIB_DUT_COM_H_INCLUDED

