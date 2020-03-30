/*******************************************************************
 *
 *    PROJECT:     FlexCluster
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        UDS_private.c
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

#include <windows.h>
#ifdef _CVI_
  #include <formatio.h>
  #include <utility.h>
  #include <ansi_c.h>
#else // !_CVI_
  #include <stdlib.h>
  #include <stdio.h>
#endif // !_CVI_
#include <canlib.h>

#include "Uds.h"
#include "Uds_private.h"
#include "Uds_IoControl.h"
#include "Uds_StoredData.h"

#include "Trace.h"


/**
 * FlexCluster antwortet auf (fast) alle Telegramme mit NAK (Code Run) 0x7F 0xXX 0x78
 * was bedeutet, dass er das Telegramm empfangen hat und noch am bearbeiten ist.
 * Erst am Schluss der Bearbeitung antwortet er dann mit ACK.
 */
#define SWALLOW_CAN_NAK        // NAK (Code Run) schlucken --> NAK ignorieren und auf ACK warten
//#define SWALLOW_CAN_NAK_TRACE  // NAK (Code Run) schlucken Tracen


extern BOOL bUds_SuppressTesterPresentAutomatic_m;
extern BOOL bUds_SuppressTesterPresentByUser_m;


HANDLE hThreadSender_m=NULL;

// _WIN32_WINNT > 0x0400 is needed to enable
// inclusion of TryEnterCriticalSection()
//
// _WIN32_WINNT==0x0400 means Windows2000
//
#if _WIN32_WINNT < 0x0400
  #error _WIN32_WINNT should be at least defined as 0x0400 to use TryEnterCriticalSection()
#endif

CRITICAL_SECTION csSender_m;
CRITICAL_SECTION csReadWrite_m;


BOOL bIoCtrlInfoInitialized_m=FALSE;
extern TagIoCtrlInfo atagIoCtrlInfo_m[];

BOOL bStoredDataInitialized_m=FALSE;
extern TagStoredDataInfo atagStoredData_m[];


/**
 * Seed für berechnung von Key für Security-access
 */
#define SEED_KEY_SIZE 4   // 2 oder 4 byte für Key/Seed
unsigned char ucaSeed_m[SEED_KEY_SIZE];
//EUdsSecurityAccess eCurrentSecurityAccess_m;

#if SEED_KEY_SIZE!=2
  #if SEED_KEY_SIZE!=4
    #error SEED_KEY_SIZE falsch! Darf nur 2 oder 4 sein.
  #endif
#endif


/**
 * bUds_WaitBoolFlag_m()
 *
 * Wartet bis Bool-Flag den geforderten Zustand hat.
 *
 * @author uidc9013 (12.06.2009)
 * @param pbFlag Pointer auf abzuwartendes Flag
 * @param bState gewünschter Zustand
 * @param ulTimeoutMs
 *               Wartezeit in ms
 *
 * @return TRUE, wenn der Zustand erreicht wurde<BR>
 *         FALSE bei timeout
 */
BOOL bUds_WaitBoolFlag_m(BOOL *pbFlag,
                         BOOL bState,
                         unsigned long ulTimeoutMs)
{
  unsigned long ulTimePassed=0;
  unsigned long ulTimeStep=10;   // Verzögerungszeit beim Pollen

  if(ulTimeStep>ulTimeoutMs)
    ulTimeStep=ulTimeoutMs;

  do
  {
    Sleep(ulTimeStep);
    ulTimePassed+=ulTimeStep;
    if(ulTimePassed>=ulTimeoutMs)
    {
      // Timeout
      return(FALSE);
    }
  } while((*pbFlag)!=bState);
  //printf("ulTimePassed = %d\n",ulTimePassed);
  return(TRUE);
} // bUds_WaitBoolFlag_m()



void vUds_SendTesterPresent_m(void)
{
  unsigned char ucaTxData[16];
  int iIndex=0;

  bUds_AckTesterPresent_m=FALSE;
  memset(ucaTxData,0,sizeof(ucaTxData));
  ucaTxData[iIndex++]=0x3E;   // Service "tester present"
  ucaTxData[iIndex++]=0x00;   // 0x01: response required
                              // 0x02: response not required

  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,2,"Tester present");
} // vUds_SendTesterPresent_m();



/**
 * Zuordnungen RxId zu TxId
 */
const Uds_CANType Uds_CAN[]=
{ // Tx             Rx
  //{0X18DA1700,      0X18DA0017},
  { ECU_CAN_ADDRESS, 0X07C2 },
  { 0x18FF0100     , 0X18FF0017 },   // TODO: set RX-ID
  //{0x7E2,      0x7EA},      // Uds_BCM
  //{0x7F2,      0x7FA},      // Uds_SPD
  //{0x7F3,      0x7FB},      // Uds_TAC
  //{0x7DF,      0x7FA},      //
  { 0,          0 }           // Terminierung
};

long vUds_GetRxId_m(long lTxId)
{
  int iIndex;

  for(iIndex=0;
      Uds_CAN[iIndex].TxID!=0;
      iIndex++)
  {
    if(Uds_CAN[iIndex].TxID==lTxId)
    {
      return(Uds_CAN[iIndex].RxID);
    }
  }
  if(bTraceEnabled_m)
  {
    vTrace_AppendText_g("vUds_GetRxId_m() no Rx for 0x%08x",lTxId);
  }
  return(0);
} // vUds_GetRxId_m()


typedef struct
{
  long lTxId;
  unsigned char ucaTxData[100];
  unsigned int uiTxDataLen;
  char caMessage[200];
}
TagUdsCmdInfo;

TagUdsCmdInfo tagUdsCmdInfo;


