/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   0000
 *
 *    FILE:        LibDutComImport.c
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


HINSTANCE hDllDutCom_m;

char caDutComImportErrorMessage_m[500];


/**
 * vDutCom_GetErrorText()
 *
 * returns Error message of last error occured
 *
 * @author uidc9013 (05.01.2012)
 * @param pcErrorMessage
 *               pointer to buffer to receive error message
 * @param uiBufferSize
 *               size [bytes] of buffer to receive error message
 */
void vDutCom_GetImportErrorText(char *pcErrorMessage,
                          unsigned int uiBufferSize)
{
  strncpy(pcErrorMessage,caDutComImportErrorMessage_m,uiBufferSize);
} // vDutCom_GetErrorText()



#define INIT_DUT_COM_FUNCS(f) *(void**)&f=(void*)GetProcAddress(hDllDutCom_m,#f); \
                                if(!f) \
                                { \
                                  printf("   %s()\n",#f); \
                                  ++iMissingFunctions; \
                                }

#define INIT_DUT_COM_FUNCS_OPT(f) *(void**)&f=(void*)GetProcAddress(hDllDutCom_m,#f); \
                                    if(!f) \
                                    { \
                                      f=NULL; \
                                    }


/**
 * Loads specified LibDutCom_NNNN and initializes all function pointers and start commuiation with DUT.<br>
 * To start communication with DUT either set parameter bStartCommunication=TRUE or call vDutCom_CommunicationStart()<br>
 * afterwards.
 * Call vDutCom_CommunicationStop() to stop the communication.<br>
 * Unloading LibDutCom_NNNN stops communication automatically or stop it manually by calling vDutCom_Unload().
 *
 * @author uidc9013 (21.12.2011)
 * @param uiLibNr Number of LibDutCom to load (Range: 1...9999)
 * @param bStartCommunication
 *                Start Communication after load?
 *
 * @return TRUE if dll loaded successfully<br>
 *         FALSE on error
 * @see vDutCom_CommunicationStart()
 * @see vDutCom_CommunicationStop()
 * @see bDutCom_CommunicationStarted()
 * @see bDutCom_CommunicationStopped()
 */
BOOL bDutCom_Load(unsigned int  uiLibNr,
                  BOOL          bStartCommunication)
{
  char caLibName1[200];
  char caLibName2[200];
  char caLibNameLoaded[200];

  int iMissingFunctions;

  hDllDutCom_m=NULL;

#ifdef _CVI_DEBUG_
  // try debug dll first
  sprintf(caLibName1,"LibDutCom_%04u_dbg.dll",uiLibNr);
  sprintf(caLibName2,"LibDutCom_%04u.dll",uiLibNr);
#else // !_CVI_DEBUG_
  // try release dll first
  sprintf(caLibName1,"LibDutCom_%04u.dll",uiLibNr);
  sprintf(caLibName2,"LibDutCom_%04u_dbg.dll",uiLibNr);
#endif // _CVI_DEBUG_

  hDllDutCom_m=LoadLibrary(caLibName1);
  if(!hDllDutCom_m)
  {
    hDllDutCom_m=LoadLibrary(caLibName2);
    if(!hDllDutCom_m)
    {
      unsigned long ulErrorCode;

      ulErrorCode=GetLastError();
      switch(ulErrorCode)
      {
        case ERROR_MOD_NOT_FOUND:
          sprintf(caDutComImportErrorMessage_m,"File \"%s\" not found",caLibName1);
          break;
        default:
          FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        ulErrorCode,
                        MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
                        (LPTSTR)caDutComImportErrorMessage_m,
                        500,
                        NULL);
      }
      return(FALSE);
    }
    else
    {
      strcpy(caLibNameLoaded,caLibName2);
    }
  }
  else
  {
    strcpy(caLibNameLoaded,caLibName1);
  }

  iMissingFunctions=0;

  /**
   * initialize all function pointers to the exported functions
   */
  INIT_DUT_COM_FUNCS(bDutCom_SetParam);
  INIT_DUT_COM_FUNCS(vDutCom_GetInfo);
  INIT_DUT_COM_FUNCS(iDutCom_GetOneTraceLine);
  INIT_DUT_COM_FUNCS(pcDutCom_GetTraceHeader);
  INIT_DUT_COM_FUNCS(vDutCom_TraceEnable);
  INIT_DUT_COM_FUNCS(vDutCom_TraceClear);
  INIT_DUT_COM_FUNCS(bDutCom_GetErrorMessage);

  INIT_DUT_COM_FUNCS(vDutCom_CommunicationStart);
  INIT_DUT_COM_FUNCS(vDutCom_CommunicationStop);
  INIT_DUT_COM_FUNCS(bDutCom_CommunicationStarted);
  INIT_DUT_COM_FUNCS(bDutCom_CommunicationStopped);

  INIT_DUT_COM_FUNCS(vDutCom_DiagModeStart);
  INIT_DUT_COM_FUNCS(vDutCom_DiagModeStop);
  INIT_DUT_COM_FUNCS(bDutCom_DiagModeStarted);
  INIT_DUT_COM_FUNCS(bDutCom_DiagModeStopped);

  INIT_DUT_COM_FUNCS_OPT(vDutCom_DiagModeSet);
  INIT_DUT_COM_FUNCS_OPT(bDutCom_DiagModeIsSet);

  INIT_DUT_COM_FUNCS(vDutCom_SecurityAccess);
  INIT_DUT_COM_FUNCS(eDutCom_SecurityGetCurrentAccess);

  INIT_DUT_COM_FUNCS(vDutCom_Reset);

  INIT_DUT_COM_FUNCS(vDutCom_ClockReset);
  INIT_DUT_COM_FUNCS(vDutCom_ClockSetSeconds);
  INIT_DUT_COM_FUNCS(vDutCom_ClockSetMinutes);
  INIT_DUT_COM_FUNCS(vDutCom_ClockSetHours);
  INIT_DUT_COM_FUNCS_OPT(vDutCom_ClockSetTime);
  INIT_DUT_COM_FUNCS_OPT(bDutCom_ClockSetTimeDone);
  INIT_DUT_COM_FUNCS(vDutCom_ClockRequestTime);
  INIT_DUT_COM_FUNCS(uiDutCom_ClockGetSeconds);
  INIT_DUT_COM_FUNCS(uiDutCom_ClockGetMinutes);
  INIT_DUT_COM_FUNCS(uiDutCom_ClockGetHours);
  INIT_DUT_COM_FUNCS_OPT(bDutCom_ClockGetTime);

  INIT_DUT_COM_FUNCS(vDutCom_DataEepromByteWrite);
  INIT_DUT_COM_FUNCS(bDutCom_DataEepromByteWritten);

  INIT_DUT_COM_FUNCS(vDutCom_DataEepromByteRequest);
  INIT_DUT_COM_FUNCS(bDutCom_DataEepromByteRead);




  INIT_DUT_COM_FUNCS(vDutCom_DataEepromBulkWrite);
  INIT_DUT_COM_FUNCS(bDutCom_DataEepromBulkWritten);

  INIT_DUT_COM_FUNCS(vDutCom_DataEepromBulkRequest);
  INIT_DUT_COM_FUNCS(bDutCom_DataEepromBulkRead);


  INIT_DUT_COM_FUNCS(vDutCom_DataSerialNumberWrite);
  INIT_DUT_COM_FUNCS(bDutCom_DataSerialNumberWritten);

  INIT_DUT_COM_FUNCS(vDutCom_DataSerialNumberRequest);
  INIT_DUT_COM_FUNCS(bDutCom_DataSerialNumberRead);


  INIT_DUT_COM_FUNCS(vDutCom_DataStringWrite);
  INIT_DUT_COM_FUNCS(bDutCom_DataStringWritten);

  INIT_DUT_COM_FUNCS(vDutCom_DataStringRequest);
  INIT_DUT_COM_FUNCS(bDutCom_DataStringRead);
  INIT_DUT_COM_FUNCS(vDutCom_DataStringGetDataInfo);


  INIT_DUT_COM_FUNCS(uiDutCom_DataProductionInterlockGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_DataProductionInterlockGetText);

  INIT_DUT_COM_FUNCS(vDutCom_DataProductionInterlockWrite);
  INIT_DUT_COM_FUNCS(bDutCom_DataProductionInterlockWritten);

  INIT_DUT_COM_FUNCS(vDutCom_DataProductionInterlockRequest);
  INIT_DUT_COM_FUNCS(bDutCom_DataProductionInterlockRead);


  INIT_DUT_COM_FUNCS(uiDutCom_PointerGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_PointerGetText);

  INIT_DUT_COM_FUNCS(vDutCom_PointerAdjustInit);
  INIT_DUT_COM_FUNCS(bDutCom_PointerAdjustInitialized);

  INIT_DUT_COM_FUNCS(vDutCom_PointerAdjustSetOffset);
  INIT_DUT_COM_FUNCS(bDutCom_PointerAdjustOffsetIsSet);

  INIT_DUT_COM_FUNCS_OPT(vDutCom_PointerAdjustSetGain);
  INIT_DUT_COM_FUNCS_OPT(bDutCom_PointerAdjustGainIsSet);

  INIT_DUT_COM_FUNCS(vDutCom_PointerAdjustStoreOffset);
  INIT_DUT_COM_FUNCS(bDutCom_PointerAdjustOffsetIsStored);


  INIT_DUT_COM_FUNCS(vDutCom_PointerControlInit);
  INIT_DUT_COM_FUNCS(bDutCom_PointerControlInitialized);

  INIT_DUT_COM_FUNCS(vDutCom_PointerControlSetPositionAll);
  INIT_DUT_COM_FUNCS(bDutCom_PointerControlPositionAllIsSet);

  INIT_DUT_COM_FUNCS(vDutCom_PointerControlSetPositionSingle);
  INIT_DUT_COM_FUNCS(bDutCom_PointerControlPositionSingleIsSet);


  INIT_DUT_COM_FUNCS(vDutCom_TellTaleCtrlInit);
  INIT_DUT_COM_FUNCS(bDutCom_TellTaleCtrlInitialized);

  INIT_DUT_COM_FUNCS(uiDutCom_TellTaleCtrlGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_TellTaleGetText);

  INIT_DUT_COM_FUNCS(vDutCom_TellTaleCtrlSetBitmap);
  INIT_DUT_COM_FUNCS(bDutCom_TellTaleCtrlBitmapIsSet);

  INIT_DUT_COM_FUNCS(vDutCom_TellTaleCtrlSetTellTale);
  INIT_DUT_COM_FUNCS(bDutCom_TellTaleCtrlTellTaleIsSet);


  INIT_DUT_COM_FUNCS(vDutCom_BackLightCtrlInit);
  INIT_DUT_COM_FUNCS(bDutCom_BackLightCtrlInitialized);

  INIT_DUT_COM_FUNCS(uiDutCom_BackLightCtrlGetCount);
  INIT_DUT_COM_FUNCS(uiDutCom_BackLightCtrlGetColorCount);
  INIT_DUT_COM_FUNCS(pcDutCom_BacklightGetText);

  INIT_DUT_COM_FUNCS(vDutCom_BackLightCtrlSet);
  INIT_DUT_COM_FUNCS(bDutCom_BackLightCtrlIsSet);


  INIT_DUT_COM_FUNCS(vDutCom_LcdCtrlInit);
  INIT_DUT_COM_FUNCS(bDutCom_LcdCtrlInitialized);

  INIT_DUT_COM_FUNCS(uiDutCom_LcdCtrlGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_LcdCtrlGetText);
  INIT_DUT_COM_FUNCS(uiDutCom_LcdCtrlPatternGetCount);

  INIT_DUT_COM_FUNCS(vDutCom_LcdCtrlSetPattern);
  INIT_DUT_COM_FUNCS(bDutCom_LcdCtrlPatternIsSet);


  INIT_DUT_COM_FUNCS(vDutCom_DigInputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_DigInputsInitialized);

  INIT_DUT_COM_FUNCS(uiDutCom_DigInputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_DigInputsGetText);
  INIT_DUT_COM_FUNCS(vDutCom_DigInputsRequestAll);
  INIT_DUT_COM_FUNCS(bDutCom_DigInputsReadAll);
  INIT_DUT_COM_FUNCS(vDutCom_DigInputsRequestSingle);
  INIT_DUT_COM_FUNCS(bDutCom_DigInputsReadSingle);


  INIT_DUT_COM_FUNCS(vDutCom_DigOutputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_DigOutputsInitialized);

  INIT_DUT_COM_FUNCS(uiDutCom_DigOutputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_DigOutputsGetText);
  INIT_DUT_COM_FUNCS(vDutCom_DigOutputsWriteAll);
  INIT_DUT_COM_FUNCS(bDutCom_DigOutputsAllWritten);
  INIT_DUT_COM_FUNCS(vDutCom_DigOutputsWriteSingle);
  INIT_DUT_COM_FUNCS(bDutCom_DigOutputsSingleWritten);


  INIT_DUT_COM_FUNCS(vDutCom_AnalogInputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogInputsInitialized);
  INIT_DUT_COM_FUNCS(uiDutCom_AnalogInputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_AnalogInputsGetText);
  INIT_DUT_COM_FUNCS(vDutCom_AnalogInputsRequestAll);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogInputsReadAll);
  INIT_DUT_COM_FUNCS(vDutCom_AnalogInputsRequestSingle);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogInputsReadSingle);

  INIT_DUT_COM_FUNCS(vDutCom_AnalogOutputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogOutputsInitialized);
  INIT_DUT_COM_FUNCS(uiDutCom_AnalogOutputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_AnalogOutputsGetText);
  INIT_DUT_COM_FUNCS(vDutCom_AnalogOutputsWriteAll);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogOutputsAllWritten);
  INIT_DUT_COM_FUNCS(vDutCom_AnalogOutputsWriteSingle);
  INIT_DUT_COM_FUNCS(bDutCom_AnalogOutputsSingleWritten);

  INIT_DUT_COM_FUNCS(vDutCom_FreqInputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_FreqInputsInitialized);
  INIT_DUT_COM_FUNCS(uiDutCom_FreqInputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_FreqInputsGetText);

  INIT_DUT_COM_FUNCS(vDutCom_FreqInputsRequestAll);
  INIT_DUT_COM_FUNCS(bDutCom_FreqInputsReadAll);
  INIT_DUT_COM_FUNCS(vDutCom_FreqInputsRequestSingle);
  INIT_DUT_COM_FUNCS(bDutCom_FreqInputsReadSingle);

  INIT_DUT_COM_FUNCS(vDutCom_FreqOutputsInit);
  INIT_DUT_COM_FUNCS(bDutCom_FreqOutputsInitialized);
  INIT_DUT_COM_FUNCS(uiDutCom_FreqOutputsGetCount);
  INIT_DUT_COM_FUNCS(pcDutCom_FreqOutputsGetText);
  INIT_DUT_COM_FUNCS(vDutCom_FreqOutputsWriteAll);
  INIT_DUT_COM_FUNCS(bDutCom_FreqOutputsAllWritten);
  INIT_DUT_COM_FUNCS(vDutCom_FreqOutputsWriteSingle);
  INIT_DUT_COM_FUNCS(bDutCom_FreqOutputsSingleWritten);

  INIT_DUT_COM_FUNCS_OPT(uiDutCom_DeviceSpecificTestGetCount);
  INIT_DUT_COM_FUNCS_OPT(pcDutCom_DeviceSpecificTestGetText);

  INIT_DUT_COM_FUNCS_OPT(vDutCom_DeviceSpecificTestAssignInput);
  INIT_DUT_COM_FUNCS_OPT(vDutCom_DeviceSpecificTestAssignOutput);
  INIT_DUT_COM_FUNCS_OPT(vDutCom_DeviceSpecificTestStart);
  INIT_DUT_COM_FUNCS_OPT(bDutCom_DeviceSpecificTestEnded);


  if(iMissingFunctions>0)
  {
    if(iMissingFunctions>1)
      printf("%d functions missing in dll %s\n\n",iMissingFunctions,caLibNameLoaded);
    else
      printf("%d function missing in dll %s\n\n",iMissingFunctions,caLibNameLoaded);
    return(FALSE);
  }

  if(bStartCommunication)
  {
    vDutCom_CommunicationStart();
  }

  return(TRUE);
} // bDutCom_Load()


