/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   0000: Vorlage   @@@
 *
 *    FILE:        LibDutCom_0000.c
 *
 *    DESCRIPTION: Template für LibDutCom_XXXX
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


#define DLL_EXPORTS

#include <windows.h>
#ifdef _CVI_
  #include <ansi_c.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include "LibDutCom.h"
#include "LibDutCom_private.h"

#include "DeviceSpecific.h"

#include "Trace.h"


HANDLE hThreadPoller_m=NULL;

TagComControl tagComControl_m;

char caDutComErrorMessage_m[DUT_COM_ERROR_MESSAGE_LEN+1];

extern TagComSendData     tagComSendData_m;
extern TagComRecvData     tagComRecvData_m;
extern TagLibDutComParam  tagLibDutComParam_m;


BOOL WINAPI DllMain(HINSTANCE hinstDLL,
                    DWORD fdwReason,
                    LPVOID lpvReserved)
{
  switch(fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      //if(lpvReserved)
      //  printf("dll static loaded at 0x%x\n");
      //else
      //  printf("dll dynamic loaded at 0x%x\n");
      vResetComSendData_m();
      vResetComRecvData_m();
      vResetDigIOData_m();
      break;
    case DLL_PROCESS_DETACH:
      //if(lpvReserved)
      //  printf("dll unloaded from 0x%x by termintating process\n");
      //else
      //  printf("dll unloaded from 0x%x by FreeLibrary\n");
      vTrace_Clear_g();
      break;
    case DLL_THREAD_ATTACH:
      //      printf("DLL_THREAD_ATTACH\n");
      break;
    case DLL_THREAD_DETACH:
      //      printf("DLL_THREAD_DETACH\n");
      break;
  }
  return(TRUE);
}


MYDLL_API BOOL WINAPI bDutCom_SetParam(TagLibDutComParam *ptagParam)
{
  memcpy(&tagLibDutComParam_m,
         ptagParam,
         sizeof(TagLibDutComParam));

  return(TRUE);
} // bDutCom_SetParam()



/**
 * Returns information about the current used LibDutCom.
 *
 * @param pcDutInfo Name of device
 * @param puiDutId  Numeric unique id of this LibDutCom.
 * @param pcVersion Version of LibDutCom (Buffer size at least 20 bytes)
 * @param pcBuild   Build date and time of LibDutCom (Buffer size at least 30 bytes)
 * @param pcCapabilities
 *                  Specific capabilities of LibDutCom in text form.
 *                  (Buffer size about 10'000 bytes)<br>
 *                  This info is to be seen as a documentation contained in the dll.<br>
 *                  Includes i.e.:
 *                  <li>-number of pointers with display values of each (if applicable)</li>
 *                  <li>-number of tell-tales with symbol of each (if applicable)</li>
 *                  <li>-...</li>
 *
 */
MYDLL_API void WINAPI vDutCom_GetInfo(char *pcDutInfo,
                                      UINT *puiDutId,
                                      char *pcVersion,
                                      char *pcBuild,
                                      char *pcCapabilities)
{
  if(pcDutInfo)
    strcpy(pcDutInfo,LIB_DUT_INFO);

  if(puiDutId)
    *puiDutId=LIB_DUT_ID;

  if(pcVersion)
  {
    char caVersion[10];   // Format "%d.%03d" => "123.123"
    sprintf(caVersion,"%d.%03d",
            min(999,VERSION_MAJOR),
            min(999,VERSION_MINOR));
    strcpy(pcVersion,caVersion);
  }

  if(pcBuild)
  {
#ifdef _CVI_
  #if _CVI_ > 800
    // CVI > Version 8.00
    sprintf(pcBuild,"%s, %s cvi",__TIME__,__DATE__);
  #else
    // CVI < Version 8.00
    strcpy(pcBuild,
#include "Build_Date.h"
           );
  #endif
#ifdef _CVI_DEBUG_
    strcat(pcBuild," (Debug)");
#endif // _CVI_DEBUG_
#else // !_CVI_
    sprintf(pcBuild,"%s, %s win32",__TIME__,__DATE__);
#endif // !_CVI_
  }

  if(pcCapabilities)
  {
    vDevSpec_GetCapabilities_m(pcCapabilities);
  }
} // vDutCom_GetInfo()


/**
 * Get one line of the trace.
 *
 * @author uidc9013 (27.01.2012)
 * @param pcTraceLine
 *               char pointer to buffer to receive the trace text.
 *               (size about 100 chars)
 * @param piLinesRemaining
 *               int pointer to variable to receive the number of lines<br>
 *               remaining in the trace or NULL if this value is not needed.
 *
 * @return 1 on success<br>
 *         0 on empty trace
 */
MYDLL_API int WINAPI iDutCom_GetOneTraceLine(char *pcTraceLine,
                                             int  *piLinesRemaining)
{
  return(iTrace_GetTextLine_g(pcTraceLine,
                              piLinesRemaining));
} // iDutCom_GetOneTraceLine()

/**
 * Returns the head line for the trace.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return head line for trace.
 */
MYDLL_API char WINAPI *pcDutCom_GetTraceHeader(void)
{
  return(pcDutCom_TraceGetHeader_m());
} // pcDutCom_GetTraceHeader()

/**
 * Enable/Disable the capture of data for trace.
 *
 * @author uidc9013 (27.01.2012)
 * @param bEnable TRUE = enable collection of trace data<br>
 *                FALSE = do not collect trace data.
 */
MYDLL_API void WINAPI vDutCom_TraceEnable(BOOL bEnable)
{
  //printf("vDutCom_TraceEnable(%d)\n",bEnable);
  vDutCom_TraceEnable_m(bEnable);
} // vDutCom_TraceEnable()


/**
 * clears the trace.
 *
 * @author uidc9013 (27.01.2012)
 * @param bEnable TRUE = enable collection of trace data<br>
 *                FALSE = do not collect trace data.
 */
MYDLL_API void WINAPI vDutCom_TraceClear(void)
{
  vTrace_Clear_g();
} // vDutCom_TraceEnable()


/**
 * Returns the error message of the last function, if there was an error.
 * If the last function call returned successfully the content of the
 * error message is undefined.
 * Call this function only if there was an error.
 *
 * @author uidc9013 (15.11.2012)
 * @param pcErrorMessage
 *               pointer to buffer to receive error message
 * @param uiBufferSize
 *               size [bytes] of buffer to receive error message
 *
 * @return TRUE if an error message could be returend otherwise<br>
 *         FALSE if there was no error message.
 */
MYDLL_API BOOL WINAPI bDutCom_GetErrorMessage(char *pcErrorMessage,
                                              UINT uiBufferSize)
{
  strncpy(pcErrorMessage,caDutComErrorMessage_m,uiBufferSize);
  return(strlen(caDutComErrorMessage_m)>0);
} // bDutCom_GetErrorMessage()


/**
 * Start communication with DUT
 *
 * @author uidc9013 (21.12.2011)
 */
MYDLL_API void WINAPI vDutCom_CommunicationStart(void)
{
  // Start communication thread

  if(tagComControl_m.bIsRunning)
  {
    // thread is already running
    return;
  }

  // Start communication thread
  tagComControl_m.bTerminate=FALSE;
  tagComControl_m.bIsRunning=FALSE;

  hThreadPoller_m=CreateThread(NULL,                          // __in_opt  lpSecurityDescriptor
                               0,                             // __in      StackSize
                               (LPTHREAD_START_ROUTINE)dwDutCom_SenderThread_m,       // __in      lpStartAddress,
                               (void*)(&tagComControl_m),     // __in_opt  lpParameter,
                               0,                             // __in      dwCreationFlags,
                               NULL);                         // __out_opt lpThreadId

} // vDutCom_CommunicationStart()

/**
 * Stops communication with DUT
 *
 * @author uidc9013 (21.12.2011)
 */
MYDLL_API void WINAPI vDutCom_CommunicationStop(void)
{
  // Stop communication thread
  tagComControl_m.bTerminate=TRUE;
} // vDutCom_CommunicationStop()

/**
 * Returns whether communication with DUT has started
 * Response to vDutCom_CommunicationStart()
 *
 * @author uidc9013 (21.12.2011)
 * @see vDutCom_CommunicationStart()
 */
MYDLL_API BOOL WINAPI bDutCom_CommunicationStarted(void)
{
  return(tagComControl_m.bIsRunning);
} // bDutCom_CommunicationStarted()

/**
 * Returns whether communication with DUT has stopped
 * Response to vDutCom_CommunicationStop()
 *
 * @author uidc9013 (21.12.2011)
 * @see vDutCom_CommunicationStop()
 */
MYDLL_API BOOL WINAPI bDutCom_CommunicationStopped(void)
{
  return(!tagComControl_m.bIsRunning);
} // bDutCom_CommunicationStopped()



/**
 * Initiate communication with DUT in diagnostic mode
 *
 * @author uidc9013 (16.12.2011)
 */
MYDLL_API void WINAPI vDutCom_DiagModeStart(void)
{
  vResetComSendData_m();
  tagComSendData_m.tagDiagMode.bStart=TRUE;
  tagComSendData_m.tagDiagMode.bSend=TRUE;
} // vDutCom_DiagModeStart()

/**
 * Stop DUT in diagnostic mode. Return to normal mode
 *
 * @author uidc9013 (16.12.2011)
 */
MYDLL_API void WINAPI vDutCom_DiagModeStop(void)
{
  vResetComSendData_m();
  tagComSendData_m.tagDiagMode.bStart=FALSE;
  tagComSendData_m.tagDiagMode.bSend=TRUE;
} // vDutCom_DiagModeStop()

/**
 * Returns whether diagnostic mode is started
 * Response to vDutCom_DiagModeStart()
 *
 * @author uidc9013 (21.12.2011)
 *
 * @return TRUE if diagnostic mode is started
 * @see vDutCom_DiagModeStart()
 */
MYDLL_API BOOL WINAPI bDutCom_DiagModeStarted(void)
{
  return(tagComRecvData_m.tagDiagMode.bStarted);
} // bDutCom_DiagModeStarted()


/**
 * Initiate communication with DUT in specified diagnostic mode
 *
 * @author uidc9013 (02.09.2015)
 */
MYDLL_API void WINAPI vDutCom_DiagModeSet(EDiagMode eDiagMode)
{
  vResetComSendData_m();
  tagComRecvData_m.tagDiagMode.bIsSet=FALSE;
  tagComSendData_m.tagDiagMode.eDiagMode=eDiagMode;
  tagComSendData_m.tagDiagMode.bSet=TRUE;
} // vDutCom_DiagModeStart()

/**
 * Returns whether diagnostic mode is set
 * Response to vDutCom_DiagModeSet()
 *
 * @author uidc9013 (02.09.2015)
 *
 * @return TRUE if diagnostic mode is set
 * @see vDutCom_DiagModeSet()
 */
MYDLL_API BOOL WINAPI bDutCom_DiagModeIsSet(void)
{
  return(tagComRecvData_m.tagDiagMode.bIsSet);
} // bDutCom_DiagModeIsSet()



/**
 * Returns whether diagnostic mode is stopped
 * Response to vDutCom_DiagModeStop()
 *
 * @author uidc9013 (21.12.2011)
 *
 * @return TRUE if diagnostic mode is stopped
 * @see vDutCom_DiagModeStop()
 */
MYDLL_API BOOL WINAPI bDutCom_DiagModeStopped(void)
{
  return(!tagComRecvData_m.tagDiagMode.bStarted);
} // bDutCom_DiagModeStopped()


