/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   .
 *
 *    FILE:        LibDutCom_private.c
 *
 *    DESCRIPTION: implementation of private, non-public functions
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
  #include <ansi_c.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include "LibDutCom_private.h"

#include "Uds.h"
#include "Uds_private.h"
#include "ProdInterlock.h"

#include "DeviceSpecific.h"
#include "Trace.h"


unsigned int uiBinaryDivision(unsigned char *upcDividend,int iLen,unsigned int uiDivisor)
{
  unsigned uiQuotient = 0;
  unsigned uiRest = 0;
  int iIndex;
  unsigned char ucTempDividend[10];

  memset(ucTempDividend,0,sizeof(ucTempDividend));
  for(iIndex=0; iIndex<iLen; iIndex++)
    ucTempDividend[iLen-1-iIndex]=upcDividend[iIndex];

  for(iIndex=iLen; iIndex>=0; --iIndex)
  {
    unsigned char *pcZiffer =&ucTempDividend[iIndex];
    unsigned uiDividend =(uiRest<<8)|*pcZiffer;
    unsigned uiTemp = uiDividend/uiDivisor;
    uiQuotient =(uiQuotient<<8)|uiTemp;
    uiRest = uiDividend%uiDivisor;
  }
  return(uiQuotient);
}


/**
 * USE_TIMED_SENDER_THREAD:
 * use separate thread to send cyclic messages to DUT.
 * I.e. to keep DUT alive or other cyclic messages
 */
#define USE_TIMED_SENDER_THREAD



/**
 * pcDutCom_TraceGetHeader_m()<p>
 * Returns the header for the trace as string
 *
 * @author uidc9013 (13.01.2012)
 * @return header for trace as string
 */
char *pcDutCom_TraceGetHeader_m(void)
{
  return(LOG_HEADER_UDS);
} // pcDutCom_TraceGetHeader_m()


/**
 * vDutCom_TraceEnable_m()<p>
 * enable or disable the collection of trace data
 *
 * @author uidc9013 (16.01.2012)
 * @return header for trace as string
 */
void vDutCom_TraceEnable_m(BOOL bEnable)
{
  bUds_TraceEnable_g(bEnable);
} // vDutCom_TraceEnable_m()


/**
 * Wartet bis das Flag pbFlag den Zustand bStateSoll hat.
 *
 * @param pbFlag     Pointer auf zu überwachendes Flag
 * @param bStateSoll zu erwartender Zustand von Flag
 * @param dwTimeOut  zu wartende Zeit in ms
 *
 * @return TRUE => erwarteter Zustand ist eingetreten<br>
 *         FALSE => erwarteter Zustand ist nicht eingetreten.
 */
EWaitResult bWaitResponseFlag_m(BOOL *pbFlag,                  // Pointer auf abzuwartendes Flag
                                BOOL bStateSoll,              // erwarteter Zustand von Flag
                                DWORD dwTimeOut)               // timeout
{
  DWORD dwWaitInterval;
  DWORD dwTimeWaited;
  //vSetComState_Tx_g();

  dwTimeWaited=0;
  dwWaitInterval=dwTimeOut/10;
  do
  {
    Sleep(dwWaitInterval);
    dwTimeWaited+=dwWaitInterval;

    if(dwTimeWaited>dwTimeOut)
    {
      // timeout
      return(FALSE);
    }
  } while(*pbFlag!=bStateSoll);
  //vSetComState_Rx_g();
  return(TRUE);
} // bWaitResponseFlag_m()


extern char caDutComErrorMessage_m[DUT_COM_ERROR_MESSAGE_LEN+1];

TagComSendData tagComSendData_m;
TagComRecvData tagComRecvData_m;
TagLibDutComParam tagLibDutComParam_m;

TagDigInp tagaDigInp[DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX];
TagDigOutp tagaDigOutp[DUT_COM_DEV_SPEC_TEST_DIG_OUTPUT_MAX];


/**
 * Sets digital output to state bSet.
 * If digital output is not defined (may happen if run off-line) then
 * this function prints out an informative text.
 *
 * @author WeU (14.12.2012)
 * @param uiTestNr   Number of dev specific test. (0-based)
 * @param uiOutputNr Number of output. (0-based)
 * @param bSet       State to set output to.
 */
void vSetDO_m(UINT uiTestNr,
              UINT uiOutputNr,BOOL bSet)
{
  if(tagaDigOutp[uiOutputNr].IBS_Node)
  {
    *tagaDigOutp[uiOutputNr].IBS_Node=(T_IBS_BOOL)bSet;
  }
  else
  {
    printf("Ausgang %d %sschalten (%s)\n",
           uiOutputNr,
           (bSet)?"ein":"aus",
           pcDevSpec_DeviceSpecificTestOutputGetText_m(uiTestNr,uiOutputNr));
  }
} // vSetDO_m()

/**
 * Returns the state of the digital input.
 * If this digital input is not assigned, may happen if run off-line,<br>
 * then this function returns bSollState.
 *
 * @author WeU (14.12.2012)
 * @param uiTestNr  Number of dev specific test. (0-based)
 * @param uiInputNr Number of input. (0-based)
 * @param bStateExpected
 *                  expected state of the digital input.
 *
 * @return BOOL
 */
BOOL bReadDI_m(UINT uiTestNr,
               UINT uiInputNr,
               BOOL bStateExpected)
{
  if(tagaDigInp[uiInputNr].IBS_Node)
  {
    return((*(tagaDigInp[uiInputNr].IBS_Node))>0);
  }
  else
  {
    printf("Warte auf Eingang %d (%s) = %s\n",
           uiInputNr,
           pcDevSpec_DeviceSpecificTestInputGetText_m(uiTestNr,uiInputNr),
           (bStateExpected)?"True":"False");
    return(bStateExpected);
  }
} // iReadDI_m()



void vResetComSendData_m(void)
{
  if(tagComSendData_m.tagStaticDataBulk.pucWriteData)
    free(tagComSendData_m.tagStaticDataBulk.pucWriteData);

  memset((void*)&tagComSendData_m,0,sizeof(TagComSendData));
} // vResetComSendData_m()

void vResetComRecvData_m(void)
{
  memset((void*)&tagComRecvData_m,0,sizeof(TagComRecvData));
} // vResetComRecvData_m()


