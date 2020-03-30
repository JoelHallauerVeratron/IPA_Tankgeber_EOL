/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   0000: Vorlage
 *
 *    FILE:        LibDutComImport.h
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
/*******************************************************************
 *
 * This file must not be modified in any project other than LibDutCom_0000!!!
 *
 *
 * Make modifications only in the root project LibDutCom_0000!!!
 *
 *******************************************************************/

#ifndef LIB_DUT_COM_IMPORT_H_INCLUDED
  #define LIB_DUT_COM_IMPORT_H_INCLUDED

  #include <windows.h>

  #include "LibDutCom_Interface.h"


/**
 * Function prototypes for user of dll
 */
void vDutCom_GetImportErrorText(char *pcErrorMessage,
                                unsigned int uiBufferSize);

BOOL bDutCom_Load(unsigned int  uiLibNr,
                  BOOL          bStartCommunication);

void vDutCom_Unload(void);

int iDutCom_GetTraceLine(char *pcTraceLine,
                         int *piLinesRemaining);


/**
 * Adapted from http://www.flipcode.com/archives/LoadLibrary-GetProcAddress_Fun.shtml
 */

/**
 * Function types for functions exported by dll
 */

typedef BOOL              (*pFbDutCom_SetParam)(TagLibDutComParam *ptagParam);

typedef void              (*pFvDutCom_GetInfo)(char*, UINT*, char*, char*, char*);
typedef int               (*pFiDutCom_GetOneTraceLine)(char*, int*);
typedef char             *(*pFpcDutCom_GetTraceHeader)(void);
typedef void              (*pFvDutCom_TraceEnable)(BOOL);
typedef void              (*pFvDutCom_TraceClear)(void);
typedef BOOL              (*pFbDutCom_GetErrorMessage)(char*, UINT);


typedef void              (*pFvDutCom_CommunicationStart)(void);
typedef void              (*pFvDutCom_CommunicationStop)(void);
typedef BOOL              (*pFbDutCom_CommunicationStarted)(void);
typedef BOOL              (*pFbDutCom_CommunicationStopped)(void);

typedef void              (*pFvDutCom_DiagModeStart)(void);
typedef void              (*pFvDutCom_DiagModeStop)(void);
typedef BOOL              (*pFbDutCom_DiagModeStarted)(void);
typedef BOOL              (*pFbDutCom_DiagModeStopped)(void);
typedef void              (*pFvDutCom_DiagModeSet)(EDiagMode eDiagMode);
typedef BOOL              (*pFbDutCom_DiagModeIsSet)(void);

typedef void              (*pFvDutCom_SecurityAccess)(ESecurityAccess eSecurityAccess);
typedef ESecurityAccess   (*pFeDutCom_SecurityGetCurrentAccess)(void);

typedef void              (*pFvDutCom_Reset)(void);

typedef void              (*pFvDutCom_ClockReset)(void);
typedef void              (*pFvDutCom_ClockSetSeconds)(UINT uiSeconds);
typedef void              (*pFvDutCom_ClockSetMinutes)(UINT uiMinutes);
typedef void              (*pFvDutCom_ClockSetHours)(UINT uiHours);
typedef void              (*pFvDutCom_ClockSetTime)(UINT uiHours,
                                                    UINT uiMinutes,
                                                    UINT uiSeconds);
typedef BOOL              (*pFbDutCom_ClockSetTimeDone)(void);
typedef void              (*pFvDutCom_ClockRequestTime)(void);
typedef UINT              (*pFuiDutCom_ClockGetSeconds)(void);
typedef UINT              (*pFuiDutCom_ClockGetMinutes)(void);
typedef UINT              (*pFuiDutCom_ClockGetHours)(void);
typedef BOOL              (*pFbDutCom_ClockGetTime)(UINT *puiHours,
                                                    UINT *puiMinutes,
                                                    UINT *puiSeconds);

typedef void              (*pFvDutCom_DataEepromByteWrite)(UINT uiAddress,
                                                           UCHAR ucData);
typedef BOOL              (*pFbDutCom_DataEepromByteWritten)(void);

typedef void              (*pFvDutCom_DataEepromByteRequest)(UINT uiAddress);
typedef BOOL              (*pFbDutCom_DataEepromByteRead)(UCHAR *pucData);




typedef void              (*pFvDutCom_DataEepromBulkWrite)(UINT   uiAddress,
                                                           UCHAR *pucData,
                                                           UINT   uiBytes);