DWORD WINAPI dwUds_SendCmd_m(LPVOID lpParam)
{
  int iPortNr_m;
  long lTxId;
  char caMessage[200];
  char *pcMessage;
  char caMsg[100];
  char *pcSendData;
  char caErrorText[100];
  unsigned int uiTxDataLen;   // bytes zum senden gesammt
  unsigned int uiTxSize;      // bytes für aktuelles Telegramm
  unsigned long ulTxTimeoutMs=500;
  unsigned long ulRxTimeoutMs=500;

  long lRxId;
  unsigned char ucaTxBuffer[20];
  unsigned char ucaRxBuffer[20];
  unsigned char ucaRxBufferAccu[200];    // Buffer zum sammeln aller einzelnen zusammengehörenden Telegramme
  unsigned int  uiRxDataLen=0;
  unsigned int  uiTxFrameCounter;
  unsigned int  uiTxStartPos;
  unsigned int  uiRxStartPos;
  unsigned int  uiBytesToReceive;
  canStatus     canRc;
  int iTxFrameType;
  int iRxFrameType;

#define MAX_READ_TRIES 100
  int iReadTry;

  TagUdsCmdInfo tagUdsCmdInfo;

  if(!TryEnterCriticalSection(&csReadWrite_m))
  {
    // CriticalSection already owned
    //printf("CriticalSection 2 owned\n");
    //vTrace_AppendText_g("*** CriticalSection ReadWrite 1 owned");
    ExitThread(0);
  }

  iPortNr_m=canOpenChannel(0,canOPEN_EXCLUSIVE);
  if(iPortNr_m<0)
  {
    char caErrMessage[200];
    if(iPortNr_m==canERR_NOTFOUND)
    {
#ifdef _CVI_
      MessagePopup("CAN ERROR","Kvaser CAN-interface not found");
#else // !_CVI_
      printf("Kvaser CAN-interface not found\n");
#endif // !_CVI_
    }
    else
    {
      canGetErrorText((canStatus)iPortNr_m,caErrMessage,sizeof(caErrMessage));
#ifdef _CVI_
      MessagePopup("CAN ERROR",caErrMessage);
#else // !_CVI_
      printf("CAN ERROR: %s\n",caErrMessage);
#endif // !_CVI_
    }
    goto labelAbortWrite;
  }

  canRc=canSetBusParams(iPortNr_m,iaUds_Baudrate_m[0],0,0,0,0,0);
  //if(canRc!=canOK) printf("canSetBusParams() returned %d\n",canRc);

  canRc=canSetBusOutputControl(iPortNr_m,canDRIVER_NORMAL);
  //if(canRc!=canOK) printf("canSetBusOutputControl() returned %d\n",canRc);

  canRc=canBusOn(iPortNr_m);
  //if(canRc!=canOK) printf("canBusOn() returned %d\n",canRc);


  canFlushReceiveQueue(iPortNr_m);
  canFlushTransmitQueue(iPortNr_m);


  memcpy((void*)&tagUdsCmdInfo,lpParam,sizeof(TagUdsCmdInfo));

  lTxId=tagUdsCmdInfo.lTxId;
  strcpy(caMessage,tagUdsCmdInfo.caMessage);

  uiTxDataLen=tagUdsCmdInfo.uiTxDataLen;
  memcpy(caMsg,tagUdsCmdInfo.ucaTxData,uiTxDataLen);

  pcSendData=caMsg;
  pcMessage=caMessage;


  memset(ucaTxBuffer,0x0,sizeof(ucaTxBuffer));
  uiTxStartPos=0;

  if(uiTxDataLen<8)
  {
    // Single Frame
    //
    // byte 0
    // xxxxxxxx
    // ====----> Frame Type
    //     ====----> Message length
    //
    iTxFrameType=0x00;   // 0x0_ = Single Frame
    ucaTxBuffer[0]=(UCHAR)iTxFrameType;
    ucaTxBuffer[0]=(uiTxDataLen&0xff);   // Message length bits 0-3
    memcpy(&ucaTxBuffer[1],pcSendData,uiTxDataLen);
  }
  else
  {
    // First Frame of multiple frames
    //
    // byte 0   byte 1
    // xxxxxxxx xxxxxxxx
    // ====----> Frame Type
    //     =============----> Message length
    //
    iTxFrameType=0x10;   // 0x1_ = First Frame
    ucaTxBuffer[0]=(UCHAR)iTxFrameType;
    if(uiTxDataLen>0xff)
      ucaTxBuffer[0]|=(0xf&(uiTxDataLen%0xff));   // 0x_x = Message length bits 8-11
    ucaTxBuffer[1]=(uiTxDataLen&0xff);   // Message length bits 0-7

    memcpy(&ucaTxBuffer[2],pcSendData,6);
  }


  canRc=canFlushReceiveQueue(iPortNr_m);
  //if(canRc!=canOK) printf("canFlushReceiveQueue() returned %d\n",canRc);

  if(uiTxDataLen>7)
    uiTxSize=8;
  else
    uiTxSize=uiTxDataLen+1;
  canRc=canWriteWait(iPortNr_m,lTxId,ucaTxBuffer,uiTxSize,canMSG_STD,ulTxTimeoutMs);
  if(bTraceEnabled_m)
  {
    vTrace_AppendMessageData_g(eComTX,lTxId,ucaTxBuffer,uiTxSize,pcMessage);
  }

  if(canRc!=canOK)
  {
    canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
    if(bTraceEnabled_m)
    {
      vTrace_AppendText_g("1 canWriteWait(0x%04x) returned %d (%s)",lTxId,canRc,caErrorText);
    }
    goto labelAbortWrite;
  }

  memset(ucaRxBuffer,0,sizeof(ucaRxBuffer));
  lRxId=vUds_GetRxId_m(lTxId);
  if(lRxId!=0)
  {
    canRc=canReadSyncSpecific(iPortNr_m,lRxId,ulRxTimeoutMs);
    if(canRc==canERR_TIMEOUT)
    {
      if(bTraceEnabled_m)
      {
        vTrace_AppendText_g("dwUds_SendCmd_m() Timeout 1 0x%08lx",lRxId);
      }
      goto labelAbortWrite;
    }
    canRc=canReadSpecificSkip(iPortNr_m,lRxId,&ucaRxBuffer,&uiRxDataLen,NULL,NULL);
    if(canRc==canOK)
    {
      if((ucaRxBuffer[1]==0x7F)
         &&(ucaRxBuffer[3]==0x78)
        )
      {
        //if(bTraceEnabled_m)
        //{
        //  vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"swallow NAK (Code Run)");
        //}
        ;
      }
      else
      {
        if(bTraceEnabled_m)
        {
          vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"");
        }
      }
    }
    if(canRc==canERR_NOMSG)
    {
      if(bTraceEnabled_m)
      {
        vTrace_AppendText_g("1. no answer received");
      }
    }

    if((ucaRxBuffer[1]==0x7F)
       &&(ucaRxBuffer[3]==0x78)
      )
    {
      // NAK -> auf ACK warten
      canRc=canReadSyncSpecific(iPortNr_m,lRxId,ulRxTimeoutMs);
      if(canRc==canERR_TIMEOUT)
      {
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("dwUds_SendCmd_m() Timeout 2 0x%08lx",lRxId);
        }
        goto labelAbortWrite;
      }
      canRc=canReadSpecific(iPortNr_m,lRxId,&ucaRxBuffer,&uiRxDataLen,NULL,NULL);
      if(canRc==canOK)
      {
        if(bTraceEnabled_m)
        {
          vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"");
        }
      }
      else
      {
        canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("canReadSpecific() returned %d (%s)",lTxId,canRc,caErrorText);
        }
        goto labelAbortWrite;
      }
    }
  }
  else
  {
    //printf("no RxId for TxId 0x%04x\n",lTxId);
    goto labelAbortWrite;
  }

  vUds_CheckBufferOverrun_m(iPortNr_m);

  iRxFrameType=ucaRxBuffer[0]&0xf0;

  if(iTxFrameType==0x00)
  {
    // Single Frame gesendet
    // First Frame von Gerät empfangen
    if(iRxFrameType==0x00)
    {
      // Single Frame
      // RX-Daten von CAN (ucaRxBuffer) in entsprechende Strukturen speichern
      vUds_ParseRxData_m(&ucaRxBuffer[1],uiRxDataLen-1);
    }
    else if(iRxFrameType==0x10)
    {
      // First Frame
      //printf("1. First Frame von Gerät empfangen 0x%02x\n",ucaRxBuffer[0]);
      // mit Flow Control antworten

      iTxFrameType=0x30;   // 0x3_ = Flow control
      ucaTxBuffer[0]=(UCHAR)iTxFrameType;
      ucaTxBuffer[1]=0x00;   // Block size
      ucaTxBuffer[2]=0x0A;
      uiTxSize=3;
      canRc=canWriteWait(iPortNr_m,lTxId,ucaTxBuffer,uiTxSize,canMSG_STD,ulTxTimeoutMs);
      if(bTraceEnabled_m)
      {
        vTrace_AppendMessageData_g(eComTX,lTxId,ucaTxBuffer,uiTxSize,"Flow Control");
      }
      if(canRc!=canOK)
      {
        canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("2 canWriteWait(0x%04x) returned %d (%s)",lTxId,canRc,caErrorText);
        }
      }

      uiBytesToReceive=ucaRxBuffer[1];

      memcpy(ucaRxBufferAccu,&ucaRxBuffer[2],6);
      uiRxStartPos=6;   // 6 byte aus First Frame mitzählen
      lRxId=vUds_GetRxId_m(lTxId);
      iReadTry=0;
      if(lRxId!=0)
      {
        do
        {
          Sleep(1);  // sonst manchmal beim schreiben der ProdDaten probleme canReadSyncSpecific
          canRc=canReadSyncSpecific(iPortNr_m,lRxId,100);
          if(canRc!=canOK)
          {
            //printf("1. canReadSyncSpecific() no data received with Id=0x%04x\n",lRxId);
            goto labelAbortWrite;
          }
          canRc=canReadSpecificSkip(iPortNr_m,lRxId,ucaRxBuffer,&uiRxDataLen,NULL,NULL);
          switch(canRc)
          {
            case canERR_NOMSG:
              iReadTry++;
              break;
            case canOK:
              if(bTraceEnabled_m)
              {
                if(uiRxDataLen>8)
                  vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,8,"consecutive frame");
                else
                  vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"consecutive frame");
              }
              if(iReadTry>0)
                //printf("iReadTry=%d\n",iReadTry);
              iReadTry=0;
              memcpy(&ucaRxBufferAccu[uiRxStartPos],&ucaRxBuffer[1],7);
              uiRxStartPos+=7;
              break;
            //default:
              //printf("canReadSpecificSkip(0x%04x) returned %d\n",lRxId,canRc);
          }
        } while((uiRxStartPos<uiBytesToReceive)
                &&(iReadTry<MAX_READ_TRIES)
               );

        if(iReadTry>MAX_READ_TRIES-1)
        {
          //printf("canReadSpecificSkip(0x%04x) aborting\n",lRxId);
        }

        // RX-Daten von CAN (ucaRxBuffer) in entsprechende Strukturen speichern
        vUds_ParseRxData_m(ucaRxBufferAccu,uiBytesToReceive);
      }
    }
    /*
    else
      printf("??? unerwarteter Frame von Gerät empfangen 0x%02x\n",ucaRxBuffer[0]);
    */
  }
  else
  {
    // First Frame gesendet. Weitere Frames senden
    uiTxFrameCounter=1;

    if((ucaRxBuffer[0]&0x30)!=0x30)
    {
      // kein Flow-Control telegramm erhalten
      //printf("canReadSpecificSkip(0x%04x) kein Flow-Control telegramm erhalten (0x%02x)\n",lRxId,ucaRxBuffer[0]);
      //bUds_GenNak_m=FALSE;
      ;
    }

    if(ucaRxBuffer[1]!=0)   // Blocksize
    {
      // TODO: andere Blocksize als 0 auch behandeln?
      //vTraceAppendText_m("Blocksize=%d empfangen! noch nicht implementiert",ucaRxBuffer[1]);
      ;
    }

    uiTxDataLen-=6;
    uiTxStartPos+=6;
    do
    {
      // den Rest ohne weitere Flow-Controls senden
      memset(ucaTxBuffer,0x0,sizeof(ucaTxBuffer));
      ucaTxBuffer[0]=(UCHAR)(0x20+uiTxFrameCounter++);
      if(uiTxDataLen>7)
      {
        // ersten oder weiteren Consecutive Frame senden
        memcpy(&ucaTxBuffer[1],&pcSendData[uiTxStartPos],7);
        uiTxStartPos+=7;
        uiTxSize=8;
      }
      else
      {
        // letzen Consecutive Frame senden
        memcpy(&ucaTxBuffer[1],&pcSendData[uiTxStartPos],uiTxDataLen+1);
        uiTxStartPos+=uiTxDataLen;
        uiTxSize=uiTxDataLen+1;
        bUds_GenAck_m=TRUE;
      }

      canRc=canWriteWait(iPortNr_m,lTxId,ucaTxBuffer,uiTxSize,canMSG_STD,ulTxTimeoutMs);
      if(bTraceEnabled_m)
      {
        vTrace_AppendMessageData_g(eComTX,lTxId,ucaTxBuffer,uiTxSize,"consecutive frame");
      }
      if(canRc!=canOK)
      {
        canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("3 canWriteWait(0x%04x) returned %d (%s)",lTxId,canRc,caErrorText);
        }
        goto labelAbortWrite;
      }
      if(uiTxDataLen>7)
        uiTxDataLen-=7;
      else
        uiTxDataLen=0;

      Sleep(10);
    } while(uiTxDataLen>0);

    Sleep(50);

    lRxId=vUds_GetRxId_m(lTxId);
    if(lRxId==0)
    {
      // nichts zu empfangen
      //ReleaseMutex(hReadWriteMutex_m);
      goto labelAbortWrite;
    }

    canRc=canReadSyncSpecific(iPortNr_m,lRxId,ulRxTimeoutMs);
    if(canRc==canERR_TIMEOUT)
    {
      if(bTraceEnabled_m)
      {
        vTrace_AppendText_g("dwUds_SendCmd_m() Timeout 3 0x%08lx",lRxId);
      }
      goto labelAbortWrite;
    }

    canRc=canReadSpecificSkip(iPortNr_m,lRxId,&ucaRxBuffer,&uiRxDataLen,NULL,NULL);
    if(canRc==canERR_NOMSG)
    {
      if(bTraceEnabled_m)
      {
        vTrace_AppendText_g("2. no answer received");
      }
      goto labelAbortWrite;
    }
    if(canRc!=canOK)
      goto labelAbortWrite;


    // Antwort empfangen
    if((ucaRxBuffer[1]==0x7F)
       &&(ucaRxBuffer[3]==0x78)
      )
    {
      //if(bTraceEnabled_m)
      //{
      //  vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"swallow NAK (Code Run)");
      //}
      ;
    }
    else
    {
      if(bTraceEnabled_m)
        vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"");
    }

    if((ucaRxBuffer[1]==0x7F)
       &&(ucaRxBuffer[3]==0x78)
      )
    {
      // NAK -> auf ACK warten
      canRc=canReadSyncSpecific(iPortNr_m,lRxId,ulRxTimeoutMs);
      if(canRc==canERR_TIMEOUT)
      {
        //vTraceAppendText_m("dwUds_SendCmd_m() Timeout 4 0x%08lx",lRxId);
        goto labelAbortWrite;
      }
      canRc=canReadSpecific(iPortNr_m,lRxId,&ucaRxBuffer,&uiRxDataLen,NULL,NULL);
      if(canRc==canOK)
      {
        if(bTraceEnabled_m)
          vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"");
      }
      else
      {
        canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("canReadSpecific() returned %d (%s)",lTxId,canRc,caErrorText);
        }
        goto labelAbortWrite;
      }
    }


    iRxFrameType=ucaRxBuffer[0]&0xF0;
    if(iRxFrameType==0x00)
    {
      // single-frame telegram empfangen
      vUds_ParseRxData_m(&ucaRxBuffer[1],uiRxDataLen-1);
    }
    else
    {
      // Multi-frame telegram empfangen
      uiRxDataLen=ucaRxBuffer[1];

      // First Frame
      //printf("2. First Frame von Gerät empfangen 0x%02x\n",ucaRxBuffer[0]);
      // mit Flow Control antworten

      iTxFrameType=0x30;   // 0x3_ = Flow control
      ucaTxBuffer[0]=(UCHAR)iTxFrameType;
      ucaTxBuffer[1]=0x00;   // Block size
      ucaTxBuffer[2]=0x0A;
      canRc=canWriteWait(iPortNr_m,lTxId,ucaTxBuffer,3,canMSG_STD,ulTxTimeoutMs);
      if(bTraceEnabled_m)
      {
        vTrace_AppendMessageData_g(eComTX,lTxId,ucaTxBuffer,3,"Flow Control");
      }
      if(canRc!=canOK)
      {
        canGetErrorText(canRc,caErrorText,sizeof(caErrorText));
        if(bTraceEnabled_m)
        {
          vTrace_AppendText_g("4 canWriteWait(0x%04x) returned %d (%s)",lTxId,canRc,caErrorText);
        }
        goto labelAbortWrite;
      }
      //Sleep(10);

      uiBytesToReceive=ucaRxBuffer[1];

      memcpy(ucaRxBufferAccu,&ucaRxBuffer[2],6);
      uiRxStartPos=6;   // 6 byte aus First Frame mitzählen
      lRxId=vUds_GetRxId_m(lTxId);
      iReadTry=0;
      if(lRxId!=0)
      {
        do
        {
          canRc=canReadSyncSpecific(iPortNr_m,lRxId,ulRxTimeoutMs);
          if(canRc!=canOK)
          {
            //printf("2. canReadSyncSpecific() no data received with Id=0x%04x\n",lRxId);
            goto labelAbortWrite;
          }
          canRc=canReadSpecificSkip(iPortNr_m,lRxId,ucaRxBuffer,&uiRxDataLen,NULL,NULL);
          switch(canRc)
          {
            case canERR_NOMSG:
              iReadTry++;
              break;
            case canOK:
              if(bTraceEnabled_m)
              {
                vTrace_AppendMessageData_g(eComRX,lRxId,ucaRxBuffer,uiRxDataLen,"consecutive frame");
              }
              if(iReadTry>0)
                //printf("iReadTry=%d\n",iReadTry);
              iReadTry=0;
              memcpy(&ucaRxBufferAccu[uiRxStartPos],&ucaRxBuffer[1],7);
              uiRxStartPos+=7;
              break;
            //default:
              //printf("canReadSpecificSkip(0x%04x) returned %d\n",lRxId,canRc);
          }
        } while((uiRxStartPos<uiBytesToReceive)
                &&(iReadTry<MAX_READ_TRIES)
               );

        if(iReadTry>MAX_READ_TRIES-1)
        {
          //printf("canReadSpecificSkip(0x%04x) aborting\n",lRxId);
        }

        // RX-Daten von CAN (ucaRxBuffer) in entsprechende Strukturen speichern
        vUds_ParseRxData_m(ucaRxBufferAccu,uiBytesToReceive);
      }
    }
  }