void vResetDigIOData_m(void)
{
  memset(tagaDigInp,0,sizeof(TagDigInp)*DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX);
  memset(tagaDigOutp,0,sizeof(TagDigOutp)*DUT_COM_DEV_SPEC_TEST_DIG_OUTPUT_MAX);
} // vResetDigIOData_m()



BOOL bSendTesterPresent_m=FALSE;
BOOL bEnableTesterPresent_m=FALSE;


#ifdef USE_TIMED_SENDER_THREAD
/**
 * Thread to manage all repetitive timely telegrams
 *
 * @author uidc9013 (19.01.2012)
 *
 * @param lpParam LPVOID pointer to thread control data of type TagComControl
 *
 * @return returns 0
 * @see TagComControl
 */
DWORD WINAPI dwDutCom_TimedSenderThread_m(LPVOID lpParam)
{
  TagComControl *ptagComControlTimer;
  int iDelayCounter;

  //printf("LibDutCom: dwDutCom_TimedSenderThread_m started\n");
  ptagComControlTimer=(TagComControl*)lpParam;
  ptagComControlTimer->bIsRunning=TRUE;

  while(1)
  {
    if(bEnableTesterPresent_m)
      bSendTesterPresent_m=TRUE;

    // wait until message has been sent
    while(bSendTesterPresent_m==TRUE)
    {
      Sleep(50);
    }

    iDelayCounter=0;
    while((iDelayCounter<20000)   // alle 20s ein Telegramm senden.
          &&!(ptagComControlTimer->bTerminate)
         )
    {
      Sleep(100);
      iDelayCounter+=100;
    }

    if(ptagComControlTimer->bTerminate)
      break;
  }

  ptagComControlTimer->bIsRunning=FALSE;

  //printf("LibDutCom: dwDutCom_TimedSenderThread_m ended\n");

  return(0);
} // dwDutCom_TimedSenderThread_m()
#endif // USE_TIMED_SENDER_THREAD


/**
 * Thread function to manage all dut specific communications
 *
 * @author uidc9013 (22.12.2011)
 * @param lpParam LPVOID pointer to thread control data of type TagComControl
 *
 * @return returns 0
 * @see TagComControl
 */