typedef BOOL              (*pFbDutCom_DataEepromBulkWritten)(void);

typedef void              (*pFvDutCom_DataEepromBulkRequest)(UINT uiAddress,
                                                             UINT uiBytes);
typedef BOOL              (*pFbDutCom_DataEepromBulkRead)(UINT   uiAddress,
                                                          UCHAR *pucData,
                                                          UINT   uiBytes);


typedef void              (*pFvDutCom_DataSerialNumberWrite)(UINT uiSerialNumber);
typedef BOOL              (*pFbDutCom_DataSerialNumberWritten)(void);

typedef void              (*pFvDutCom_DataSerialNumberRequest)(void);
typedef BOOL              (*pFbDutCom_DataSerialNumberRead)(UINT *puiSerialNumber);


typedef void              (*pFvDutCom_DataStringWrite)(EDataType  eDataType,
                                                       char      *pcDataString);
typedef BOOL              (*pFbDutCom_DataStringWritten)(void);

typedef void              (*pFvDutCom_DataStringRequest)(EDataType eDataType);
typedef BOOL              (*pFbDutCom_DataStringRead)(char *pcDataString);
typedef void              (*pFvDutCom_DataStringGetDataInfo)(EDataType    eDataType,
                                                             EDataFormat  *peDataFormat,
                                                             UINT         *puiDataSizeBytes);


typedef UINT              (*pFuiDutCom_DataProductionInterlockGetCount)(void);
typedef char             *(*pFpcDutCom_DataProductionInterlockGetText)(UINT uiTool);

typedef void              (*pFvDutCom_DataProductionInterlockWrite)(UINT uiToolNumber,
                                                                    UINT uiToolVersionMajor,
                                                                    UINT uiToolVersionMinor,
                                                                    UINT uiRunCounter);
typedef BOOL              (*pFbDutCom_DataProductionInterlockWritten)(void);

typedef void              (*pFvDutCom_DataProductionInterlockRequest)(UINT uiToolNumber);
typedef BOOL              (*pFbDutCom_DataProductionInterlockRead)(UINT *puiVersionMajor,
                                                                   UINT *puiVersionMinor,
                                                                   UINT *puiRunCounter,
                                                                   UINT *puiYear,
                                                                   UINT *puiMonth,
                                                                   UINT *puiDay,
                                                                   UINT *puiHours,
                                                                   UINT *puiMinutes,
                                                                   UINT *puiSeconds);


typedef UINT              (*pFuiDutCom_PointerGetCount)(void);
typedef char             *(*pFpcDutCom_PointerGetText)(UINT uiPointer);

typedef void              (*pFvDutCom_PointerAdjustInit)(void);
typedef BOOL              (*pFbDutCom_PointerAdjustInitialized)(void);

typedef void              (*pFvDutCom_PointerAdjustSetOffset)(UINT uiPointer,
                                                              UINT uiOffset);
typedef BOOL              (*pFbDutCom_PointerAdjustOffsetIsSet)(void);

typedef void              (*pFvDutCom_PointerAdjustSetGain)(UINT uiPointer,
                                                            UINT uiPosition,
                                                            float fGain);
typedef BOOL              (*pFbDutCom_PointerAdjustGainIsSet)(void);

typedef void              (*pFvDutCom_PointerAdjustStoreOffset)(UINT uiPointer);
typedef BOOL              (*pFbDutCom_PointerAdjustOffsetIsStored)(void);


typedef void              (*pFvDutCom_PointerControlInit)(void);
typedef BOOL              (*pFbDutCom_PointerControlInitialized)(void);

typedef void              (*pFvDutCom_PointerControlSetPositionAll)(UINT *puiPosition);
typedef BOOL              (*pFbDutCom_PointerControlPositionAllIsSet)(void);

typedef void              (*pFvDutCom_PointerControlSetPositionSingle)(UINT uiPointer,
                                                                       UINT uiPosition);
typedef BOOL              (*pFbDutCom_PointerControlPositionSingleIsSet)(UINT uiPointer);


typedef void              (*pFvDutCom_TellTaleCtrlInit)(void);
typedef BOOL              (*pFbDutCom_TellTaleCtrlInitialized)(void);

typedef UINT              (*pFuiDutCom_TellTaleCtrlGetCount)(void);
typedef char             *(*pFpcDutCom_TellTaleGetText)(UINT uiTellTale);