/**
 * Gain security access with level eSecurityAccess
 *
 * @author uidc9013 (21.12.2011)
 * @param eSecurityAccess
 *               Security access level
 *
 * @see enum ESecurityAccess
 */
MYDLL_API void WINAPI vDutCom_SecurityAccess(ESecurityAccess eSecurityAccess)
{
  vResetComSendData_m();
  tagComSendData_m.tagSecurityAccess.eSecurityAccess=eSecurityAccess;
  tagComSendData_m.tagSecurityAccess.bSend          =TRUE;
} // vDutCom_SecurityAccess()

/**
 * Returns the current security access level
 *
 * @author uidc9013 (21.12.2011)
 * @return The current security level of the dut
 * @see enum ESecurityAccess
 */
MYDLL_API ESecurityAccess WINAPI eDutCom_SecurityGetCurrentAccess(void)
{
  return(tagComRecvData_m.tagSecurityAccess.eSecurityAccess);
} // eDutCom_SecurityGetCurrentAccess()


/**
 * Resets the dut
 *
 * @author uidc9013 (18.01.2012)
 */
MYDLL_API void WINAPI vDutCom_Reset(void)
{
  vResetComSendData_m();
  tagComSendData_m.tagReset.bSend=TRUE;
} // vDutCom_Reset()


/**
 * resets the clock
 *
 * @author uidc9013 (18.01.2012)
 */
MYDLL_API void WINAPI vDutCom_ClockReset(void)
{
  vResetComSendData_m();
  tagComSendData_m.tagClock.bReset=TRUE;
} // vDutCom_ClockReset()

/**
 * set the seconds of the clock to the specified value
 *
 * @author uidc9013 (18.01.2012)
 * @param uiSeconds new value for the seconds
 */
MYDLL_API void WINAPI vDutCom_ClockSetSeconds(UINT uiSeconds)
{
  vResetComSendData_m();
  tagComSendData_m.tagClock.uiSeconds=uiSeconds;
  tagComSendData_m.tagClock.bSetSeconds=TRUE;
} // vDutCom_ClockSetSeconds()

/**
 * set the minutes of the clock to the specified value
 *
 * @author uidc9013 (18.01.2012)
 * @param uiMinutes new value for the minutes
 */
MYDLL_API void WINAPI vDutCom_ClockSetMinutes(UINT uiMinutes)
{
  vResetComSendData_m();
  tagComSendData_m.tagClock.uiMinutes=uiMinutes;
  tagComSendData_m.tagClock.bSetMinutes=TRUE;
} // vDutCom_ClockSetMinutes()

/**
 * set the hours of the clock to the specified value
 *
 * @author uidc9013 (18.01.2012)
 * @param uiHours new value for the hours
 */
MYDLL_API void WINAPI vDutCom_ClockSetHours(UINT uiHours)
{
  vResetComSendData_m();
  tagComSendData_m.tagClock.uiHours=uiHours;
  tagComSendData_m.tagClock.bSetHours=TRUE;
} // vDutCom_ClockSetHours()


/**
 * set the hours of the clock to the specified value
 *
 * @author uidc9013 (10.06.2015)
 * @param uiHours new value for the hours
 */
MYDLL_API void WINAPI vDutCom_ClockSetTime(UINT uiHours,
                                           UINT uiMinutes,
                                           UINT uiSeconds)
{
  vResetComSendData_m();
  tagComSendData_m.tagClock.uiHours=uiHours;
  tagComSendData_m.tagClock.uiMinutes=uiMinutes;
  tagComSendData_m.tagClock.uiSeconds=uiSeconds;
  tagComSendData_m.tagClock.bSetTime=TRUE;
} // vDutCom_ClockSetHours()


/**
* Returns whether the time could be set.
*
* @author WeU (10.06.2015)
*
* @return TRUE on success.
*/
MYDLL_API BOOL WINAPI bDutCom_ClockSetTimeDone(void)
{
  return(tagComRecvData_m.tagClock.bTimeIsSet);
} // bDutCom_ClockSetTimeDone()


/**
 * Requests the current set time from the dut
 *
 * @author uidc9013 (18.01.2012)
 * @see uiDutCom_ClockGetSeconds()
 * @see uiDutCom_ClockGetMinutes()
 * @see uiDutCom_ClockGetHours()
 */
MYDLL_API void WINAPI vDutCom_ClockRequestTime(void)
{
  vResetComSendData_m();
  tagComRecvData_m.tagClock.uiHours  =0;
  tagComRecvData_m.tagClock.uiMinutes=0;
  tagComRecvData_m.tagClock.uiSeconds=0;
  tagComRecvData_m.tagClock.bReceived=FALSE;

  tagComSendData_m.tagClock.bRequestTime=TRUE;
} // vDutCom_ClockRequestTime()

/**
 * returns the requested seconds from the dut
 *
 * @author uidc9013 (18.01.2012)
 * @see vDutCom_ClockRequestTime()
 */
MYDLL_API UINT WINAPI uiDutCom_ClockGetSeconds(void)
{
  return(tagComRecvData_m.tagClock.uiSeconds);
} // uiDutCom_ClockGetSeconds()

/**
 * returns the requested minutes from the dut
 *
 * @author uidc9013 (18.01.2012)
 * @see vDutCom_ClockRequestTime()
 */
MYDLL_API UINT WINAPI uiDutCom_ClockGetMinutes(void)
{
  return(tagComRecvData_m.tagClock.uiMinutes);
} // uiDutCom_ClockGetMinutes()

/**
 * returns the requested hours from the dut
 *
 * @author uidc9013 (18.01.2012)
 * @see vDutCom_ClockRequestTime()
 */
MYDLL_API UINT WINAPI uiDutCom_ClockGetHours(void)
{
  return(tagComRecvData_m.tagClock.uiHours);
} // uiDutCom_ClockGetHours()

/**
 * returns the requested time from the dut
 *
 * @author uidc9013 (10.06.2015)
 * @param puiHours   pointer to UINT to receive the value for hour.
 * @param puiMinutes pointer to UINT to receive the value for minutes.
 * @param puiSeconds pointer to UINT to receive the value for seconds.
 *
 * @return TRUE on success.
 * @see vDutCom_ClockRequestTime()
 */
MYDLL_API BOOL WINAPI bDutCom_ClockGetTime(UINT *puiHours,
                                           UINT *puiMinutes,
                                           UINT *puiSeconds)
{
  if(!tagComRecvData_m.tagClock.bReceived)
  {
    return(FALSE);
  }

  if(puiHours)
    *puiHours=tagComRecvData_m.tagClock.uiHours;
  if(puiMinutes)
    *puiMinutes=tagComRecvData_m.tagClock.uiMinutes;
  if(puiSeconds)
    *puiSeconds=tagComRecvData_m.tagClock.uiSeconds;

  return(TRUE);
} // uiDutCom_ClockGetHours()



/**
 * Writes 1 byte at specified address
 * To get whether the byte could be written call bDutCom_DataEepromByteWritten()
 *
 * @author uidc9013 (18.01.2012)
 * @param uiAddress address to write byte into
 * @param ucData    data to be written
 * @see bDutCom_DataEepromByteWritten()
 */
MYDLL_API void WINAPI vDutCom_DataEepromByteWrite(UINT uiAddress,
                                                  UCHAR ucData)
{
  //printf("vDutCom_DataEepromByteWrite()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataByteWise.uiWriteAddress=uiAddress;
  tagComSendData_m.tagStaticDataByteWise.ucWriteData   =ucData;
  tagComSendData_m.tagStaticDataByteWise.bWrite        =TRUE;
  tagComRecvData_m.tagStaticDataByteWise.bWritten      =FALSE;
} // vDutCom_DataEepromByteWrite()

/**
 * Returns whether the byte could be written or not
 *
 * @author uidc9013 (18.01.2012)
 * @return TRUE if the byte was written
 * @see vDutCom_DataEepromByteWrite()
 */
MYDLL_API BOOL WINAPI bDutCom_DataEepromByteWritten(void)
{
  //printf("bDutCom_DataEepromByteWritten()\n");
  return(tagComRecvData_m.tagStaticDataByteWise.bWritten);
} // bDutCom_DataEepromByteWritten()

/**
 * Request a byte from a address
 * Call bDutCom_DataEepromByteRead() to get the requested byte
 *
 * @author uidc9013 (18.01.2012)
 *
 * @param uiAddress of the byte to read
 * @see bDutCom_DataEepromByteRead()
 */
MYDLL_API void WINAPI vDutCom_DataEepromByteRequest(UINT uiAddress)
{
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataByteWise.uiRequestAddress=uiAddress;
  tagComSendData_m.tagStaticDataByteWise.bRequest        =TRUE;
  tagComRecvData_m.tagStaticDataByteWise.bReceived       =FALSE;
} // vDutCom_DataEepromByteRequest()

/**
 * returns a byte requested by vDutCom_DataEepromByteRequest()
 *
 * @author uidc9013 (18.01.2012)
 *
 * @param pucData returned data
 *
 * @return MYDLL_API BOOL
 * @see vDutCom_DataEepromByteRequest()
 */
MYDLL_API BOOL WINAPI bDutCom_DataEepromByteRead(UCHAR *pucData)
{
  if(!tagComRecvData_m.tagStaticDataByteWise.bReceived)
    return(FALSE);

  if(pucData)
    *pucData=tagComRecvData_m.tagStaticDataByteWise.ucReceivedData;

  return(tagComRecvData_m.tagStaticDataByteWise.bReceived);
} // bDutCom_DataEepromByteRead()









/**
 * static data: bulk wise access
 */
/**
 * Writes 1 byte at specified address
 * To get whether the byte could be written call bDutCom_DataEepromBulkWritten()
 *
 * @author uidc9013 (19.01.2012)
 * @param uiAddress address to write byte into
 * @param pucData   data to be written
 * @param uiBytes   number bytes to be written
 *
 * @see bDutCom_DataEepromBulkWritten()
 */
MYDLL_API void WINAPI vDutCom_DataEepromBulkWrite(UINT   uiAddress,
                                                  UCHAR *pucData,
                                                  UINT   uiBytes)
{
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataBulk.uiWriteAddress=uiAddress;
  tagComSendData_m.tagStaticDataBulk.uiBytes=uiBytes;
  tagComSendData_m.tagStaticDataBulk.pucWriteData =(UCHAR*)malloc(tagComSendData_m.tagStaticDataBulk.uiBytes);
  memcpy(tagComSendData_m.tagStaticDataBulk.pucWriteData,
         pucData,
         tagComSendData_m.tagStaticDataBulk.uiBytes);
  tagComSendData_m.tagStaticDataBulk.bWrite      =TRUE;
  tagComRecvData_m.tagStaticDataBulk.bWritten    =FALSE;
} // vDutCom_DataEepromBulkWrite()

/**
 * Returns whether the bytes could be written or not
 *
 * @author uidc9013 (19.01.2012)
 * @return TRUE if the bytes were written
 * @see vDutCom_DataEepromBulkWrite()
 */
MYDLL_API BOOL WINAPI bDutCom_DataEepromBulkWritten(void)
{
  //printf("bDutCom_DataEepromBulkWritten()\n");
  return(tagComRecvData_m.tagStaticDataBulk.bWritten);
} // bDutCom_DataEepromBulkWritten()

