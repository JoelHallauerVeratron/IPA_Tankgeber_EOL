/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        UDS.c
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
  #include <cvirte.h>
  #include <toolbox.h>
  #include <userint.h>
  #include <ansi_c.h>
  #include <utility.h>
  #include <rs232.h>
#else // !_CVI_
  #include <stdlib.h>
  #include <stdio.h>
  #define PI 3.14159265358979323846
#endif // !_CVI_
#include <canlib.h>

#include "Uds.h"
#include "Uds_private.h"
#include "Uds_IoControl.h"

#include "Trace.h"
#include "ProdInterlock.h"
#include "DeviceSpecific.h"
#define PROD_INTERLOCK_SIZE  10

/**
 * Modulglobale-Variablen
 */
int iaUds_Baudrate_m[4]={ canBITRATE_250K };
BOOL bUds_Initialized_m=FALSE;

BOOL bUds_SuppressTesterPresentAutomatic_m;   // für Automatische Abläufe "TesterPresent" unterdrücken
BOOL bUds_SuppressTesterPresentByUser_m;      // enable/disable "TesterPresent" für Benutzer der Library

BOOL bUds_AckTesterPresent_m;
BOOL bUds_AckStartCommunication_m;
BOOL bUds_AckStopCommunication_m;
BOOL bUds_AckAccessTimingParameter_m;

BOOL bUds_AckStartRoutine_m;
BOOL bUds_AckStopRoutine_m;

BOOL bUds_AckDiagSession_m;
BOOL bUds_AckSeed_m;
BOOL bUds_AckKey_m;

BOOL bUds_GenAck_m;   // generelles Acknowledge
BOOL bUds_GenNak_m;   // generelles NoAcknowledge

//BOOL bUds_ReqDPIDAck_m;
BOOL bUds_SetCPIDAck_m;

// Variablen für Trace
BOOL bTraceEnabled_m=FALSE;

//char caUdsErrorMessage_m[Uds_ERROR_MESSAGE_LEN+1];

UCHAR ucInterlockData_m[DID_PROD_INTRLOCK_LEN];



/**
 * Trace ein- oder ausschalten
 *
 * @author uidc9013 (13.03.2012)
 * @param bEnable TRUE: Trace wird aufgezeichnet<br>
 *                FALSE: Trace wird nicht aufgezeichnet
 *
 * @return vorheriger Zustand:<br>
 *         TRUE: Trace war enabled<br>
 *         FALSE: Trace war nicht disabled
 */
BOOL bUds_TraceEnable_g(BOOL bEnable)
{
  BOOL bPreviousEnabled;
  bPreviousEnabled=bTraceEnabled_m;
  bTraceEnabled_m=bEnable;
  return(bPreviousEnabled);
} // bUds_TraceEnable_g()


/**
 * bUds_Init_g()<p>
 * Kommunikations-Port mit default-Werten initialisieren
 *
 * @author uidc9013 (10.06.2009)
 * @param iBaudrate Baudrate als canBITRATE_XXX (in canLib.h definiert)<br>
 *                  Bei 0 wird 250k als Standart verwendet.
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bUds_Init_g(void)
{
#ifdef HAVE_MULTIPLE_KVASERS
  canStatus canRc;
  int iChannelCount;
  int iIndex;
  char caSerialNr[8+1];
  unsigned long ulSerialNr;
#endif // HAVE_MULTIPLE_KVASERS
  if(bUds_Initialized_m)
  {
    // Port schon geöffnet --> nichts zu öffnen
    //printf("bPintaCom_PortOpen_g(): Port schon geöffnet --> nichts zu öffnen\n");
    return(TRUE);
  }

  bUds_SuppressTesterPresentAutomatic_m=FALSE;   // TesterPresent zyklisch senden
  bUds_SuppressTesterPresentByUser_m=FALSE;

  canInitializeLibrary();

  InitializeCriticalSection(&csSender_m);
  InitializeCriticalSection(&csReadWrite_m);

#ifdef HAVE_MULTIPLE_KVASERS
  canRc=canGetNumberOfChannels(&iChannelCount);
  if(canRc != canOK)
  {
    //printf("bUds_Init_g(): canGetNumberOfChannels() returned error\n");
    return(FALSE);
  }
  if(iChannelCount<1)
  {
    //printf("bUds_Init_g(): no Kvaser interfaces found\n");
    return(FALSE);
  }

  for(iIndex=0;
      iIndex<iChannelCount;
      iIndex++)
  {
    canRc=canGetChannelData(iIndex,canCHANNELDATA_CARD_SERIAL_NO,caSerialNr,8);
    if(canRc != canOK)
    {
      //printf("bUds_Init_g(): canGetChannelData(%d) returned error\n",iIndex);
      return(FALSE);
    }
    memcpy((void*)&ulSerialNr,(void*)caSerialNr,4);
    //printf("interface %d: Serial = %ld\n",iIndex,ulSerialNr);
  }
#endif // HAVE_MULTIPLE_KVASERS

  bUds_Initialized_m=TRUE;

  return(TRUE);
} // bUds_Init_g()


/**
 * vUds_PortClose_g()
 *
 * Kommunikations-Port schliessen
 *
 * @author uidc9013 (10.06.2009)
 */
void vUds_Close_g(void)
{
  if(!bUds_Initialized_m)
  {
    // Port nicht geöffnet --> nichts zu schliessen
    //printf("vPintaCom_PortClose_g(): Port nicht geöffnet --> nichts zu schliessen\n");
    return;
  }

  DeleteCriticalSection(&csSender_m);
  DeleteCriticalSection(&csReadWrite_m);

  vUds_IoCtrlInfoFreeMem_m();
  //vUds_MemoryInfoFreeMem_m();
  vUds_StoredDataInfoFreeMem_m();

  bUds_Initialized_m=FALSE;
} // vUds_PortClose_g()


/**
 * bUds_Initialized_g()
 *
 * Abfrage, ob Kommunikations-Port initialisiert ist
 *
 * @author uidc9013 (03.12.2009)
 * @return TRUE wenn Port ge&ouml;ffnet ist
 */
BOOL bUds_IsInitialized_g(void)
{
  return(bUds_Initialized_m);
} // bUds_IsInitialized_g()




/**
 * bUds_SetBaudrate_g()<p>
 * Baudrate von CAN-Bus setzen
 *
 * @author uidc9013 (19.09.2011)
 * @param iCanBus   CAN-Bus. 1 oder 2
 * @param iBaudrate Baudrate als canBITRATE_XXX (in canLib.h definiert)<br>
 *                  Als Parameter kann die Baudrate angegeben werden<br>
 *                  oder canBITRATE_XXX aus canlib.h<br>
 *                  Bei 0 wird 250k als Standart verwendet.
 */
void vUds_SetBaudrate_g(int iCanBus,
                        int iBaudrate)
{
  if(iBaudrate<0)
    iaUds_Baudrate_m[iCanBus-1]= iBaudrate;
  else
    iaUds_Baudrate_m[iCanBus-1]= canBITRATE_250K;
} // bUds_SetBaudrate_g()