typedef void              (*pFvDutCom_TellTaleCtrlSetBitmap)(UCHAR *pcaTellTaleState);
typedef BOOL              (*pFbDutCom_TellTaleCtrlBitmapIsSet)(void);

typedef void              (*pFvDutCom_TellTaleCtrlSetTellTale)(UINT uiTellTale,
                                                               BOOL bOn);
typedef BOOL              (*pFbDutCom_TellTaleCtrlTellTaleIsSet)(void);


typedef void              (*pFvDutCom_BackLightCtrlInit)(void);
typedef BOOL              (*pFbDutCom_BackLightCtrlInitialized)(void);

typedef UINT              (*pFuiDutCom_BackLightCtrlGetCount)(void);
typedef UINT              (*pFuiDutCom_BackLightCtrlGetColorCount)(UINT uiBacklight);
typedef char             *(*pFpcDutCom_BacklightGetText)(UINT uiBacklight);

typedef void              (*pFvDutCom_BackLightCtrlSet)(UINT uiBackLightBitmap,
                                                        UINT uiColor);
typedef BOOL              (*pFbDutCom_BackLightCtrlIsSet)(void);


typedef void              (*pFvDutCom_LcdCtrlInit)(void);
typedef BOOL              (*pFbDutCom_LcdCtrlInitialized)(void);

typedef UINT              (*pFuiDutCom_LcdCtrlGetCount)(void);
typedef char             *(*pFpcDutCom_LcdCtrlGetText)(UINT uiLcd);
typedef UINT              (*pFuiDutCom_LcdCtrlPatternGetCount)(UINT uiLcd);

typedef void              (*pFvDutCom_LcdCtrlSetPattern)(UINT        uiLcd,
                                                         ELcdPattern eLcdPattern);
typedef BOOL              (*pFbDutCom_LcdCtrlPatternIsSet)(void);


typedef void              (*pFvDutCom_DigInputsInit)(void);
typedef BOOL              (*pFbDutCom_DigInputsInitialized)(void);

typedef UINT              (*pFuiDutCom_DigInputsGetCount)(void);
typedef char             *(*pFpcDutCom_DigInputsGetText)(UINT uiInput);
typedef void              (*pFvDutCom_DigInputsRequestAll)(void);
typedef BOOL              (*pFbDutCom_DigInputsReadAll)(UCHAR *pucDigInputStates);
typedef void              (*pFvDutCom_DigInputsRequestSingle)(UINT uiDigInput);
typedef BOOL              (*pFbDutCom_DigInputsReadSingle)(BOOL *pbDigInputState);


typedef void              (*pFvDutCom_DigOutputsInit)(void);
typedef BOOL              (*pFbDutCom_DigOutputsInitialized)(void);

typedef UINT              (*pFuiDutCom_DigOutputsGetCount)(void);
typedef char             *(*pFpcDutCom_DigOutputsGetText)(UINT uiOutput);
typedef void              (*pFvDutCom_DigOutputsWriteAll)(UCHAR *pucDigOutputState);
typedef BOOL              (*pFbDutCom_DigOutputsAllWritten)(void);
typedef void              (*pFvDutCom_DigOutputsWriteSingle)(UINT uiDigOutput,
                                                             BOOL bDigOutputState);
typedef BOOL              (*pFbDutCom_DigOutputsSingleWritten)(void);


typedef void              (*pFvDutCom_AnalogInputsInit)(void);
typedef BOOL              (*pFbDutCom_AnalogInputsInitialized)(void);
typedef UINT              (*pFuiDutCom_AnalogInputsGetCount)(void);
typedef char             *(*pFpcDutCom_AnalogInputsGetText)(UINT uiInput);
typedef void              (*pFvDutCom_AnalogInputsRequestAll)(void);
typedef BOOL              (*pFbDutCom_AnalogInputsReadAll)(UINT *puiAnalogInputValues);
typedef void              (*pFvDutCom_AnalogInputsRequestSingle)(UINT uiAnalogInput);
typedef BOOL              (*pFbDutCom_AnalogInputsReadSingle)(UINT *puiAnalogInputValue);