/**
 * Request uiBytes bytes from a address
 * Call bDutCom_DataEepromBulkRead() to get the requested bytes
 *
 * @author uidc9013 (19.01.2012)
 *
 * @param uiAddress address of the bytes to read
 * @param uiBytes
 * @see bDutCom_DataEepromBulkRead()
 */
MYDLL_API void WINAPI vDutCom_DataEepromBulkRequest(UINT uiAddress,
                                                    UINT uiBytes)
{
  //printf("vDutCom_DataEepromBulkRequest()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataBulk.uiRequestAddress=uiAddress;
  tagComSendData_m.tagStaticDataBulk.uiBytes         =uiBytes;
  tagComSendData_m.tagStaticDataBulk.bRequest        =TRUE;
  tagComRecvData_m.tagStaticDataBulk.bReceived       =FALSE;
  tagComRecvData_m.tagStaticDataBulk.pucReceivedData =(UCHAR*)malloc(uiBytes);
} // vDutCom_DataEepromBulkRequest()

/**
 * returns a byte requested by vDutCom_DataEepromBulkRequest()
 *
 * @author uidc9013 (19.01.2012)
 * @param uiAddress address of the bytes to read
 * @param pucData   returned data bytes
 * @param uiBytes   number of received bytes
 *
 * @see vDutCom_DataEepromBulkRequest()
 */
MYDLL_API BOOL WINAPI bDutCom_DataEepromBulkRead(UINT   uiAddress,
                                                 UCHAR *pucData,
                                                 UINT   uiBytes)
{
  //printf("bDutCom_DataEepromBulkRead()\n");
  if(!tagComRecvData_m.tagStaticDataBulk.bReceived)
    return(FALSE);

  if(pucData)
    memcpy(pucData,
           tagComRecvData_m.tagStaticDataBulk.pucReceivedData,
           uiBytes);
  free(tagComRecvData_m.tagStaticDataBulk.pucReceivedData);

  return(tagComRecvData_m.tagStaticDataBulk.bReceived);
} // bDutCom_DataEepromBulkRead()


/**
 * static data: serial number access
 */
/**
 * write serial number
 *
 * @author uidc9013 (19.01.2012)
 * @param uiSerialNumber
 *               serial number to be written
 *
 * @see bDutCom_DataSerialNumberWritten()
 */
MYDLL_API void WINAPI vDutCom_DataSerialNumberWrite(UINT uiSerialNumber)
{
  //printf("vDutCom_DataSerialNumberWrite()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataSerialNumber.uiSerialNumber=uiSerialNumber;
  tagComSendData_m.tagStaticDataSerialNumber.bWrite        =TRUE;
  tagComRecvData_m.tagStaticDataSerialNumber.bWritten      =FALSE;
} // vDutCom_DataSerialNumberWrite()

/**
 * Queries whether the serial number could be written
 *
 * @author uidc9013 (19.01.2012)
 * @return TRUE when the serial number was written successfully
 * @see vDutCom_DataSerialNumberWrite()
 */
MYDLL_API BOOL WINAPI bDutCom_DataSerialNumberWritten(void)
{
  //printf("bDutCom_DataSerialNumberWritten()\n");
  return(tagComRecvData_m.tagStaticDataSerialNumber.bWritten);
} // bDutCom_DataSerialNumberWritten()

/**
 * Requests the serial number
 *
 * @author uidc9013 (19.01.2012)
 * @see bDutCom_DataSerialNumberRead()
 */
MYDLL_API void WINAPI vDutCom_DataSerialNumberRequest(void)
{
  //printf("vDutCom_DataSerialNumberRequest()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataSerialNumber.bRequest =TRUE;
  tagComRecvData_m.tagStaticDataSerialNumber.bReceived=FALSE;
} // vDutCom_DataSerialNumberRequest()

/**
 * Returns the serial number requested by vDutCom_DataSerialNumberRequest()
 *
 * @author uidc9013 (19.01.2012)
 *
 * @param puiSerialNumber Serial number from DUT
 *
 * @return TRUE on success.
 * @see vDutCom_DataSerialNumberRequest
 */
MYDLL_API BOOL WINAPI bDutCom_DataSerialNumberRead(UINT *puiSerialNumber)
{
  //printf("bDutCom_DataSerialNumberRead()\n");
  if(!tagComRecvData_m.tagStaticDataSerialNumber.bReceived)
    return(FALSE);

  if(puiSerialNumber)
    *puiSerialNumber=tagComRecvData_m.tagStaticDataSerialNumber.uiReceivedSerialNumber;

  return(tagComRecvData_m.tagStaticDataSerialNumber.bReceived);
} // bDutCom_DataSerialNumberRead()



/**
 * static data: string data access
 */
/**
 * Writes a data string
 *
 * @author uidc9013 (19.01.2012)
 * @param eDataType
 *               Type of data to write
 * @param pcDataString
 *               data to write
 *
 * @see enum EProductNumberType
 * @see bDutCom_DataStringWritten()
 */
MYDLL_API void WINAPI vDutCom_DataStringWrite(EDataType  eDataType,
                                              char      *pcDataString)
{
  //printf("vDutCom_DataStringWrite(%d)\n",eDataType);
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataString.eDataType=eDataType;
  memcpy(tagComSendData_m.tagStaticDataString.ucaDataString,
         pcDataString,
         uiDevSpec_GetDataSize_m(tagComSendData_m.tagStaticDataString.eDataType));
  tagComSendData_m.tagStaticDataString.bWrite  =TRUE;
  tagComRecvData_m.tagStaticDataString.bWritten=FALSE;
} // vDutCom_DataStringWrite()

/**
 * Queries whether the product number could be written
 *
 * @author uidc9013 (19.01.2012)
 * @return TRUE when the product number was written successfully.
 * @see vDutCom_DataStringWrite()
 */
MYDLL_API BOOL WINAPI bDutCom_DataStringWritten(void)
{
  //printf("bDutCom_DataStringWritten()\n");
  return(tagComRecvData_m.tagStaticDataString.bWritten);
} // bDutCom_DataStringWritten()

/**
 * Requests a product number specified by eProdNrType
 *
 * @author uidc9013 (19.01.2012)
 *
 * @param eDataType type of data to be requested
 *
 * @see enum EProductNumberType
 * @see bDutCom_DataStringRead()
 */
MYDLL_API void WINAPI vDutCom_DataStringRequest(EDataType eDataType)
{
  //printf("vDutCom_DataStringRequest(%d)\n",eDataType);
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataString.eDataType=eDataType;
  tagComSendData_m.tagStaticDataString.bRequest =TRUE;
  tagComRecvData_m.tagStaticDataString.eDataType=eDataType;
  tagComRecvData_m.tagStaticDataString.bReceived=FALSE;
} // vDutCom_DataStringRequest()

/**
 * Returns the data string requested by vDutCom_DataStringRequest()
 *
 * @author uidc9013 (19.01.2012)
 * @param pcDataString
 *               buffer to receive the data string.
 *               The caller has to make sure this buffer is big enough.
 *
 * @return TRUE on success.
 * @see vDutCom_DataStringRequest()
 */
MYDLL_API BOOL WINAPI bDutCom_DataStringRead(char *pcDataString)
{
  //printf("bDutCom_DataStringRead()\n");
  if(!tagComRecvData_m.tagStaticDataString.bReceived)
    return(FALSE);

  if(pcDataString)
  {
    memcpy(pcDataString,
           tagComRecvData_m.tagStaticDataString.ucaDataString,
           uiDevSpec_GetDataSize_m(tagComRecvData_m.tagStaticDataString.eDataType));
    pcDataString[uiDevSpec_GetDataSize_m(tagComRecvData_m.tagStaticDataString.eDataType)]=0;
  }

  return(tagComRecvData_m.tagStaticDataString.bReceived);
} // bDutCom_DataStringRead()


/**
 * Returns info for the specified data type
 *
 * @author WeU (11.12.2012)
 * @param eDataType Data type for which the info is requested
 * @param peDataFormat
 *                  pointer to type enum EDataFormat to receive the data format.
 *                  You may specify NULL if this info is not needed.
 * @param puiDataSizeBytes
 *                  pointer to type UINT to receive the size of the data type in bytes.
 *                  You may specify NULL if this info is not needed.
 *
 * @return
 */
MYDLL_API void WINAPI vDutCom_DataStringGetDataInfo(EDataType    eDataType,
                                                    EDataFormat  *peDataFormat,
                                                    UINT         *puiDataSizeBytes)
{
  if(peDataFormat)
  {
    *peDataFormat=eDevSpec_GetDataFormat_m(eDataType);
  }
  if(puiDataSizeBytes)
  {
    *puiDataSizeBytes=uiDevSpec_GetDataSize_m(eDataType);
  }
} // vDutCom_DataStringGetDataInfo()


/**
 * Returns the number of production tools
 *
 * @author uidc9013 (06.11.2012)
 * @return number of production tools.
 */
MYDLL_API UINT WINAPI uiDutCom_DataProductionInterlockGetCount(void)
{
  return(NUM_PROD_INTERLOCK_TOOLS);
} // uiDutCom_DataProductionInterlockGetCount()


/**
 * Returns the descriptive text for the specified production tool
 *
 * @author uidc9013 (06.11.2012)
 * @param uiInput number of tool to return the text
 *
 * @return descriptive text for specified production tool
 */
MYDLL_API char WINAPI *pcDutCom_DataProductionInterlockGetText(UINT uiTool)
{
  return(pcDevSpec_DataProductionInterlockGetText_m(uiTool));
} // pcDutCom_DataProductionInterlockGetText()


/**
 * Writes the production interlock data
 *
 * @author uidc9013 (20.01.2012)
 * @param uiToolNumber
 *               number of tool (0 based)
 * @param uiToolVersionMajor
 *               major version of tool software X.___
 * @param uiToolVersionMinor
 *               minor version of tool software _.XXX
 * @param uiRunCounter
 *               number of run on this tool
 *
 * @see bDutCom_DataProductionInterlockWritten()
 */
MYDLL_API void WINAPI vDutCom_DataProductionInterlockWrite(UINT uiToolNumber,
                                                           UINT uiToolVersionMajor,
                                                           UINT uiToolVersionMinor,
                                                           UINT uiRunCounter)
{
  //printf("vDutCom_DataProductionInterlockWrite()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber      =uiToolNumber;
  tagComSendData_m.tagStaticDataProductionInterlock.uiToolVersionMajor=uiToolVersionMajor;
  tagComSendData_m.tagStaticDataProductionInterlock.uiToolVersionMinor=uiToolVersionMinor;
  tagComSendData_m.tagStaticDataProductionInterlock.uiRunCounter      =uiRunCounter;
  tagComSendData_m.tagStaticDataProductionInterlock.bWrite            =TRUE;
  tagComRecvData_m.tagStaticDataProductionInterlock.bWritten          =FALSE;
} // vDutCom_DataProductionInterlockWrite()

/**
 * Queries whether the production interlock data could be written
 *
 * @author uidc9013 (20.01.2012)
 * @return TRUE on success.
 * @see vDutCom_DataProductionInterlockWrite()
 */
MYDLL_API BOOL WINAPI bDutCom_DataProductionInterlockWritten(void)
{
  //printf("bDutCom_DataProductionInterlockWritten()\n");
  return(tagComRecvData_m.tagStaticDataProductionInterlock.bWritten);
} // bDutCom_DataProductionInterlockWritten()

/**
 * Requests a product number specified by eProdNrType
 *
 * @author uidc9013 (19.01.2012)
 * @param uiToolNumber
 *               number of tool (0 based)
 *
 * @see bDutCom_DataProductionInterlockRead()
 */
