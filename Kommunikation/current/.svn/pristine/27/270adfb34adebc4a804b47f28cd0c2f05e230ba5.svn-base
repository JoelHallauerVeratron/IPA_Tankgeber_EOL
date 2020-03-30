/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   .
 *
 *    FILE:        LibDutCom_private.h
 *
 *    DESCRIPTION: Header file for private, non-public functions
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

#ifndef LIBDUTCOM_PRIVATE_H_INCLUDED
  #define LIBDUTCOM_PRIVATE_H_INCLUDED

  #include <windows.h>

  #include "LibDutCom_Interface.h"

  #include "DeviceSpecific.h"


char *pcDutCom_TraceGetHeader_m(void);
void vDutCom_TraceEnable_m(BOOL bEnable);


typedef struct
{
  BOOL bTerminate;
  BOOL bIsRunning;
  char caMessage[200];
} TagComControl;

  #ifndef _CVI_
    #undef WINAPI
    #define WINAPI
  #endif // !_CVI_

DWORD WINAPI dwDutCom_SenderThread_m(LPVOID lpParam);
DWORD WINAPI dwDutCom_TimedSenderThread_m(LPVOID lpParam);


typedef enum
{
  eWaitResult_Ok      =1,
  eWaitResult_Timeout =2,
  eWaitResult_Failed  =3
} EWaitResult;


/**
 * send data
 */
typedef struct
{
  int iComStep;   // counter for multi-command telegrams

  // Diag mode
  struct
  {
    BOOL bStart;   // TRUE => Start; FALSE => Stop
    BOOL bSend;    // TRUE => send telegram
    EDiagMode eDiagMode;
    BOOL bSet;    // TRUE => set diag mode "eDiagMode"
  }tagDiagMode;

  // security access
  struct
  {
    ESecurityAccess eSecurityAccess;
    int iKey;
    BOOL bSend;    // TRUE => send telegram
  }tagSecurityAccess;

  // reset dut
  struct
  {
    BOOL bSend;
  }tagReset;

  // clock
  struct
  {
    BOOL bReset;
    BOOL bSetSeconds;
    UINT uiSeconds;   // param. to bSetSeconds
    BOOL bSetMinutes;
    UINT uiMinutes;   // param. to bSetMinutes
    BOOL bSetHours;
    UINT uiHours;     // param. to bSetHours
    BOOL bSetTime;    // set HH:MM:SS together at once
    BOOL bRequestTime;
  }tagClock;

  // static data: byte wise access
  struct
  {
    BOOL bWrite;
    UINT uiWriteAddress;    // param zu bWrite
    UCHAR ucWriteData;      // param zu bWrite

    BOOL bRequest;
    UINT uiRequestAddress;  // param zu bRequest
  }tagStaticDataByteWise;

  // static data: bulk wise access
  struct
  {
    BOOL bWrite;
    UINT uiWriteAddress;   // param zu bWrite
    UCHAR *pucWriteData;   // param zu bWrite
    UINT  uiBytes;

    BOOL bRequest;
    UINT uiRequestAddress;   // param zu bRequest
  }tagStaticDataBulk;

  // static data: serial number access
  struct
  {
    BOOL bWrite;
    UINT uiSerialNumber;   // param. to bWrite
    BOOL bRequest;
  }tagStaticDataSerialNumber;

  // static data: data string access
  struct
  {
    BOOL bWrite;
    EDataType eDataType;  // param. to bWrite
    UCHAR ucaDataString[DATA_STRING_LEN+1];      // param. to bWrite
    BOOL bRequest;
  }tagStaticDataString;

  // static data: production interlock access
  struct
  {
    BOOL bWrite;
    UINT uiToolNumber;         // param. to bWrite
    UINT uiToolVersionMajor;   // param. to bWrite
    UINT uiToolVersionMinor;   // param. to bWrite
    UINT uiRunCounter;         // param. to bWrite
    BOOL bRequest;
  }tagStaticDataProductionInterlock;

  // pointer adjust
  struct
  {
    BOOL bInit;
    BOOL bSetOffset;
    BOOL bSetGain;
    UINT uiPointer;   // current pointer
    UINT uiOffset;    // param to bSetOffset
    float fGain;      // param to bSetGain
    UINT uiPosition;  // param to bSetGain
    BOOL bStoreOffset;
  }tagPointerAdjust;

  // pointer control
  struct
  {
    BOOL bInit;
    BOOL bSetPositionAll;
    UINT uiaPosition[NUM_POINTERS+1];   // param. to bSetPositionAll
    BOOL bSetPositionSingle;
    UINT uiPointer;      // param. to bSetPositionSingle
    UINT uiPosition;     // param. to bSetPositionSingle
  }tagPointerControl;

  // tell-tale control
  struct
  {
    BOOL bInit;
    BOOL bSetBitmap;
    UCHAR ucaTellTaleState[NUM_TELLTALES+1];   // param. to bSetBitmap
    BOOL bSetSingle;
    UINT uiTellTale;   // param. to bSetSingle
    BOOL bState;       // param. to bSetSingle
  }tagTellTaleControl;

  // backlight control
  struct
  {
    BOOL bInit;
    BOOL bSetBackLight;
    UINT uiBackLightBitmap;   // param. to bSetBackLight
    UINT uiColor;             // param. to bSetBackLight
  }tagBackLightControl;

  // lcd control
  struct
  {
    BOOL bInit;
    BOOL bSetPattern;
    UINT uiLcd;
    ELcdPattern eLcdPattern;       // param. to bSetPattern
  }tagLcdControl;

  // read digital inputs
  struct
  {
    BOOL bInit;
    BOOL bRequestAll;
    BOOL bReadAll;
    BOOL bRequestSingle;
    UINT uiDigInput;
    BOOL bReadSingle;
  }tagDigInputs;

  // write digital outputs
  struct
  {
    BOOL bInit;
    BOOL bWriteAll;
    UCHAR ucaDigOutputState[NUM_DIG_OUTPUTS+1];   // param. to bWriteAll
    BOOL bWriteSingle;
    UINT uiDigOutput;          // param. to bWriteSingle
    BOOL bDigOutputState;      // param. to bWriteSingle
  }tagDigOutputs;

  // read analog inputs
  struct
  {
    BOOL bInit;
    BOOL bRequestAll;
    BOOL bReadAll;
    BOOL bRequestSingle;
    UINT uiAnalogInput;
    BOOL bReadSingle;
  }tagAnalogInputs;

  // write analog outputs
  struct
  {
    BOOL bInit;
    BOOL bWriteAll;
    UINT auiAnalogOutputValues[NUM_ANALOG_OUTPUTS+1]; // param. to bWriteAll
    BOOL bWriteSingle;
    UINT uiAnalogOutput;        // param. to bWriteSingle
    UINT uiAnalogOutputValue;   // param. to bWriteSingle
  }tagAnalogOutputs;

  // read frequency inputs
  struct
  {
    BOOL bInit;
    BOOL bRequestAll;
    BOOL bRequest;
    UINT uiFreqInput;   // param. to bRequest
    BOOL bRead;
  }tagFreqInputs;

  // write frequency inputs
  struct
  {
    BOOL bInit;
    BOOL bWriteAll;
    UINT auiFrequency[NUM_FREQ_OUTPUTS+1]; // param. to bWriteAll
    UINT auiDutyCycle[NUM_FREQ_OUTPUTS+1]; // param. to bWriteAll
    BOOL bWrite;
    UINT uiFreqOutput;    // param. to bWrite
    UINT uiFrequency;     // param. to bWrite
    UINT uiDutyCycle;     // param. to bWrite
  }tagFreqOutputs;

  // device specific test
  struct
  {
    BOOL bStart;
    UINT uiTestNumber;    // param. to bStart
  }tagDevSpecTest;



  // TODO: add further send parameters






} TagComSendData;