labelAbortWrite:
  canBusOff(iPortNr_m);
  canClose(iPortNr_m);

  LeaveCriticalSection(&csReadWrite_m);

  ExitThread(0);
  return(0);   // to satisfy the compiler
} // dwUds_SendCmd_m()



char *pcALCom_canGetErrorText_m(canStatus err)
{
  static char caErrorText[200+1];
  canGetErrorText(err,caErrorText,200);
  return(caErrorText);
}



/**
 * This function sends a CAN message. It returns when the
 * message is sent, or the timeout expires.
 *
 * @param ulTxId    The identifier of the CAN message to send.
 * @param pucData   Data to be sent.
 * @param uiBytes   The length of the message. Can be at most 8.
 * @param pcDbgText diagnostic message for trace.
 */
void vALCom_SendMessage_m(ULONG ulTxId,
                          UCHAR *pucData,
                          UINT   uiBytes,
                          char  *pcDbgText)
{
  canStatus canRc;
  UINT uiFlag;
  int iPortNr_m;

  if(!TryEnterCriticalSection(&csReadWrite_m))
  {
    // CriticalSection already owned
    //printf("CriticalSection 2 owned\n");
    //vTrace_AppendText_g("*** CriticalSection ReadWrite 1 owned");
    return;
  }

  iPortNr_m=canOpenChannel(0,canOPEN_EXCLUSIVE);
  if(iPortNr_m<0)
  {
    char caErrMessage[200];
    if(iPortNr_m==canERR_NOTFOUND)
    {
#ifdef _CVI_
      MessagePopup("CAN ERROR","Kvaser CAN-interface not found");
#else // !_CVI_
      printf("Kvaser CAN-interface not found\n");
#endif // !_CVI_
    }
    else
    {
      canGetErrorText((canStatus)iPortNr_m,caErrMessage,sizeof(caErrMessage));
#ifdef _CVI_
      MessagePopup("CAN ERROR",caErrMessage);
#else // !_CVI_
      printf("CAN ERROR: %s\n",caErrMessage);
#endif // !_CVI_
    }
    return;
  }

  canRc=canSetBusParams(iPortNr_m,iaUds_Baudrate_m[0],0,0,0,0,0);
  //if(canRc!=canOK) printf("canSetBusParams() returned %d\n",canRc);

  canRc=canSetBusOutputControl(iPortNr_m,canDRIVER_NORMAL);
  //if(canRc!=canOK) printf("canSetBusOutputControl() returned %d\n",canRc);

  canRc=canBusOn(iPortNr_m);
  //if(canRc!=canOK) printf("canBusOn() returned %d\n",canRc);

  canFlushReceiveQueue(iPortNr_m);
  canFlushTransmitQueue(iPortNr_m);

  if(ulTxId>0x7FF)
    uiFlag=canMSG_EXT;
  else
    uiFlag=canMSG_STD;
  canRc=canWriteWait(iPortNr_m,ulTxId,pucData,uiBytes,uiFlag,500);

  if(bTraceEnabled_m)
  {
    vTrace_AppendMessageData_g(eComTX,  // EComDirection eComDirection,
                               ulTxId,  // unsigned long ulMsgId,
                               pucData, // unsigned char *pcData,
                               uiBytes,       // int  iBytes,
                               pcDbgText); // char *pcText, ...);
  }

  if(canRc!=canOK)
  {
    vTrace_AppendText_g("canWriteWait() returned \"%s\"",pcALCom_canGetErrorText_m(canRc));
    goto labelAbortWrite2;
  }

labelAbortWrite2:
  canBusOff(iPortNr_m);
  canClose(iPortNr_m);

  LeaveCriticalSection(&csReadWrite_m);

} // vALCom_SendMessage_m()





/**
 * This function sends a CAN message. It returns when the
 * message is sent, or the timeout expires.
 *
 * @param lTxId     The identifier of the CAN message to send.
 * @param pucTxData
 * @param uiTxDataLen
 *                  The length of the message. Can be at most 8.
 * @param pcMessage diagnostic message for trace.
 */
void vUds_SendCmd_m(long lTxId,
                    unsigned char *pucTxData,
                    unsigned int uiTxDataLen,
                    char *pcMessage)
{
  DWORD dwThreadId;

  if(!bUds_IsInitialized_g())
  {
    //printf("vUds_SendCmd_m(): Port noch nicht initialisiert\n");
    return;
  }

  if(!TryEnterCriticalSection(&csSender_m))
  {
    // CriticalSection already owned
    //vTrace_AppendText_g("*** CriticalSection Sender owned");
    return;
  }

  bUds_GenAck_m=FALSE;
  bUds_GenNak_m=FALSE;

  tagUdsCmdInfo.lTxId=lTxId;
  tagUdsCmdInfo.uiTxDataLen=uiTxDataLen;
  memcpy(tagUdsCmdInfo.ucaTxData,pucTxData,uiTxDataLen);
  strcpy(tagUdsCmdInfo.caMessage,pcMessage);

  hThreadSender_m=CreateThread(NULL,                          // __in_opt  lpSecurityDescriptor
                               0,                             // __in      StackSize
                               dwUds_SendCmd_m,           // __in      lpStartAddress,
                               (void*)(&tagUdsCmdInfo),   // __in_opt  lpParameter,
                               0,                             // __in      dwCreationFlags,
                               &dwThreadId);                  // __out_opt lpThreadId
  //printf("1. dwThreadId = %d\n",dwThreadId);

  LeaveCriticalSection(&csSender_m);

} // vUds_SendCmd_m()



/**
 * Daten-Speicherplatz für die IoControl-Struktur allozieren
 *
 * @author uidc9013 (26.02.2010)
 */
void vUds_IoCtrlInfoAllocMem_m(void)
{
  int iIndex;

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    atagIoCtrlInfo_m[iIndex].pcIoCtrlValue=malloc(atagIoCtrlInfo_m[iIndex].iSize);
    memset(atagIoCtrlInfo_m[iIndex].pcIoCtrlValue,0,atagIoCtrlInfo_m[iIndex].iSize);
  }
  bIoCtrlInfoInitialized_m=TRUE;
} // vUds_IoCtrlInfoAllocMem_m()


/**
 * Daten-Speicherplatz für die IoControl-Struktur freigeben
 *
 * @author uidc9013 (26.02.2010)
 */
void vUds_IoCtrlInfoFreeMem_m(void)
{
  int iIndex;

  if(!bIoCtrlInfoInitialized_m)
  {
    // Es wurde nichts allozieret --> nichts zum freigeben
    return;
  }

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    free(atagIoCtrlInfo_m[iIndex].pcIoCtrlValue);
  }
  bIoCtrlInfoInitialized_m=FALSE;
} // vUds_IoCtrlInfoFreeMem_m()


void vUds_IoCtrlInfoDump_m(void)
{
  int iIndex;
  int iByteIndex;

  iIndex=0;
  while(atagIoCtrlInfo_m[iIndex].iId!=0)
  {
    if(atagIoCtrlInfo_m[iIndex].bValid)
    {
      printf("%s  0x",atagIoCtrlInfo_m[iIndex].pcDescription);
      for(iByteIndex=0;
          iByteIndex<atagIoCtrlInfo_m[iIndex].iSize;
          iByteIndex++)
      {
        printf("%02x ",(unsigned char)atagIoCtrlInfo_m[iIndex].pcIoCtrlValue[iByteIndex]);
      }
      putchar('\n');
    }
    ++iIndex;
  }
} // vIoCtrlInfoDump_m()


/**
 * Invalidate control value for subsequent request of current value
 *
 * @author uidc9013 (16.03.2010)
 * @param usLocalIdentifier
 *               LID to invalidate
 */
void vUds_IOControlByCommonIdentifierInvalidateCtrlValue_m(unsigned short usLocalIdentifier)
{
  int iIndex;

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagIoCtrlInfo_m[iIndex].iId==usLocalIdentifier)
    {
      atagIoCtrlInfo_m[iIndex].bValid=FALSE;
      memset(atagIoCtrlInfo_m[iIndex].pcIoCtrlValue,0,atagIoCtrlInfo_m[iIndex].iSize);
    }
  }
} // vUds_IOControlByCommonIdentifierInvalidateCtrlValue_m()