MYDLL_API void WINAPI vDutCom_DataProductionInterlockRequest(UINT uiToolNumber)
{
  //printf("vDutCom_DataProductionInterlockRequest()\n");
  vResetComSendData_m();
  tagComSendData_m.tagStaticDataProductionInterlock.uiToolNumber=uiToolNumber;
  tagComSendData_m.tagStaticDataProductionInterlock.bRequest    =TRUE;
  tagComRecvData_m.tagStaticDataProductionInterlock.bReceived   =FALSE;
} // vDutCom_DataProductionInterlockRequest()

/**
 * Returns the production interlock data requested by<br>
 * vDutCom_DataProductionInterlockRequest()
 *
 * if a value is not needed then pass NULL for that parameter.
 *
 * @author uidc9013 (19.01.2012)
 * @param puiVersionMajor
 *                   major version of tool X.___ or NULL if not needed
 * @param puiVersionMinor
 *                   minor version of tool _.XXX or NULL if not needed
 * @param puiRunCounter
 *                   counts of run on this tool or NULL if not needed
 * @param puiYear
 *                   pointer to year of date or NULL if not needed
 * @param puiMonth
 *                   pointer to month of date or NULL if not needed
 * @param puiDay
 *                   pointer to day of date or NULL if not needed
 * @param puiHours
 *                   pointer to hours of time or NULL if not needed
 * @param puiMinutes
 *                   pointer to minutes of time or NULL if not needed
 * @param puiSeconds
 *                   pointer to seconds of time or NULL if not needed
 *
 * @return TRUE on success.
 * @see vDutCom_DataProductionInterlockRequest()
 */
MYDLL_API BOOL WINAPI bDutCom_DataProductionInterlockRead(UINT *puiVersionMajor,
                                                          UINT *puiVersionMinor,
                                                          UINT *puiRunCounter,
                                                          UINT *puiYear,
                                                          UINT *puiMonth,
                                                          UINT *puiDay,
                                                          UINT *puiHours,
                                                          UINT *puiMinutes,
                                                          UINT *puiSeconds)
{
  //printf("bDutCom_DataProductionInterlockRead()\n");
  if(!tagComRecvData_m.tagStaticDataProductionInterlock.bReceived)
    return(FALSE);

  if(puiVersionMajor)
    *puiVersionMajor=tagComRecvData_m.tagStaticDataProductionInterlock.uiVersionMajor;
  if(puiVersionMinor)
    *puiVersionMinor=tagComRecvData_m.tagStaticDataProductionInterlock.uiVersionMinor;
  if(puiRunCounter)
    *puiRunCounter=tagComRecvData_m.tagStaticDataProductionInterlock.uiRunCounter;
  if(puiYear)
    *puiYear=tagComRecvData_m.tagStaticDataProductionInterlock.uiYear;
  if(puiMonth)
    *puiMonth=tagComRecvData_m.tagStaticDataProductionInterlock.uiMonth;
  if(puiDay)
    *puiDay=tagComRecvData_m.tagStaticDataProductionInterlock.uiDay;
  if(puiHours)
    *puiHours=tagComRecvData_m.tagStaticDataProductionInterlock.uiHours;
  if(puiMinutes)
    *puiMinutes=tagComRecvData_m.tagStaticDataProductionInterlock.uiMinutes;
  if(puiSeconds)
    *puiSeconds=tagComRecvData_m.tagStaticDataProductionInterlock.uiSeconds;

  return(tagComRecvData_m.tagStaticDataProductionInterlock.bReceived);
} // bDutCom_DataProductionInterlockRead()


/**
 * pointer info
 */
/**
 * Returns the number of pointers on the dut
 *
 * @author uidc9013 (20.01.2012)
 * @return number of pointers on dut.
 */
MYDLL_API UINT WINAPI uiDutCom_PointerGetCount(void)
{
  return(NUM_POINTERS);
} // uiDutCom_PointerGetCount()


/**
 * Returns the descriptive text for the specified pointer
 *
 * @author uidc9013 (01.02.2012)
 * @param uiTellTale number of pointer to return the text
 *
 * @return descriptive text for specified tell-tale
 */
MYDLL_API char WINAPI *pcDutCom_PointerGetText(UINT uiPointer)
{
  return(pcDevSpec_PointerGetText_m(uiPointer));
} // pcDutCom_PointerGetText()


/**
 * pointer adjust
 */
/**
 * Initializes the pointer adjustment
 *
 * @author uidc9013 (20.01.2012)
 *
 * @see bDutCom_PointerAdjustInitialized()
 */
MYDLL_API void WINAPI vDutCom_PointerAdjustInit(void)
{
  //printf("vDutCom_PointerAdjustInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerAdjust.bInit       =TRUE;
  tagComRecvData_m.tagPointerAdjust.bInitialized=FALSE;
} // vDutCom_PointerAdjustInit()

/**
 * Queries whether the pointer adjustment could initialize.
 *
 * @author uidc9013 (20.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_PointerAdjustInit()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerAdjustInitialized(void)
{
  //printf("bDutCom_PointerAdjustInitialized()\n");
  return(tagComRecvData_m.tagPointerAdjust.bInitialized);
} // bDutCom_PointerAdjustInitialized()

/**
 * Sets the offset of the specified pointer
 *
 * @author uidc9013 (20.01.2012)
 *
 * @param uiPointer pointer to set offset (1 based)
 * @param uiOffset offset to set
 *
 * @bDutCom_PointerAdjustOffsetIsSet()
 */
MYDLL_API void WINAPI vDutCom_PointerAdjustSetOffset(UINT uiPointer,
                                                     UINT uiOffset)
{
  //printf("vDutCom_PointerAdjustSetOffset()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerAdjust.bSetOffset  =TRUE;
  tagComSendData_m.tagPointerAdjust.uiPointer   =uiPointer;
  tagComSendData_m.tagPointerAdjust.uiOffset    =uiOffset;
  tagComRecvData_m.tagPointerAdjust.bOffsetIsSet=FALSE;
} // vDutCom_PointerAdjustSetOffset()

/**
 * Queries whether the pointer offset could be set
 *
 * @author uidc9013 (20.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_PointerAdjustSetOffset()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerAdjustOffsetIsSet(void)
{
  //printf("bDutCom_PointerAdjustOffsetIsSet()\n");
  return(tagComRecvData_m.tagPointerAdjust.bOffsetIsSet);
} // bDutCom_PointerAdjustOffsetIsSet()

/**
 * Sets the gain of the specified pointer at the specified position
 *
 * @author uidc9013 (25.10.2012)
 * @param uiPointer  pointer to set offset (1 based)
 * @param uiPosition position of pointer at which gain is set (display-value of pointer)
 * @param fGain      gain to set.<br>
 *                   1.0 means no gain<br>
 *                   a value greater than 1.0 means the pointer moves higher<br>
 *                   a value lower than 1.0 means the pointer moves lower
 *
 * @see bDutCom_PointerAdjustGainIsSet ()
 */
MYDLL_API void WINAPI vDutCom_PointerAdjustSetGain(UINT uiPointer,
                                                   UINT uiPosition,
                                                   float fGain)
{
  //printf("vDutCom_PointerAdjustSetGain()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerAdjust.bSetGain    =TRUE;
  tagComSendData_m.tagPointerAdjust.uiPointer   =uiPointer;
  tagComSendData_m.tagPointerAdjust.uiPosition  =uiPosition;
  tagComSendData_m.tagPointerAdjust.fGain       =fGain;
  tagComRecvData_m.tagPointerAdjust.bGainIsSet=FALSE;
} // vDutCom_PointerAdjustSetGain()

/**
 * Queries whether the pointer gain could be set
 *
 * @author uidc9013 (25.10.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_PointerAdjustSetGain()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerAdjustGainIsSet(void)
{
  //printf("bDutCom_PointerAdjustGainIsSet()\n");
  return(tagComRecvData_m.tagPointerAdjust.bGainIsSet);
} // bDutCom_PointerAdjustGainIsSet()

/**
 * stores the offset of the specified pointer
 *
 * @author uidc9013 (20.01.2012)
 * @param uiPointer pointer to store offset (1 based)
 * @param uiOffset  value of offset to store
 *
 * @see bDutCom_PointerAdjustOffsetIsStored()
 */
MYDLL_API void WINAPI vDutCom_PointerAdjustStoreOffset(UINT uiPointer)
{
  //printf("vDutCom_PointerAdjustStoreOffset()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerAdjust.bStoreOffset   =TRUE;
  tagComSendData_m.tagPointerAdjust.uiPointer      =uiPointer;
  tagComRecvData_m.tagPointerAdjust.bOffsetIsStored=FALSE;
} // vDutCom_PointerAdjustStoreOffset()

/**
 * Queries whether the pointer offset could be stored
 *
 * @author uidc9013 (20.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_PointerAdjustStoreOffset()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerAdjustOffsetIsStored(void)
{
  //printf("bDutCom_PointerAdjustOffsetIsStored()\n");
  return(tagComRecvData_m.tagPointerAdjust.bOffsetIsStored);
} // bDutCom_PointerAdjustOffsetIsStored()



/**
 * pointer control
 */
/**
 * Initializes the pointer control
 *
 * @author uidc9013 (20.01.2012)
 * @see bDutCom_PointerControlInitialized()
 */
MYDLL_API void WINAPI vDutCom_PointerControlInit(void)
{
  //printf("vDutCom_PointerControlInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerControl.bInit       =TRUE;
  tagComRecvData_m.tagPointerControl.bInitialized=FALSE;
} // vDutCom_PointerControlInit()

/**
 * Returns whether the pointer control could be initialized
 *
 * @author uidc9013 (20.01.2012)
 * @see bDutCom_PointerControlInitialized()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerControlInitialized(void)
{
  //printf("bDutCom_PointerControlInitialized()\n");
  return(tagComRecvData_m.tagPointerControl.bInitialized);
} // bDutCom_PointerControlInitialized()

/**
 * Sets all pointer at once to a specified position.
 *
 * @author uidc9013 (20.01.2012)
 *
 * @param puiPosition
 *               pointer to UINT array. Each UINT represents the value for<br>
 *               1 pointer to set it to.<br>
 *               Items in this array = uiDutCom_PointerGetCount().
 * @see bDutCom_PointerControlPositionAllIsSet()
 */
MYDLL_API void WINAPI vDutCom_PointerControlSetPositionAll(UINT *puiPosition)
{
  //printf("vDutCom_PointerControlSetPositionAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerControl.bSetPositionAll   =TRUE;
  memcpy(tagComSendData_m.tagPointerControl.uiaPosition,
         puiPosition,
         sizeof(UINT)*NUM_POINTERS);
  tagComRecvData_m.tagPointerControl.bPositionAllAreSet=FALSE;
} // vDutCom_PointerControlSetPositionAll()

/**
 * Returns whether the pointer positions could be set.
 *
 * @author uidc9013 (20.01.2012)
 * @see vDutCom_PointerControlSetPositionAll()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerControlPositionAllIsSet(void)
{
  //printf("bDutCom_PointerControlPositionAllIsSet()\n");
  return(tagComRecvData_m.tagPointerControl.bPositionAllAreSet);
} // bDutCom_PointerControlPositionAllIsSet()

/**
 * Sets 1 pointer to a specified position.
 *
 * @author uidc9013 (20.01.2012)
 *
 * @param uiPointer
 *               pointer to set.
 * @param uiPosition
 *               position to set specified pointer to.
 * @see bDutCom_PointerControlPositionSingleIsSet()
 */