/**
 * vNmea2k_EnableUds_g()<p>
 * Enables or disables UDS Communications.<br>
 * This command is sent as a NMEA2000 telegram.
 *
 * @author WeU (06.10.2015)
 * @param bEnable TRUE = enable UDS communication.<br>
 *                FALSE = disable UDS communication.
 */
void vNmea2k_EnableUds_g(BOOL bEnable)
{
  unsigned char ucaTxData[16];
  int iIndex=0;
  char caMessage[20];

  memset(ucaTxData,0,sizeof(ucaTxData));

  ucaTxData[iIndex++]=0xBB;
  ucaTxData[iIndex++]=0x99;
  ucaTxData[iIndex++]=0x07;
  ucaTxData[iIndex++]=0xFF;
  ucaTxData[iIndex++]=(bEnable)?0x01:0x00;
  ucaTxData[iIndex++]=0xFF;
  ucaTxData[iIndex++]=0xFF;
  ucaTxData[iIndex++]=0xFF;
  sprintf(caMessage,"%s UDS",(bEnable)?"enable":"disable");
  vALCom_SendMessage_m(0x18FF0100,ucaTxData,iIndex,caMessage);
} // vNmea2k_EnableUds_g()



//  Communication initialization and de- initialization
//      startCommunication: SID 81h
/**
 * pbUds_StartCommunication_g()<p>
 * Mode 0x81
 *
 * @author uidc9013 (20.01.2010)
 */
BOOL *pbUds_StartCommunication_g(void)
{
  unsigned char ucaTxData[16];

  memset(ucaTxData,0,sizeof(ucaTxData));

  ucaTxData[0]= 0x81;   // Start communication

  bUds_AckStartCommunication_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,1,"start communication");

  return(&bUds_AckStartCommunication_m);
} // vUds_StartCommunication_g()


//      stopCommunication: SID 82h
/**
 * pbUds_StopCommunication_g()<p>
 * Mode 0x82
 *
 * @author uidc9013 (20.01.2010)
 */
BOOL *pbUds_StopCommunication_g(void)
{
  unsigned char ucaTxData[16];

  memset(ucaTxData,0,sizeof(ucaTxData));

  ucaTxData[0]= 0x82;   // Start communication

  bUds_AckStopCommunication_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,1,"stop communication");

  return(&bUds_AckStopCommunication_m);
} // vUds_StopCommunication_g()


//       testerPresent: SID 3Eh
/**
 * vUds_SendTesterPresent_g()<p>
 * Mode 0x3E
 *
 * @author uidc9013 (20.01.2010)
 */
void vUds_SendTesterPresent_g(void)
{
  if((!bUds_SuppressTesterPresentAutomatic_m)
     &&(!bUds_SuppressTesterPresentByUser_m)
    )
  {
    vUds_SendTesterPresent_m();
  }
#if 0
  else
  {
    // Anzeigen, wer das senden verhindert
    vTraceAppendText_m("suppress TesterPresent by:%s%s",
                       (bUds_SuppressTesterPresentAutomatic_m)?" Autom.":" ",
                       (bUds_SuppressTesterPresentByUser_m)?" User.":" ");
  }
#endif
} // vUds_SendTesterPresent_g()



/**
 * Senden von "Tester-Present" von Benutzer aus unterdrücken
 *
 * @param bEnable TRUE="Tester Present" kann gesendet werden<br>
 *                FALSE="Tester Present" wird nicht gesendet.
 */
void vUds_EnableTesterPresent_g(BOOL bEnable)
{
  bUds_SuppressTesterPresentByUser_m =!bEnable;
} // vUds_EnableTesterPresent_g()


//      ecuReset: SID 11h
/**
 * pbUds_ResetDevice_g()
 *
 * Mode 0x20  Return to normal operation
 *
 * @author uidc9013 (13.10.2009)
 *
 * @return Pointer auf auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_ResetDevice_g(void)
{
  unsigned char ucaTxData[16];

  memset(ucaTxData,0,sizeof(ucaTxData));

  ucaTxData[0]= 0x11;   // Uds Service "ecuReset"
  ucaTxData[1]= 0x01;   // [0x00] ???
                        // [0x01] powerOn on K-Line

  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,2,"Reset device");

  return(&bUds_GenAck_m);
} // pbUds_ResetDevice_g()


//  Diagnostic Sessions and Security Access
//       startDiagnosticSession: SID 10h
/**
 * pbUds_StartDiagSession_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 * @param eDiagMode diagnostic mode
 *
 * @return Pointer auf ACK-Flag
 * @see EDiagMode
 * @see bUds_AckDiagSession_g()
 */
BOOL *pbUds_StartDiagSession_g(EUdsDiagMode eUdsDiagMode)
{
  unsigned char ucaTxData[16];
  char caMessage[100];

  memset(ucaTxData,0,sizeof(ucaTxData));

  strcpy(caMessage,"start diag. mode: ");
  switch(eUdsDiagMode)
  {
    case eUdsDiagModeExtended: strcat(caMessage,"Ext"); break;
    case eUdsDiagModeStandard: strcat(caMessage,"Std"); break;
      //case eUdsDiagModeVmsEol:   strcat(caMessage,"vehicle manuf. EOL"); break;
    case eUdsDiagModeSssEol: strcat(caMessage,"system suppl. EOL"); break;
      //case eUdsDiagModeEcuProg:  strcat(caMessage,"ECU progr."); break;
      //case eUdsDiagModeDevelop:  strcat(caMessage,"development"); break;
      //case eUdsDiagModeCvTest:   strcat(caMessage,"CV EOL test"); break;
    default: strcat(caMessage,"???");
  }

  ucaTxData[0]= 0x10;   // Uds Service "startDiagnosticSession"
  ucaTxData[1]= eUdsDiagMode;

  bUds_AckDiagSession_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,2,caMessage);

  return(&bUds_AckDiagSession_m);
} // pbUds_StartDiagSession_g()


//       stopDiagnosticSession: SID 20h
/**
 * pbUds_StopDiagSession_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_AckDiagSession_g()
 */
BOOL *pbUds_StopDiagSession_g(void)
{
  unsigned char ucaTxData[16];

  memset(ucaTxData,0,sizeof(ucaTxData));

  ucaTxData[0]= 0x20;   // Uds Service "StopDiagnosticSession"

  bUds_AckDiagSession_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,1,"stop diag. mode");

  return(&bUds_AckDiagSession_m);
} // pbUds_StopDiagSession_g()




EUdsSecurityAccess eCurrentSecurityAccess_m;

//       securityAccess#1: SID 27h
/**
 * pbUds_SecurityAccessReqSeed_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_AckSeed_g()
 */