/**
 * verschiedene Infos von LID abfragen
 *
 * @author uidc9013 (03.03.2010)
 *
 * @param usLocalIdentifier
 * @param pbWrite
 * @param pbRead
 * @param pcName
 * @param pcDescription
 *
 * @return int
 */
BOOL bUds_IOControlByCommonIdentifierGetControlInfos_m(unsigned short usLocalIdentifier,
                                                       BOOL *pbWrite,
                                                       BOOL *pbRead,
                                                       int  *piSize,
                                                       char *pcDescription)
{
  int iIndex;

  if(!bIoCtrlInfoInitialized_m)
  {
    vUds_IoCtrlInfoAllocMem_m();
  }

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagIoCtrlInfo_m[iIndex].iId==usLocalIdentifier)
    {
      if(pbWrite)
        *pbWrite=atagIoCtrlInfo_m[iIndex].bWrite;
      if(pbRead)
        *pbRead=atagIoCtrlInfo_m[iIndex].bRead;
      if(pcDescription)
        strncpy(pcDescription,atagIoCtrlInfo_m[iIndex].pcDescription,40);
      if(piSize)
        *piSize=atagIoCtrlInfo_m[iIndex].iSize;
      return(TRUE);
    }
  }
  if(bTraceEnabled_m)
  {
    vTrace_AppendText_g("iUds_IOControlByCommonIdentifierGetControlSize_m() unknown id 0x%04x",usLocalIdentifier);
  }
  return(FALSE);
} // iUds_IOControlByCommonIdentifierGetControlSize2_m()

/**
 * Gr&ouml;sse des Speichers des LID zur&uuml;ckgeben
 *
 * @author uidc9013 (03.03.2010)
 *
 * @param usLocalIdentifier
 *               Nummer des LID
 *
 * @return Gr&ouml;sse des Speichers von LID in bytes
 */
int iUds_IOControlByCommonIdentifierGetControlSize_m(unsigned short usLocalIdentifier)
{
  int iSize;
  if(!bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,&iSize,NULL))
    return(0);

  return(iSize);
} // iUds_IOControlByCommonIdentifierGetControlSize_m()

/**
 * Ist LID schreibbar?
 *
 * @author uidc9013 (03.03.2010)
 * @param usLocalIdentifier
 *               Nummer des LID
 *
 * @return TRUE wenn LID schreibbar ist<BR>
 *         FALSE wenn LID nicht schreibbar ist
 */
BOOL bUds_IOControlByCommonIdentifierGetWritable_m(unsigned short usLocalIdentifier)
{
  BOOL bWritable;
  if(!bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,&bWritable,NULL,NULL,NULL))
    return(FALSE);

  return(bWritable);
} // iUds_IOControlByCommonIdentifierGetWritable_m()

/**
 * Ist LID lesbar?
 *
 * @author uidc9013 (03.03.2010)
 * @param usLocalIdentifier
 *               Nummer des LID
 *
 * @return TRUE wenn LID lesbar ist<BR>
 *         FALSE wenn LID nicht lesbar ist
 */
BOOL bUds_IOControlByCommonIdentifierGetReadable_m(unsigned short usLocalIdentifier)
{
  BOOL bReadable;
  if(!bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,&bReadable,NULL,NULL))
    return(FALSE);

  return(bReadable);
} // iUds_IOControlByCommonIdentifierGetReadable_m()

/**
 * Beschreibung zu LID zur&uuml;ckgeben
 *
 * @author uidc9013 (03.03.2010)
 * @param usLocalIdentifier
 *               Nummer des LID
 *
 * @return char* Beschreibung zu LID
 */
char *pcUds_IOControlByCommonIdentifierGetDescription_m(unsigned short usLocalIdentifier)
{
  static char caDescription[200];

  /*
  printf("0x%04x = \"%s\" w=%d r=%d\n",usLocalIdentifier,
         pcUds_IOControlByCommonIdentifierGetName_m(usLocalIdentifier),
         bUds_IOControlByCommonIdentifierGetWritable_m(usLocalIdentifier),
         bUds_IOControlByCommonIdentifierGetReadable_m(usLocalIdentifier));
  */

  if(!bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription))
    return(NULL);

  return(caDescription);
} // pcUds_IOControlByCommonIdentifierGetDescription_m()


/**
 * vUds_IOControlByCommonIdentifierSetCtrlValue_m()<BR>
 * aktuellen Zustand des LID speichern
 *
 * Empfangene Daten für das LID in das Speicherabbild schreiben.
 *
 * @author uidc9013 (04.03.2010)
 * @param usLocalIdentifier
 *                Nummer des LID
 * @param pcValue aktueller Zustand
 */
void vUds_IOControlByCommonIdentifierSetCtrlValue_m(unsigned short usLocalIdentifier,
                                                    char *pcValue)
{
  int iIndex;

  if(!bIoCtrlInfoInitialized_m)
  {
    vUds_IoCtrlInfoAllocMem_m();
  }

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagIoCtrlInfo_m[iIndex].iId==usLocalIdentifier)
    {
      memcpy(atagIoCtrlInfo_m[iIndex].pcIoCtrlValue,
             pcValue,
             atagIoCtrlInfo_m[iIndex].iSize);
      atagIoCtrlInfo_m[iIndex].bValid=TRUE;
#if 0
      vTrace_AppendText_g("Data for %s  data = 0x%02x %02x %02x %02x %02x ...",atagIoCtrlInfo_m[iIndex].pcDescription,pcValue[0],pcValue[1],pcValue[2],pcValue[3],pcValue[4]);
#endif
      return;
    }
  }
} // vUds_IOControlByCommonIdentifierSetCtrlValue_m()


/**
 * bUds_IOControlByCommonIdentifierGetCtrlValue_m()<BR>
 * aktuellen Zustand des LID zur&uuml;ckgeben
 *
 * @author uidc9013 (04.03.2010)
 * @param usLocalIdentifier
 *                Nummer des LID
 * @param pcValue aktueller Zustand als R&uuml;ckgabe.<BR>
 *                Grösse von pcValue muss von Aufrufer dem LID entsprechend gen&uuml;gend gross alloziert werden.
 * @param piBytes Anzahl zur&uuml;ckgegebener bytes
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<BR>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 *         Bei FALSE werden die Parameter pcValue und piBytes nicht verändert.
 */
BOOL bUds_IOControlByCommonIdentifierGetCtrlValue_m(unsigned short usLocalIdentifier,
                                                    char *pcValue,
                                                    int  *piBytes)
{
  int iIndex;

  if(!bIoCtrlInfoInitialized_m)
    return(FALSE);

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagIoCtrlInfo_m[iIndex].iId==usLocalIdentifier)
    {
      if(!atagIoCtrlInfo_m[iIndex].bValid)
      {
        // noch keine Daten empfangen
        //printf("bUds_ReadDataByCommonIdentifierGetCtrlValue_m(): no data\n");
        return(FALSE);
      }
      memcpy(pcValue,
             atagIoCtrlInfo_m[iIndex].pcIoCtrlValue,
             atagIoCtrlInfo_m[iIndex].iSize);
#if 0
      vTrace_AppendText_g("Data from %s  iIndex=%d %02x %02x",atagIoCtrlInfo_m[iIndex].pcDescription,iIndex,pcValue[0],pcValue[1]);
#endif
      *piBytes=atagIoCtrlInfo_m[iIndex].iSize;
      return(TRUE);
    }
  }
  //printf("bUds_IOControlByCommonIdentifierGetCtrlValue_m(): ID 0x%04x not existing\n",usLocalIdentifier);
  return(FALSE);
} // bUds_IOControlByCommonIdentifierGetCtrlValue_m()



/**
 * pbUds_IOControlByCommonIdentifier_m()<p>
 *
 * @author uidc9013 (22.01.2010)
 * @param usLocalIdentifier
 *                  Local-Identifier. Range: 0xA000 to 0xCF03 (see 10.3 for further information)
 * @param eIoCtrlMode
 *                  Control mode
 * @param pucIoCtrlState
 *                  State to set
 * @param pcMessage Debug message
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IOControlByCommonIdentifier_m(unsigned short usLocalIdentifier,
                                          EIoCtrlMode    eIoCtrlMode,
                                          unsigned char *pucIoCtrlState,
                                          char          *pcMessage)
{
  unsigned char *pucTxData;
  int iControlStateSize;
  int iTxDataSize;
  int iIndex;

  bUds_GenAck_m=FALSE;

  iControlStateSize=iUds_IOControlByCommonIdentifierGetControlSize_m(usLocalIdentifier);
  if(iControlStateSize<1)
  {
    return(NULL);
  }
  if(eIoCtrlMode!=eIoCtrlModeShortTermAdjust)
    iTxDataSize=4;
  else
    iTxDataSize=3;
  /**
   * vorerst wird anhand von pucIoCtrlState entschieden, ob ich Parameter an DUT weitergeben muss.
   * Kann vielleicht mit eIoCtrlMode und bUds_IOControlByCommonIdentifierGetWritable_m()
   * eleganter/besser gelöst werden.
   */
  if(bUds_IOControlByCommonIdentifierGetWritable_m(usLocalIdentifier))
  {
    if(pucIoCtrlState)
      iTxDataSize+=iControlStateSize;
    else
    {
      // Versuch LID schreiben, aber keine Daten vorhanden
      //printf("pbUds_IOControlByCommonIdentifier_m(): Fehler LID schreiben ohne Daten!\n");
      return(NULL);
    }
  }

  pucTxData=(unsigned char*)malloc(iTxDataSize);
  memset(pucTxData,0,sizeof(iTxDataSize));

  iIndex=0;
  pucTxData[iIndex++]=0x2F;   // Uds Service "inputOutputControlByCommonIdentifier"
  pucTxData[iIndex++]=(unsigned char)((usLocalIdentifier>>8)&0xFF);  // MSB identifier
  pucTxData[iIndex++]=(unsigned char)(usLocalIdentifier&0xFF);  // LSB identifier

  if(eIoCtrlMode!=eIoCtrlModeShortTermAdjust)
    pucTxData[iIndex++]=(unsigned char)eIoCtrlMode;

  if(pucIoCtrlState)
  {
    memcpy((void*)&pucTxData[iIndex],(void*)pucIoCtrlState,iControlStateSize);
  }

  if(bUds_IOControlByCommonIdentifierGetWritable_m(usLocalIdentifier))
  {
    vUds_IOControlByCommonIdentifierSetCtrlValue_m(usLocalIdentifier,
                                                   pucIoCtrlState);
  }
  vUds_IOControlByCommonIdentifierInvalidateCtrlValue_m(usLocalIdentifier);
  vUds_SendCmd_m(ECU_CAN_ADDRESS,pucTxData,iTxDataSize,pcMessage);
  free(pucTxData);

  return(&bUds_GenAck_m);
} // pbUds_IOControlByCommonIdentifier_m()