MYDLL_API void WINAPI vDutCom_PointerControlSetPositionSingle(UINT uiPointer,
                                                              UINT uiPosition)
{
  //printf("vDutCom_PointerControlSetPositionSingle()\n");
  vResetComSendData_m();
  tagComSendData_m.tagPointerControl.bSetPositionSingle  =TRUE;
  tagComSendData_m.tagPointerControl.uiPointer           =uiPointer;
  tagComSendData_m.tagPointerControl.uiPosition          =uiPosition;
  tagComRecvData_m.tagPointerControl.bPositionSingleIsSet=FALSE;
} // vDutCom_PointerControlSetPositionSingle()

/**
 * Returns whether the pointer could be set to the specified position.
 *
 * @author uidc9013 (20.01.2012)
 * @see vDutCom_PointerControlSetPositionAll()
 */
MYDLL_API BOOL WINAPI bDutCom_PointerControlPositionSingleIsSet(UINT uiPointer)
{
  //printf("bDutCom_PointerControlPositionSingleIsSet()\n");
  return(tagComRecvData_m.tagPointerControl.bPositionSingleIsSet);
} // bDutCom_PointerControlPositionSingleIsSet()





/**
 * tell-tale control
 */
/**
 * Initializes the Tell-Tale control
 *
 * @author uidc9013 (23.01.2012)
 *
 * @see bDutCom_TellTaleCtrlInitialized()
 */
MYDLL_API void WINAPI vDutCom_TellTaleCtrlInit(void)
{
  //printf("vDutCom_TellTaleCtrlInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagTellTaleControl.bInit=TRUE;
  tagComRecvData_m.tagTellTaleControl.bInitialized=FALSE;
} // vDutCom_TellTaleCtrlInit()

/**
 * Queries whether the tell-tale control could initialize.
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_TellTaleCtrlInit()
 */
MYDLL_API BOOL WINAPI bDutCom_TellTaleCtrlInitialized(void)
{
  //printf("bDutCom_TellTaleCtrlInitialized()\n");
  return(tagComRecvData_m.tagTellTaleControl.bInitialized);
} // bDutCom_TellTaleCtrlInitialized()


/**
 * Returns the number of tell-tales on dut
 *
 * @author uidc9013 (23.01.2012)
 * @return number of tell-tales on dut.
 */
MYDLL_API UINT WINAPI uiDutCom_TellTaleCtrlGetCount(void)
{
  return(NUM_TELLTALES);
} // uiDutCom_TellTaleCtrlGetCount

/**
 * Returns the descriptive text for the specified tell-tale
 *
 * @author uidc9013 (01.02.2012)
 * @param uiTellTale number of tell-tale to return the text
 *
 * @return descriptive text for specified tell-tale
 */
MYDLL_API char WINAPI *pcDutCom_TellTaleGetText(UINT uiTellTale)
{
  return(pcDevSpec_TellTaleGetText_m(uiTellTale));
} // pcDutCom_TellTaleGetText()

/**
 * Controls all Tell-tales with one telegram
 *
 * @author uidc9013 (23.01.2012)
 * @param puiTellTaleState
 *               bitmap of tell-tales to set. 1 byte represents 1 tell-tale<br>
 *               this byte > 0 means switch on this tell-tale
 *
 * @see bDutCom_TellTaleCtrlBitmapIsSet()
 */
MYDLL_API void WINAPI vDutCom_TellTaleCtrlSetBitmap(UCHAR *pcaTellTaleState)
{
  //printf("vDutCom_TellTaleCtrlSetBitmap()\n");
  vResetComSendData_m();
  tagComSendData_m.tagTellTaleControl.bSetBitmap=TRUE;
  memcpy(tagComSendData_m.tagTellTaleControl.ucaTellTaleState,
         pcaTellTaleState,
         sizeof(UCHAR)*NUM_TELLTALES);
  tagComRecvData_m.tagTellTaleControl.bBitmapIsSet=FALSE;
} // vDutCom_TellTaleCtrlSetBitmap()

/**
 * Returns whether the tell-tales bitmap pattern is set
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_TellTaleCtrlSetBitmap()
 */
MYDLL_API BOOL WINAPI bDutCom_TellTaleCtrlBitmapIsSet(void)
{
  //printf("bDutCom_TellTaleCtrlBitmapIsSet()\n");
  return(tagComRecvData_m.tagTellTaleControl.bBitmapIsSet);
} // bDutCom_TellTaleCtrlBitmapIsSet()


/**
 * Sets an individual tell-tale in state bOn an switches all other off
 *
 * @author uidc9013 (23.01.2012)
 * @param uiTellTale tell-tale to switch on (0 based)
 * @param bOn        new state of tell-tale
 *
 * @see bDutCom_TellTaleCtrlTellTaleIsSet()
 */
MYDLL_API void WINAPI vDutCom_TellTaleCtrlSetTellTale(UINT uiTellTale,
                                                      BOOL bOn)
{
  //printf("vDutCom_TellTaleCtrlSetTellTale()\n");
  vResetComSendData_m();
  tagComSendData_m.tagTellTaleControl.bSetSingle  =TRUE;
  tagComSendData_m.tagTellTaleControl.uiTellTale  =uiTellTale;
  tagComSendData_m.tagTellTaleControl.bState      =bOn;
  tagComRecvData_m.tagTellTaleControl.bSingleIsSet=FALSE;
} // vDutCom_TellTaleCtrlSetTellTale()

/**
 * Returns whether the state of a single tell-tale has been set
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_TellTaleCtrlSetTellTale()
 */
MYDLL_API BOOL WINAPI bDutCom_TellTaleCtrlTellTaleIsSet(void)
{
  //printf("bDutCom_TellTaleCtrlTellTaleIsSet()\n");
  return(tagComRecvData_m.tagTellTaleControl.bSingleIsSet);
} // bDutCom_TellTaleCtrlTellTaleIsSet()





/**
 * backlight control
 */
/**
 * Initializes the backlight control
 *
 * @author uidc9013 (23.01.2012)
 *
 * @see bDutCom_BackLightCtrlInitialized()
 */
MYDLL_API void WINAPI vDutCom_BackLightCtrlInit(void)
{
  //printf("vDutCom_BackLightCtrlInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagBackLightControl.bInit       =TRUE;
  tagComRecvData_m.tagBackLightControl.bInitialized=FALSE;
} // vDutCom_BackLightCtrlInit()

/**
 * Returns whether the backlight control coul be initialized
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return TRUE on success.
 */
MYDLL_API BOOL WINAPI bDutCom_BackLightCtrlInitialized(void)
{
  //printf("bDutCom_BackLightCtrlInitialized()\n");
  return(tagComRecvData_m.tagBackLightControl.bInitialized);
} // bDutCom_BackLightCtrlInitialized()

/**
 * Returns the number of individually controllable illuminations
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return number of individually controllable illuminations<br>
 * of the specified part
 */
MYDLL_API UINT WINAPI uiDutCom_BackLightCtrlGetCount(void)
{
  return(NUM_BACKLIGHT_ILLUMINATIONS);
} // uiDutCom_BackLightCtrlGetCount()

/**
 * Returns the number of individually controllable colors<br>
 * of the specified part
 *
 * @author uidc9013 (23.01.2012)
 *
 * @param uiBacklight
 *
 * @return number of individually controllable colors<br>
 * of the specified part
 */
MYDLL_API UINT WINAPI uiDutCom_BackLightCtrlGetColorCount(UINT uiBacklight)
{
  return(uiDevSpec_BacklightGetColors_m(uiBacklight));
} // uiDutCom_BackLightCtrlGetColorCount()


/**
 * Returns the descriptive text for the specified backlight illumination
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of backlight illumination to return the text for
 *
 * @return descriptive text for specified backlight illumination
 */
MYDLL_API char WINAPI *pcDutCom_BacklightGetText(UINT uiBacklight)
{
  return(pcDevSpec_BacklightGetText_m(uiBacklight));
} // pcDutCom_BacklightGetText()


/**
 * Sets the state and color of the specified back illumination
 *
 * @author uidc9013 (23.01.2012)
 * @param uiBackLightBitmap
 *                what illumination to switch (bit0=Illumination 0, ...)
 * @param uiColor color to set if illumination has more colors.<br>
 *                Use enum ERgbColors.<br>
 *                This paramter is ignored if the illumination has only 1 color.
 *
 * @return
 * @see bDutCom_BackLightCtrlIsSet()
 */
MYDLL_API void WINAPI vDutCom_BackLightCtrlSet(UINT uiBackLightBitmap,
                                               UINT uiColor)
{
  //printf("vDutCom_BackLightCtrlSet()\n");
  vResetComSendData_m();
  tagComSendData_m.tagBackLightControl.bSetBackLight     =TRUE;
  tagComSendData_m.tagBackLightControl.uiBackLightBitmap =uiBackLightBitmap;
  tagComSendData_m.tagBackLightControl.uiColor           =uiColor;
  tagComRecvData_m.tagBackLightControl.bBackLightIsSet   =FALSE;
} // vDutCom_BackLightCtrlSet()

/**
 * Returns whether the state of the back illumination could be set
 *
 * @author uidc9013 (23.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_BackLightCtrlSet()
 */
MYDLL_API BOOL WINAPI bDutCom_BackLightCtrlIsSet(void)
{
  //printf("bDutCom_BackLightCtrlIsSet()\n");
  return(tagComRecvData_m.tagBackLightControl.bBackLightIsSet);
} // bDutCom_BackLightCtrlIsSet()


/**
 * lcd control
 */
/**
 * Initializes the lcd control
 *
 * @author uidc9013 (24.01.2012)
 *
 * @see bDutCom_LcdCtrlInitialized()
 */
MYDLL_API void WINAPI vDutCom_LcdCtrlInit(void)
{
  //printf("vDutCom_LcdCtrlInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagLcdControl.bInit       =TRUE;
  tagComRecvData_m.tagLcdControl.bInitialized=FALSE;
} // vDutCom_LcdCtrlInit()

/**
 * Returns whether the lcd control could be initialized
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_LcdCtrlInit()
 */
MYDLL_API BOOL WINAPI bDutCom_LcdCtrlInitialized(void)
{
  //printf("bDutCom_LcdCtrlInitialized()\n");
  return(tagComRecvData_m.tagLcdControl.bInitialized);
} // bDutCom_LcdCtrlInitialized()

/**
 * Returns the number of lcds
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return number of lcds.
 */
MYDLL_API UINT WINAPI uiDutCom_LcdCtrlGetCount(void)
{
  return(NUM_LCDS);
} // uiDutCom_LcdCtrlGetCount()

/**
 * Returns the descriptive text for the specified lcd
 *
 * @author uidc9013 (27.09.2012)
 * @param uiLcd number of lcd to return the text for
 *
 * @return descriptive text for specified lcd
 */
MYDLL_API char WINAPI *pcDutCom_LcdCtrlGetText(UINT uiLcd)
{
  return(pcDevSpec_LcdGetText_m(uiLcd));
} // pcDutCom_LcdCtrlGetText()

/**
 * Returns the number of lcd patterns to test (without the pattern "All segments")
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return number of lcd test patterns.
 */