BOOL *pbUds_SecurityAccessReqSeed_g(EUdsSecurityAccess eSecurityAccess)
{
  unsigned char ucaTxData[16];
  char caMessage[100];

  memset(ucaTxData,0,sizeof(ucaTxData));

  strcpy(caMessage,"security request seed: ");
  eCurrentSecurityAccess_m=eSecurityAccess;
  switch(eCurrentSecurityAccess_m)
  {
    case eUdsSecurityAccessDefault: strcat(caMessage,"default");
      break;
    case eUdsSecurityAccessVmsEol: strcat(caMessage,"vehicle manuf. EOL");
      break;
    case eUdsSecurityAccessVmsProg: strcat(caMessage,"vehicle manuf. progr.");
      break;
    case eUdsSecurityAccessVmsDev: strcat(caMessage,"vehicle manuf. develop.");
      break;
    case eUdsSecurityAccessSssEol: strcat(caMessage,"system suppl. EOL");
      break;
    case eUdsSecurityAccessSssDev: strcat(caMessage,"system suppl. develop.");
      break;
    default: strcat(caMessage,"???");
  }

  ucaTxData[0]= 0x27;   // Uds Service "security Access#1"
  ucaTxData[1]= eSecurityAccess;

  bUds_AckSeed_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,2,caMessage);

  return(&bUds_AckSeed_m);
} // pbUds_SecurityAccessReqSeed_g()


//       securityAccess#2: SID 27h
/**
 * pbUds_SecurityAccessSendKey_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_AckKey_g()
 */
BOOL *pbUds_SecurityAccessSendKey_g(void)
{
  unsigned char ucaTxData[16];
  char caMessage[100];
  int iKeySize;

  memset(ucaTxData,0,sizeof(ucaTxData));

  strcpy(caMessage,"security send key: ");
  switch(eCurrentSecurityAccess_m)
  {
    case eUdsSecurityAccessDefault: strcat(caMessage,"default");
      break;
    case eUdsSecurityAccessVmsEol: strcat(caMessage,"vehicle manuf. EOL");
      break;
    case eUdsSecurityAccessVmsProg: strcat(caMessage,"vehicle manuf. progr.");
      break;
    case eUdsSecurityAccessVmsDev: strcat(caMessage,"vehicle manuf. develop.");
      break;
    case eUdsSecurityAccessSssEol: strcat(caMessage,"system suppl. EOL");
      break;
    case eUdsSecurityAccessSssDev: strcat(caMessage,"system suppl. develop.");
      break;
    default: strcat(caMessage,"???");
  }

  ucaTxData[0]= 0x27;   // Uds Service "security Access#2"
  ucaTxData[1]= eCurrentSecurityAccess_m+1;
  vUds_CalcKey_m(eCurrentSecurityAccess_m,&ucaTxData[2],&iKeySize);

  bUds_AckKey_m=FALSE;
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,2+iKeySize,caMessage);

  return(&bUds_AckKey_m);
} // pbUds_SecurityAccessSendKey_g()


//  Identification and parameterization data
//       writeDataByLocalIdentifier: SID: 3Bh
/**
 * pbUds_WriteDataByIdentifier_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 * @param uiIdentifier
 *                CAN-Identifier
 * @param pucData zu sendende Daten
 * @param iBytes  Anzahl zu sendende bytes
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteDataByIdentifier_g(unsigned int uiIdentifier,
                                    unsigned char *pucData,
                                    int iBytes)
{
  unsigned char *pucTxData;
  char caMessage[100];
  int iTxDataSize;
  char caIdName[100];

  iTxDataSize=iBytes+3;
  pucTxData =(unsigned char*)malloc(iTxDataSize);
  memset(pucTxData,0,iTxDataSize);

  strcpy(caIdName,pcUds_StoredDataByIdentifierGetDescription_m((USHORT)uiIdentifier));
  sprintf(caMessage,"write data ID 0x%02x (%s)",uiIdentifier,caIdName);
  pucTxData[0]= 0x2E;   // Uds Service "writeDataByIdentifier"
  pucTxData[1]=(uiIdentifier>>8)&0xff;
  pucTxData[2]=(uiIdentifier)&0xff;
  memcpy(&pucTxData[3],pucData,iBytes);

  vUds_SendCmd_m(ECU_CAN_ADDRESS,pucTxData,iTxDataSize,caMessage);
  free(pucTxData);

  return(&bUds_GenAck_m);
} // pbUds_WriteDataByIdentifier_g()


//       readDataByLocalIdentifier: SID: 21h
/**
 * pbUds_RequestDataByIdentifier_g()<p>
 *
 * @author uidc9013 (20.01.2010)
 * @param uiIdentifier
 *                Identifier für SID 0x22
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_RequestDataByIdentifier_g(unsigned int uiIdentifier)
{
  unsigned char ucaTxData[16];
  char caMessage[100];

  memset(ucaTxData,0,sizeof(ucaTxData));
  ucaTxData[0]= 0x22;   // Uds Service "readDataByIdentifier"
  ucaTxData[1]=(unsigned char)((uiIdentifier>>8)&0xFF);  // MSB address
  ucaTxData[2]=(unsigned char)(uiIdentifier&0xFF);  // LSB address
  sprintf(caMessage,"read data ID 0x%04x",uiIdentifier);
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,3,caMessage);

  return(&bUds_GenAck_m);
} // pbUds_RequestDataByIdentifier_g()


//  Identifier access
//       readMemoryByAddress: SID 23h
/**
 * pbUds_ReadMemoryByAddress_g()<p>
 *
 * @author uidc9013 (22.01.2010)
 * @param ulAddress Startadresse vom zu lesenden Speicher
 * @param iBytes    Anzahl zu lesende bytes. Max=0xFE
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 * @see bUds_GetMemoryByAddress_g()
 */
BOOL *pbUds_ReadMemoryByAddress_g(unsigned long ulAddress,
                                  int iBytes)
{
  unsigned char ucaTxData[16];
  char caMessage[100];

  memset(ucaTxData,0,sizeof(ucaTxData));

  sprintf(caMessage,"read memory 0x%06x %d bytes",ulAddress,iBytes);

  if(iBytes>0xFE)
    iBytes=0xFE;

  ucaTxData[0]= 0x23;   // Uds Service "readMemoryByAddress"
  ucaTxData[1]=(unsigned char)((ulAddress>>16)&0xFF);  // MSB address
  ucaTxData[2]=(unsigned char)((ulAddress>>8)&0xFF);  //
  ucaTxData[3]=(unsigned char)(ulAddress&0xFF);  // LSB address
  ucaTxData[4]=(unsigned char)iBytes;

  //ulMemoryAccessAddress_m=ulAddress;   // Speicheradresse für späteres empfangen speichern
  //uiMemoryAccessSize_m=iBytes;
  //vUds_InvalidateMemoryData_m(ulAddress);
  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,5,caMessage);

  return(&bUds_GenAck_m);
} // pbUds_ReadMemoryByAddress_g()


/**
 *
 * Wrapper zu SID 0x23
 *
 */
BOOL *pbUds_RequestSerialNumber_g(void)
{
  return(pbUds_RequestDataByIdentifier_g(DID_SERIAL_NUMBER));
}
BOOL *pbUds_RequestCalibration_g(void)
{
  return(pbUds_RequestDataByIdentifier_g(DID_CALIBRATION));
}
BOOL *pbUds_RequestDataString_g(ULONG ulAddress,
                                UINT uiBytes)
{
  return(pbUds_RequestDataByIdentifier_g(ulAddress));
} // pbUds_RequestDataString_g()