/**
 * pbUds_ReadDataByIdentifier_m()<p>
 *
 * @author uidc9013 (04.06.2013)
 * @param usIdentifier
 *                  Local-Identifier.
 * @param pcMessage Debug message
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_ReadDataByIdentifier_m(unsigned short usIdentifier,
                                   char          *pcMessage)
{
  unsigned char ucaTxData[5];
  int iTxDataSize=0;

  bUds_GenAck_m=FALSE;

  memset(ucaTxData,0,sizeof(ucaTxData));
  ucaTxData[iTxDataSize++]=0x22;   // Uds Service "ReadDataByIdentifier"
  ucaTxData[iTxDataSize++]=(unsigned char)((usIdentifier>>8)&0xFF);  // MSB identifier
  ucaTxData[iTxDataSize++]=(unsigned char)(usIdentifier&0xFF);  // LSB identifier

  vUds_IOControlByCommonIdentifierInvalidateCtrlValue_m(usIdentifier);
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,iTxDataSize,pcMessage);

  return(&bUds_GenAck_m);
} // pbUds_ReadDataByIdentifier_m()


/**
 * bUds_ReadDataByIdentifierGetCtrlValue_m()<BR>
 * aktuellen Zustand des LID zur&uuml;ckgeben
 *
 * @author uidc9013 (04.03.2010)
 * @param usLocalIdentifier
 *                Nummer des LID
 * @param pcValue aktueller Zustand als R&uuml;ckgabe.<BR>
 *                Grösse von pcValue muss von Aufrufer dem LID entsprechend gen&uuml;gend gross alloziert werden.
 * @param piBytes Anzahl zur&uuml;ckgegebener bytes
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<BR>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 *         Bei FALSE werden die Parameter pcValue und piBytes nicht verändert.
 */
BOOL bUds_ReadDataByIdentifierGetCtrlValue_m(unsigned short usLocalIdentifier,
                                             char *pcValue,
                                             int  *piBytes)
{
  int iIndex;

  if(!bIoCtrlInfoInitialized_m)
    return(FALSE);

  for(iIndex=0;
      atagIoCtrlInfo_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagIoCtrlInfo_m[iIndex].iId==usLocalIdentifier)
    {
      if(!atagIoCtrlInfo_m[iIndex].bValid)
      {
        // noch keine Daten empfangen
        //printf("bUds_ReadDataByIdentifierGetCtrlValue_m(): no data\n");
        return(FALSE);
      }
      memcpy(pcValue,
             atagIoCtrlInfo_m[iIndex].pcIoCtrlValue,
             atagIoCtrlInfo_m[iIndex].iSize);
#if 0
      vTrace_AppendText_g("Data from %s  iIndex=%d %02x %02x",atagIoCtrlInfo_m[iIndex].pcDescription,iIndex,pcValue[0],pcValue[1]);
#endif
      *piBytes=atagIoCtrlInfo_m[iIndex].iSize;
      return(TRUE);
    }
  }
  //printf("bUds_IOControlByIdentifierGetCtrlValue_m(): ID 0x%04x not existing\n",usLocalIdentifier);
  return(FALSE);
} // bUds_ReadDataByIdentifierGetCtrlValue_m()




BOOL bUds_StoredDataByIdentifierGetControlInfos_m(unsigned short usLocalIdentifier,
                                                  BOOL *pbWrite,
                                                  BOOL *pbRead,
                                                  int  *piSize,
                                                  char *pcDescription)
{
  int iIndex;

  if(!bStoredDataInitialized_m)
  {
    vUds_StoredDataInfoAllocMem_m();
  }

  for(iIndex=0;
      atagStoredData_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagStoredData_m[iIndex].iId==usLocalIdentifier)
    {
      if(pbWrite)
        *pbWrite=(atagStoredData_m[iIndex].iAccessLevelWrite>0);
      if(pbRead)
        *pbRead=(atagStoredData_m[iIndex].iAccessLevelRead>0);
      if(pcDescription)
        strncpy(pcDescription,atagStoredData_m[iIndex].pcDescription,40);
      if(piSize)
        *piSize=atagStoredData_m[iIndex].iSize;
      return(TRUE);
    }
  }
  if(bTraceEnabled_m)
  {
    vTrace_AppendText_g("bUds_StoredDataByIdentifierGetControlInfos_m() unknown id 0x%04x",usLocalIdentifier);
  }
  return(FALSE);
} // bUds_StoredDataByIdentifierGetControlInfos_m()
int iUds_StoredDataByIdentifierGetControlSize_m(unsigned short usLocalIdentifier)
{
  int iSize;
  if(!bUds_StoredDataByIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,&iSize,NULL))
    return(0);
  return(iSize);
} // iUds_StoredDataByIdentifierGetControlSize_m()
BOOL bUds_StoredDataByIdentifierGetWritable_m(unsigned short usLocalIdentifier)
{
  BOOL bWritable;
  if(!bUds_StoredDataByIdentifierGetControlInfos_m(usLocalIdentifier,&bWritable,NULL,NULL,NULL))
    return(FALSE);
  return(bWritable);
} // bUds_StoredDataByIdentifierGetWritable_m()
BOOL bUds_StoredDataByIdentifierGetReadable_m(unsigned short usLocalIdentifier)
{
  BOOL bReadable;
  if(!bUds_StoredDataByIdentifierGetControlInfos_m(usLocalIdentifier,NULL,&bReadable,NULL,NULL))
    return(FALSE);
  return(bReadable);
} // bUds_StoredDataByIdentifierGetReadable_m()

char *pcUds_StoredDataByIdentifierGetDescription_m(unsigned short usLocalIdentifier)
{
  static char caDescription[200];

  if(!bUds_StoredDataByIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription))
    return(NULL);
  return(caDescription);
} // pcUds_StoredDataByIdentifierGetDescription_m()





// ECU specific services for EEPROM variables and EEPROM parameters
//     startRoutineByLocalId - SID 31h, FAh, 0Ch  Manufacturing Display Test:
//     startRoutineByLocalId - SID 31h, FAh, 0Ah  Static CAN Test:
//     startRoutineByLocalId - SID 31h, FAh, 0Dh  CAN Test TX:
//     startRoutineByLocalId - SID 31h, FAh, 0Bh  CAN Test RX:
//     startRoutineByLocalId - SID 31h, FAh, 0Eh  Dot-Matrix Contrast Voltage:
//     startRoutineByLocalId - SID 31h, FAh, 11h  Dot-Matrix Contrast Voltage read:
//     stopRoutineByLocalId  - SID 32h, FCh, 01h  plcStop:
//     startRoutineByLocalId - SID 31h, FCh, 01h  plcStart:
//     startRoutineByLocalId - SID 31h, FCh, 02h  transferEepromVar:
//     startRoutineByLocalId - SID 31h, FCh, 03h  transferEepromParam.:
// ECU specific services for general purpose functionality
//     startRoutineByLocalId - SID 31h, FCh, 20h  getCustomerNumber:
//     startRoutineByLocalId - SID 31h, FCh, 21h  ODOWrite:

/**
 * pbUds_StartRoutineByLocalId_m()<p>
 *
 * @author uidc9013 (21.01.2010)
 * @param lTxId    CAN Id
 * @param eRoutine auszuführende Routine
 * @param ucParam1 Parameter 1
 * @param ucParam2 Parameter 2
 * @param ucParam3 Parameter 3
 * @param ucParam4 Parameter 4
 * @param ucParam5 Parameter 5
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_AckStartRoutine_g()
 * @see bUds_AckStopRoutine_g()
 */
BOOL *pbUds_StartRoutineByLocalId_m(long lTxId,
                                    ERoutineByLocalId eRoutine,
                                    unsigned char ucParam1,
                                    unsigned char ucParam2,
                                    unsigned char ucParam3,
                                    unsigned char ucParam4,
                                    unsigned char ucParam5)
{
  unsigned char ucaTxData[16];
  char caMessage[100];
  int iTxByteCount;
  BOOL *pbAckFlag;

  memset(ucaTxData,0,sizeof(ucaTxData));

  pbAckFlag=&bUds_AckStartRoutine_m;

  iTxByteCount=0;
  ucaTxData[iTxByteCount++]=0x31;   // Uds Service "startRoutineByLocalId"
  switch(eRoutine)
  {
    case eRoutineByLocalId_TelltaleTest:    // startRoutineByLocalId - SID 31h 01h FAh 0Ah  Telltale Test:
      sprintf(caMessage,"Telltale Test %02x %02x %02x %02x",ucParam2,ucParam3,ucParam4,ucParam5);
      ucaTxData[iTxByteCount++]=0x01;
      ucaTxData[iTxByteCount++]=0xFA;
      ucaTxData[iTxByteCount++]=0x0A;
      ucaTxData[iTxByteCount++]=ucParam1;   // 0= normal Mode ->plc running
                                            // 1= output test pattern only -> plc stopped
      ucaTxData[iTxByteCount++]=ucParam2;   // Telltale state 0-7
      ucaTxData[iTxByteCount++]=ucParam3;   // Telltale state 8-15
      ucaTxData[iTxByteCount++]=ucParam4;   // Telltale state 16-23
      ucaTxData[iTxByteCount++]=ucParam5;   // Telltale state 24-26
      break;
    case eRoutineByLocalId_AdcRawData:      // startRoutineByLocalId - SID 31h 01h FAh 0Bh  ADC Raw data:
      sprintf(caMessage,"ADC Raw data");
      ucaTxData[iTxByteCount++]=0x01;
      ucaTxData[iTxByteCount++]=0xFA;
      ucaTxData[iTxByteCount++]=0x0B;
      break;
    case eRoutineByLocalId_DisplayTest:          // startRoutineByLocalId - SID 31h 01h FAh 0Ch  Manufacturing Display Test:
      sprintf(caMessage,"Display Test Pattern %d, Mode %d",ucParam2,ucParam1);
      ucaTxData[iTxByteCount++]=0x01;
      ucaTxData[iTxByteCount++]=0xFA;
      ucaTxData[iTxByteCount++]=0x0C;
      ucaTxData[iTxByteCount++]=ucParam1;   // 0= normal Mode ->plc running
                                            // 1= output test pattern only -> plc stopped
      ucaTxData[iTxByteCount++]=ucParam2;   // display pattern 1-5
      break;
    case eRoutineByLocalId_GaugeSetSteps:        // startRoutineByLocalId - SID 31h 01h FAh 12h  Gauge set steps:
      sprintf(caMessage,"Gauge %d set steps %d",ucParam1,(ucParam2<<8)+ucParam3);
      ucaTxData[iTxByteCount++]=0x01;
      ucaTxData[iTxByteCount++]=0xFA;
      ucaTxData[iTxByteCount++]=0x12;
      ucaTxData[iTxByteCount++]=ucParam1;   // Gauge: 0x00 = Major gauge 1
                                            //        0x01 = Minor gauge 1
                                            //        0x02 = Minor gauge 2
                                            //        0x03 = Major gauge 2
      ucaTxData[iTxByteCount++]=ucParam2;   // steps MSB
      ucaTxData[iTxByteCount++]=ucParam3;   // steps LSB
      break;
  }
  bUds_AckStartRoutine_m=FALSE;
  bUds_AckStopRoutine_m=FALSE;
  vUds_SendCmd_m(lTxId,ucaTxData,iTxByteCount,caMessage);

  return(pbAckFlag);
} // pbUds_StartRoutineByLocalId_m()