MYDLL_API UINT WINAPI uiDutCom_LcdCtrlPatternGetCount(UINT uiLcd)
{
  UINT uiLcdTestPattern=0;
  switch(uiLcd)
  {
    case 0: uiLcdTestPattern=NUM_LCD_TEST_PATTERN_0; break;
    case 1: uiLcdTestPattern=NUM_LCD_TEST_PATTERN_1; break;
    case 2: uiLcdTestPattern=NUM_LCD_TEST_PATTERN_2; break;
    case 3: uiLcdTestPattern=NUM_LCD_TEST_PATTERN_3; break;
  }
  return(uiLcdTestPattern);
} // uiDutCom_LcdCtrlPatternGetCount()

/**
 * Sets the specified lcd pattern
 *
 * @author uidc9013 (24.01.2012)
 * @param uiLcd  lcd to set pattern on
 * @param eLcdPattern
 *               lcd pattern to set (0-based)
 *
 * @see enum ELcdPattern
 * @see uiDutCom_LcdCtrlGetCount()
 * @see bDutCom_LcdCtrlPatternIsSet()
 */
MYDLL_API void WINAPI vDutCom_LcdCtrlSetPattern(UINT        uiLcd,
                                                ELcdPattern eLcdPattern)
{
  //printf("vDutCom_LcdCtrlSetPattern()\n");
  vResetComSendData_m();
  tagComSendData_m.tagLcdControl.bSetPattern  =TRUE;
  tagComSendData_m.tagLcdControl.uiLcd        =uiLcd;
  tagComSendData_m.tagLcdControl.eLcdPattern  =eLcdPattern;
  tagComRecvData_m.tagLcdControl.bPatternIsSet=FALSE;
} // vDutCom_LcdCtrlSetPattern()

/**
 * Returns whether the lcd test pattern could be set
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_LcdCtrlSetPattern
 */
MYDLL_API BOOL WINAPI bDutCom_LcdCtrlPatternIsSet(void)
{
  //printf("bDutCom_LcdCtrlPatternIsSet()\n");
  return(tagComRecvData_m.tagLcdControl.bPatternIsSet);
} // bDutCom_LcdCtrlPatternIsSet()



/**
 * read digital inputs
 */
/**
 * Initializes the reading of the digital inputs
 *
 * @author uidc9013 (24.01.2012)
 *
 * @see bDutCom_DigInputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_DigInputsInit(void)
{
  //printf("vDutCom_DigInputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagDigInputs.bInit       =TRUE;
  tagComRecvData_m.tagDigInputs.bInitialized=FALSE;
} // vDutCom_DigInputsInit()

/**
 * Returns whether reading of digital inputs could be initialized
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_DigInputsInit()
 */
MYDLL_API BOOL WINAPI bDutCom_DigInputsInitialized(void)
{
  //printf("bDutCom_DigInputsInitialized()\n");
  return(tagComRecvData_m.tagDigInputs.bInitialized);
} // bDutCom_DigInputsInitialized()

/**
 * Returns the number of digital inputs
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return number of digital inputs.
 */
MYDLL_API UINT WINAPI uiDutCom_DigInputsGetCount(void)
{
  return(NUM_DIG_INPUTS);
} // uiDutCom_DigInputsGetCount()


/**
 * Returns the descriptive text for the specified digital input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of digital input to return the text
 *
 * @return descriptive text for specified digital input
 */
MYDLL_API char WINAPI *pcDutCom_DigInputsGetText(UINT uiInput)
{
  return(pcDevSpec_DigInputsGetText_m(uiInput));
} // pcDutCom_DigInputsGetText()


/**
 * Requests the state of all digital inputs
 *
 * @author uidc9013 (24.01.2012)
 *
 * @return MYDLL_API void.
 * @see bDutCom_DigInputsReadAll()
 */
MYDLL_API void WINAPI vDutCom_DigInputsRequestAll(void)
{
  //printf("vDutCom_DigInputsRequestAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagDigInputs.bRequestAll =TRUE;
  tagComRecvData_m.tagDigInputs.bReceivedAll=FALSE;
} // vDutCom_DigInputsRequestAll()

/**
 * Returns the state of all digital inputs
 *
 * @author uidc9013 (24.01.2012)
 * @param pucDigInputStates
 *               pointer to UCHAR array. 1 byte for each input.
 *
 * @return True on success.
 * @see vDutCom_DigInputsRequestAll()
 */
MYDLL_API BOOL WINAPI bDutCom_DigInputsReadAll(UCHAR *pucDigInputStates)
{
  //printf("bDutCom_DigInputsReadAll()\n");
  if(!tagComRecvData_m.tagDigInputs.bReceivedAll)
    return(FALSE);

  if(pucDigInputStates)
  {
    memcpy(pucDigInputStates,
           tagComRecvData_m.tagDigInputs.ucaDigInputStates,
           sizeof(UCHAR)*NUM_DIG_INPUTS);
  }

  return(tagComRecvData_m.tagDigInputs.bReceivedAll);
} // bDutCom_DigInputsReadAll()

/**
 * Requests the state of one single specified digital input
 *
 * @author uidc9013 (25.01.2012)
 * @param uiDigInput number of digital input (0 based)
 *
 * @see bDutCom_DigInputsReadSingle()
 */
MYDLL_API void WINAPI vDutCom_DigInputsRequestSingle(UINT uiDigInput)
{
  //printf("vDutCom_DigInputsRequestSingle(%d)\n",uiDigInput);
  vResetComSendData_m();
  tagComSendData_m.tagDigInputs.bRequestSingle =TRUE;
  tagComSendData_m.tagDigInputs.uiDigInput     =uiDigInput;
  tagComRecvData_m.tagDigInputs.bReceivedSingle=FALSE;
} // vDutCom_DigInputsRequestSingle()

/**
 * Returns the state of one single specified digital input
 *
 * @author uidc9013 (25.01.2012)
 * @param pbDigInputState
 *                   pointer to BOOL variable to receive the state
 *
 * @return TRUE on success.
 */
MYDLL_API BOOL WINAPI bDutCom_DigInputsReadSingle(BOOL *pbDigInputState)
{
  //printf("bDutCom_DigInputsReadSingle()\n");
  if(!tagComRecvData_m.tagDigInputs.bReceivedSingle)
    return(FALSE);

  if(pbDigInputState)
    *pbDigInputState=tagComRecvData_m.tagDigInputs.bDigInputState;

  return(tagComRecvData_m.tagDigInputs.bReceivedSingle);
} // bDutCom_DigInputsReadSingle()


/**
 * write digital outputs
 */
/**
 * Initializes the control of the digital outputs
 *
 * @author uidc9013 (24.01.2012)
 *
 * @see bDutCom_DigInputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_DigOutputsInit(void)
{
  //printf("vDutCom_DigOutputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagDigOutputs.bInit       =TRUE;
  tagComRecvData_m.tagDigOutputs.bInitialized=FALSE;
} // vDutCom_DigOutputsInit()

/**
 * Returns whether the initialization of digital output control succeeded
 *
 * @author uidc9013 (25.01.2012)
 *
 * @return TRUE on success.
 */
MYDLL_API BOOL WINAPI bDutCom_DigOutputsInitialized(void)
{
  //printf("bDutCom_DigOutputsInitialized()\n");
  return(tagComRecvData_m.tagDigOutputs.bInitialized);
} // bDutCom_DigOutputsInitialized()

/**
 * Returns the number of digital outputs
 *
 * @author uidc9013 (25.01.2012)
 *
 * @return number of digital outputs.
 */
MYDLL_API UINT WINAPI uiDutCom_DigOutputsGetCount(void)
{
  return(NUM_DIG_OUTPUTS);
} // uiDutCom_DigOutputsGetCount()


/**
 * Returns the descriptive text for the specified digital output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiOutput number of digital output to return the text
 *
 * @return descriptive text for specified digital output
 */
MYDLL_API char WINAPI *pcDutCom_DigOutputsGetText(UINT uiOutput)
{
  return(pcDevSpec_DigOutputsGetText_m(uiOutput));
} // pcDutCom_DigOutputsGetText()


/**
 * Sets all digital outputs at once
 *
 * @author uidc9013 (25.01.2012)
 * @param pucDigOutputState
 *               pointer to UCHAR array. Each UCHAR represents the state of<br>
 *               1 output to set. An value greater than 0 sets the output active.<br>
 *               Size of this array = uiDutCom_DigOutputsGetCount()
 *
 * @see uiDutCom_DigOutputsGetCount()
 * @see bDutCom_DigOutputsAllWritten()
 */
MYDLL_API void WINAPI vDutCom_DigOutputsWriteAll(UCHAR *pucDigOutputState)
{
  //printf("vDutCom_DigOutputsWriteAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagDigOutputs.bWriteAll  =TRUE;
  memcpy(tagComSendData_m.tagDigOutputs.ucaDigOutputState,
         pucDigOutputState,
         sizeof(UCHAR)*NUM_DIG_OUTPUTS);
  tagComRecvData_m.tagDigOutputs.bAllWritten=FALSE;
} // vDutCom_DigOutputsWriteAll()

/**
 * Returns TRUE if the outputs could be set.
 *
 * @author uidc9013 (25.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_DigOutputsWriteAll()
 */
MYDLL_API BOOL WINAPI bDutCom_DigOutputsAllWritten(void)
{
  //printf("bDutCom_DigOutputsAllWritten()\n");
  return(tagComRecvData_m.tagDigOutputs.bAllWritten);
} // bDutCom_DigOutputsAllWritten()

/**
 * Sets 1 digital output only
 *
 * @author uidc9013 (25.01.2012)
 * @param uiDigOutput
 *               digital output to (re)set. (0 based)
 * @param bDigOutputState
 *               State to set the specified output to.
 *
 * @see bDutCom_DigOutputsSingleWritten()
 */
MYDLL_API void WINAPI vDutCom_DigOutputsWriteSingle(UINT uiDigOutput,
                                                    BOOL bDigOutputState)
{
  //printf("vDutCom_DigOutputsWriteSingle(%d)\n",uiDigOutput);
  vResetComSendData_m();
  tagComSendData_m.tagDigOutputs.bWriteSingle   =TRUE;
  tagComSendData_m.tagDigOutputs.uiDigOutput    =uiDigOutput;
  tagComSendData_m.tagDigOutputs.bDigOutputState=bDigOutputState;
  tagComRecvData_m.tagDigOutputs.bAllWritten    =FALSE;
} // vDutCom_DigOutputsWriteSingle()

/**
 * Returns TRUE if the single specified output could be set.
 *
 * @author uidc9013 (25.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_DigOutputsWriteSingle()
 */
MYDLL_API BOOL WINAPI bDutCom_DigOutputsSingleWritten(void)
{
  //printf("bDutCom_DigOutputsSingleWritten()\n");
  return(tagComRecvData_m.tagDigOutputs.bSingleWritten);
} // bDutCom_DigOutputsSingleWritten()



/**
 * read analog inputs
 */
/**
 * Initializes the control for analog inputs
 *
 * @author uidc9013 (26.01.2012)
 *
 * @see bDutCom_AnalogInputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_AnalogInputsInit(void)
{
  //printf("vDutCom_AnalogInputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagAnalogInputs.bInit       =TRUE;
  tagComRecvData_m.tagAnalogInputs.bInitialized=FALSE;
} // vDutCom_AnalogInputsInit()

/**
 * Returns whether the control of analog inputs could initialize
 *
 * @author uidc9013 (26.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogInputsInit()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogInputsInitialized(void)
{
  //printf("bDutCom_AnalogInputsInitialized()\n");
  return(tagComRecvData_m.tagAnalogInputs.bInitialized);
} // bDutCom_AnalogInputsInitialized()

/**
 * Returns the number of analog inputs inputs
 *
 * @author uidc9013 (26.01.2012)
 *
 * @return number of analog inputs.
 */