/**
 * Unloads currently loaded LibDutCom_XXXX and releases all function pointers
 *
 * @author uidc9013 (21.12.2011)
 */
void vDutCom_Unload(void)
{

  if(hDllDutCom_m)
  {
    if(bDutCom_CommunicationStarted)
    {
      if(bDutCom_CommunicationStarted())
      {
        vDutCom_CommunicationStop();
        // wait unitl communication thread has stopped
        while(!bDutCom_CommunicationStopped())
        {
          Sleep(10);
        }
      }
    }

    FreeLibrary(hDllDutCom_m);
  }
  hDllDutCom_m=NULL;


  /**
   * de-initialize all function pointers to the exported functions
   */
  bDutCom_SetParam                       =NULL;
  vDutCom_GetInfo                        =NULL;
  iDutCom_GetOneTraceLine                =NULL;
  pcDutCom_GetTraceHeader                =NULL;
  vDutCom_TraceEnable                    =NULL;
  vDutCom_TraceClear                     =NULL;
  bDutCom_GetErrorMessage                =NULL;


  vDutCom_CommunicationStart             =NULL;
  vDutCom_CommunicationStop              =NULL;
  bDutCom_CommunicationStarted           =NULL;
  bDutCom_CommunicationStopped           =NULL;

  vDutCom_DiagModeStart                  =NULL;
  vDutCom_DiagModeStop                   =NULL;
  bDutCom_DiagModeStarted                =NULL;
  bDutCom_DiagModeStopped                =NULL;
  vDutCom_DiagModeSet                    =NULL;
  bDutCom_DiagModeIsSet                  =NULL;

  vDutCom_SecurityAccess                 =NULL;
  eDutCom_SecurityGetCurrentAccess       =NULL;

  vDutCom_Reset                          =NULL;

  vDutCom_ClockReset                     =NULL;
  vDutCom_ClockSetSeconds                =NULL;
  vDutCom_ClockSetMinutes                =NULL;
  vDutCom_ClockSetHours                  =NULL;
  vDutCom_ClockSetTime                   =NULL;
  bDutCom_ClockSetTimeDone               =NULL;
  vDutCom_ClockRequestTime               =NULL;
  uiDutCom_ClockGetSeconds               =NULL;
  uiDutCom_ClockGetMinutes               =NULL;
  uiDutCom_ClockGetHours                 =NULL;
  bDutCom_ClockGetTime                   =NULL;

  vDutCom_DataEepromByteWrite            =NULL;
  bDutCom_DataEepromByteWritten          =NULL;

  vDutCom_DataEepromByteRequest          =NULL;
  bDutCom_DataEepromByteRead             =NULL;


  vDutCom_DataEepromBulkWrite            =NULL;
  bDutCom_DataEepromBulkWritten          =NULL;

  vDutCom_DataEepromBulkRequest          =NULL;
  bDutCom_DataEepromBulkRead             =NULL;


  vDutCom_DataSerialNumberWrite          =NULL;
  bDutCom_DataSerialNumberWritten        =NULL;

  vDutCom_DataSerialNumberRequest        =NULL;
  bDutCom_DataSerialNumberRead           =NULL;


  vDutCom_DataStringWrite                =NULL;
  bDutCom_DataStringWritten              =NULL;

  vDutCom_DataStringRequest              =NULL;
  bDutCom_DataStringRead                 =NULL;
  vDutCom_DataStringGetDataInfo          =NULL;


  uiDutCom_DataProductionInterlockGetCount=NULL;
  pcDutCom_DataProductionInterlockGetText=NULL;

  vDutCom_DataProductionInterlockWrite   =NULL;
  bDutCom_DataProductionInterlockWritten =NULL;

  vDutCom_DataProductionInterlockRequest =NULL;
  bDutCom_DataProductionInterlockRead    =NULL;


  uiDutCom_PointerGetCount               =NULL;
  pcDutCom_PointerGetText                =NULL;

  vDutCom_PointerAdjustInit              =NULL;
  bDutCom_PointerAdjustInitialized       =NULL;

  vDutCom_PointerAdjustSetOffset         =NULL;
  bDutCom_PointerAdjustOffsetIsSet       =NULL;

  vDutCom_PointerAdjustSetGain           =NULL;
  bDutCom_PointerAdjustGainIsSet         =NULL;

  vDutCom_PointerAdjustStoreOffset       =NULL;
  bDutCom_PointerAdjustOffsetIsStored    =NULL;


  vDutCom_PointerControlInit             =NULL;
  bDutCom_PointerControlInitialized      =NULL;

  vDutCom_PointerControlSetPositionAll   =NULL;
  bDutCom_PointerControlPositionAllIsSet =NULL;

  vDutCom_PointerControlSetPositionSingle =NULL;
  bDutCom_PointerControlPositionSingleIsSet =NULL;


  vDutCom_TellTaleCtrlInit               =NULL;
  bDutCom_TellTaleCtrlInitialized        =NULL;

  uiDutCom_TellTaleCtrlGetCount          =NULL;
  pcDutCom_TellTaleGetText               =NULL;

  vDutCom_TellTaleCtrlSetBitmap          =NULL;
  bDutCom_TellTaleCtrlBitmapIsSet        =NULL;

  vDutCom_TellTaleCtrlSetTellTale        =NULL;
  bDutCom_TellTaleCtrlTellTaleIsSet      =NULL;


  vDutCom_BackLightCtrlInit              =NULL;
  bDutCom_BackLightCtrlInitialized       =NULL;

  uiDutCom_BackLightCtrlGetCount         =NULL;
  uiDutCom_BackLightCtrlGetColorCount    =NULL;
  pcDutCom_BacklightGetText              =NULL;

  vDutCom_BackLightCtrlSet               =NULL;
  bDutCom_BackLightCtrlIsSet             =NULL;


  vDutCom_LcdCtrlInit                    =NULL;
  bDutCom_LcdCtrlInitialized             =NULL;

  uiDutCom_LcdCtrlGetCount               =NULL;
  pcDutCom_LcdCtrlGetText                =NULL;
  uiDutCom_LcdCtrlPatternGetCount        =NULL;

  vDutCom_LcdCtrlSetPattern              =NULL;
  bDutCom_LcdCtrlPatternIsSet            =NULL;


  vDutCom_DigInputsInit                  =NULL;
  bDutCom_DigInputsInitialized           =NULL;
  uiDutCom_DigInputsGetCount             =NULL;
  pcDutCom_DigInputsGetText              =NULL;
  vDutCom_DigInputsRequestAll            =NULL;
  bDutCom_DigInputsReadAll               =NULL;
  vDutCom_DigInputsRequestSingle         =NULL;
  bDutCom_DigInputsReadSingle            =NULL;


  vDutCom_DigOutputsInit                 =NULL;
  bDutCom_DigOutputsInitialized          =NULL;
  uiDutCom_DigOutputsGetCount            =NULL;
  pcDutCom_DigOutputsGetText             =NULL;
  vDutCom_DigOutputsWriteAll             =NULL;
  bDutCom_DigOutputsAllWritten           =NULL;
  vDutCom_DigOutputsWriteSingle          =NULL;
  bDutCom_DigOutputsSingleWritten        =NULL;


  vDutCom_AnalogInputsInit               =NULL;
  bDutCom_AnalogInputsInitialized        =NULL;
  uiDutCom_AnalogInputsGetCount          =NULL;
  pcDutCom_AnalogInputsGetText           =NULL;
  vDutCom_AnalogInputsRequestAll         =NULL;
  bDutCom_AnalogInputsReadAll            =NULL;
  vDutCom_AnalogInputsRequestSingle      =NULL;
  bDutCom_AnalogInputsReadSingle         =NULL;

  vDutCom_AnalogOutputsInit              =NULL;
  bDutCom_AnalogOutputsInitialized       =NULL;
  uiDutCom_AnalogOutputsGetCount         =NULL;
  pcDutCom_AnalogOutputsGetText          =NULL;
  vDutCom_AnalogOutputsWriteAll          =NULL;
  bDutCom_AnalogOutputsAllWritten        =NULL;
  vDutCom_AnalogOutputsWriteSingle       =NULL;
  bDutCom_AnalogOutputsSingleWritten     =NULL;

  vDutCom_FreqInputsInit                 =NULL;
  bDutCom_FreqInputsInitialized          =NULL;
  uiDutCom_FreqInputsGetCount            =NULL;
  pcDutCom_FreqInputsGetText             =NULL;
  vDutCom_FreqInputsRequestAll           =NULL;
  bDutCom_FreqInputsReadAll              =NULL;
  vDutCom_FreqInputsRequestSingle        =NULL;
  bDutCom_FreqInputsReadSingle           =NULL;

  vDutCom_FreqOutputsInit                =NULL;
  bDutCom_FreqOutputsInitialized         =NULL;
  uiDutCom_FreqOutputsGetCount           =NULL;
  pcDutCom_FreqOutputsGetText            =NULL;
  vDutCom_FreqOutputsWriteAll            =NULL;
  bDutCom_FreqOutputsAllWritten          =NULL;
  vDutCom_FreqOutputsWriteSingle         =NULL;
  bDutCom_FreqOutputsSingleWritten       =NULL;

  uiDutCom_DeviceSpecificTestGetCount    =NULL;
  pcDutCom_DeviceSpecificTestGetText     =NULL;
  vDutCom_DeviceSpecificTestAssignInput  =NULL;
  vDutCom_DeviceSpecificTestAssignOutput =NULL;
  vDutCom_DeviceSpecificTestStart        =NULL;
  bDutCom_DeviceSpecificTestEnded        =NULL;

} // vDutCom_Unload()


/**
 * iDutCom_GetTraceLine()
 *
 * @author uidc9013 (16.01.2012)
 * @param pcTraceLine
 *               pointer to buffer to receive the trace line
 *
 * @return 1 on success<br>
 *         0 on empty trace
 */
int iDutCom_GetTraceLine(char *pcTraceLine,
                         int *piLinesRemaining)
{
  if(iDutCom_GetOneTraceLine)
    return(iDutCom_GetOneTraceLine(pcTraceLine,piLinesRemaining));
  else
    return(0);
} // iDutCom_GetTraceLine()