void vUds_ParseRxData_m(unsigned char *pucRxBuffer,
                        unsigned int uiBytes)
{
  int iMsgIdentifier;
  unsigned short usIdentifier;

  iMsgIdentifier=pucRxBuffer[0]&0x3f;

  switch(pucRxBuffer[0])
  {
    case 0x50:  // startDiagnosticMode
      bUds_AckDiagSession_m=TRUE;
      //bUds_GenAck_m=TRUE;
#if 0
      pucRxBuffer[1]=0x00;   // diagnostic mode set
#endif
      break;

    case 0x51:   // Ecu Reset
      bUds_GenAck_m=TRUE;
      break;

    case 0x54:   // ACK für clearDiagnosticInformation
      bUds_GenAck_m=TRUE;
      break;

    case 0x58:   // ACK für readDTCByStatus
      bUds_GenAck_m=TRUE;
      // store Dtc state
      break;

      //case 0x5A:  // ACK für readEcuIdentification
      //bUds_GenAck_m=TRUE;
      //vUds_StoreLidData_m(pucRxBuffer[1],&pucRxBuffer[2]);
      //break;

    case 0x60:  // stopDiagnosticMode
      bUds_AckDiagSession_m=TRUE;
      //bUds_GenAck_m=TRUE;
      break;

    case 0x61:  // ACK readDataByLocalIdentifier
#if 0
      if(pucRxBuffer[1]>0x7F)
      {
        // received unexpected data
        vTraceAppendText_m("received unexpected locId 0x%02x (Range 0x00-0x7F)",pucRxBuffer[1]);
        return;
      }
#endif
      bUds_GenAck_m=TRUE;
      //vUds_StoreLidData_m(pucRxBuffer[1],&pucRxBuffer[2]);
      break;

    case 0x62:  // ReadDataByIdentifier
      iMsgIdentifier=(pucRxBuffer[2]);
      iMsgIdentifier+=pucRxBuffer[1]<<8;
      // Dummerweise werden statische Daten (aus Eeprom) gleich wie dynamische
      // Daten (inputs) über SID 0x22 abgefragt.
      // Da das speichern dieser Daten über verschiedene Funktionen / Strukturen
      // geschieht, müssen beide Speicherfunktionen aufgerufen werden. Die Funktion,
      // welche etwas speichern kann speichert es und die andere wird halt sinnlos
      // aufgerufen.
      vUds_IOControlByCommonIdentifierSetCtrlValue_m((unsigned short)iMsgIdentifier,&pucRxBuffer[3]);
      vUds_SaveStoredDataByDid_m(iMsgIdentifier,&pucRxBuffer[3]);
      bUds_GenAck_m=TRUE;
      //bUds_ReqDPIDAck_m=TRUE;
      break;

#if 0
    case 0x63:  // ACK readMemoryByAddress
      bUds_GenAck_m=TRUE;
      vUds_StoreMemoryData_m(&pucRxBuffer[1]);
      break;
#endif

    case 0x67:   // securityAccess
      if((pucRxBuffer[1]%2)!=0)
      {
        // ungerade Zahl => ACK für request Seed
        // Seed speichern
        memcpy(ucaSeed_m,&pucRxBuffer[2],SEED_KEY_SIZE);
        bUds_AckSeed_m=TRUE;
      }
      else
      {
        // gerade Zahl => ACK für request Send Key
        bUds_AckKey_m=TRUE;
      }
      break;

    case 0x6E:  // WriteDataByIdentifier
      bUds_GenAck_m=TRUE;
      break;

    case 0x6F:  // InputOutputControlByIdentifier
      bUds_GenAck_m=TRUE;
      bUds_SetCPIDAck_m=TRUE;

      // store received data localy
      usIdentifier=pucRxBuffer[2];
      usIdentifier+=pucRxBuffer[1]<<8;
      //pucRxBuffer[3] ist I/O Control Parameter
      vUds_IOControlByCommonIdentifierSetCtrlValue_m(usIdentifier,&pucRxBuffer[4]);
      break;

    case 0x71:   // startRoutineByLocalId
      switch(pucRxBuffer[2])
      {
        case 0xFA:
          switch(pucRxBuffer[3])
          {
            case 0x0A:   // startRoutineByLocalId - SID 31h 01h FAh 0Ah  Telltale Test:
            case 0x0C:   // startRoutineByLocalId - SID 31h 01h FAh 0Ch  Manufacturing Display Test:
            case 0x12:   // startRoutineByLocalId - SID 31h 01h FAh 12h  Gauge set steps:
              bUds_AckStartRoutine_m=TRUE;
              break;
            default:
              if(bTraceEnabled_m)
              {
                vTrace_AppendText_g("unexpected Test command 0x%02x 0x%02x",pucRxBuffer[2],pucRxBuffer[3]);
              }
          }
          break;
        default:
          if(bTraceEnabled_m)
          {
            vTrace_AppendText_g("unexpected Test command 0x%02x",pucRxBuffer[1]);
          }
      }
      break;
    case 0x72:   // stopRoutineByLocalId
      switch(pucRxBuffer[1])
      {
        case 0xFC:
          switch(pucRxBuffer[2])
          {
            case 0x01:   //      stopRoutineByLocalId  - SID 32h, FCh, 01h  plcStop:
              bUds_AckStopRoutine_m=TRUE;
              break;
            default:
              if(bTraceEnabled_m)
              {
                vTrace_AppendText_g("unexpected Test command 0x%02x 0x%02x",pucRxBuffer[1],pucRxBuffer[2]);
              }
          }
          break;
        default:
          if(bTraceEnabled_m)
          {
            vTrace_AppendText_g("unexpected Test command 0x%02x",pucRxBuffer[1]);
          }
      }
      break;

    case 0x74:  // got positive answer to Download Request
      break;

    case 0x7B:  // ACK writeDataByLocalIdentifier
      bUds_GenAck_m=TRUE;
      break;

    case 0x7D:  // ACK writeMemoryByAddress
      bUds_GenAck_m=TRUE;
      break;

    case 0x7E:  // Tester present
      bUds_AckTesterPresent_m=TRUE;
      break;

    case 0x7F:
      bUds_GenNak_m=TRUE;
      vUds_TraceErrorCode_m(pucRxBuffer[1],
                            pucRxBuffer[2]);
      break;

    case 0xC0:   // ACK for 0x80
      bUds_GenAck_m=TRUE;
      break;

    case 0xC1:   // ACK start communication
      bUds_AckStartCommunication_m=TRUE;
      break;

    case 0xC2:   // ACK stop communication
      bUds_AckStopCommunication_m=TRUE;
      break;

    case 0xC3:   // ACK AccessTimingParameter
      bUds_AckAccessTimingParameter_m=TRUE;
      if((pucRxBuffer[1]==0x00)   // read limits
         ||(pucRxBuffer[1]==0x02)   // read current values
        )
      {
        if(bTraceEnabled_m)
          vTrace_AppendText_g("P2min=%d P2max=%d P3min=%d P3max=%d P4min=%d",pucRxBuffer[2],pucRxBuffer[3],pucRxBuffer[4],pucRxBuffer[5],pucRxBuffer[6]);
      }
      break;

    default:
      if(bTraceEnabled_m)
      {
        vTrace_AppendText_g("unexpected identifier 0x%02x (%02x %02x %02x)",iMsgIdentifier,pucRxBuffer[0],pucRxBuffer[1],pucRxBuffer[2]);
      }
  }

} // vUds_ParseRxData_m()





/**
 * Daten-Speicherplatz für die DID-Struktur allozieren
 *
 * @author uidc9013 (16.03.2010)
 */
void vUds_StoredDataInfoAllocMem_m(void)
{
  int iIndex;

  for(iIndex=0;
      atagStoredData_m[iIndex].iId!=0;
      iIndex++)
  {
    atagStoredData_m[iIndex].pcLidValue=malloc(atagStoredData_m[iIndex].iSize);
    memset(atagStoredData_m[iIndex].pcLidValue,0,atagStoredData_m[iIndex].iSize);
  }
  bStoredDataInitialized_m=TRUE;
} // vUds_StoredDataInfoAllocMem_m()


/**
 * Daten-Speicherplatz für die DID-Struktur freigeben
 *
 * @author uidc9013 (26.02.2010)
 */
void vUds_StoredDataInfoFreeMem_m(void)
{
  int iIndex;

  if(!bStoredDataInitialized_m)
  {
    // Es wurde nichts allozieret --> nichts zum freigeben
    return;
  }

  for(iIndex=0;
      atagStoredData_m[iIndex].iId!=0;
      iIndex++)
  {
    free(atagStoredData_m[iIndex].pcLidValue);
  }
  bStoredDataInitialized_m=FALSE;
} // vUds_StoredDataInfoFreeMem_m()


/**
 * vUds_SaveStoredDataByDid_m()<p>
 * store received LID data in local structure.<BR>
 *
 * @author uidc9013 (16.03.2010)
 * @param ucLocalIdentifier
 *                Identifier of LID to store
 * @param pucData data to store localy
 */
void vUds_SaveStoredDataByDid_m(unsigned int   uiLocalIdentifier,
                                unsigned char *pucData)
{
  int iIndex;

  if(!bStoredDataInitialized_m)
  {
    vUds_StoredDataInfoAllocMem_m();
  }

  for(iIndex=0;
      atagStoredData_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagStoredData_m[iIndex].iId==uiLocalIdentifier)
    {
      atagStoredData_m[iIndex].bValid=TRUE;
      memcpy(atagStoredData_m[iIndex].pcLidValue,
             pucData,
             atagStoredData_m[iIndex].iSize);
      return;
    }
  }
  //vTrace_AppendText_g("vUds_SaveStoredDataByDid_m(): unexpected data for address 0x%x",uiLocalIdentifier);
} // vUds_SaveStoredDataByDid_m()


/**
 * bUds_GetStoredDataByDid_m()<p>
 * returns received data from device
 *
 * @author uidc9013 (15.03.2010)
 * @param uiIdentifier
 *                Identifier of LID to store
 * @param pucData data returned from local structure
 * @param puiDataSize
 *                number of bytes returned
 *
 * @return TRUE if there was data to return<BR>
 *         FALSE if there was NO data to return or any other error.
 */