DWORD WINAPI dwDutCom_SenderThread_m(LPVOID lpParam)
{
  TagComControl *ptagComControl;
#ifdef USE_TIMED_SENDER_THREAD
  TagComControl tagComControlTimer;
#endif // USE_TIMED_SENDER_THREAD
  //int iTimeMs=0;
  BOOL *pbAck=NULL;
  BOOL bReceivedAck=FALSE;
  int iTimeOutMs;
  int iRepeats=0;
  int iIndex;
  UCHAR ucaLocalTime[8+1];

  UINT uiPointerAdjustOffset_m=0;
  //static int iaPointerPosition_m[NUM_POINTERS];
  static UINT uiDialScaleX_m;
  static UINT uiDialScaleY_m;

  DWORD dwThreadId;

  ESecurityAccess eSecurityAccess_m=eSecurityAccessDefault;

  ptagComControl=(TagComControl*)lpParam;
  ptagComControl->bIsRunning=TRUE;

  // open communication port(s)
  bUds_Init_g();

  vUds_EnableTesterPresent_g(TRUE);


#ifdef USE_TIMED_SENDER_THREAD
  // start timer thread
  tagComControlTimer.bIsRunning=FALSE;
  tagComControlTimer.bTerminate=FALSE;

  /*hThreadPoller_m=*/CreateThread(NULL,                             // __in_opt  lpSecurityDescriptor
                                   0,                                // __in      StackSize
                                   (LPTHREAD_START_ROUTINE)dwDutCom_TimedSenderThread_m,     // __in      lpStartAddress,
                                   (void*)&tagComControlTimer,       // __in_opt  lpParameter,
                                   0,                                // __in      dwCreationFlags,
                                   &dwThreadId);                     // __out_opt lpThreadId
                                                                     //printf("3. dwThreadId = %d\n",dwThreadId);
#endif // USE_TIMED_SENDER_THREAD


  while(1)
  {
    iTimeOutMs=500;   // set default timeout
    pbAck=NULL;

#ifdef USE_TIMED_SENDER_THREAD
    if(bSendTesterPresent_m)
    {
      pbAck=pbUds_StartDiagSession_g(eUdsDiagModeExtended);
      //vUds_SendTesterPresent_g();
      bSendTesterPresent_m=FALSE;
      Sleep(100);
    }
#endif // USE_TIMED_SENDER_THREAD


    /**
     * Step 1:
     *
     * - Reset response data
     * and
     * - Send command
     */

    // Diag.Mode
    if(tagComSendData_m.tagDiagMode.bSend)
    {
      if(tagComSendData_m.tagDiagMode.bStart)
      {
        switch(tagComSendData_m.iComStep)
        {
          case 0:
            vNmea2k_EnableUds_g(TRUE);
            break;
          case 1:
            pbAck=pbUds_StartDiagSession_g(eUdsDiagModeExtended);
            bEnableTesterPresent_m=TRUE;   // Ab jetzt TesterPresent senden
            break;
        }
      }
      else
      {
        switch(tagComSendData_m.iComStep)
        {
          case 0:
            bEnableTesterPresent_m=FALSE;   // Ab jetzt TesterPresent nicht mehr senden
            pbAck=pbUds_StartDiagSession_g(eUdsDiagModeStandard);
            break;
          case 1:
            vNmea2k_EnableUds_g(FALSE);
            break;
        }
      }
    }
    if(tagComSendData_m.tagDiagMode.bSet)
    {
      EUdsDiagMode eUdsDiagMode=eUdsDiagModeStandard;
      tagComRecvData_m.tagDiagMode.bIsSet=FALSE;
      switch(tagComSendData_m.tagDiagMode.eDiagMode)
      {
        case eDiagModeStandard: eUdsDiagMode=eUdsDiagModeStandard; break;
        //case eDiagModeProgSession: eUdsDiagMode=eUdsDiagModeProgSession; break;
        case eDiagModeExtended: eUdsDiagMode=eUdsDiagModeExtended;    break;
        //case eDiagModeSssEol:   eUdsDiagMode=eUdsDiagModeSssEol; break;
        //case eDiagModeVmsEol:   eUdsDiagMode=eUdsDiagModeVmsEol; break;
        //case eDiagModeEcuProg:  eUdsDiagMode=eUdsDiagModeEcuProg; break;
        //case eDiagModeDevelop:  eUdsDiagMode=eUdsDiagModeDevelop; break;
        //case eDiagModeCvTest:   eUdsDiagMode=eUdsDiagModeCvTest; break;
      }
      pbAck=pbUds_StartDiagSession_g(eUdsDiagMode);
    }

    // security access
    if(tagComSendData_m.tagSecurityAccess.bSend)
    {
      switch(tagComSendData_m.iComStep)
      {
        case 0:
          switch(tagComSendData_m.tagSecurityAccess.eSecurityAccess)
          {
            case eSecurityAccessDefault:   // AccessModeDefault
            case eSecurityAccessVmsEol:    // AccessModeVehicleManufacturerSpecificEndOfLine
            case eSecurityAccessVmsProg:   // AccessModeVehicleManufacturerSpecificProgramming
            case eSecurityAccessVmsDev:    // AccessModeVehicleManufacturerSpecificDevelopment
              pbAck=pbUds_SecurityAccessReqSeed_g(tagComSendData_m.tagSecurityAccess.eSecurityAccess);
              break;
            case eSecurityAccessSssEol: // AccessSystemSupplierSpecificEndOfLine
              pbAck=pbUds_SecurityAccessReqSeed_g(eUdsSecurityAccessSssEol);
              break;
            case eSecurityAccessSssDev: // AccessSystemSupplierSpecificDevelopment
              pbAck=pbUds_SecurityAccessReqSeed_g(eUdsSecurityAccessSssDev);
              break;
          }
          break;
        case 1:
          pbAck=pbUds_SecurityAccessSendKey_g();
          break;
        default:
          printf("LibDutCom: security access: invalid iComStep=%d\n",tagComSendData_m.iComStep);
          tagComSendData_m.iComStep=0;
      }
    }

    // reset dut
    if(tagComSendData_m.tagReset.bSend)
    {
      pbUds_ResetDevice_g();
    }

    // clock access
    if(tagComSendData_m.tagClock.bReset)
    {
      // TODO: implement reset clock
    }

    if(tagComSendData_m.tagClock.bSetSeconds)
    {
      // TODO: implement set seconds of clock
      //UINT uiSeconds;   // param. to bSetSeconds
    }

    if(tagComSendData_m.tagClock.bSetMinutes)
    {
      // TODO: implement set minutes of clock
      //UINT uiMinutes;   // param. to bSetMinutes
    }

    if(tagComSendData_m.tagClock.bSetHours)
    {
      // TODO: implement set hours of clock
      //UINT uiHours;     // param. to bSetHours
    }

    if(tagComSendData_m.tagClock.bRequestTime)
    {
      pbAck=pbUds_RequestLocalTime_g();
    }



    if(tagComSendData_m.tagStaticDataByteWise.bWrite)
    {
      ; //pbAck=pbPintaCom_BlockWrite_g(tagComSendData_m.tagStaticDataByteWise.uiWriteAddress,
        //                              &(tagComSendData_m.tagStaticDataByteWise.ucWriteData),
        //                              1);
    }
    if(tagComSendData_m.tagStaticDataByteWise.bRequest)
    {
      ; //pbAck=pbPintaCom_DpidRequestMatrix_g(tagComSendData_m.tagStaticDataByteWise.uiRequestAddress);
    }

    if(tagComSendData_m.tagStaticDataBulk.bWrite)
    {
      switch(tagComSendData_m.tagStaticDataBulk.uiRequestAddress)
      {
        case DID_CALIBRATION:   // 0x0121
        default:
          pbAck=pbUds_WriteCalibration_g(tagComSendData_m.tagStaticDataBulk.pucWriteData);
          break;
      }
    }

    if(tagComSendData_m.tagStaticDataBulk.bRequest)
    {
      switch(tagComSendData_m.tagStaticDataBulk.uiRequestAddress)
      {
        case DID_CALIBRATION:   // 0x0121
        default:
          pbAck=pbUds_RequestCalibration_g();
          break;
      }
    }


    if(tagComSendData_m.tagStaticDataSerialNumber.bWrite)
    {
      pbAck=pbUds_WriteSerialNumber_g(tagComSendData_m.tagStaticDataSerialNumber.uiSerialNumber);
    }
    if(tagComSendData_m.tagStaticDataSerialNumber.bRequest)
    {
      pbAck=pbUds_RequestSerialNumber_g();
    }


    if(tagComSendData_m.tagStaticDataString.bWrite)
    {
      switch(tagComSendData_m.tagStaticDataString.eDataType)
      {
        case eProdNrUnpacked:
          {
            UINT uiNr;
            UCHAR ucaNr[4+1];
            tagComSendData_m.tagStaticDataString.ucaDataString[11]=0;
            // store as follows:
            if(strncmp(tagComSendData_m.tagStaticDataString.ucaDataString,"A2C",3)==0)
            {
              // A2C-Nr: w/o "A2C" as int
              uiNr=atoi((char*)&tagComSendData_m.tagStaticDataString.ucaDataString[3]);
            }
            if(tagComSendData_m.tagStaticDataString.ucaDataString[0]=='B')
            {
              // B-Nr: w/o "B" as int
              uiNr=atoi((char*)&tagComSendData_m.tagStaticDataString.ucaDataString[1]);
            }
            memcpy((void*)ucaNr,(void*)&uiNr,4);
            pbAck=pbUds_WriteDataByIdentifier_g(uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                                ucaNr,
                                                4);
          }
          break;
        case eProductCode:
          pbAck=pbUds_WriteDataByIdentifier_g(uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                              tagComSendData_m.tagStaticDataString.ucaDataString,
                                              2);
          break;
          //case eVersionHardware:
          //case eProdNrPcb:
          //case eVersionSoftware:
          //case eProgDatePcb:

        //case eDialScaleX:
        //  memcpy(&uiDialScaleX_m,tagComSendData_m.tagStaticDataString.ucaDataString,2);
        //  pbAck=pbUds_WriteDialScaleValues_g(uiDialScaleX_m,
        //                                     uiDialScaleY_m);
        //  break;
        //case eDialScaleY:
        //  memcpy(&uiDialScaleY_m,tagComSendData_m.tagStaticDataString.ucaDataString,2);
        //  pbAck=pbUds_WriteDialScaleValues_g(uiDialScaleX_m,
        //                                     uiDialScaleY_m);
        //  break;
        //case eGaugeType:
        //  pbAck=pbUds_WriteGaugeType_g(tagComSendData_m.tagStaticDataString.ucaDataString[0]);
        //  break;
        default:
          vResetComSendData_m();
      }
    }
    if(tagComSendData_m.tagStaticDataString.bRequest)
    {
      iTimeOutMs=800;
      switch(tagComSendData_m.tagStaticDataString.eDataType)
      {
        case eProdNrUnpacked:
        case eProductCode:
        //case eVersionHardware:
        case eVersionSoftware:
        //case eProdNrPcb:
        //case eProgDatePcb:
        //case eDialScaleX:
        //case eDialScaleY:
        //case eGaugeType:
          pbAck=pbUds_RequestDataString_g((ULONG)uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                          uiDevSpec_GetDataSize_m(tagComSendData_m.tagStaticDataString.eDataType));
          break;
        default:
          vResetComSendData_m();
      }
    }

    if(tagComSendData_m.tagStaticDataProductionInterlock.bWrite)
    {
      iTimeOutMs=1000;
      switch(tagComSendData_m.iComStep)
      {
        case 0:
          pbAck=pbUds_RequestProductInterlock_g(0);
          break;
        case 1:
          pbAck=pbUds_SetProductInterlock_g(tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber,
                                            tagComSendData_m.tagStaticDataProductionInterlock.uiToolVersionMajor,
                                            tagComSendData_m.tagStaticDataProductionInterlock.uiToolVersionMinor,
                                            tagComSendData_m.tagStaticDataProductionInterlock.uiRunCounter);
          break;
      }
      tagComRecvData_m.tagStaticDataProductionInterlock.bWritten=FALSE;
    }
    if(tagComSendData_m.tagStaticDataProductionInterlock.bRequest)
    {
      if(tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber<NUM_PROD_INTERLOCK_TOOLS)
      {
        iTimeOutMs=800;
        pbAck=pbUds_RequestProductInterlock_g(tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber);
      }
      tagComRecvData_m.tagStaticDataProductionInterlock.bReceived=FALSE;
    }


    // pointer adjust
    if(tagComSendData_m.tagPointerAdjust.bInit)
    {
      uiPointerAdjustOffset_m=20;
      pbAck=pbUds_StepperMotorSetOffset_g(uiPointerAdjustOffset_m);
    }
    if(tagComSendData_m.tagPointerAdjust.bSetOffset)
    {
      uiPointerAdjustOffset_m=tagComSendData_m.tagPointerAdjust.uiOffset;
      pbAck=pbUds_StepperMotorSetOffset_g(uiPointerAdjustOffset_m);
    }

    if(tagComSendData_m.tagPointerAdjust.bSetGain)
    {
      tagComRecvData_m.tagPointerAdjust.bGainIsSet=TRUE;   // Remove this line as soon as this function is implemented

      //tagComSendData_m.tagPointerAdjust.uiPointer
      //tagComSendData_m.tagPointerAdjust.uiPosition
      //tagComSendData_m.tagPointerAdjust.fGain
    }

    if(tagComSendData_m.tagPointerAdjust.bStoreOffset)
    {
      pbAck=pbUds_StepperMotorSetOffset_g(uiPointerAdjustOffset_m);
    }


    // pointer control
    if(tagComSendData_m.tagPointerControl.bInit)
    {
      vResetComSendData_m();
      //memset(iaPointerPosition_m,0,sizeof(iaPointerPosition_m));
    }

    if(tagComSendData_m.tagPointerControl.bSetPositionAll)
    {
      vResetComSendData_m();
      //iaPointerPosition_m[0]=(int)tagComSendData_m.tagPointerControl.uiaPosition[0];
      //iaPointerPosition_m[1]=(int)tagComSendData_m.tagPointerControl.uiaPosition[1];
      //
      //pbAck=pbUds_IoCtrlStepperMotorSetPosition_g(iaPointerPosition_m[0],
      //                                            iaPointerPosition_m[1]);
    }

    if(tagComSendData_m.tagPointerControl.bSetPositionSingle)
    {
      vResetComSendData_m();
      //iaPointerPosition_m[tagComSendData_m.tagPointerControl.uiPointer]=(int)tagComSendData_m.tagPointerControl.uiPosition;
      //switch(tagComSendData_m.tagPointerControl.uiPointer)
      //{
      //  case 0:
      //  case 1:
      //    pbAck=pbUds_IoCtrlStepperMotorSetPosition_g(iaPointerPosition_m[0],
      //                                                iaPointerPosition_m[1]);
      //    break;
      //}
    }


    // tell tale control
    if(tagComSendData_m.tagTellTaleControl.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagTellTaleControl.bSetBitmap)
    {
      ULONG ulBitmap;
      int iMaxIndex;

      ulBitmap=0;
      iMaxIndex=min(32,NUM_TELLTALES);
      for(iIndex=0;
          iIndex<iMaxIndex;
          iIndex++)
      {
        if(tagComSendData_m.tagTellTaleControl.ucaTellTaleState[iIndex]!=0)
        {
          ulBitmap+=(1<<iIndex);
          //printf("TT %d = ON: 0x%08x\n",iIndex,(1<<iIndex));
        }
      }
      pbAck=pbUds_IoCtrlSetTTBitmap_g(ulBitmap);
    }

    if(tagComSendData_m.tagTellTaleControl.bSetSingle)
    {
      ULONG ulBitmap=0;
      if(tagComSendData_m.tagTellTaleControl.bState)
      {
        ulBitmap=(1<<tagComSendData_m.tagTellTaleControl.uiTellTale);
      }
      //printf("TT = ON: 0x%08x\n",ulBitmap);
      pbAck=pbUds_IoCtrlSetTTBitmap_g(ulBitmap);
    }


    // backlight control
    if(tagComSendData_m.tagBackLightControl.bInit)
    {
      tagComRecvData_m.tagBackLightControl.bInitialized=TRUE;   // TODO: remove this if implemented
    }

    if(tagComSendData_m.tagBackLightControl.bSetBackLight)
    {
      pbAck=pbUds_IoCtrlSetBacklight_g((tagComSendData_m.tagBackLightControl.uiBackLightBitmap&0x01),
                                       (tagComSendData_m.tagBackLightControl.uiBackLightBitmap&0x02),
                                       (tagComSendData_m.tagBackLightControl.uiBackLightBitmap&0x04),
                                       (tagComSendData_m.tagBackLightControl.uiBackLightBitmap&0x08));
    }


    // lcd control
    if(tagComSendData_m.tagLcdControl.bInit)
    {
      pbAck=pbUds_IoCtrlSetBacklight_g(FALSE,   // Pointer
                                       FALSE,   // Dial
                                       TRUE,    // LCD
                                       FALSE);  // EasyLink
    }

    if(tagComSendData_m.tagLcdControl.bSetPattern)
    {
      //tagComSendData_m.tagLcdControl.uiLcd
      switch(tagComSendData_m.tagLcdControl.eLcdPattern)
      {
        case eLcdPatternA:
          pbAck=pbUds_IoCtrlLcdSetPattern_g(5);
          break;
        case eLcdPatternAll:
          pbAck=pbUds_IoCtrlLcdSetPattern_g(8);
          break;
        default:
          printf("LibDutCom: invalid lcd pattern %d specified\n",tagComSendData_m.tagLcdControl.eLcdPattern);
          break;
      }
    }

    // read digital inputs
    if(tagComSendData_m.tagDigInputs.bInit)
    {
      ;
    }
    if(tagComSendData_m.tagDigInputs.bRequestAll)
    {
      pbAck=pbUds_IoCtrlRequestDigitalInput_g();
    }

    if(tagComSendData_m.tagDigInputs.bRequestSingle)
    {
      pbAck=pbUds_IoCtrlRequestDigitalInput_g();
    }

    // write digital outputs
    if(tagComSendData_m.tagDigOutputs.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagDigOutputs.bWriteAll)
    {
      ULONG ulBitmap;
      int iMaxIndex;

      ulBitmap=0;
      iMaxIndex=min(8,NUM_DIG_OUTPUTS);
      for(iIndex=0;
          iIndex<iMaxIndex;
          iIndex++)
      {
        if(tagComSendData_m.tagDigOutputs.ucaDigOutputState[iIndex]!=0)
        {
          ulBitmap+=(1<<iIndex);
        }
      }
      pbAck=pbUds_IoCtrlSetDigOutput_g(ulBitmap);
    }

#if 0

      switch(tagComSendData_m.iComStep)
      {
        case 0:
          uiBitmap=
          pbAck=pbUds_IoCtrlSetDigOut_g()
          //pbAck=pbUds_IoCtrlSetEasyLink_g(tagComSendData_m.tagDigOutputs.ucaDigOutputState[0]>0);
          break;
      }
    }
 #endif

    if(tagComSendData_m.tagDigOutputs.bWriteSingle)
    {
      ULONG ulBitmap=0;
      if(tagComSendData_m.tagDigOutputs.bDigOutputState)
      {
        ulBitmap=(1<<tagComSendData_m.tagDigOutputs.uiDigOutput);
      }
      pbAck=pbUds_IoCtrlSetDigOutput_g(ulBitmap);
    }
#if 0
      switch(tagComSendData_m.tagDigOutputs.uiDigOutput)
      {
        case 0:
          pbAck=pbUds_IoCtrlSetEasyLink_g(tagComSendData_m.tagDigOutputs.bDigOutputState);
          break;
      }
    }
