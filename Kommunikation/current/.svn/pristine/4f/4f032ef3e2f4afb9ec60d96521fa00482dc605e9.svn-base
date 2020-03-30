/*******************************************************************
 *
 *    PROJECT:     FlexCluster
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        UDS_private.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      uidc9013
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        16.09.2011
 *
 *    HISTORY:
 *      -16.09.2011: initial implementation
 *      -
 *
 *******************************************************************/

#ifndef UDS_PRIVATE_H_INCLUDED
  #define UDS_PRIVATE_H_INCLUDED

  #include <windows.h>
  #ifdef _CVI_
    #include <toolbox.h>
    #include "cvidef.h"
  #else // !_CVI_
    #include <stdlib.h>
    #include <stdio.h>
  #endif // !_CVI_

  #include "Uds.h"
  #include "Uds_IoControl.h"
  #include "Uds_StoredData.h"


extern int  iaUds_Baudrate_m[4];
extern BOOL bUds_GenAck_m;
extern BOOL bUds_GenNak_m;
//extern BOOL bUds_ReqDPIDAck_m;
extern BOOL bUds_SetCPIDAck_m;

extern BOOL bUds_AckTesterPresent_m;
extern BOOL bUds_AckStartCommunication_m;
extern BOOL bUds_AckStopCommunication_m;

extern BOOL bUds_AckAccessTimingParameter_m;

extern BOOL bUds_AckStartRoutine_m;
extern BOOL bUds_AckStopRoutine_m;

extern BOOL bUds_AckDiagSession_m;
extern BOOL bUds_AckSeed_m;
extern BOOL bUds_AckKey_m;


extern EUdsSecurityAccess eCurrentSecurityAccess_m;


extern BOOL bTraceEnabled_m;
  #define Uds_ERROR_MESSAGE_LEN 500
extern char caUdsErrorMessage_m[Uds_ERROR_MESSAGE_LEN+1];

extern HANDLE hThreadSender_m;

  #define USE_WIN32_MUTEX

extern CRITICAL_SECTION csSender_m;
extern CRITICAL_SECTION csReadWrite_m;


/**
 *
 * defines:
 *
 */


/**
 * ECU-Eeprom-Adressen
 */

  #define EE_DIGITAL_INPUT_TYPE               (0x600000)+0x0298
  #define EE_DIGITAL_INPUT_TYPE_LEN                 3

  #define EE_OUTPUT_SINK_TYPE                 (0x600000)+0x0296
  #define EE_OUTPUT_SINK_TYPE_LEN                   1



/**
 * Funktionsprototypen
 */
BOOL bUds_WaitBoolFlag_m(BOOL *pbFlag,
                         BOOL bState,
                         unsigned long ulMilliseconds);

void vUds_SendTesterPresent_m(void);

  #if 0
typedef enum
{
  eComRX=1,
  eComTX=2
}
EComDirection;
  #endif


void vALCom_SendMessage_m(ULONG ulTxId,
                          UCHAR *pucData,
                          UINT   uiBytes,
                          char  *pcDbgText);

void vUds_SendCmd_m(long lId,
                    unsigned char *pucTxData,
                    unsigned int uiDlc,
                    char *pcMessage);

typedef struct
{
  unsigned long TxID;
  unsigned long RxID;
} Uds_CANType;



void vUds_IoCtrlInfoAllocMem_m(void);
void vUds_IoCtrlInfoFreeMem_m(void);
void vUds_IoCtrlInfoDump_m(void);

void vUds_IOControlByCommonIdentifierInvalidateCtrlValue_m(unsigned short usLocalIdentifier);

BOOL bUds_IOControlByCommonIdentifierGetControlInfos_m(unsigned short usLocalIdentifier,
                                                       BOOL *pbWrite,
                                                       BOOL *pbRead,
                                                       int  *piSize,
                                                       char *pcDescription);
int iUds_IOControlByCommonIdentifierGetControlSize_m(unsigned short usLocalIdentifier);
BOOL bUds_IOControlByCommonIdentifierGetWritable_m(unsigned short usLocalIdentifier);
BOOL bUds_IOControlByCommonIdentifierGetReadable_m(unsigned short usLocalIdentifier);

char *pcUds_IOControlByCommonIdentifierGetDescription_m(unsigned short usLocalIdentifier);
void vUds_IOControlByCommonIdentifierSetCtrlValue_m(unsigned short usLocalIdentifier,
                                                    char *pcValue);
BOOL bUds_IOControlByCommonIdentifierGetCtrlValue_m(unsigned short usLocalIdentifier,
                                                    char *pcValue,
                                                    int  *piBytes);
BOOL *pbUds_IOControlByCommonIdentifier_m(unsigned short usLocalIdentifier,
                                          EIoCtrlMode    eIoCtrlMode,
                                          unsigned char *pucIoCtrlState,
                                          char          *pcMessage);


BOOL *pbUds_ReadDataByIdentifier_m(unsigned short usIdentifier,
                                   char          *pcMessage);
BOOL bUds_ReadDataByIdentifierGetCtrlValue_m(unsigned short usLocalIdentifier,
                                             char *pcValue,
                                             int  *piBytes);


BOOL bUds_StoredDataByIdentifierGetControlInfos_m(unsigned short usLocalIdentifier,
                                                  BOOL *pbWrite,
                                                  BOOL *pbRead,
                                                  int  *piSize,
                                                  char *pcDescription);