/**
 * Funktionen zum lesen der Produktions-Interlock aus dem Eeprom
 */

/**
 * pbUds_RequestProductInterlock_g()<p>
 * Wrapper-Funktion zu SID 3Dh
 *
 * @author uidc9013 (23.03.2010)
 * @param iTool    Nummer der Vorrichtung (0-basiert)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_RequestProductInterlock_g(int iTool)
{
  return(pbUds_RequestDataByIdentifier_g(DID_PROD_INTRLOCK));
} // pbUds_RequestProductInterlock_g()









#if 0
/**
 * Returns the received data for memory at address ulAddress
 *
 * @param ulAddress address to read memory from
 * @param pucData   Buffer to return the bytes read
 * @param iBytes    size of data buffer pucBuffer
 *
 * @return TRUE on success<br>
 *         FALSE when no data available or failure
 * @see pbUds_ReadMemoryByAddress_g()
 */
BOOL bUds_GetMemoryByAddress_g(unsigned long ulAddress,
                               unsigned char *pucData,
                               int iBytes)
{
  unsigned int uiSize;

  if(!bUds_GetMemoryData_m(ulAddress,pucData,&uiSize))
  {
    // noch keine Daten vorhanden
    return(FALSE);
  }

  if(uiSize != iBytes)
  {
    printf("bUds_GetMemoryByAddress_g(): received unexpected number of bytes (%d instead of %d)\n",uiSize,iBytes);
    return(FALSE);
  }

  return(TRUE);
} // bUds_GetMemoryByAddress_g()
#endif


/**
 * Returns the serial number
 *
 * @param puiSerialNumber
 *               pointer to Serialnumber
 *
 * @return TRUE on success<br>
 *         FALSE when no data available or failure
 * @see pbUds_ReadMemoryByAddress_g()
 */
BOOL bUds_GetSerialNumber_g(unsigned int *puiSerialNumber)
{
  unsigned int uiSize;
  unsigned char ucaSerialNumber[DID_SERIAL_NUMBER_LEN];

  if(!bUds_GetStoredDataByDid_m(DID_SERIAL_NUMBER,
                                ucaSerialNumber,
                                &uiSize))
  {
    // noch keine Daten vorhanden
    //printf("no data\n");
    return(FALSE);
  }

  if(uiSize != 4)
  {
    //printf("bUds_GetSerialNumber(): received unexpected number of bytes (%d instead of 4)\n",uiSize);
    return(FALSE);
  }

  if(puiSerialNumber)
  {
    *puiSerialNumber=0;
    memcpy(puiSerialNumber,ucaSerialNumber,DID_SERIAL_NUMBER_LEN);
    if((*puiSerialNumber&0xFFFF00)==0xFFFF00) // Geraet wurde erst programmiert (steht FF FF FF FF im Speicher)
      *puiSerialNumber=0;
  }

  return(TRUE);
} // bUds_GetSerialNumber_g()


/**
 * Returns the calibration data
 *
 * @param pucCalibration
 *               pointer to Buffer to receive calibration data.
 *
 * @return TRUE on success<br>
 *         FALSE when no data available or failure
 * @see pbUds_ReadMemoryByAddress_g()
 */
BOOL bUds_GetCalibration_g(UCHAR *pucCalibration)
{
  unsigned int uiSize;
  unsigned char ucaCalibration[DID_CALIBRATION_LEN];

  if(!bUds_GetStoredDataByDid_m(DID_CALIBRATION,
                                ucaCalibration,
                                &uiSize))
  {
    // noch keine Daten vorhanden
    //printf("no data\n");
    return(FALSE);
  }

  if(uiSize!=DID_CALIBRATION_LEN)
  {
    //printf("bUds_GetCalibration_g(): received unexpected number of bytes (%d instead of %d)\n",uiSize,DID_CALIBRATION_LEN);
    return(FALSE);
  }

  if(pucCalibration)
  {
    memcpy(pucCalibration,ucaCalibration,DID_CALIBRATION_LEN);
  }

  return(TRUE);
} // bUds_GetCalibration_g()


/**
 * Returns the product number
 *
 * @param uiAddress
 *               DID of data to retrieve
 * @param pucDataString
 *               char array 16 bytes long
 *
 * @return TRUE on success<br>
 *         FALSE when no data available or failure
 * @see pbUds_ReadMemoryByAddress_g()
 */
BOOL bUds_GetDataString_g(unsigned int uiAddress,
                          unsigned char *pucDataString,
                          unsigned int *puiSize)
{
  if(!bUds_GetStoredDataByDid_m(uiAddress,
                                pucDataString,
                                puiSize))
  {
    // noch keine Daten vorhanden
    //printf("no data\n");
    return(FALSE);
  }
  pucDataString[*puiSize]= 0;   // String terminieren

  return(TRUE);
} // bUds_GetProductNumber_g()



/**
 * Funktionen zum lesen der Produktions-Interlock aus dem Eeprom
 */

/**
 * bUds_GetProductInterlockData_g()<p>
 *
 * @author uidc9013 (26.03.2010)
 * @param uiTool    Nummer der Vorrichtung (0-basiert)
 * @param puiVersionMajor
 *                  Buffer für Rückgabe der Version 1.xxx
 * @param puiVersionMinor
 *                  Buffer für Rückgabe der Version x.123
 * @param puiRunCounter
 *                  Buffer für Rückgabe der Anzahl Durchläufe
 * @param puiYear   Buffer für Rückgabe des Jahres
 * @param puiMonth  Buffer für Rückgabe des Monates
 * @param puiDay    Buffer für Rückgabe des Tages
 * @param puiHour   Buffer für Rückgabe der Stunden
 * @param puiMinute Buffer für Rückgabe der Minuten
 * @param puiSecond Buffer für Rückgabe der Sekunden
 *
 * @return Pointer auf ACK-Flag
 * @see pbUds_RequestProductInterlock_g()
 */
BOOL bUds_GetProductInterlockData_g(unsigned int uiTool,
                                    unsigned int *puiVersionMajor,
                                    unsigned int *puiVersionMinor,
                                    unsigned int *puiRunCounter,
                                    unsigned int *puiYear,
                                    unsigned int *puiMonth,
                                    unsigned int *puiDay,
                                    unsigned int *puiHour,
                                    unsigned int *puiMinute,
                                    unsigned int *puiSecond)
{
  UINT uiSize;

  if(!bUds_GetStoredDataByDid_m(DID_PROD_INTRLOCK,ucInterlockData_m,&uiSize))
  {
    // noch keine Daten vorhanden
    return(FALSE);
  }

  if(uiSize!=DID_PROD_INTRLOCK_LEN)
  {
    //printf("bUds_GetProductInterlockData_g(): received unexpected number of bytes (%d instead of %d)\n",uiSize,DID_PROD_INTRLOCK_LEN);
    return(FALSE);
  }

  return(bPIL_GetData_g(eInterlockFormat_Complete,
                        &ucInterlockData_m[uiTool*PROD_INTERLOCK_SIZE],
                        PROD_INTERLOCK_SIZE,
                        puiVersionMajor,
                        puiVersionMinor,
                        puiRunCounter,
                        puiYear,
                        puiMonth,
                        puiDay,
                        puiHour,
                        puiMinute,
                        puiSecond
                       ));
} // bUds_GetProductInterlockData_g()