BOOL bUds_GetStoredDataByDid_m(unsigned int   uiIdentifier,
                               unsigned char *pucData,
                               unsigned int  *puiDataSize)
{
  int iIndex;

  if(!bStoredDataInitialized_m)
  {
    // noch nichts alloziert --> noch nichts empfangen
    return(FALSE);
  }

  for(iIndex=0;
      atagStoredData_m[iIndex].iId!=0;
      iIndex++)
  {
    if(atagStoredData_m[iIndex].iId==uiIdentifier)
    {
      if(!atagStoredData_m[iIndex].bValid)
      {
        return(FALSE);
      }
      memcpy(pucData,
             atagStoredData_m[iIndex].pcLidValue,
             atagStoredData_m[iIndex].iSize);
      if(puiDataSize)
        *puiDataSize=atagStoredData_m[iIndex].iSize;
#if 0
      // zurückgegebene Daten tracen
      {
        int iByteIndex;
        printf("bUds_GetLidData_m(): LID=%x iIndex=%d data = 0x",uiIdentifier,iIndex);
        for(iByteIndex=0;
            iByteIndex<atagStoredData_m[iIndex].iSize;
            iByteIndex++)
        {
          printf("%02x ",atagStoredData_m[iIndex].pcLidValue[iByteIndex]);
        }
        printf("\n");
      }
#endif
      return(TRUE);
    }
  }
  return(FALSE);
} // bUds_GetStoredDataByDid_m()


/**
 * Achtung:
 *
 * Reihenfolge der Variablenwerte muss mit der Struktur TagMemoryInfo übereinstimmen!
 *
 * @author uidc9013 (16.03.2010)
 */
//BOOL bMemoryInfoInitialized_m=FALSE;
#if 0
TagMemoryInfo atagMemoryInfo_m[] =
{
  /*
  +--- iAddress
  |                                +--- iSize
  |                                |                                    +--- iCount
  |                                |                                    |  +--- iAccessLevel
  |                                |                                    |  |  +--- bValid
  |                                |                                    |  |  |      +--- pcMemoryValue
  |                                |                                    |  |  |      |     +--- pcName
  |                                |                                    |  |  |      |     |                             +--- pcDescription
  |                                |                                    |  |  |      |     |                             |
  |                                |                                    |  |  |      |     |                             |                                                   */
  (0x600000)+0x40,                 1,            EE_INSPECTION_STATUS_LEN, 7, FALSE, NULL, "InspectionStatus",           "Bit 7=1 => PTM active. Bit 5 starts run-in-test",
  0x600041,                        1,                                   4, 7, FALSE, NULL, "PCBInspectionDate",          "PCB check date, written by the ICT",
  0x600045,                        1,                                   4, 7, FALSE, NULL, "PCBSerialNumber",            "PCB serial number",
  0x600049,                        1,                                  14, 7, FALSE, NULL, "PCBNumber",                  "PCB number, responsible ICT [ASCII]",
  (0x600000)+0x57,                 1,      EE_PRODUCT_INSPECTION_DATE_LEN, 7, FALSE, NULL, "ProductInspectionDate",      "Product inspection date, responsible EOL",
  (0x600000)+0x5B,                 1,        EE_PRODUCT_SERIAL_NUMBER_LEN, 7, FALSE, NULL, "ProductSerialNumber",        "Serial number unit, responsible EOL",
  (0x600000)+0x5F,                 1,               EE_PRODUCT_NUMBER_LEN, 7, FALSE, NULL, "ProductNumber",              "Product number, responsible EOL",
  (0x600000)+0x6F,                 1,              EE_ECU_PART_NUMBER_LEN, 7, FALSE, NULL, "ECUPartNumber",              "ECU part number [ASCII]",
  (0x600000)+0x7A,                 1,          EE_ECU_HARDWARE_NUMBER_LEN, 7, FALSE, NULL, "ECUHardwareNumber",          "ECU hardware number [ASCII]",
  (0x600000)+0x85,                 1,  EE_ECU_HARDWARE_VERSION_NUMBER_LEN, 7, FALSE, NULL, "ECUHardwareVersionNumber",   "ECU Hardware version number [ASCII]",
  (0x600000)+0x91,                 1,             EE_PROGRAMMING_DATE_LEN, 7, FALSE, NULL, "ProgrammingDate",            "Date of last programming [DD-MM-YYYY,  BCD]",
  0x600095,                        1,                                   1, 7, FALSE, NULL, "StressCounter",              "This byte stores the stress test counter",

  (0x600000)+0x96,                 1,          EE_PROD_INTERLOCK_DATA_LEN, 7, FALSE, NULL, "ProdInterlockPtrAssembly",   "Production-Interlock pointer assembly",
#if 0
  0x600096,                        1,                                   2, 7, FALSE, NULL, "ToolVersionPointerAssembly", "Software version of pointer assembly and adjust",
  0x600098,                        1,                                   4, 7, FALSE, NULL, "DatePointerAssembly",        "Date of pointer assembly",
  0x60009C,                        1,                                   3, 7, FALSE, NULL, "TimePointerAssembly",        "Time of pointer assembly",
  0x60009F,                        1,                                   1, 7, FALSE, NULL, "InterlockPointerAssembly",   "<>0 => PointerAssembly done",
#endif

  0x6000A0,                        1,                                  10, 7, FALSE, NULL, "ProdInterlockEOL",           "Production-Interlock EOL",
#if 0
  0x6000A0,                        1,                                   2, 7, FALSE, NULL, "ToolVersionEOL",             "Software version of EOL",
  0x6000A2,                        1,                                   4, 7, FALSE, NULL, "DateEOL",                    "Date of EOL",
  0x6000A6,                        1,                                   3, 7, FALSE, NULL, "TimeEOL",                    "Time of EOL",
  0x6000A9,                        1,                                   1, 7, FALSE, NULL, "InterlockEOL",               "<>0 => EOL done",
#endif

  0x600100,                        2,                                   7, 7, FALSE, NULL, "Gauge1XAxis",                "X1 ... X7 values for Gauge 1. Resolution 0.01",
  0x60010E,                        2,                                   7, 7, FALSE, NULL, "Gauge1YAxis",                "Y1 ... Y7 values for Gauge 1",
  0x60011C,                        2,                                   7, 7, FALSE, NULL, "Gauge2XAxis",                "X1 ... X7 values for Gauge 2. Resolution 0.01",
  0x60012A,                        2,                                   7, 7, FALSE, NULL, "Gauge2YAxis",                "Y1 ... Y7 values for Gauge 2",
  0x600138,                        2,                                   7, 7, FALSE, NULL, "Gauge3XAxis",                "X1 ... X7 values for Gauge 3. Resolution 0.01",
  0x600146,                        2,                                   7, 7, FALSE, NULL, "Gauge3YAxis",                "Y1 ... Y7 values for Gauge 3",
  0x600154,                        2,                                   7, 7, FALSE, NULL, "Gauge4XAxis",                "X1 ... X7 values for Gauge 4. Resolution 0.01",
  0x600162,                        2,                                   7, 7, FALSE, NULL, "Gauge4YAxis",                "Y1 ... Y7 values for Gauge 4",
  (0x600000)+0x0170,               1,                 EE_GAUGE_OFFSET_LEN, 7, FALSE, NULL, "GaugeOffset",                "Offsets for Gauges 1...4",
  0x600174,                        2,                                  12, 7, FALSE, NULL, "DotMatrixTempXAxis",         "X1 ... X12 values for DotMatrix Temperature ADC",
  0x60018C,                        2,                                  12, 7, FALSE, NULL, "DotMatrixContrastYAxis",     "Y1 ... Y12 values for DotMatrix Contrast voltage",
  0x6001A4,                        1,                                   1, 7, FALSE, NULL, "DotMatrixTempOffsetX",       "Offset for DotMatrix Temperature X Axis",
  (0x600000)+0x01A5,               1, EE_DOT_MATRIX_CONTRAST_OFFSET_Y_LEN, 7, FALSE, NULL, "DotMatrixContrastOffsetY",   "Offset for DotMatrix Contrast Y Axis",
  0x6001A6,                        1,                                  12, 7, FALSE, NULL, "DotMatrixFrameFreqYAxis",    "Y1 ... Y12 values for DotMatrix Frame Frequency Y Axis",
  0x6001B2,                        1,                                  12, 7, FALSE, NULL, "DotMatrixNLineYAxis",        "Y1 ... Y12 values for DotMatrix NLine Y Axis",

  0x600280,                        2,                                   6, 7, FALSE, NULL, "AnIn_R_PullUp",              "HW Config. Pull Up resistor value. Byte 0=Res_ANIN1.. Byte 5=Res_ANIN6",
  0x60028C,                        2,                                   1, 7, FALSE, NULL, "CurrentIn_Gain",             "HW Config. Current input gain value. Resolution 0.001",
  0x60028E,                        2,                                   1, 7, FALSE, NULL, "DigitalInput_Gain",          "HW Config. Digital input gain value. Resolution 0.001",
  0x600290,                        2,                                   1, 7, FALSE, NULL, "DigitalInput_VTH_Low",       "HW Config. Digital input low threshold. Resolution 0.001",
  0x600292,                        2,                                   1, 7, FALSE, NULL, "DigitalInput_VTH_High",      "HW Config. Digital input high threshold. Resolution 0.001",
  0x600294,                        2,                                   1, 7, FALSE, NULL, "AlternatorMES_Gain",         "HW Config. Alternator MES gain. Resolution 0.001",
  (0x600000)+0x0296,               1,             EE_OUTPUT_SINK_TYPE_LEN, 7, FALSE, NULL, "OutputSink_Type",            "HW Config. Output sink type. Bit 0 = OUTSink1 (0-PWM, 1-GIO).. Bit 2=OUTSink3 (0-PWM, 1-GIO)",
  0x600297,                        1,                                   1, 7, FALSE, NULL, "SoundOutput_Type",           "HW Config. Sound output type. Bit 0 = Buzzer type ( 0-active [GIO], 1-passive [SGO])",
  (0x600000)+0x0298,               1,           EE_DIGITAL_INPUT_TYPE_LEN, 5, FALSE, NULL, "DigitalInput_Type",          "HW Config. Digital input type. Bit 0 = digital input 1... bit 23 = digital input 24 (0-active High, 1-active Low)",
  0x60029B,                        1,                                   1, 7, FALSE, NULL, "PowerSupply",                "Power supply type. Bit 0 = 0 (12V) or 1 (24V)",

  0x60029C,                        2,                                   1, 7, FALSE, NULL, "ADC_TRM30_MES_RefVoltage",   "ADC TRM30 MES Reference Voltage",
  0x60029E,                        2,                                   1, 7, FALSE, NULL, "ADCLowerThresholdUV",        "ADC Under Voltage Lower Threshold",
  0x6002A0,                        2,                                   1, 7, FALSE, NULL, "ADCUpperThresholdUV",        "ADC Under Voltage Upper Threshold",
  0x6002A2,                        2,                                   1, 7, FALSE, NULL, "ADCLowerThresholdOV",        "ADC Over Voltage LowerThreshold",
  0x6002A4,                        2,                                   1, 7, FALSE, NULL, "ADCUpperThresholdOV",        "ADC Over Voltage Upper Threshold",
  0x6002A6,                        1,                                   1, 7, FALSE, NULL, "CANWakeUp",                  "CAN Wake Up feature. 0-disabled, 1-enabled",
  0x600300,                        2,                                  32, 7, FALSE, NULL, "TripDistance",               "TSU Trip distance",
  0x600340,                        2,                                  32, 7, FALSE, NULL, "TotalDistance",              "TSU Total distance",
  0x600380,                        2,                                  32, 7, FALSE, NULL, "OdoDecimal",                 "Decimal value of odometer, uncoded. Values: 0 to 9",

  0,                               0,                                   0, 0, FALSE, NULL, NULL,                         NULL
};
#endif