typedef void              (*pFvDutCom_AnalogOutputsInit)(void);
typedef BOOL              (*pFbDutCom_AnalogOutputsInitialized)(void);
typedef UINT              (*pFuiDutCom_AnalogOutputsGetCount)(void);
typedef char             *(*pFpcDutCom_AnalogOutputsGetText)(UINT uiOutput);
typedef void              (*pFvDutCom_AnalogOutputsWriteAll)(UINT *puiAnalogOutputValues);
typedef BOOL              (*pFbDutCom_AnalogOutputsAllWritten)(void);
typedef void              (*pFvDutCom_AnalogOutputsWriteSingle)(UINT uiAnalogOutput,
                                                                UINT uiAnalogOutputValue);
typedef BOOL              (*pFbDutCom_AnalogOutputsSingleWritten)(void);

typedef void              (*pFvDutCom_FreqInputsInit)(void);
typedef BOOL              (*pFbDutCom_FreqInputsInitialized)(void);
typedef UINT              (*pFuiDutCom_FreqInputsGetCount)(void);
typedef char             *(*pFpcDutCom_FreqInputsGetText)(UINT uiInput);
typedef void              (*pFvDutCom_FreqInputsRequestAll)(void);
typedef BOOL              (*pFbDutCom_FreqInputsReadAll)(UINT *puiFreqInputValue);
typedef void              (*pFvDutCom_FreqInputsRequestSingle)(UINT uiFreqInput);
typedef BOOL              (*pFbDutCom_FreqInputsReadSingle)(UINT *puiFreqInputValue);

typedef void              (*pFvDutCom_FreqOutputsInit)(void);
typedef BOOL              (*pFbDutCom_FreqOutputsInitialized)(void);
typedef UINT              (*pFuiDutCom_FreqOutputsGetCount)(void);
typedef char             *(*pFpcDutCom_FreqOutputsGetText)(UINT uiOutput);
typedef void              (*pFvDutCom_FreqOutputsWriteAll)(UINT *puiFrequency,
                                                           UINT *puiDutyCycle);
typedef BOOL              (*pFbDutCom_FreqOutputsAllWritten)(void);
typedef void              (*pFvDutCom_FreqOutputsWriteSingle)(UINT uiFreqOutput,
                                                              UINT uiFrequency,
                                                              UINT uiDutyCycle);
typedef BOOL              (*pFbDutCom_FreqOutputsSingleWritten)(void);

typedef UINT              (*pFuiDutCom_DeviceSpecificTestGetCount)(void);
typedef char             *(*pFpcDutCom_DeviceSpecificTestGetText)(UINT uiTestNumber);
typedef void              (*pFvDutCom_DeviceSpecificTestAssignInput)(UINT  uiTestNumber,
                                                                     UINT  uiInputNr,
                                                                     void *pvTagDigInp,
                                                                     char *pcDescription);
typedef void              (*pFvDutCom_DeviceSpecificTestAssignOutput)(UINT  uiTestNumber,
                                                                      UINT  uiOutputNr,
                                                                      void *pvTagDigOutp,
                                                                      char *pcDescription);
typedef void              (*pFvDutCom_DeviceSpecificTestStart)(UINT uiTestNumber);
typedef BOOL              (*pFbDutCom_DeviceSpecificTestEnded)(BOOL *pbResult);










/**
 * Function pointers for functions exported by dll
 */
pFbDutCom_SetParam                                bDutCom_SetParam;
pFvDutCom_GetInfo                                 vDutCom_GetInfo;
pFiDutCom_GetOneTraceLine                         iDutCom_GetOneTraceLine;
pFpcDutCom_GetTraceHeader                         pcDutCom_GetTraceHeader;
pFvDutCom_TraceEnable                             vDutCom_TraceEnable;
pFvDutCom_TraceClear                              vDutCom_TraceClear;
pFbDutCom_GetErrorMessage                         bDutCom_GetErrorMessage;


pFvDutCom_CommunicationStart                      vDutCom_CommunicationStart;
pFvDutCom_CommunicationStop                       vDutCom_CommunicationStop;
pFbDutCom_CommunicationStarted                    bDutCom_CommunicationStarted;
pFbDutCom_CommunicationStopped                    bDutCom_CommunicationStopped;

pFvDutCom_DiagModeStart                           vDutCom_DiagModeStart;
pFvDutCom_DiagModeStop                            vDutCom_DiagModeStop;
pFbDutCom_DiagModeStarted                         bDutCom_DiagModeStarted;
pFbDutCom_DiagModeStopped                         bDutCom_DiagModeStopped;
pFvDutCom_DiagModeSet                             vDutCom_DiagModeSet;
pFbDutCom_DiagModeIsSet                           bDutCom_DiagModeIsSet;