//       writeMemoryByAddress: SID 3Dh
/**
 * pbUds_WriteMemoryByAddress_g()<p>
 *
 * @author uidc9013 (22.01.2010)
 * @param ulAddress Startadresse vom zu lesenden Speicher
 * @param iBytes    Anzahl zu lesende bytes. Max=0xFE
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteMemoryByAddress_g(unsigned long ulAddress,
                                   unsigned char *pucData,
                                   int iBytes)
{
  unsigned char *pucTxData;
  char caMessage[100];
  int iTxDataSize;

  iTxDataSize=iBytes+5;

  pucTxData =(unsigned char*)malloc(iTxDataSize);
  memset(pucTxData,0,sizeof(iTxDataSize));

  sprintf(caMessage,"write memory 0x%06x %d bytes",ulAddress,iBytes);

  // Maximal 250 bytes/Telegram
  if(iBytes>0xFA)
    iBytes=0xFA;

  pucTxData[0]= 0x3D;   // Uds Service "writeMemoryByAddress"
  pucTxData[1]=(unsigned char)((ulAddress>>16)&0xFF);  // MSB address
  pucTxData[2]=(unsigned char)((ulAddress>>8)&0xFF);  //
  pucTxData[3]=(unsigned char)(ulAddress&0xFF);  // LSB address
  pucTxData[4]=(unsigned char)iBytes;
  memcpy(&pucTxData[5],pucData,iBytes);

  vUds_SendCmd_m(ECU_CAN_ADDRESS,pucTxData,iTxDataSize,caMessage);
  free(pucTxData);

  return(&bUds_GenAck_m);
} // pbUds_WriteMemoryByAddress_g()


/**
 * pbUds_StepperMotorSetOffset_g()<p>
 * Wrapper-Funktion zu SID 3Dh
 *
 * @author uidc9013 (19.03.2010)
 * @param auiOffset
 *                zu schreibenden Offset in Steps
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_StepperMotorSetOffset_g(UINT uiOffset)
{
  unsigned char ucaData[DID_GAUGE_OFFSET_LEN];

  ucaData[0]=uiOffset&0xFF;   // MSB
  ucaData[1]=(uiOffset&0xFF00)>>8;   // LSB

  return(pbUds_WriteDataByIdentifier_g(DID_GAUGE_OFFSET,
                                       ucaData,
                                       DID_GAUGE_OFFSET_LEN));
} // pbUds_StepperMotorSetOffset_g()


/**
 * pbUds_WriteSerialNumber_g()<p>
 * Wrapper-Funktion zu SID 2Eh
 *
 * @author uidc9013 (14.04.2010)
 * @param uiSerialNumber
 *               zu schreibende Serienummer. Range 0..65535.
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteSerialNumber_g(unsigned int uiSerialNumber)
{
  unsigned char ucaData[DID_SERIAL_NUMBER_LEN];

  memcpy(ucaData,&uiSerialNumber,DID_SERIAL_NUMBER_LEN);
  return(pbUds_WriteDataByIdentifier_g(DID_SERIAL_NUMBER,
                                       ucaData,
                                       DID_SERIAL_NUMBER_LEN));
} // pbUds_WriteSerialNumber_g()


/**
 * pbUds_WriteDialScaleValues_g()<p>
 * Wrapper-Funktion zu SID 2Eh
 *
 * @author uidc9013 (22.03.2017)
 * @param uiXScaleValue
 *               X-Scale value. Range 0..65535.
 * @param uiYScaleValue
 *               Y-Scale value. Range 0..65535.
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteDialScaleValues_g(UINT uiXScaleValue,
                                   UINT uiYScaleValue)
{
  unsigned char ucaData[DID_DIAL_SCALE_VALUES_LEN];
  uiXScaleValue=min(uiXScaleValue,0xffff);
  uiYScaleValue=min(uiYScaleValue,0xffff);
  memcpy(ucaData,&uiXScaleValue,2);
  memcpy(&ucaData[2],&uiYScaleValue,2);
  return(pbUds_WriteDataByIdentifier_g(DID_DIAL_SCALE_VALUES,
                                       ucaData,
                                       DID_DIAL_SCALE_VALUES_LEN));
} // pbUds_WriteDialScaleValues_g()


/**
 * pbUds_WriteGaugeType_g()<p>
 * Wrapper-Funktion zu SID 2Eh
 *
 * @author uidc9013 (22.03.2017)
 * @param uiGaugeType
 *               Gauge type: 1=Tacho, 2=Speedo
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteGaugeType_g(UINT uiGaugeType)
{
  unsigned char ucaData[DID_GAUGE_TYPE_LEN];
  memcpy(ucaData,&uiGaugeType,DID_GAUGE_TYPE_LEN);
  return(pbUds_WriteDataByIdentifier_g(DID_GAUGE_TYPE,
                                       ucaData,
                                       DID_GAUGE_TYPE_LEN));
} // pbUds_WriteGaugeType_g()


/**
 * pbUds_WriteCalibration_g()<p>
 * Wrapper-Funktion zu SID 2Eh
 *
 * @author uidc9013 (14.04.2010)
 * @param uiSerialNumber
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteCalibration_g(unsigned char *pucCalibration)
{
  unsigned char ucaData[DID_CALIBRATION_LEN];

  memset(ucaData,0,DID_CALIBRATION_LEN);
  memcpy(ucaData,pucCalibration,DID_CALIBRATION_LEN);
  return(pbUds_WriteDataByIdentifier_g(DID_CALIBRATION,
                                       ucaData,
                                       DID_CALIBRATION_LEN));
} // pbUds_WriteCalibration_g()


/**
 * pbUds_WriteDataString_g()<p>
 * Wrapper-Funktion zu SID 3Dh
 *
 * @author uidc9013 (20.04.2010)
 * @param pcDataString
 *               zu schreibende Daten
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_WriteDataString_g(ULONG ulAddress,
                              char *pcDataString,
                              UINT uiStrlen)
{
  UCHAR *pucData;
  BOOL *pbAck;

  pucData=malloc(uiStrlen);

  memset(pucData,0,uiStrlen);
  memcpy(pucData,pcDataString,uiStrlen);

  pbAck=pbUds_WriteMemoryByAddress_g(ulAddress,
                                     pucData,
                                     uiStrlen);
  free(pucData);
  return(pbAck);
} // pbUds_WriteDataString_g()


/**
 * Funktionen zum schreiben des Produktions-Interlock in den Eeprom
 */