#endif

    // read analog inputs
    if(tagComSendData_m.tagAnalogInputs.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagAnalogInputs.bRequestAll)
    {
      switch(tagComSendData_m.iComStep)
      {
        case 0:   // Battery voltage
          pbAck=pbUds_IoCtrlRequestAnalogInputs_g();
          break;
        case 1:   // SNR GPS
          pbAck=pbUds_IoCtrlRequestDigitalInput_g();
          break;
        default:
          printf("LibDutCom: request all analog inputs: invalid iComStep=%d\n",tagComSendData_m.iComStep);
          tagComSendData_m.iComStep=0;
      }
    }

    if(tagComSendData_m.tagAnalogInputs.bRequestSingle)
    {
      switch(tagComSendData_m.tagAnalogInputs.uiAnalogInput)
      {
        case 0:   // Battery voltage
          pbAck=pbUds_IoCtrlRequestAnalogInputs_g();
          break;
        case 1:   // SNR GPS
          pbAck=pbUds_IoCtrlRequestDigitalInput_g();
          break;
      }
    }

    // write analog outputs
    if(tagComSendData_m.tagAnalogOutputs.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagAnalogOutputs.bWriteAll)
    {
      ;
    }

    if(tagComSendData_m.tagAnalogOutputs.bWriteSingle)
    {
      ;
    }

    // read frequency inputs
    if(tagComSendData_m.tagFreqInputs.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagFreqInputs.bRequestAll)
    {
      pbAck=pbUds_IoCtrlRequestFrequencyInput_g();
    }

    if(tagComSendData_m.tagFreqInputs.bRequest)
    {
      pbAck=pbUds_IoCtrlRequestFrequencyInput_g();
    }

    // write frequency outputs
    if(tagComSendData_m.tagFreqOutputs.bInit)
    {
      ;
    }

    if(tagComSendData_m.tagFreqOutputs.bWriteAll)
    {
      //tagComSendData_m.tagFreqOutputs.auiFrequency
      //tagComSendData_m.tagFreqOutputs.auiDutyCycle
    }

    if(tagComSendData_m.tagFreqOutputs.bWrite)
    {
      //tagComSendData_m.tagFreqOutputs.uiFreqOutput
      //tagComSendData_m.tagFreqOutputs.uiDutyCycle
      //tagComSendData_m.tagFreqOutputs.uiFrequency
    }

    if(tagComSendData_m.tagDevSpecTest.bStart)
    {
      ;
    }




    // TODO: add further send functions



    /**
     * Step 2:
     *
     * wait for ACK response
     */
    if(pbAck)
    {
      //printf("LibDutCom: wait for Ack\n");
      bReceivedAck=FALSE;
      // wait for ACK-flag
      if(bWaitResponseFlag_m(pbAck,TRUE,iTimeOutMs))
      {
        // received response
        iRepeats=0;   // reset for next use
        bReceivedAck=TRUE;
        //printf("LibDutCom: got Ack\n");
      }
      else
        ++iRepeats;
    }
    else
    {
      // don't wait for ACK-flag
      //vResetComSendData_m();
      bReceivedAck=TRUE;
    }
    if(!bReceivedAck)
    {
      strcpy(caDutComErrorMessage_m,"no Ack");
    }


    /**
     * Step 3:
     *
     * Store response data
     */

    if(bReceivedAck)
    {
      // Diag.Mode
      if(tagComSendData_m.tagDiagMode.bSend)
      {
        switch(tagComSendData_m.iComStep)
        {
          case 0:
            ++tagComSendData_m.iComStep;
            Sleep(20);
            break;
          case 1:
            if(tagComSendData_m.tagDiagMode.bStart)
              tagComRecvData_m.tagDiagMode.bStarted=TRUE;
            else
              tagComRecvData_m.tagDiagMode.bStarted=FALSE;
            vResetComSendData_m();
            break;
        }
      }
      if(tagComSendData_m.tagDiagMode.bSet)
      {
        tagComRecvData_m.tagDiagMode.bIsSet=TRUE;
        vResetComSendData_m();
      }

      // security access
      if(tagComSendData_m.tagSecurityAccess.bSend)
      {
        if(pbAck)
        {
          switch(tagComSendData_m.iComStep)
          {
            case 0:
              break;
            case 1:
              tagComRecvData_m.tagSecurityAccess.eSecurityAccess=tagComSendData_m.tagSecurityAccess.eSecurityAccess;
              eSecurityAccess_m=tagComSendData_m.tagSecurityAccess.eSecurityAccess;
              vResetComSendData_m();
              break;
          }
          ++tagComSendData_m.iComStep;
        }
      }

      // reset dut
      if(tagComSendData_m.tagReset.bSend)
      {
        vResetComSendData_m();
      }

      // clock access
      if(tagComSendData_m.tagClock.bReset)
      {
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagClock.bSetSeconds)
      {
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagClock.bSetMinutes)
      {
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagClock.bSetHours)
      {
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagClock.bRequestTime)
      {
        if(bUds_GetLocalTime_g(ucaLocalTime))
        {
          // ucaLocalTime => (Wertigkeit 25ms...89468321d19h42m34s400ms?)
          ULONG ulTime;
          struct tm *timeinfo;
          time_t timeSec=0;

          //Division durch 1000 ms->s
          //Zeit (8Byte) kann durch die Division auf 4Byte gekürtzt werden.
          ulTime=uiBinaryDivision(&ucaLocalTime[2],6,1000);
          timeSec=ulTime;
          timeinfo=gmtime(&timeSec); //GMT

          tagComRecvData_m.tagClock.uiHours =timeinfo->tm_hour;
          tagComRecvData_m.tagClock.uiMinutes=timeinfo->tm_min;
          tagComRecvData_m.tagClock.uiSeconds=timeinfo->tm_sec;
          vResetComSendData_m();
        }
      }


      // static data: byte wise access
      if(tagComSendData_m.tagStaticDataByteWise.bWrite)
      {
        tagComRecvData_m.tagStaticDataByteWise.bWritten=TRUE;
        vResetComSendData_m();
      }
      if(tagComSendData_m.tagStaticDataByteWise.bRequest)
      {
        //if(bPintaCom_IdentifierReadMatrix_g(tagComSendData_m.tagStaticDataByteWise.uiRequestAddress,
        //                                    0,
        //                                    ucaData,
        //                                    &ucDataLen))
        {
          //tagComRecvData_m.tagStaticDataByteWise.ucReceivedData=ucaData[0];
          //tagComRecvData_m.tagStaticDataByteWise.bReceived=TRUE;
          vResetComSendData_m();
        }
      }

      // static data: bulk wise access
      if(tagComSendData_m.tagStaticDataBulk.bWrite)
      {
        tagComRecvData_m.tagStaticDataBulk.bWritten=TRUE;
        vResetComSendData_m();
      }
      if(tagComSendData_m.tagStaticDataBulk.bRequest)
      {
        BOOL bReceived;
        switch(tagComSendData_m.tagStaticDataBulk.uiRequestAddress)
        {
          case DID_CALIBRATION:   // 0x0121
          default:
            bReceived=bUds_GetCalibration_g(tagComRecvData_m.tagStaticDataBulk.pucReceivedData);
            break;
        }
        if(bReceived)
        {
          tagComRecvData_m.tagStaticDataBulk.bReceived=TRUE;
          vResetComSendData_m();
        }
      }


      // static data: serial number access
      if(tagComSendData_m.tagStaticDataSerialNumber.bWrite)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagStaticDataSerialNumber.bWritten=TRUE;
          vResetComSendData_m();
        }
      }
      if(tagComSendData_m.tagStaticDataSerialNumber.bRequest)
      {
        if(pbAck)
        {
          if(bUds_GetSerialNumber_g(&tagComRecvData_m.tagStaticDataSerialNumber.uiReceivedSerialNumber))
          {
            tagComRecvData_m.tagStaticDataSerialNumber.bReceived=TRUE;
            vResetComSendData_m();
          }
        }
      }


      // static data: product data access
      if(tagComSendData_m.tagStaticDataString.bWrite)
      {
        tagComRecvData_m.tagStaticDataString.bWritten=TRUE;
        vResetComSendData_m();
      }
      if(tagComSendData_m.tagStaticDataString.bRequest)
      {
        UINT uiSize;
        switch(tagComSendData_m.tagStaticDataString.eDataType)
        {
          case eProdNrUnpacked:
            if(bUds_GetDataString_g(uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                    (UCHAR*)&tagComRecvData_m.tagStaticDataString.ucaDataString,
                                    &uiSize))
            {
              UINT uiNr;
              //  0.09 Tx 0560 03 22 01 32               read data ID 0x0132
              //  0.01 Rx 07c2 07 62 01 32 ff ff ff ff
              //                           ===========--> B-Nr w/o "B" as int if smaller than 9999999
              //                           ===========--> A2C-Nr w/o "A2C" as int otherwise
              memcpy((void*)&uiNr,(void*)tagComRecvData_m.tagStaticDataString.ucaDataString,4);
              if(uiNr>999999)   // max. B999999 possible
              {
                if(uiNr<99999999)
                  sprintf(tagComRecvData_m.tagStaticDataString.ucaDataString,"A2C%08d",uiNr);
                else
                  sprintf(tagComRecvData_m.tagStaticDataString.ucaDataString,"A2C%010d",uiNr);
              }
              else
              {
                sprintf(tagComRecvData_m.tagStaticDataString.ucaDataString,"B%06d",uiNr);
              }
              tagComRecvData_m.tagStaticDataString.bReceived=TRUE;
              vResetComSendData_m();
            }
            break;
          case eProductCode:
            //  0.09 Tx 0560 03 22 01 33               read data ID 0x0133
            //  0.01 Rx 07c2 07 62 01 33 ff ff 00 00
            //                           =====--> NMEA Product Code
            if(bUds_GetDataString_g(uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                    (UCHAR*)&tagComRecvData_m.tagStaticDataString.ucaDataString,
                                    &uiSize))
            {
              tagComRecvData_m.tagStaticDataString.bReceived=TRUE;
              vResetComSendData_m();
            }
            break;
          //case eVersionHardware:
          //case eProdNrPcb:
          case eVersionSoftware:
          //case eProgDatePcb:
            if(bUds_GetDataString_g(uiDevSpec_GetDataAddress_m(tagComSendData_m.tagStaticDataString.eDataType),
                                    (UCHAR*)&tagComRecvData_m.tagStaticDataString.ucaDataString,
                                    &uiSize))
            {
              // 2.78 Tx 07f2 03 22 01 20               read data ID 0x0120
              // 0.00 Rx 07fa 07 62 01 20 01 c9 ff 05
              //                          ==--> Start of eeprom (fix value)
              //                             ==--> EEProm Valid
              //                                   ==--> Software Version
              vTrace_AppendText_g("EE start=0x%x",tagComRecvData_m.tagStaticDataString.ucaDataString[0]);
              vTrace_AppendText_g("EE valid=0x%x",tagComRecvData_m.tagStaticDataString.ucaDataString[1]);
              vTrace_AppendText_g("EE map version=%d",tagComRecvData_m.tagStaticDataString.ucaDataString[2]);
              vTrace_AppendText_g("SW version=%d",tagComRecvData_m.tagStaticDataString.ucaDataString[3]);
              sprintf(tagComRecvData_m.tagStaticDataString.ucaDataString,"%d",tagComRecvData_m.tagStaticDataString.ucaDataString[3]);
              tagComRecvData_m.tagStaticDataString.bReceived=TRUE;
              vResetComSendData_m();
            }
            break;
          default:
            vResetComSendData_m();
        }
      }

      // static data: production interlock access
      if(tagComSendData_m.tagStaticDataProductionInterlock.bWrite)
      {
        if(pbAck)
        {
          switch(tagComSendData_m.iComStep)
          {
            case 0:
              ++tagComSendData_m.iComStep;
              break;
            case 1:
              tagComRecvData_m.tagStaticDataProductionInterlock.bWritten=TRUE;
              vResetComSendData_m();
              break;
          }
        }
      }
      if(tagComSendData_m.tagStaticDataProductionInterlock.bRequest)
      {
        if(pbAck)
        {
          if(bUds_GetProductInterlockData_g(tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiVersionMajor,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiVersionMinor,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiRunCounter,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiYear,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiMonth,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiDay,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiHours,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiMinutes,
                                            &tagComRecvData_m.tagStaticDataProductionInterlock.uiSeconds))
          {
            tagComRecvData_m.tagStaticDataProductionInterlock.bReceived=TRUE;
            vResetComSendData_m();
          }
        }
      }


      // pointer adjust
      if(tagComSendData_m.tagPointerAdjust.bInit)
      {
        tagComRecvData_m.tagPointerAdjust.bInitialized=TRUE;
        vResetComSendData_m();
      }
      if(tagComSendData_m.tagPointerAdjust.bSetOffset)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagPointerAdjust.bOffsetIsSet=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagPointerAdjust.bSetGain)
      {
        tagComRecvData_m.tagPointerAdjust.bGainIsSet=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagPointerAdjust.bStoreOffset)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagPointerAdjust.bOffsetIsStored=TRUE;
          vResetComSendData_m();
        }
      }


      // pointer control
      if(tagComSendData_m.tagPointerControl.bInit)
      {
        tagComRecvData_m.tagPointerControl.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagPointerControl.bSetPositionAll)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagPointerControl.bPositionAllAreSet=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagPointerControl.bSetPositionSingle)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagPointerControl.bPositionSingleIsSet=TRUE;
          vResetComSendData_m();
        }
      }


      // tell tale control
      if(tagComSendData_m.tagTellTaleControl.bInit)
      {
        tagComRecvData_m.tagTellTaleControl.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagTellTaleControl.bSetBitmap)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagTellTaleControl.bBitmapIsSet=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagTellTaleControl.bSetSingle)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagTellTaleControl.bSingleIsSet=TRUE;
          vResetComSendData_m();
        }
      }


      // backlight control
      if(tagComSendData_m.tagBackLightControl.bInit)
      {
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagBackLightControl.bSetBackLight)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagBackLightControl.bBackLightIsSet=TRUE;
          vResetComSendData_m();
        }
      }


      // lcd control
      if(tagComSendData_m.tagLcdControl.bInit)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagLcdControl.bInitialized=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagLcdControl.bSetPattern)
      {
        if(pbAck)
        {
          tagComRecvData_m.tagLcdControl.bPatternIsSet=TRUE;
          vResetComSendData_m();
        }
      }

      // read digital inputs
      if(tagComSendData_m.tagDigInputs.bInit)
      {
        tagComRecvData_m.tagDigInputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagDigInputs.bRequestAll)
      {
        int iInput;
        BOOL bState;
        for(iInput=0;iInput<NUM_DIG_INPUTS;iInput++)
        {
          if(bUds_IoCtrlGetDigitalInput_g((UCHAR)iInput,
                                          &bState))
          {
            tagComRecvData_m.tagDigInputs.ucaDigInputStates[iInput]=(bState)?1:0;
          }
          else
            break;
        }
        tagComRecvData_m.tagDigInputs.bReceivedAll=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagDigInputs.bRequestSingle)
      {
        if(bUds_IoCtrlGetDigitalInput_g((UCHAR)tagComSendData_m.tagDigInputs.uiDigInput,
                                        &tagComRecvData_m.tagDigInputs.bDigInputState))
        {
          tagComRecvData_m.tagDigInputs.bReceivedSingle=TRUE;
          vResetComSendData_m();
        }
      }


      // write digital outputs
      if(tagComSendData_m.tagDigOutputs.bInit)
      {
        tagComRecvData_m.tagDigOutputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagDigOutputs.bWriteAll)
      {
        if(++tagComSendData_m.iComStep>NUM_DIG_OUTPUTS)
        {
          tagComRecvData_m.tagDigOutputs.bAllWritten=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagDigOutputs.bWriteSingle)
      {
        tagComRecvData_m.tagDigOutputs.bSingleWritten=TRUE;
        vResetComSendData_m();
      }


      // read analog inputs
      if(tagComSendData_m.tagAnalogInputs.bInit)
      {
        tagComRecvData_m.tagAnalogInputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagAnalogInputs.bRequestAll)
      {
        unsigned int uiAnalogInputValue;

        switch(tagComSendData_m.iComStep)
        {
          case 0:   // Battery voltage
            if(bUds_IoCtrlGetAnalogInput_g((UCHAR)(tagComSendData_m.iComStep),&uiAnalogInputValue))
            {
              tagComRecvData_m.tagAnalogInputs.uiaAnalogInputValues[tagComSendData_m.iComStep]=uiAnalogInputValue;
              ++tagComSendData_m.iComStep;
            }
            break;
          case 1:   // SNR GPS
            if(bUds_IoCtrlGetGpsSnr_g(&uiAnalogInputValue))
            {
              tagComRecvData_m.tagAnalogInputs.uiaAnalogInputValues[tagComSendData_m.iComStep]=uiAnalogInputValue;
              tagComRecvData_m.tagAnalogInputs.bReceivedAll=TRUE;
              vResetComSendData_m();
            }
            break;
        }
      }

      if(tagComSendData_m.tagAnalogInputs.bRequestSingle)
      {
        if(pbAck)
        {
          switch(tagComSendData_m.tagAnalogInputs.uiAnalogInput)
          {
            case 0:   // Battery voltage
              if(bUds_IoCtrlGetAnalogInput_g((UCHAR)(tagComSendData_m.tagAnalogInputs.uiAnalogInput),
                                             &tagComRecvData_m.tagAnalogInputs.uiAnalogInputValue))
              {
                tagComRecvData_m.tagAnalogInputs.bReceivedSingle=TRUE;
                vResetComSendData_m();
              }
              break;
            case 1:   // SNR GPS
              if(bUds_IoCtrlGetGpsSnr_g(&tagComRecvData_m.tagAnalogInputs.uiAnalogInputValue))
              {
                tagComRecvData_m.tagAnalogInputs.bReceivedSingle=TRUE;
                vResetComSendData_m();
              }
              break;
          }
        }
      }


      // write analog outputs
      if(tagComSendData_m.tagAnalogOutputs.bInit)
      {
        tagComRecvData_m.tagAnalogOutputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagAnalogOutputs.bWriteAll)
      {
        tagComRecvData_m.tagAnalogOutputs.bAllWritten=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagAnalogOutputs.bWriteSingle)
      {
        tagComRecvData_m.tagAnalogOutputs.bSingleWritten=TRUE;
        vResetComSendData_m();
      }


      // read frequency inputs
      if(tagComSendData_m.tagFreqInputs.bInit)
      {
        tagComRecvData_m.tagFreqInputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagFreqInputs.bRequestAll)
      {
        tagComRecvData_m.tagFreqInputs.bReceivedAll=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagFreqInputs.bRequest)
      {
        tagComRecvData_m.tagFreqInputs.bReceived=TRUE;
        vResetComSendData_m();
      }

      // write frequency outputs
      if(tagComSendData_m.tagFreqOutputs.bInit)
      {
        tagComRecvData_m.tagFreqOutputs.bInitialized=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagFreqOutputs.bWriteAll)
      {
        if(tagComSendData_m.iComStep<(NUM_FREQ_OUTPUTS*2))
        {
          //++tagComSendData_m.iComStep;
          tagComSendData_m.iComStep+=2;   // pbUds_PwmSetFreq_g() currently not supported by RT2
        }
        if(tagComSendData_m.iComStep>=(NUM_FREQ_OUTPUTS*2))
        {
          tagComRecvData_m.tagFreqOutputs.bAllWritten=TRUE;
          vResetComSendData_m();
        }
      }

      if(tagComSendData_m.tagFreqOutputs.bWrite)
      {
        tagComRecvData_m.tagFreqOutputs.bWritten=TRUE;
        vResetComSendData_m();
      }

      if(tagComSendData_m.tagDevSpecTest.bStart)
      {
        tagComRecvData_m.tagDevSpecTest.bEnded=TRUE;
        vResetComSendData_m();
      }



      // TODO: add further receive functions

    }
    if(iRepeats>3)
    {
      // too many retries --> abort
      vResetComSendData_m();
      iRepeats=0;
    }



    if(ptagComControl->bTerminate)
    {
#ifdef USE_TIMED_SENDER_THREAD
      tagComControlTimer.bTerminate=TRUE;
#endif // USE_TIMED_SENDER_THREAD
      break;
    }

    Sleep(20);
  }

#ifdef USE_TIMED_SENDER_THREAD
  // wait until timer thread stops
  while(tagComControlTimer.bIsRunning)
  {
    Sleep(50);
  }
#endif // USE_TIMED_SENDER_THREAD

  // close communication port(s)
  // TEST: Beginn Pinta specific ...
  vUds_Close_g();
  //vPintaCom_PortClose_g();
  // TEST: ... End Pinta specific

  ptagComControl->bIsRunning=FALSE;

  ExitThread(0);
  return(0);   // to satisfy the compiler
} // dwDutCom_SenderThread_m()