void vResetComSendData_m(void);


/**
 * received data
 */
typedef struct
{
  // Diag mode
  struct
  {
    BOOL bStarted;
    BOOL bIsSet;
  }tagDiagMode;

  // security access
  struct
  {
    ESecurityAccess eSecurityAccess;
    int iSeed;
  }tagSecurityAccess;

  // reset dut
  struct
  {
    BOOL bSend;
  }tagReset;

  // clock access
  struct
  {
    UINT uiSeconds;
    UINT uiMinutes;
    UINT uiHours;
    BOOL bTimeIsSet;
    BOOL bReceived;
    //BOOL bRequestTime;
  }tagClock;

  // static data: byte wise access
  struct
  {
    BOOL bWritten;
    BOOL bReceived;
    UCHAR ucReceivedData;    // param. to bByteReceived
  }tagStaticDataByteWise;

  // static data: bulk wise access
  struct
  {
    BOOL bWritten;
    BOOL bReceived;
    UCHAR *pucReceivedData;    // param. to bByteReceived
    UINT  uiBytes;                 // param. to bByteReceived
  }tagStaticDataBulk;

  // static data: serial number access
  struct
  {
    BOOL bWritten;
    BOOL bReceived;
    UINT uiReceivedSerialNumber;   // param. to bReceived
  }tagStaticDataSerialNumber;

  // static data: Ascii access
  struct
  {
    BOOL bWritten;
    BOOL bReceived;
    EDataType eDataType;  // param. to bReceived
    UCHAR ucaDataString[DATA_STRING_LEN+1];   // param. to bReceived
  }tagStaticDataString;

  // static data: production interlock access
  struct
  {
    BOOL bWritten;
    BOOL bReceived;
    UINT uiVersionMajor;   // param. to bReceived
    UINT uiVersionMinor;   // param. to bReceived
    UINT uiRunCounter;     // param. to bReceived
    UINT uiYear;           // param. to bReceived
    UINT uiMonth;          // param. to bReceived
    UINT uiDay;            // param. to bReceived
    UINT uiHours;          // param. to bReceived
    UINT uiMinutes;        // param. to bReceived
    UINT uiSeconds;        // param. to bReceived
  }tagStaticDataProductionInterlock;

  // pointer adjust
  struct
  {
    BOOL bInitialized;
    BOOL bOffsetIsSet;     // response to bSetOffset
    BOOL bGainIsSet;       // response to bSetGain
    BOOL bOffsetIsStored;  // response to bStoreOffset
  }tagPointerAdjust;

  // pointer control
  struct
  {
    BOOL bInitialized;
    BOOL bPositionAllAreSet;   // response to bSetPositionAll
    BOOL bPositionSingleIsSet; // response to bSetPositionSingle
  }tagPointerControl;

  // tell-tale control
  struct
  {
    BOOL bInitialized;
    BOOL bBitmapIsSet;   // response to bSetBitmap
    BOOL bSingleIsSet;   // response to bSetSingle
  }tagTellTaleControl;

  // backlight control
  struct
  {
    BOOL bInitialized;
    BOOL bBackLightIsSet;   // response to bSetBackLight
  }tagBackLightControl;

  // lcd control
  struct
  {
    BOOL bInitialized;
    BOOL bPatternIsSet;   // response to bSetPattern
  }tagLcdControl;


  // read digital inputs
  struct
  {
    BOOL bInitialized;
    BOOL bReceivedAll;        // response to bRequestAll
    UCHAR ucaDigInputStates[NUM_DIG_INPUTS+1];  // param. to bReceivedAll
    BOOL bReceivedSingle;     // response to bRequestSingle
    BOOL bDigInputState;      // param. to bRequestSingle
  }tagDigInputs;

  // write digital outputs
  struct
  {
    BOOL bInitialized;
    BOOL bAllWritten;            // response to bWriteAll
    BOOL bSingleWritten;         // response to bWriteSingle
  }tagDigOutputs;

  // read analog inputs
  struct
  {
    BOOL bInitialized;
    BOOL bReceivedAll;         // response to bRequestAll
    UINT uiaAnalogInputValues[NUM_ANALOG_INPUTS+1]; // param. to bReceivedAll
    BOOL bReceivedSingle;      // response to bRequestSingle
    UINT uiAnalogInputValue;    // param. to bRequestSingle
  }tagAnalogInputs;

  // write analog outputs
  struct
  {
    BOOL bInitialized;
    BOOL bAllWritten;            // response to bWriteAll
    BOOL bSingleWritten;         // response to bWriteSingle
  }tagAnalogOutputs;

  // read frequency inputs
  struct
  {
    BOOL bInitialized;
    BOOL bReceivedAll;   // response to bRequestAll
    UINT uiaFrequency[NUM_FREQ_INPUTS+1]; // param. to bReceivedAll
    BOOL bReceived;      // response to bRequest
    UINT uiFrequency;    // param. to bReceived
  }tagFreqInputs;

  // write frequency inputs
  struct
  {
    BOOL bInitialized;
    BOOL bAllWritten;    // response to bWriteAll
    BOOL bWritten;       // response to bWrite
  }tagFreqOutputs;

  // device specific test
  struct
  {
    BOOL bEnded;
    BOOL bResult;    // param. to bEnded
  }tagDevSpecTest;




  // TODO: add further receive parameters



} TagComRecvData;

void vResetComRecvData_m(void);


void vResetDigIOData_m(void);


/**
 * lokale Datentypen als Pendant für die Datentypen aus zol.h
 */
typedef USHORT T_IBS_BOOL;
// digitaler Input
typedef struct
{
  int          iModulNr;
  int          iControl;
  T_IBS_BOOL  *IBS_Node;
  char        *pcTxt;
} TagDigInp;
  #if 0
// Liste von digitalen Inputs
struct TagDigInpList_S;
struct
{
  TagDigInp tagDigInp;
  struct TagDigInpList_S *pNext;
} TagDigInpList_S;
typedef struct TagDigInpList_S TagDigInpList;
  #endif

// digitaler Output
typedef struct
{
  int          iModulNr;
  int          iControl;
  T_IBS_BOOL  *IBS_Node;
  char        *pcTxt;
} TagDigOutp;
  #if 0
// Liste von digitalen Outputs
struct TagDigOutpList_S;
struct
{
  TagDigOutp tagDigOutp;
  struct TagDigOutpList_S *pNext;
} TagDigOutpList_S;
typedef struct TagDigOutpList_S TagDigOutpList;
  #endif


#endif // LIBDUTCOM_PRIVATE_H_INCLUDED