/**
 * pbUds_SetProductInterlock_g()<p>
 * Wrapper-Funktion zu SID 2Eh
 *
 * @author uidc9013 (23.03.2010)
 * @param iTool    Nummer der Vorrichtung (0-basiert)
 * @param iVersionMajor
 *                 Version 1.xxx
 * @param iVersionMinor
 *                 Version x.123
 * @param iCounter Durchgangszähler dieser Vorrichtung
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_SetProductInterlock_g(int iTool,
                                  int iVersionMajor,
                                  int iVersionMinor,
                                  int iCounter)
{
  if(!bPIL_SetData_g(eInterlockFormat_Complete,
                     iVersionMajor,
                     iVersionMinor,
                     iCounter,
                     &ucInterlockData_m[iTool*PROD_INTERLOCK_SIZE],
                     DID_PROD_INTRLOCK_LEN))
    return(NULL);

  return(pbUds_WriteDataByIdentifier_g(DID_PROD_INTRLOCK,
                                       ucInterlockData_m,
                                       DID_PROD_INTRLOCK_LEN));
} // pbUds_SetProductInterlock_g()













/**
 * Diese Funktion ist nur für Testzwecke!!!
 *
 * @author uidc9013 (04.03.2010)
 */
void vUds_IoCtrlInfoDump_g(void)
{
  vUds_IoCtrlInfoDump_m();
} // vUds_IoCtrlInfoDump_g()



//  I/O Control
//      inputOutputControlByCommonIdentifier: SID 2Fh
/**
 * pbUds_IoCtrlRequestDigitalInput_g()
 *
 * @author uidc9013 (22.01.2010)
 *
 * @return Pointer auf ACK-Flag
 */