//unsigned long ulMemoryAccessAddress_m;
//unsigned int uiMemoryAccessSize_m;


#if 0
/**
 * Daten-Speicherplatz für die IoControl-Struktur allozieren
 *
 * @author uidc9013 (26.02.2010)
 */
void vUds_MemoryInfoAllocMem_m(void)
{
  int iIndex;
  int iMemorySize;

  for(iIndex=0;
      atagMemoryInfo_m[iIndex].ulAddress!=0;
      iIndex++)
  {
    iMemorySize=atagMemoryInfo_m[iIndex].iSize*atagMemoryInfo_m[iIndex].iCount;
    atagMemoryInfo_m[iIndex].pcMemoryValue=malloc(iMemorySize);
    memset(atagMemoryInfo_m[iIndex].pcMemoryValue,0,iMemorySize);
  }
  bMemoryInfoInitialized_m=TRUE;
} // vUds_MemoryInfoAllocMem_m()
#endif


#if 0
/**
 * Daten-Speicherplatz für die IoControl-Struktur freigeben
 *
 * @author uidc9013 (26.02.2010)
 */
void vUds_MemoryInfoFreeMem_m(void)
{
  int iIndex;

  if(!bMemoryInfoInitialized_m)
  {
    // Es wurde nichts allozieret --> nichts zum freigeben
    return;
  }

  for(iIndex=0;
      atagMemoryInfo_m[iIndex].ulAddress!=0;
      iIndex++)
  {
    free(atagMemoryInfo_m[iIndex].pcMemoryValue);
  }
  bMemoryInfoInitialized_m=FALSE;
} // vUds_MemoryInfoFreeMem_m()
#endif


#if 0
/**
 * Invalidate memory value for subsequent read request of current memory value
 *
 * @author uidc9013 (16.03.2010)
 * @param ulAddress memory address to invalidate
 */
void vUds_InvalidateMemoryData_m(unsigned long  ulAddress)
{
  int iIndex;

  for(iIndex=0;
      atagMemoryInfo_m[iIndex].ulAddress!=0;
      iIndex++)
  {
    if(atagMemoryInfo_m[iIndex].ulAddress==ulAddress)
    {
      atagMemoryInfo_m[iIndex].bValid=FALSE;
    }
  }
} // vUds_InvalidateMemoryData_m()
#endif


#if 0
/**
 * vUds_StoreMemoryData_m()
 *
 * store received memory data in local structure.<BR>
 * The address in wich the data has to be stored will be retrieved from<BR>
 * the modul-global variable ulMemoryAccessAddress_m.<BR>
 * ulMemoryAccessAddress_m is set at the data request.<BR>
 *
 * @author uidc9013 (15.03.2010)
 * @param pucData data to store at address
 */
void vUds_StoreMemoryData_m(unsigned char *pucData)
{
  int iIndex;
  int iBytes;

  if(!bMemoryInfoInitialized_m)
  {
    vUds_MemoryInfoAllocMem_m();
  }

  for(iIndex=0;
      atagMemoryInfo_m[iIndex].ulAddress!=0;
      iIndex++)
  {
    if(atagMemoryInfo_m[iIndex].ulAddress==ulMemoryAccessAddress_m)
    {
      iBytes=atagMemoryInfo_m[iIndex].iSize*atagMemoryInfo_m[iIndex].iCount;
      atagMemoryInfo_m[iIndex].bValid=TRUE;
      //printf("received %d bytes for 0x%06x (%d bytes remaining)\n",iBytes,atagMemoryInfo_m[iIndex].ulAddress,uiMemoryAccessSize_m-iBytes);
      if(iBytes>uiMemoryAccessSize_m)
      {
        // nicht soviel data wie in Memory-Block Platz hätte
        memcpy(atagMemoryInfo_m[iIndex].pcMemoryValue,
               pucData,
               uiMemoryAccessSize_m);
        //printf("  too few\n");
      }
      else
      {
        // nicht soviel data wie in Memory-Block Platz hätte
        memcpy(atagMemoryInfo_m[iIndex].pcMemoryValue,
               pucData,
               iBytes);
        //printf("  enough\n");
      }

      if(uiMemoryAccessSize_m>iBytes)
      {
        // Mehr Daten empfangen als in diesem Memory-Block platz haben
        // --> Restliche Daten in nächsten Memory-Block speichern
        //     Dazu müssen die Speicherblöcke in aufsteigender Reihenfolge
        //     initialisiert worden sein (atagMemoryInfo_m)! Sonst iIndex=0 und von vorne neu suchen.
        ulMemoryAccessAddress_m+=iBytes;
        uiMemoryAccessSize_m-=iBytes;
        pucData=&pucData[iBytes];
      }
      else
      return;
    }
  }
  if(bTraceEnabled_m)
  {
    vTrace_AppendText_g("vUds_StoreMemoryData_m(): unexpected data for address 0x%x",ulMemoryAccessAddress_m);
  }
} // vUds_StoreMemoryData_m()
#endif


#if 0
/**
 * bUds_GetMemoryData_m()
 *
 * returns received data from device
 *
 * @author uidc9013 (15.03.2010)
 * @param ulAddress Memory address
 * @param pucData   data to store at address
 * @param puiDataSize
 *                  number of bytes returned
 *
 * @return TRUE if there was data to return<BR>
 *         FALSE if there was NO data to return or any other error.
 */
BOOL bUds_GetMemoryData_m(unsigned long  ulAddress,
                          unsigned char *pucData,
                          unsigned int  *puiDataSize)
{
  int iIndex;
  int iMemorySize;

  if(!bMemoryInfoInitialized_m)
  {
    // noch nichts alloziert --> noch nichts empfangen
    return(FALSE);
  }

  for(iIndex=0;
      atagMemoryInfo_m[iIndex].ulAddress!=0;
      iIndex++)
  {
    if(atagMemoryInfo_m[iIndex].ulAddress==ulAddress)
    {
      if(!atagMemoryInfo_m[iIndex].bValid)
      {
        return(FALSE);
      }
      iMemorySize=atagMemoryInfo_m[iIndex].iSize*atagMemoryInfo_m[iIndex].iCount;
      if(pucData)
      memcpy(pucData,
             atagMemoryInfo_m[iIndex].pcMemoryValue,
             iMemorySize);
      if(puiDataSize)
      *puiDataSize=iMemorySize;
      return(TRUE);
    }
  }
  return(FALSE);
} // bUds_GetMemoryData_m()
#endif




void vUds_TraceErrorCode_m(unsigned char ucMsgCode,
                           unsigned char ucErrorCode)
{
  char *pcError;

  switch(ucErrorCode)
  {
    case 0x10: pcError="General reject"; break;
    case 0x11: pcError="Service not supported"; break;
    case 0x12: pcError="Subfunction not supported"; break;
    case 0x13: pcError="Incorrect message length or invalid format"; break;
    case 0x14: pcError="Response too long"; break;
    case 0x21: pcError="Busy repeat request"; break;
    case 0x22: pcError="Conditions not correct"; break;
    case 0x24: pcError="Request sequence error"; break;
    case 0x31: pcError="Request out of range"; break;
    case 0x33: pcError="Security access denied"; break;
    case 0x35: pcError="Invalid key"; break;
    case 0x36: pcError="Exceed number of attempts"; break;
    case 0x37: pcError="Required time delay not expired"; break;
    case 0x70: pcError="Upload / Download not accepted"; break;
    case 0x71: pcError="Transfer data suspended"; break;
    case 0x72: pcError="General programming failure"; break;
    case 0x73: pcError="Wrong block sequence counter"; break;
    case 0x78: pcError="Request correctly received - response pending"; break;
    case 0x7E: pcError="Subfunction not supported in active session"; break;
    case 0x7F: pcError="Service not supported in active session"; break;
    case 0x81: pcError="RPM too high"; break;
    case 0x82: pcError="RPM too low"; break;
    case 0x83: pcError="Engine is running"; break;
    case 0x84: pcError="Engine is not running"; break;
    case 0x85: pcError="Engine runtime too low"; break;
    case 0x86: pcError="Temperature too high"; break;
    case 0x87: pcError="Temperature too low"; break;
    case 0x88: pcError="Vehicle speed too high"; break;
    case 0x89: pcError="Vehicle speed too low"; break;
    case 0x8A: pcError="Throttle / Pedal too high"; break;
    case 0x8B: pcError="Throttle / Pedal too low"; break;
    case 0x8C: pcError="Transmission range not in neutral"; break;
    case 0x8D: pcError="Transmission range not in gear"; break;
    case 0x8F: pcError="Break switch(es) not closed"; break;
    case 0x90: pcError="Shifter lever not in park"; break;
    case 0x91: pcError="Torque converter clutch locked"; break;
    case 0x92: pcError="Voltage too high"; break;
    case 0x93: pcError="Voltage too low"; break;
    default: pcError="???";
  }
  if(bTraceEnabled_m)
  {
    vTrace_AppendText_g("   NAK (%s)",pcError);
  }
}


void vUds_CheckBufferOverrun_m(int iPortNr)
{
  canStatus canRc;
  {
    // Vor zu übervollem CAN-Empfangs-Buffer warnen
    DWORD dwBuffer;
    canRc=canIoCtl(iPortNr,
                   canIOCTL_GET_RX_BUFFER_LEVEL,
                   &dwBuffer,
                   sizeof(dwBuffer));
    //if(canRc!=canOK)
      //printf("canIoCtl(canIOCTL_GET_RX_BUFFER_LEVEL) returned %d\n",canRc);
    //else if(dwBuffer>20)
      //printf("canIoCtl(canIOCTL_GET_RX_BUFFER_LEVEL) = %d\n",dwBuffer);
  }
} // vUds_CheckBufferOverrun_m()



/**
 * FMI und SPN zu DTC-group umrechnen
 *
 * @author uidc9013 (28.01.2010)
 * @param iSpn     SPN = Suspect Part Number
 * @param iFmi     FMI = Failure Mode Identifier
 * @param pucByte1 Byte1 für DTC-group
 * @param pucByte2 Byte2 für DTC-group
 * @param pucByte3 Byte3 für DTC-group
 */
void vUds_GetDtcGroup_m(unsigned int iSpn,
                        unsigned int iFmi,
                        unsigned char *pucByte1,
                        unsigned char *pucByte2,
                        unsigned char *pucByte3)
{
  *pucByte1=iSpn&0xFF;
  *pucByte2=(iSpn&0xFF00)>>8;
  *pucByte3=(iSpn&0xFF0000)>>16;
  *pucByte3&=0xE0;

  *pucByte3|=iFmi&0x1F;
} // vUds_GetDtcGroup_m()


unsigned char ucaSeed_m[SEED_KEY_SIZE];


#include "Uds_Security.c"