MYDLL_API UINT WINAPI uiDutCom_AnalogInputsGetCount(void)
{
  return(NUM_ANALOG_INPUTS);
} // uiDutCom_AnalogInputsGetCount()


/**
 * Returns the descriptive text for the specified analog input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of analog input to return the text
 *
 * @return descriptive text for specified analog input
 */
MYDLL_API char WINAPI *pcDutCom_AnalogInputsGetText(UINT uiInput)
{
  return(pcDevSpec_AnalogInputsGetText_m(uiInput));
} // pcDutCom_AnalogInputsGetText()

/**
 * Requests the values of all analog inputs
 *
 * @author uidc9013 (26.01.2012)
 *
 * @see bDutCom_AnalogInputsReadAll()
 */
MYDLL_API void WINAPI vDutCom_AnalogInputsRequestAll(void)
{
  //printf("vDutCom_AnalogInputsRequestAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagAnalogInputs.bRequestAll =TRUE;
  tagComRecvData_m.tagAnalogInputs.bReceivedAll=FALSE;
} // vDutCom_AnalogInputsRequestAll()

/**
 * Returns the values of all analog inputs
 *
 * @author uidc9013 (26.01.2012)
 * @param puiaAnalogInputValues
 *               pointer to UINT buffer to contain all values of the analog inputs<br>
 *               size of the buffer = uiDutCom_AnalogInputsGetCount()
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogInputsRequestAll()
 * @see uiDutCom_AnalogInputsGetCount()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogInputsReadAll(UINT *puiAnalogInputValues)
{
  //printf("bDutCom_AnalogInputsReadAll()\n");
  if(!tagComRecvData_m.tagAnalogInputs.bReceivedAll)
    return(FALSE);

  if(puiAnalogInputValues)
  {
    memcpy(puiAnalogInputValues,
           tagComRecvData_m.tagAnalogInputs.uiaAnalogInputValues,
           sizeof(UINT)*NUM_ANALOG_INPUTS);
  }
  return(tagComRecvData_m.tagAnalogInputs.bReceivedAll);
} // bDutCom_AnalogInputsReadAll()

/**
 * Requests the value of a single specified analog input
 *
 * @author uidc9013 (26.01.2012)
 *
 * @see bDutCom_AnalogInputsReadSingle()
 */
MYDLL_API void WINAPI vDutCom_AnalogInputsRequestSingle(UINT uiAnalogInput)
{
  //printf("vDutCom_AnalogInputsRequestSingle(%d)\n",uiAnalogInput);
  vResetComSendData_m();
  tagComSendData_m.tagAnalogInputs.bRequestSingle =TRUE;
  tagComSendData_m.tagAnalogInputs.uiAnalogInput  =uiAnalogInput;
  tagComRecvData_m.tagAnalogInputs.bReceivedSingle=FALSE;
} // vDutCom_AnalogInputsRequestSingle()

/**
 * Returns the value of a single specified analog input
 *
 * @author uidc9013 (26.01.2012)
 * @param puiAnalogInputValue
 *               pointer to UINT buffer to contain the values of<br>
 *               the specified analog input.
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogInputsRequestAll()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogInputsReadSingle(UINT *puiAnalogInputValue)
{
  //printf("bDutCom_AnalogInputsReadSingle()\n");
  if(!tagComRecvData_m.tagAnalogInputs.bReceivedSingle)
    return(FALSE);

  if(puiAnalogInputValue)
  {
    *puiAnalogInputValue=tagComRecvData_m.tagAnalogInputs.uiAnalogInputValue;
  }
  return(tagComRecvData_m.tagAnalogInputs.bReceivedSingle);
} // bDutCom_AnalogInputsReadSingle()


/**
 * write analog outputs
 */
/**
 * Initializes the control for analog outputs.
 *
 * @author uidc9013 (27.01.2012)
 * @see bDutCom_AnalogOutputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_AnalogOutputsInit(void)
{
  //printf("vDutCom_AnalogOutputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagAnalogOutputs.bInit       =TRUE;
  tagComRecvData_m.tagAnalogOutputs.bInitialized=FALSE;
} // vDutCom_AnalogOutputsInit()

/**
 * Returns whether the control for analog outputs is initialized.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogOutputsInit()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogOutputsInitialized(void)
{
  //printf("bDutCom_AnalogOutputsInitialized()\n");
  return(tagComRecvData_m.tagAnalogOutputs.bInitialized);
} // bDutCom_AnalogOutputsInitialized()

/**
 * Returns the number of analog outputs.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return number of analog outputs.
 */
MYDLL_API UINT WINAPI uiDutCom_AnalogOutputsGetCount(void)
{
  return(NUM_ANALOG_OUTPUTS);
} // uiDutCom_AnalogOutputsGetCount()

/**
 * Returns the descriptive text for the specified analog output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of analog output to return the text
 *
 * @return descriptive text for specified analog output
 */
MYDLL_API char WINAPI *pcDutCom_AnalogOutputsGetText(UINT uiOutput)
{
  return(pcDevSpec_AnalogOutputsGetText_m(uiOutput));
} // pcDutCom_AnalogOutputsGetText()

/**
 * Writes the values to all analog outputs at once
 *
 * @author uidc9013 (27.01.2012)
 * @param pwAnalogOutputValues
 *               pointer to UINT array containing the values to be written.
 *               Number of items in this array = uiDutCom_AnalogOutputsGetCount()
 *
 * @return MYDLL_API void.
 */
MYDLL_API void WINAPI vDutCom_AnalogOutputsWriteAll(UINT *puiAnalogOutputValues)
{
  //printf("vDutCom_AnalogOutputsWriteAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagAnalogOutputs.bWriteAll  =TRUE;
  memcpy(tagComSendData_m.tagAnalogOutputs.auiAnalogOutputValues,
         puiAnalogOutputValues,
         sizeof(UINT)*NUM_ANALOG_OUTPUTS);
  tagComRecvData_m.tagAnalogOutputs.bAllWritten=FALSE;
} // vDutCom_AnalogOutputsWriteAll()

/**
 * Returns whether the analog outputs could be written
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogOutputsWriteAll()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogOutputsAllWritten(void)
{
  //printf("bDutCom_AnalogOutputsAllWritten()\n");
  return(tagComRecvData_m.tagAnalogOutputs.bAllWritten);
} // bDutCom_AnalogOutputsAllWritten()

/**
 * Writes value to analog output.
 *
 * @author uidc9013 (27.01.2012)
 * @param uiAnalogOutput
 *               number of output to write (0 based).
 * @param uiAnalogOutputValue
 *               value to set for analog output.
 *
 * @see bDutCom_AnalogOutputsSingleWritten()
 */
MYDLL_API void WINAPI vDutCom_AnalogOutputsWriteSingle(UINT uiAnalogOutput,
                                                       UINT uiAnalogOutputValue)
{
  //printf("vDutCom_AnalogOutputsWriteSingle(%d)\n",uiAnalogOutput);
  vResetComSendData_m();
  tagComSendData_m.tagAnalogOutputs.bWriteSingle       =TRUE;
  tagComSendData_m.tagAnalogOutputs.uiAnalogOutput     =uiAnalogOutput;
  tagComSendData_m.tagAnalogOutputs.uiAnalogOutputValue=uiAnalogOutputValue;
  tagComRecvData_m.tagAnalogOutputs.bSingleWritten     =FALSE;
} // vDutCom_AnalogOutputsWriteSingle()

/**
 * Returns whether the analog output was written
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_AnalogOutputsWriteSingle()
 */
MYDLL_API BOOL WINAPI bDutCom_AnalogOutputsSingleWritten(void)
{
  //printf("bDutCom_AnalogOutputsSingleWritten()\n");
  return(tagComRecvData_m.tagAnalogOutputs.bSingleWritten);
} // bDutCom_AnalogOutputsSingleWritten()



/**
 * read frequency inputs
 */
/**
 * Initializes the reading of the frequency inputs.
 *
 * @author uidc9013 (27.01.2012)
 * @see bDutCom_FreqInputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_FreqInputsInit(void)
{
  //printf("vDutCom_FreqInputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqInputs.bInit       =TRUE;
  tagComRecvData_m.tagFreqInputs.bInitialized=FALSE;
} // vDutCom_FreqInputsInit()

/**
 * Returns whether the reading of the frequency inputs is initialized.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_FreqInputsInit()
 */
MYDLL_API BOOL WINAPI bDutCom_FreqInputsInitialized(void)
{
  //printf("bDutCom_FreqInputsInitialized()\n");
  return(tagComRecvData_m.tagFreqInputs.bInitialized);
} // bDutCom_FreqInputsInitialized()

/**
 * Returns the number of frequency inputs
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return number of frequency inputs.
 */
MYDLL_API UINT WINAPI uiDutCom_FreqInputsGetCount(void)
{
  //printf("uiDutCom_FreqInputsGetCount()\n");
  return(NUM_FREQ_INPUTS);
} // uiDutCom_FreqInputsGetCount()

/**
 * Returns the descriptive text for the specified frequency input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of frequency input to return the text
 *
 * @return descriptive text for specified frequency input
 */
MYDLL_API char WINAPI *pcDutCom_FreqInputsGetText(UINT uiInput)
{
  return(pcDevSpec_FreqInputsGetText_m(uiInput));
} // implement pcDutCom_FreqInputsGetText()

/**
 * Requests the values of all frequency inputs
 *
 * @author uidc9013 (26.01.2012)
 *
 * @see bDutCom_FreqInputsReadAll()
 */
MYDLL_API void WINAPI vDutCom_FreqInputsRequestAll(void)
{
  //printf("vDutCom_FreqInputsRequestAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqInputs.bRequestAll =TRUE;
  tagComRecvData_m.tagFreqInputs.bReceivedAll=FALSE;
} // vDutCom_FreqInputsRequestAll()

/**
 * Returns the frequencies of all frequency inputs
 *
 * @author uidc9013 (26.01.2012)
 * @param puiFreqInputValue
 *               pointer to UINT buffer to contain all frequencies of the frequency inputs<br>
 *               size of the buffer = uiDutCom_FreqInputsGetCount()
 *
 * @return TRUE on success.
 * @see vDutCom_FreqInputsRequestAll()
 * @see uiDutCom_FreqInputsGetCount()
 */
MYDLL_API BOOL WINAPI bDutCom_FreqInputsReadAll(UINT *puiFreqInputValue)
{
  //printf("bDutCom_FreqInputsReadAll()\n");
  if(!tagComRecvData_m.tagFreqInputs.bReceivedAll)
    return(FALSE);

  if(puiFreqInputValue)
  {
    memcpy(puiFreqInputValue,
           tagComRecvData_m.tagFreqInputs.uiaFrequency,
           sizeof(UINT)*NUM_FREQ_INPUTS);
  }
  return(tagComRecvData_m.tagFreqInputs.bReceivedAll);
} // bDutCom_FreqInputsReadAll()

/**
 * Requests the read back of the frequency of specified input.
 *
 * @author uidc9013 (27.01.2012)
 * @param uiFreqInput
 *               number of input to read back (0 based)
 *
 * @see bDutCom_FreqInputsReadSingle()
 */