BOOL *pbUds_IoCtrlRequestDigitalInput_g(void)
{
  char caMessage[50];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  char caDescription[100];

  usLocalIdentifier=CID_IN_DIG;
  bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription);
  sprintf(caMessage,"request %s",caDescription);
  pbAckFlag=pbUds_ReadDataByIdentifier_m(usLocalIdentifier,
                                         caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlRequestDigitalInput_g()


/**
 * pbUds_IoCtrlRequestAnalogInputs_g()<p>
 * requests
 *
 * @author uidc9013 (15.03.2010)
 *
 * @return Pointer auf ACK-Flag
 */
BOOL *pbUds_IoCtrlRequestAnalogInputs_g(void)
{
  char caMessage[50];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  char caDescription[100];

  usLocalIdentifier=CID_IN_ANA;
  bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription);
  sprintf(caMessage,"request %s",caDescription);
  pbAckFlag=pbUds_ReadDataByIdentifier_m(usLocalIdentifier,
                                         caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlRequestAnalogInputs_g()


/**
 * pbUds_IoCtrlRequestFrequencyInput_g()<p>
 *
 * @author uidc9013 (22.03.2017)
 *
 * @return Pointer auf ACK-Flag
 */
BOOL *pbUds_IoCtrlRequestFrequencyInput_g(void)
{
  char caMessage[50];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  char caDescription[100];

  usLocalIdentifier=CID_IN_FREQ;
  bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription);
  sprintf(caMessage,"request %s",caDescription);
  pbAckFlag=pbUds_ReadDataByIdentifier_m(usLocalIdentifier,
                                         caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlRequestFrequencyInput_g()


/**
 * pbUds_IoCtrlRequestStepperMotorState_g()<p>
 * requests
 *
 * @author uidc9013 (15.03.2010)
 *
 * @return Pointer auf ACK-Flag
 */
BOOL *pbUds_IoCtrlRequestStepperMotorState_g(void)
{
  char caMessage[50];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  char caDescription[100];

  usLocalIdentifier=CID_IN_ANA_STEPPER_MOTOR;
  bUds_IOControlByCommonIdentifierGetControlInfos_m(usLocalIdentifier,NULL,NULL,NULL,caDescription);
  sprintf(caMessage,"request %s",caDescription);
  pbAckFlag=pbUds_ReadDataByIdentifier_m(usLocalIdentifier,
                                         caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlRequestStepperMotorState_g()


/**
 * bUds_IoCtrlGetDigitalInput_g()
 *
 * @author uidc9013 (04.03.2010)
 * @param ucInput Input. Range: 0 to 23
 * @param bInputState
 *                Variable für R&uuml;ckgabe des Inputzustandes
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<br>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 */
BOOL bUds_IoCtrlGetDigitalInput_g(unsigned char ucInput,
                                  BOOL *bInputState)
{
  unsigned short usLocalIdentifier;
  unsigned char ucaStatus[NUM_DIG_INPUTS+5];   // 5 bytes Reserve
  int iBytes;

  if(ucInput>(NUM_DIG_INPUTS-1))
  {
    //printf("bUds_IoCtrlGetDigitalInput_g(%d): invalid input specified (Range = 0..%d )\n",ucInput,NUM_DIG_INPUTS-1);
    return(FALSE);
  }

  // Time    <ID>  0  1  2  3  4  5  6  7
  // 4.92 Tx 0560 03 22 02 04               request IN_DigInp
  // 0.08 Rx 07c2 10 08 62 02 04 01 01 00
  // 0.00 Tx 0560 30 00 0a                  Flow Control
  // 0.10 Rx 07c2 21 00 00 00 00 00 00 00   consecutive frame

  // 0.08 Rx 07c2 10 08 62 02 04 01 01 00 00 00
  //                             ==--> 0: IGN
  //                                ==--> 1: Illum
  //                                   ==--> 2: GPS module
  //                                      ==--> (GPS SNR)
  //                                         ==--> 3: Trip Switch

  switch(ucInput)
  {
    case 0:
    case 1:
    case 2:
    default:
      break;
    case 3:
      ucInput=4;
      break;
  }

  usLocalIdentifier=CID_IN_DIG;
  if(!bUds_IOControlByCommonIdentifierGetCtrlValue_m(usLocalIdentifier,ucaStatus,&iBytes))
    return(FALSE);
  *bInputState=(ucaStatus[ucInput]>0);

  return(TRUE);
} // bUds_IoCtrlGetDigitalInput_g()




/**
 * bUds_IoCtrlGetAnalogInput_g()
 *
 * <p>vorher muss pbUds_IoCtrlRequestAnalogInputs_g() aufgerufen worden sein
 *
 * @author uidc9013 (04.03.2010)
 * @param ucInput Input. Range: 0 to 1
 * @param puiAnalogInput
 *                Pointer auf int-Variable f&uuml;r R&uuml;ckgabe des Inputes
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<br>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 * @see pbUds_IoCtrlRequestAnalogInputs_g()
 */
BOOL bUds_IoCtrlGetAnalogInput_g(unsigned char ucInput,
                                 unsigned int *puiAnalogInput)
{
  unsigned short usLocalIdentifier;
  int iBytes;
  int iAnalogInputValue;
  unsigned char ucaValue[NUM_ANALOG_INPUTS*2];

  if(ucInput>(NUM_ANALOG_INPUTS-1))
  {
    printf("bUds_IoCtrlGetAnalogInput_g(%d): invalid input specified (Range = 0..%d)\n",ucInput,NUM_ANALOG_INPUTS-1);
    return(FALSE);
  }

  usLocalIdentifier=CID_IN_ANA;
  if(!bUds_IOControlByCommonIdentifierGetCtrlValue_m(usLocalIdentifier,ucaValue,&iBytes))
  {
    return(FALSE);
  }

  iAnalogInputValue=(ucaValue[ucInput*2]*256) + (ucaValue[(ucInput*2)+1]);
  if(puiAnalogInput)
    *puiAnalogInput=iAnalogInputValue;

  return(TRUE);
} // bUds_IoCtrlGetAnalogInput_g()


/**
 * bUds_IoCtrlGetGpsSnr_g()
 *
 * vorher muss pbUds_IoCtrlRequestDigitalInput_g() aufgerufen worden sein
 * GPS SNR wird mit dem Telegram der digitalen Inputs übertragen.
 *
 * @author uidc9013 (16.03.2017)
 * @param puiGpsSnr
 *                Pointer auf int-Variable f&uuml;r R&uuml;ckgabe des SNR
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<br>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 */
BOOL bUds_IoCtrlGetGpsSnr_g(UINT *puiGpsSnr)
{
  unsigned short usLocalIdentifier;
  int iBytes;
  char caValue[2];   // NUM_DIG_INPUTS
  // GPS SNR wird auch als dig. Input empfangen
  // byte 0 = GPS Status 0 oder 1
  // byte 1 = GPS SNR

  usLocalIdentifier=CID_IN_DIG;
  if(!bUds_IOControlByCommonIdentifierGetCtrlValue_m(usLocalIdentifier,caValue,&iBytes))
  {
    return(FALSE);
  }
  if(puiGpsSnr)
    *puiGpsSnr=caValue[1];

  return(TRUE);
} // bUds_IoCtrlGetAnalogInput_g()


/**
 * bUds_IoCtrlGetStepperMotorState_g()
 *
 * vorher muss pbUds_IoCtrlRequestStepperMotorState_g() aufgerufen worden sein
 *
 * @author uidc9013 (04.03.2010)
 * @param ucInput Input. Range: 0 to 5
 * @param piAnalogInput
 *                Pointer auf int-Variable f&uuml;r R&uuml;ckgabe des Inputes
 *
 * @return TRUE, wenn ein Zustand zur&uuml;ckgegeben werden konnte<br>
 *         FALSE, wenn Zustand noch nicht empfangen wurde oder einem sonstigen Fehler.
 */
BOOL bUds_IoCtrlGetStepperMotorState_g(UINT *puiAnalogInput)
{
  unsigned short usLocalIdentifier;
  int iBytes;
  int iAnalogInput;
  char caValue[1];

  usLocalIdentifier=CID_IN_ANA_STEPPER_MOTOR;
  if(!bUds_IOControlByCommonIdentifierGetCtrlValue_m(usLocalIdentifier,caValue,&iBytes))
  {
    return(FALSE);
  }

  iAnalogInput=caValue[0];
  iAnalogInput&=0xFF;   // Fuer Stepper Motor 1. Byte verwendet
  *puiAnalogInput = iAnalogInput;

  return(TRUE);
} // bUds_IoCtrlGetAnalogInput_g()







/**
 * pbUds_IoCtrlSetTTBitmap_g()<p>
 *
 * @author uidc9013 (28.01.2010)
 * @param ulBitmap Bitmap der zu schaltenden Telltales.<br>
 *                 bit=1 => TT einschalten<br>
 *                 bit=0 => TT ausschalten
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlSetTTBitmap_g(unsigned long ulBitmap)
{
  return(pbUds_StartRoutineByLocalId_m(ECU_CAN_ADDRESS,
                                       eRoutineByLocalId_TelltaleTest,
                                       1, // 0= normal Mode ->plc running
                                          //    1= output test pattern only -> plc stopped
                                       (unsigned char)(ulBitmap&0xff),
                                       (unsigned char)((ulBitmap>>8)&0xff),
                                       (unsigned char)((ulBitmap>>16)&0xff),
                                       (unsigned char)((ulBitmap>>24)&0xff)));
} // pbUds_IoCtrlSetTTBitmap_g()


#if 0
/**
 * pbUds_IoCtrlStepperMotorSetPositionPercent_g()<p>
 *
 * @author uidc9013 (25.02.2010)
 * @param uiMotor   zu bewegenden Motor (0..3)<br>
 *                  0 = Speedo (gross rechts)<br>
 *                  1 = Tacho (gross links)<br>
 *                  2 = Fuel (klein rechts)<br>
 *                  3 = Temp (klein links)<br>
 * @param uiPercent anzufahrende Position in % 0..100 [0.01%] (10000=100%)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlStepperMotorSetPosition_g(int iPosition,
                                            int iPositionEasyLink)
{
// RPM:   0.25 RPM (unsigned)
//        1 = 0.25rpm
//
// Speed: 1x10E-2 m/s (unsigned)
//        1 = 0.1m/s = 0.36km/h = 0.194knot
//
// Depth: 1x10E-2 m (unsigned)
//        1 = 0.1m

  char caMessage[200];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier=0;
  UCHAR ucaData[CID_POINTER_OVERRIDE_LEN];
  char *pcDescription;

  usLocalIdentifier=CID_POINTER_OVERRIDE;

  ucaData[0]=iPositionEasyLink&0xFF;
  ucaData[1]=(iPositionEasyLink&0xFF00)>>8;
  ucaData[2]=iPosition&0xFF;
  ucaData[3]=(iPosition&0xFF00)>>8;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"set %s to %d EasyLink:%d",
          pcDescription,
          iPosition,
          iPositionEasyLink);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                ucaData,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlStepperMotorSetPositionPercent_g()
#endif

#if 0
/**
 * pbUds_IoCtrlStepperMotorSetPositionPercent_g()<p>
 *
 * @author uidc9013 (25.02.2010)
 * @param uiMotor   zu bewegenden Motor (0..3)<br>
 *                  0 = Speedo (gross rechts)<br>
 *                  1 = Tacho (gross links)<br>
 *                  2 = Fuel (klein rechts)<br>
 *                  3 = Temp (klein links)<br>
 * @param uiPercent anzufahrende Position in % 0..100 [0.01%] (10000=100%)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlStepperMotor360SetPosition_g(int iPosition,
                                               int iPositionEasyLink)
{
// Angle: 1x10E-4 rad (rudder angle is signed, all others unsigned)
//        1 = ~0.0057°

  char caMessage[200];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier=0;
  UCHAR ucaData[CID_POINTER_OVERRIDE_LEN];
  char *pcDescription;

  usLocalIdentifier=CID_POINTER_OVERRIDE;

  iPosition*=PI/180*10000;

  ucaData[0]=iPositionEasyLink&0xFF;
  ucaData[1]=(iPositionEasyLink&0xFF00)>>8;
  ucaData[2]=iPosition&0xFF;
  ucaData[3]=(iPosition&0xFF00)>>8;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"set %s to %d EasyLink:%d",
          pcDescription,
          iPosition,
          iPositionEasyLink);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                ucaData,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlStepperMotorSetPositionPercent_g()
#endif

#if 0
/**
 * pbUds_IoCtrlSetBacklight_g()<p>
 *
 * @author uidc9013 (7.10.2015)
 * @param bPointer TRUE = Beleuchtung einschalten<br>
 *                 FALSE = Beleuchtung ausschalten
 * @param bDial    TRUE = Beleuchtung einschalten<br>
 *                 FALSE = Beleuchtung ausschalten
 * @param bLcd     TRUE = Beleuchtung einschalten<br>
 *                 FALSE = Beleuchtung ausschalten<br>
 *                 <b>!!!NOT YET TESTED!!! SUPPORTED BY DUT???</b>
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlSetBacklight_g(BOOL bPointer,
                                 BOOL bDial,
                                 BOOL bLcd,
                                 BOOL bEasyLink)
{
  char caMessage[200];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier=0;
  UCHAR ucaDutyCycle[CID_PWM_ILLUMINATION_LEN];
  char *pcDescription;

  usLocalIdentifier=CID_PWM_ILLUMINATION;

  ucaDutyCycle[0]=(bLcd)?100:0;
  ucaDutyCycle[1]=(bPointer)?100:0;
  ucaDutyCycle[2]=(bDial)?100:0;
  ucaDutyCycle[3]=(bEasyLink)?7:0;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"set %s",pcDescription);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                ucaDutyCycle,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlSetBacklight_g()
#endif

#if 0
/**
 * pbUds_IoCtrlSetEasyLink_g()<p>
 *
 * @author uidc9013 (06.07.2010)
 * @param bActivate TRUE => EasyLink einschalten<br>
 *                  FALSE => EasyLink ausschalten
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlSetEasyLink_g(BOOL bActivate)
{
  char caMessage[100];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  unsigned char ucValue;
  char *pcDescription;

  usLocalIdentifier=CID_OUT_DIG_OUT;
  ucValue=(bActivate)?1:0;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"%s %s",pcDescription,(bActivate)?"on":"off");
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                &ucValue,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlSetEasyLink_g()
#endif

/**
 * pbUds_IoCtrlSetDigOutput_g()<p>
 *
 * @author uidc9013 (06.07.2010)
 * @param bActivate TRUE => Alarm einschalten<br>
 *                  FALSE => Alarm ausschalten
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlSetDigOutput_g(unsigned long ulBitmap)
{
  char caMessage[100];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  unsigned char ucaValue[2];
  char *pcDescription;

  usLocalIdentifier=CID_OUT_DIG_OUT;
  ucaValue[0]=(ulBitmap&0x01)?1:0;
  ucaValue[1]=(ulBitmap&0x02)?1:0;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"set %s",pcDescription);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                ucaValue,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlSetEasyLink_g()




/**
 * pbUds_RequestLocalTime_g()
 *
 * @author uidc9013 (22.01.2010)
 * @return Pointer auf ACK-Flag
 * @see bUds_GetLocalTime_g()
 */
BOOL *pbUds_RequestLocalTime_g(void)
{
  char caMessage[50];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier;
  char *pcDescription;

  usLocalIdentifier=CID_IN_LOCAL_TIME;
  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"request %s",pcDescription);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeFreezeCurrentState,
                                                NULL,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_RequestLocalTime_g()


/**
 * bUds_GetLocalTime_g()
 *
 * @author uidc9013 (04.03.2010)
 * @param pucTime Buffer zur Rückgabe des gelesenen Zeitwertes<br>
 *                (Wertigkeit 25ms...89468321d19h42m34s400ms?)
 *
 * @return TRUE, wenn eine Zeit zur&uuml;ckgegeben werden konnte<br>
 *         FALSE, wenn Zeit noch nicht empfangen wurde oder einem sonstigen Fehler.
 */
BOOL bUds_GetLocalTime_g(unsigned char *pucTime)
{

  unsigned short usLocalIdentifier;
  unsigned char ucaStatus[10];
  int iBytes;

  usLocalIdentifier=CID_IN_LOCAL_TIME;
  if(!bUds_IOControlByCommonIdentifierGetCtrlValue_m(usLocalIdentifier,ucaStatus,&iBytes))
    return(FALSE);
  if(iBytes<10)
  {
    //printf("bUds_GetLocalTime_g(0x%04x): unexpected size %d returned for input. expected %d\n",usLocalIdentifier,iBytes,10);
    return(FALSE);
  }

  memcpy((void*)pucTime,(void*)ucaStatus,8);

  return(TRUE);
} // bUds_GetLocalTime_g()




//  Access to diagnostic buffers
//       clearDiagnosticInformation: SID 14h
/**
 * pbUds_ClearDtcInformation_g()<p>
 *
 * @author uidc9013 (25.01.2010)
 * @param iSpn   SPN = Suspect Part Number
 * @param iFmi   FMI = Failure Mode Identifier
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_ClearDtcInformation_g(unsigned int iSpn,
                                  unsigned int iFmi)
{
  unsigned char ucaTxData[16];
  char caMessage[100];

  memset(ucaTxData,0,sizeof(ucaTxData));

  sprintf(caMessage,"clear DTC SPN=x%06x FMI=x%1x",iSpn,iFmi);

  ucaTxData[0]= 0x14;   // Uds Service "clearDiagnosticInformation"
  vUds_GetDtcGroup_m(iSpn,iFmi,
                     &ucaTxData[1],   // MSB DTC group
                     &ucaTxData[2],   //
                     &ucaTxData[3]);  // LSB DTC group

  vUds_SendCmd_m(ECU_CAN_ADDRESS,ucaTxData,4,caMessage);

  return(&bUds_GenAck_m);
} // pbUds_ClearDtcInformation_g()


#if 0
/**
 * pbUds_IoCtrlLcdSetPattern_g()<p>
 *
 * @author uidc9013 (16.03.2017)
 * @param uiLcdPattern
 *               zu setzendes LCD Pattern. (Range: 0...3)
 *
 * @return Pointer auf ACK-Flag
 * @see bUds_GenAck_g()
 */
BOOL *pbUds_IoCtrlLcdSetPattern_g(unsigned int uiLcdPattern)
{
  char caMessage[200];
  BOOL *pbAckFlag;
  unsigned short usLocalIdentifier=0;
  UCHAR ucaData[CID_POINTER_OVERRIDE_LEN];
  char *pcDescription;

  usLocalIdentifier=CID_LCD_OVERRIDE;
  ucaData[0]=(UCHAR)uiLcdPattern;

  pcDescription=pcUds_IOControlByCommonIdentifierGetDescription_m(usLocalIdentifier);
  sprintf(caMessage,"set %s to %d",
          pcDescription,
          uiLcdPattern);
  pbAckFlag=pbUds_IOControlByCommonIdentifier_m(usLocalIdentifier,
                                                eIoCtrlModeShortTermAdjust,
                                                ucaData,
                                                caMessage);
  return(pbAckFlag);
} // pbUds_IoCtrlStepperMotorSetPositionPercent_g()
#endif