pFvDutCom_SecurityAccess                          vDutCom_SecurityAccess;
pFeDutCom_SecurityGetCurrentAccess                eDutCom_SecurityGetCurrentAccess;

pFvDutCom_Reset                                   vDutCom_Reset;

pFvDutCom_ClockReset                              vDutCom_ClockReset;
pFvDutCom_ClockSetSeconds                         vDutCom_ClockSetSeconds;
pFvDutCom_ClockSetMinutes                         vDutCom_ClockSetMinutes;
pFvDutCom_ClockSetHours                           vDutCom_ClockSetHours;
pFvDutCom_ClockSetTime                            vDutCom_ClockSetTime;
pFbDutCom_ClockSetTimeDone                        bDutCom_ClockSetTimeDone;
pFvDutCom_ClockRequestTime                        vDutCom_ClockRequestTime;
pFuiDutCom_ClockGetSeconds                        uiDutCom_ClockGetSeconds;
pFuiDutCom_ClockGetMinutes                        uiDutCom_ClockGetMinutes;
pFuiDutCom_ClockGetHours                          uiDutCom_ClockGetHours;
pFbDutCom_ClockGetTime                            bDutCom_ClockGetTime;

pFvDutCom_DataEepromByteWrite                     vDutCom_DataEepromByteWrite;
pFbDutCom_DataEepromByteWritten                   bDutCom_DataEepromByteWritten;

pFvDutCom_DataEepromByteRequest                   vDutCom_DataEepromByteRequest;
pFbDutCom_DataEepromByteRead                      bDutCom_DataEepromByteRead;





pFvDutCom_DataEepromBulkWrite                     vDutCom_DataEepromBulkWrite;
pFbDutCom_DataEepromBulkWritten                   bDutCom_DataEepromBulkWritten;

pFvDutCom_DataEepromBulkRequest                   vDutCom_DataEepromBulkRequest;
pFbDutCom_DataEepromBulkRead                      bDutCom_DataEepromBulkRead;


pFvDutCom_DataSerialNumberWrite                   vDutCom_DataSerialNumberWrite;
pFbDutCom_DataSerialNumberWritten                 bDutCom_DataSerialNumberWritten;

pFvDutCom_DataSerialNumberRequest                 vDutCom_DataSerialNumberRequest;
pFbDutCom_DataSerialNumberRead                    bDutCom_DataSerialNumberRead;


pFvDutCom_DataStringWrite                         vDutCom_DataStringWrite;
pFbDutCom_DataStringWritten                       bDutCom_DataStringWritten;

pFvDutCom_DataStringRequest                       vDutCom_DataStringRequest;
pFbDutCom_DataStringRead                          bDutCom_DataStringRead;
pFvDutCom_DataStringGetDataInfo                   vDutCom_DataStringGetDataInfo;


pFuiDutCom_DataProductionInterlockGetCount        uiDutCom_DataProductionInterlockGetCount;
pFpcDutCom_DataProductionInterlockGetText         pcDutCom_DataProductionInterlockGetText;

pFvDutCom_DataProductionInterlockWrite            vDutCom_DataProductionInterlockWrite;
pFbDutCom_DataProductionInterlockWritten          bDutCom_DataProductionInterlockWritten;

pFvDutCom_DataProductionInterlockRequest          vDutCom_DataProductionInterlockRequest;
pFbDutCom_DataProductionInterlockRead             bDutCom_DataProductionInterlockRead;


pFvDutCom_PointerAdjustInit                       vDutCom_PointerAdjustInit;
pFbDutCom_PointerAdjustInitialized                bDutCom_PointerAdjustInitialized;

pFuiDutCom_PointerGetCount                        uiDutCom_PointerGetCount;
pFpcDutCom_PointerGetText                         pcDutCom_PointerGetText;

pFvDutCom_PointerAdjustSetOffset                  vDutCom_PointerAdjustSetOffset;
pFbDutCom_PointerAdjustOffsetIsSet                bDutCom_PointerAdjustOffsetIsSet;

pFvDutCom_PointerAdjustSetGain                    vDutCom_PointerAdjustSetGain;
pFbDutCom_PointerAdjustGainIsSet                  bDutCom_PointerAdjustGainIsSet;