MYDLL_API void WINAPI vDutCom_FreqInputsRequestSingle(UINT uiFreqInput)
{
  //printf("vDutCom_FreqInputsRequestSingle()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqInputs.bRequest   =TRUE;
  tagComSendData_m.tagFreqInputs.uiFreqInput=uiFreqInput;
  tagComRecvData_m.tagFreqInputs.bReceived  =FALSE;

} // vDutCom_FreqInputsRequestSingle()

/**
 * Reads back the frequency of specified input.
 *
 * @author uidc9013 (27.01.2012)
 * @param puiFreqInputValue
 *               UINT pointer to variable to receive the frequency.
 *
 * @return TRUE on success.
 * @see vDutCom_FreqInputsRequestSingle()
 */
MYDLL_API BOOL WINAPI bDutCom_FreqInputsReadSingle(UINT *puiFreqInputValue)
{
  //printf("bDutCom_FreqInputsReadSingle()\n");
  if(!tagComRecvData_m.tagFreqInputs.bReceived)
    return(FALSE);

  if(puiFreqInputValue)
  {
    *puiFreqInputValue=tagComRecvData_m.tagFreqInputs.uiFrequency;
  }
  return(tagComRecvData_m.tagFreqInputs.bReceived);
} // bDutCom_FreqInputsReadSingle()


/**
 * set frequency outputs
 */
/**
 * Initializes the control for frequency outputs.
 *
 * @author uidc9013 (27.01.2012)
 * @see bDutCom_FreqOutputsInitialized()
 */
MYDLL_API void WINAPI vDutCom_FreqOutputsInit(void)
{
  //printf("vDutCom_FreqOutputsInit()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqOutputs.bInit       =TRUE;
  tagComRecvData_m.tagFreqOutputs.bInitialized=FALSE;
} // vDutCom_FreqOutputsInit()

/**
 * Returns whether the control for frequency outputs is initialized.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_FreqOutputsInit()
 */
MYDLL_API BOOL WINAPI bDutCom_FreqOutputsInitialized(void)
{
  //printf("bDutCom_FreqOutputsInitialized()\n");
  return(tagComRecvData_m.tagFreqOutputs.bInitialized);
} // bDutCom_FreqOutputsInitialized()

/**
 * Returns the number of frequency outputs.
 *
 * @author uidc9013 (27.01.2012)
 *
 * @return number of analog outputs.
 */
MYDLL_API UINT WINAPI uiDutCom_FreqOutputsGetCount(void)
{
  //printf("uiDutCom_FreqOutputsGetCount()\n");
  return(NUM_FREQ_OUTPUTS);
} // uiDutCom_FreqOutputsGetCount()

/**
 * Returns the descriptive text for the specified frequency output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of frequency output to return the text
 *
 * @return descriptive text for specified frequency output
 */
MYDLL_API char WINAPI *pcDutCom_FreqOutputsGetText(UINT uiOutput)
{
  return(pcDevSpec_FreqOutputsGetText_m(uiOutput));
} // pcDutCom_FreqOutputsGetText()

/**
 * Writes the values to all frequency outputs at once
 *
 * @author uidc9013 (27.01.2012)
 * @param puiFrequency
 *               pointer to UINT array containing the frequencies to be written.
 *               Number of items in this array = uiDutCom_FreqOutputsGetCount()
 * @param puiDutyCycle
 *               pointer to UINT array containing the duty-cycles to be written.
 *               Number of items in this array = uiDutCom_FreqOutputsGetCount()
 *
 * @return MYDLL_API void.
 */
MYDLL_API void WINAPI vDutCom_FreqOutputsWriteAll(UINT *puiFrequency,
                                                  UINT *puiDutyCycle)
{
  //printf("vDutCom_FreqOutputsWriteAll()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqOutputs.bWriteAll=TRUE;
  memcpy(tagComSendData_m.tagFreqOutputs.auiFrequency,
         puiFrequency,
         sizeof(UINT)*NUM_FREQ_OUTPUTS);
  memcpy(tagComSendData_m.tagFreqOutputs.auiDutyCycle,
         puiDutyCycle,
         sizeof(UINT)*NUM_FREQ_OUTPUTS);
  tagComRecvData_m.tagFreqOutputs.bAllWritten=FALSE;
} // vDutCom_FreqOutputsWriteAll()

/**
 * Returns whether the frequency outputs could be written
 *
 * @author uidc9013 (23.02.2012)
 *
 * @return TRUE on success.
 * @see vDutCom_FreqOutputsWriteAll()
 */
MYDLL_API BOOL WINAPI bDutCom_FreqOutputsAllWritten(void)
{
  //printf("bDutCom_FreqOutputsAllWritten()\n");
  return(tagComRecvData_m.tagFreqOutputs.bAllWritten);
} // bDutCom_FreqOutputsAllWritten()


/**
 * Writes the values to all analog outputs at once
 *
 * @author uidc9013 (27.01.2012)
 * @param uiFreqOutput
 *               number of frequency output to write (0 based).
 * @param uiFrequency
 *               frequency to set.
 * @param uiDutyCycle
 *               duty cycle to set.
 *
 * @see bDutCom_FreqOutputsSingleWritten()
 */
MYDLL_API void WINAPI vDutCom_FreqOutputsWriteSingle(UINT uiFreqOutput,
                                                     UINT uiFrequency,
                                                     UINT uiDutyCycle)
{
  //printf("vDutCom_FreqOutputsWriteSingle()\n");
  vResetComSendData_m();
  tagComSendData_m.tagFreqOutputs.bWrite      =TRUE;
  tagComSendData_m.tagFreqOutputs.uiFreqOutput=uiFreqOutput;
  tagComSendData_m.tagFreqOutputs.uiFrequency =uiFrequency;
  tagComSendData_m.tagFreqOutputs.uiDutyCycle =uiDutyCycle;
  tagComRecvData_m.tagFreqOutputs.bWritten    =FALSE;
} // vDutCom_FreqOutputsWriteSingle()

MYDLL_API BOOL WINAPI bDutCom_FreqOutputsSingleWritten(void)
{
  //printf("bDutCom_FreqOutputsSingleWritten()\n");
  return(tagComRecvData_m.tagFreqOutputs.bWritten);
} // bDutCom_FreqOutputsSingleWritten()



/**
 * Returns the number of device specific self running tests
 *
 * @author uidc9013 (25.10.2012)
 *
 * @return number of device specific self running tests.
 */
MYDLL_API UINT WINAPI uiDutCom_DeviceSpecificTestGetCount(void)
{
  return(NUM_DEV_SPEC_TESTS);
} // uiDutCom_DeviceSpecificTestGetCount()


/**
 * Returns the descriptive text for the specified device specific test
 *
 * @author uidc9013 (25.10.2012)
 * @param uiTestNumber number of device specific test to return the text
 *
 * @return descriptive text for specified device specific test
 */
MYDLL_API char WINAPI *pcDutCom_DeviceSpecificTestGetText(UINT uiTestNumber)
{
  return(pcDevSpec_DeviceSpecificTestGetText_m(uiTestNumber));
} // pcDutCom_DeviceSpecificTestGetText()


extern TagDigInp   tagaDigInp[DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX];
extern TagDigOutp  tagaDigOutp[DUT_COM_DEV_SPEC_TEST_DIG_OUTPUT_MAX];

/**
 * Assigns the necessary digital inputs for the next device specific test.
 *
 * @author WeU (12.12.2012)
 * @param uiTestNumber
 *                  number of device specific test. Only necessary if the description is needed.
 * @param uiInputNr Number of output (0-based)
 * @param pvTagDigInp
 *                  pointer to structure of type tagDigOutp as defined in "zol.h".
 * @param pcDescription
 *                  Pointer to char buffer to receive the desription of the digital input.
 *                  If this description is not needed the set this parameter to NULL,
 *
 * @return
 */
MYDLL_API void WINAPI vDutCom_DeviceSpecificTestAssignInput(UINT  uiTestNumber,
                                                            UINT  uiInputNr,
                                                            void *pvTagDigInp,
                                                            char *pcDescription)
{
  if(pvTagDigInp)
  {
    // Assign pvTagDigInp to local structure
    if(uiInputNr<DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX)
    {
      memcpy(&tagaDigInp[uiInputNr],pvTagDigInp,sizeof(TagDigInp));
    }
  }

  if(pcDescription)
  {
    if(pcDevSpec_DeviceSpecificTestInputGetText_m(uiTestNumber,uiInputNr))
    {
      strcpy(pcDescription,pcDevSpec_DeviceSpecificTestInputGetText_m(uiTestNumber,uiInputNr));
    }
    else
    {
      strcpy(pcDescription,"");
    }
  }
} // vDutCom_DeviceSpecificTestAssignInput()


/**
 * Assigns the necessary digital outputs for the next device specific test.
 *
 * @author WeU (12.12.2012)
 * @param uiTestNumber
 *                  number of device specific test. Only necessary if the description is needed.
 * @param uiOutputNr Number of output (0-based)
 * @param pvTagDigOutp
 *                   pointer to structure of type tagDigOutp as defined in "zol.h".
 * @param pcDescription
 *                  Pointer to char buffer to receive the desription of the digital input.
 *                  If the description is not needed then set this parameter to NULL.
 */
MYDLL_API void WINAPI vDutCom_DeviceSpecificTestAssignOutput(UINT  uiTestNumber,
                                                             UINT  uiOutputNr,
                                                             void *pvTagDigOutp,
                                                             char *pcDescription)
{
  if(pvTagDigOutp)
  {
    // Assign pvTagDigInp to local structure
    if(uiOutputNr<DUT_COM_DEV_SPEC_TEST_DIG_INPUT_MAX)
    {
      memcpy(&tagaDigOutp[uiOutputNr],pvTagDigOutp,sizeof(TagDigOutp));
    }
  }

  if(pcDescription)
  {
    if(pcDevSpec_DeviceSpecificTestOutputGetText_m(uiTestNumber,uiOutputNr))
    {
      strcpy(pcDescription,pcDevSpec_DeviceSpecificTestOutputGetText_m(uiTestNumber,uiOutputNr));
    }
    else
    {
      strcpy(pcDescription,"");
    }
  }
} // vDutCom_DeviceSpecificTestAssignOutput()


/**
 * Start a device specific self running test
 *
 * @author uidc9013 (25.10.2012)
 * @param uiTestNumber
 *               Number of test to run (0-based)
 * @see bDutCom_DeviceSpecificTestEnded()
 */
MYDLL_API void WINAPI vDutCom_DeviceSpecificTestStart(UINT uiTestNumber)
{
  //printf("vDutCom_DeviceSpecificTestStart()\n");
  vResetComSendData_m();
  tagComSendData_m.tagDevSpecTest.bStart      =TRUE;
  tagComSendData_m.tagDevSpecTest.uiTestNumber=uiTestNumber;
  tagComRecvData_m.tagDevSpecTest.bEnded      =FALSE;
} // vDutCom_DeviceSpecificTestStart()
/**
 * Returns whether the device specific test finished
 *
 * @author uidc9013 (25.10.2012)
 *
 * @return TRUE if finished.
 * @see vDutCom_DeviceSpecificTestStart()
 */
MYDLL_API BOOL WINAPI bDutCom_DeviceSpecificTestEnded(BOOL *pbResult)
{
  //printf("bDutCom_DeviceSpecificTestEnded()\n");
  *pbResult=tagComRecvData_m.tagDevSpecTest.bResult;
  return(tagComRecvData_m.tagDevSpecTest.bEnded);
} // bDutCom_DeviceSpecificTestEnded()