int iUds_StoredDataByIdentifierGetControlSize_m(unsigned short usLocalIdentifier);
BOOL bUds_StoredDataByIdentifierGetWritable_m(unsigned short usLocalIdentifier);
BOOL bUds_StoredDataByIdentifierGetReadable_m(unsigned short usLocalIdentifier);

char *pcUds_StoredDataByIdentifierGetDescription_m(unsigned short usLocalIdentifier);


typedef enum
{
  eRoutineByLocalId_DisplayTest,            // startRoutineByLocalId - SID 31h, 01h, FAh, 0Ch  Manufacturing Display Test:
  eRoutineByLocalId_TelltaleTest,           // startRoutineByLocalId - SID 31h, 01h, FAh, 0Ah  Telltale Test:
  eRoutineByLocalId_AdcRawData,             // startRoutineByLocalId - SID 31h 01h FAh 0Bh  ADC Raw data:
  eRoutineByLocalId_GaugeSetSteps,          // startRoutineByLocalId - SID 31h, 01h, FAh, 12h  Gauge set steps:
  //eRoutineByLocalId_StaticCanTest,          // startRoutineByLocalId - SID 31h, FAh, 0Ah  Static CAN Test:
  //eRoutineByLocalId_CanTestTx,              // startRoutineByLocalId - SID 31h, FAh, 0Dh  CAN Test TX:
  //eRoutineByLocalId_CanTestRx,              // startRoutineByLocalId - SID 31h, FAh, 0Bh  CAN Test RX:
  //eRoutineByLocalId_LcdContrastVoltage,     // startRoutineByLocalId - SID 31h, FAh, 0Eh  Dot-Matrix Contrast Voltage:
  //eRoutineByLocalId_LcdContrastVoltageRead, // startRoutineByLocalId - SID 31h, FAh, 11h  Dot-Matrix Contrast Voltage read:
  //eRoutineByLocalId_PlcStop,                // stopRoutineByLocalId  - SID 32h, FCh, 01h  plcStop:
  //eRoutineByLocalId_PlcStart,               // startRoutineByLocalId - SID 31h, FCh, 01h  plcStart:
  //eRoutineByLocalId_TransferEEVar,          // startRoutineByLocalId - SID 31h, FCh, 02h  transferEepromVar:
  //eRoutineByLocalId_TransferEEParam,        // startRoutineByLocalId - SID 31h, FCh, 03h  transferEepromParam.:
  //eRoutineByLocalId_GetCustomerNr,          // startRoutineByLocalId - SID 31h, FCh, 20h  getCustomerNumber:
  //eRoutineByLocalId_OdoWrite                // startRoutineByLocalId - SID 31h, FCh, 21h  ODOWrite:
} ERoutineByLocalId;


BOOL *pbUds_StartRoutineByLocalId_m(long lTxId,
                                    ERoutineByLocalId eRoutine,
                                    unsigned char ucParam1,
                                    unsigned char ucParam2,
                                    unsigned char ucParam3,
                                    unsigned char ucParam4,
                                    unsigned char ucParam5);




void vUds_ParseRxData_m(unsigned char *pucRxBuffer,
                        unsigned int uiBytes);



void vUds_StoredDataInfoAllocMem_m(void);
void vUds_StoredDataInfoFreeMem_m(void);

void vUds_SaveStoredDataByDid_m(unsigned int   uiLocalIdentifier,
                                unsigned char *pucData);
BOOL bUds_GetStoredDataByDid_m(unsigned int   uiIdentifier,
                               unsigned char *pucData,
                               unsigned int  *puiDataSize);

  #if 0
/**
 * Achtung:
 *
 * Reihenfolge der struct-Member muss mit der Initialisierung von atagMemoryInfo_m[] übereinstimmen!
 *
 * @author uidc9013 (16.03.2010)
 */
typedef struct
{
  unsigned long  ulAddress;
  int            iSize;   // size of one element
  int            iCount;   // number of elements
  int            iAccessLevel;
  BOOL           bValid;
  char          *pcMemoryValue;
  char          *pcName;
  char          *pcDescription;
}
TagMemoryInfo;
  #endif

//void vUds_MemoryInfoAllocMem_m(void);
//void vUds_MemoryInfoFreeMem_m(void);

//void vUds_InvalidateMemoryData_m(unsigned long  ulAddress);

//extern unsigned long ulMemoryAccessAddress_m;
//extern unsigned int uiMemoryAccessSize_m;
//void vUds_StoreMemoryData_m(unsigned char *pucData);

//BOOL bUds_GetMemoryData_m(unsigned long  ulAddress,
//                          unsigned char *pucData,
//                          unsigned int  *puiDataSize);


/**
 *
 * Allgemeine Funktionen
 *
 */
  #if 0
void vTraceAppend_m(EComDirection eComDirection,
                    unsigned long ulMsgId,
                    unsigned char *pcData,
                    int  iBytes,
                    char *pcText);

void vTraceAppendText_m(char * pcFormat, ...);
  #endif

void vUds_TraceErrorCode_m(unsigned char ucMsgCode,
                           unsigned char ucErrorCode);



void vUds_CheckBufferOverrun_m(int iPortNr);


void vUds_GetDtcGroup_m(unsigned int iSpn,
                        unsigned int iFmi,
                        unsigned char *pucByte1,
                        unsigned char *pucByte2,
                        unsigned char *pucByte3);

void vUds_CalcKey_m(EUdsSecurityAccess  eSecurityAccess,
                    unsigned char          *pucKey,
                    int                    *piSize);


#endif // !UDS_PRIVATE_H_INCLUDED