pFvDutCom_PointerAdjustStoreOffset                vDutCom_PointerAdjustStoreOffset;
pFbDutCom_PointerAdjustOffsetIsStored             bDutCom_PointerAdjustOffsetIsStored;


pFvDutCom_PointerControlInit                      vDutCom_PointerControlInit;
pFbDutCom_PointerControlInitialized               bDutCom_PointerControlInitialized;

pFvDutCom_PointerControlSetPositionAll            vDutCom_PointerControlSetPositionAll;
pFbDutCom_PointerControlPositionAllIsSet          bDutCom_PointerControlPositionAllIsSet;

pFvDutCom_PointerControlSetPositionSingle         vDutCom_PointerControlSetPositionSingle;
pFbDutCom_PointerControlPositionSingleIsSet       bDutCom_PointerControlPositionSingleIsSet;


pFvDutCom_TellTaleCtrlInit                        vDutCom_TellTaleCtrlInit;
pFbDutCom_TellTaleCtrlInitialized                 bDutCom_TellTaleCtrlInitialized;

pFuiDutCom_TellTaleCtrlGetCount                   uiDutCom_TellTaleCtrlGetCount;
pFpcDutCom_TellTaleGetText                        pcDutCom_TellTaleGetText;

pFvDutCom_TellTaleCtrlSetBitmap                   vDutCom_TellTaleCtrlSetBitmap;
pFbDutCom_TellTaleCtrlBitmapIsSet                 bDutCom_TellTaleCtrlBitmapIsSet;

pFvDutCom_TellTaleCtrlSetTellTale                 vDutCom_TellTaleCtrlSetTellTale;
pFbDutCom_TellTaleCtrlTellTaleIsSet               bDutCom_TellTaleCtrlTellTaleIsSet;


pFvDutCom_BackLightCtrlInit                       vDutCom_BackLightCtrlInit;
pFbDutCom_BackLightCtrlInitialized                bDutCom_BackLightCtrlInitialized;

pFuiDutCom_BackLightCtrlGetCount                  uiDutCom_BackLightCtrlGetCount;
pFuiDutCom_BackLightCtrlGetColorCount             uiDutCom_BackLightCtrlGetColorCount;
pFpcDutCom_BacklightGetText                       pcDutCom_BacklightGetText;

pFvDutCom_BackLightCtrlSet                        vDutCom_BackLightCtrlSet;
pFbDutCom_BackLightCtrlIsSet                      bDutCom_BackLightCtrlIsSet;


pFvDutCom_LcdCtrlInit                             vDutCom_LcdCtrlInit;
pFbDutCom_LcdCtrlInitialized                      bDutCom_LcdCtrlInitialized;

pFuiDutCom_LcdCtrlGetCount                        uiDutCom_LcdCtrlGetCount;
pFpcDutCom_LcdCtrlGetText                         pcDutCom_LcdCtrlGetText;
pFuiDutCom_LcdCtrlPatternGetCount                 uiDutCom_LcdCtrlPatternGetCount;

pFvDutCom_LcdCtrlSetPattern                       vDutCom_LcdCtrlSetPattern;
pFbDutCom_LcdCtrlPatternIsSet                     bDutCom_LcdCtrlPatternIsSet;


pFvDutCom_DigInputsInit                           vDutCom_DigInputsInit;
pFbDutCom_DigInputsInitialized                    bDutCom_DigInputsInitialized;
pFuiDutCom_DigInputsGetCount                      uiDutCom_DigInputsGetCount;
pFpcDutCom_DigInputsGetText                       pcDutCom_DigInputsGetText;
pFvDutCom_DigInputsRequestAll                     vDutCom_DigInputsRequestAll;
pFbDutCom_DigInputsReadAll                        bDutCom_DigInputsReadAll;
pFvDutCom_DigInputsRequestSingle                  vDutCom_DigInputsRequestSingle;
pFbDutCom_DigInputsReadSingle                     bDutCom_DigInputsReadSingle;


pFvDutCom_DigOutputsInit                          vDutCom_DigOutputsInit;
pFbDutCom_DigOutputsInitialized                   bDutCom_DigOutputsInitialized;
pFuiDutCom_DigOutputsGetCount                     uiDutCom_DigOutputsGetCount;
pFpcDutCom_DigOutputsGetText                      pcDutCom_DigOutputsGetText;
pFvDutCom_DigOutputsWriteAll                      vDutCom_DigOutputsWriteAll;
pFbDutCom_DigOutputsAllWritten                    bDutCom_DigOutputsAllWritten;
pFvDutCom_DigOutputsWriteSingle                   vDutCom_DigOutputsWriteSingle;
pFbDutCom_DigOutputsSingleWritten                 bDutCom_DigOutputsSingleWritten;


pFvDutCom_AnalogInputsInit                        vDutCom_AnalogInputsInit;
pFbDutCom_AnalogInputsInitialized                 bDutCom_AnalogInputsInitialized;
pFuiDutCom_AnalogInputsGetCount                   uiDutCom_AnalogInputsGetCount;
pFpcDutCom_AnalogInputsGetText                    pcDutCom_AnalogInputsGetText;
pFvDutCom_AnalogInputsRequestAll                  vDutCom_AnalogInputsRequestAll;
pFbDutCom_AnalogInputsReadAll                     bDutCom_AnalogInputsReadAll;
pFvDutCom_AnalogInputsRequestSingle               vDutCom_AnalogInputsRequestSingle;
pFbDutCom_AnalogInputsReadSingle                  bDutCom_AnalogInputsReadSingle;

pFvDutCom_AnalogOutputsInit                       vDutCom_AnalogOutputsInit;
pFbDutCom_AnalogOutputsInitialized                bDutCom_AnalogOutputsInitialized;
pFuiDutCom_AnalogOutputsGetCount                  uiDutCom_AnalogOutputsGetCount;
pFpcDutCom_AnalogOutputsGetText                   pcDutCom_AnalogOutputsGetText;
pFvDutCom_AnalogOutputsWriteAll                   vDutCom_AnalogOutputsWriteAll;
pFbDutCom_AnalogOutputsAllWritten                 bDutCom_AnalogOutputsAllWritten;
pFvDutCom_AnalogOutputsWriteSingle                vDutCom_AnalogOutputsWriteSingle;
pFbDutCom_AnalogOutputsSingleWritten              bDutCom_AnalogOutputsSingleWritten;

pFvDutCom_FreqInputsInit                          vDutCom_FreqInputsInit;
pFbDutCom_FreqInputsInitialized                   bDutCom_FreqInputsInitialized;
pFuiDutCom_FreqInputsGetCount                     uiDutCom_FreqInputsGetCount;
pFpcDutCom_FreqInputsGetText                      pcDutCom_FreqInputsGetText;
pFvDutCom_FreqInputsRequestAll                    vDutCom_FreqInputsRequestAll;
pFbDutCom_FreqInputsReadAll                       bDutCom_FreqInputsReadAll;
pFvDutCom_FreqInputsRequestSingle                 vDutCom_FreqInputsRequestSingle;
pFbDutCom_FreqInputsReadSingle                    bDutCom_FreqInputsReadSingle;

pFvDutCom_FreqOutputsInit                         vDutCom_FreqOutputsInit;
pFbDutCom_FreqOutputsInitialized                  bDutCom_FreqOutputsInitialized;
pFuiDutCom_FreqOutputsGetCount                    uiDutCom_FreqOutputsGetCount;
pFpcDutCom_FreqOutputsGetText                     pcDutCom_FreqOutputsGetText;
pFvDutCom_FreqOutputsWriteAll                     vDutCom_FreqOutputsWriteAll;
pFbDutCom_FreqOutputsAllWritten                   bDutCom_FreqOutputsAllWritten;
pFvDutCom_FreqOutputsWriteSingle                  vDutCom_FreqOutputsWriteSingle;
pFbDutCom_FreqOutputsSingleWritten                bDutCom_FreqOutputsSingleWritten;

pFuiDutCom_DeviceSpecificTestGetCount             uiDutCom_DeviceSpecificTestGetCount;
pFpcDutCom_DeviceSpecificTestGetText              pcDutCom_DeviceSpecificTestGetText;
pFvDutCom_DeviceSpecificTestAssignInput           vDutCom_DeviceSpecificTestAssignInput;
pFvDutCom_DeviceSpecificTestAssignOutput          vDutCom_DeviceSpecificTestAssignOutput;
pFvDutCom_DeviceSpecificTestStart                 vDutCom_DeviceSpecificTestStart;
pFbDutCom_DeviceSpecificTestEnded                 bDutCom_DeviceSpecificTestEnded;


#endif // LIB_DUT_COM_IMPORT_H_INCLUDED

