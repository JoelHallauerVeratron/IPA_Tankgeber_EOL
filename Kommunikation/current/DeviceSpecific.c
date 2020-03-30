/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   .
 *
 *    FILE:        DeviceSpecific.c
 *
 *    DESCRIPTION: implementation of private functions to gather device
 *                 specific settings and values
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

#include "DeviceSpecific.h"
#include "LibDutCom_Interface.h"
#include "UDS_private.h"

/**
 * Returns the address for the specified data
 *
 * @author uidc9013 (20.01.2012)
 * @param eDataType
 *               type of data
 *
 * @return address for data or 0 if specified<br>
 *         data type is not supported
 */
UINT uiDevSpec_GetDataAddress_m(EDataType eDataType)
{
  UINT uiAddress=0x0000;

  switch(eDataType)
  {
    case eProdNrUnpacked:    uiAddress=DID_A2C_NR_UNPACKED;            break;
    //case eProdNrPacked:      uiAddress=DID_ECU_HARDWARE_NUMBER;        break;
    case eProductCode:       uiAddress=DID_NMEA_PROD_CODE;             break;
    //case eProdNrPcb:         uiAddress=EE_ECU_HARDWARE_NUMBER;         break;
    //case eProgDatePcb:       uiAddress=DID_ECU_PROGRAMMING_DATE;       break;
    //case eVersionHardware:   uiAddress=DID_ECU_HARDWARE_VERSION;       break;
    case eVersionSoftware:   uiAddress=DID_SOFTWARE_VERSION;           break;
    //case eDigInputConfig:    uiAddress=EE_DIGITAL_INPUT_TYPE;          break;
    //case eFreqOutputConfig:  uiAddress=EE_OUTPUT_SINK_TYPE;            break;
    //case eDialScaleX:        uiAddress=DID_DIAL_SCALE_VALUES;          break;
    //case eDialScaleY:        uiAddress=DID_DIAL_SCALE_VALUES;          break;
    //case eGaugeType:         uiAddress=DID_GAUGE_TYPE;          break;
    default:
#ifdef WARN_NOT_IMPLEMENTED_FUNCTIONS
      printf("uiDevSpec_GetDataAddress_m(): data type %d not supported\n",eDataType);
#endif // WARN_NOT_IMPLEMENTED_FUNCTIONS
      uiAddress=0x0000;
  }
  return(uiAddress);
} // uiDevSpec_GetDataAddress_m()


/**
 * Returns the size in bytes for the specified data
 * Size is meant how it is represented in the DUT.
 * Example:
 *   DataFormat=Ascii, DataSize=10 => 11 bytes will be expected (10 bytes + 1byte term.)
 *  or
 *   DataFormat=Bcd, DataSize=2 => 5 bytes will be expected (4 bytes + 1byte term.)
 *   0000\0 => num. Value 0
 *   0120\0 => num. Value 120
 *   9999\0 => num. Value 9999
 *
 * @author uidc9013 (28.02.2012)
 * @param eDataType
 *               type of data
 *
 * @return size of data or 0 if specified<br>
 *         data type is not supported
 */
UINT uiDevSpec_GetDataSize_m(EDataType eDataType)
{
  UINT uiDataSize=0x0000;

  switch(eDataType)
  {
    case eProdNrUnpacked:   uiDataSize=11; break;
    //case eProdNrPacked:     uiDataSize=DID_ECU_HARDWARE_NUMBER_LEN;     break;
    case eProductCode:      uiDataSize=2;  break;
    //case eProdNrPcb:        uiDataSize=EE_ECU_HARDWARE_NUMBER_LEN;      break;
    //case eProgDatePcb:      uiDataSize=DID_ECU_PROGRAMMING_DATE_LEN;    break;
    //case eVersionHardware:  uiDataSize=DID_ECU_HARDWARE_VERSION_LEN;    break;
    case eVersionSoftware:  uiDataSize=DID_SOFTWARE_VERSION_LEN; break;
    //case eDigInputConfig:   uiDataSize=EE_DIGITAL_INPUT_TYPE_LEN;       break;
    //case eFreqOutputConfig: uiDataSize=EE_OUTPUT_SINK_TYPE_LEN;         break;
    //case eDialScaleX:        uiDataSize=DID_DIAL_SCALE_VALUES_LEN/2; break;
    //case eDialScaleY:        uiDataSize=DID_DIAL_SCALE_VALUES_LEN/2; break;
    //case eGaugeType:         uiDataSize=DID_GAUGE_TYPE_LEN;          break;
    default:
#ifdef WARN_NOT_IMPLEMENTED_FUNCTIONS
      printf("uiDevSpec_GetDataSize_m(): Data type %d not supported\n",eDataType);
#endif // WARN_NOT_IMPLEMENTED_FUNCTIONS
      uiDataSize=0;
  }
  //printf("uiDataSize=0x%02x\n",uiDataSize);
  return(uiDataSize);
} // uiDevSpec_GetDataSize_m()


/**
 * Returns the format for the specified data
 *
 * @author uidc9013 (28.02.2012)
 * @param eDataType
 *               type of data
 *
 * @return data format or eDataFormat_Unspecified if specified<br>
 *         data type is not supported
 */
EDataFormat eDevSpec_GetDataFormat_m(EDataType eDataType)
{
  EDataFormat eDataFormat=eDataFormat_Unspecified;

  switch(eDataType)
  {
    case eProdNrUnpacked:   eDataFormat=eDataFormat_Ascii;  break;
    //case eProdNrPacked:     eDataFormat=eDataFormat_Ascii;  break;
    case eProductCode:      eDataFormat=eDataFormat_Binary;  break;
    //case eProdNrPcb:        eDataFormat=eDataFormat_Ascii;  break;
    //case eProgDatePcb:      eDataFormat=eDataFormat_Bcd;    break;
    //case eVersionHardware:  eDataFormat=eDataFormat_Ascii;  break;
    case eVersionSoftware:  eDataFormat=eDataFormat_Ascii; break;
    //case eDigInputConfig:   eDataFormat=eDataFormat_Binary; break;
    //case eFreqOutputConfig: eDataFormat=eDataFormat_Binary; break;
    //case eDialScaleX:       eDataFormat=eDataFormat_Binary;  break;
    //case eDialScaleY:       eDataFormat=eDataFormat_Binary;  break;
    //case eGaugeType:        eDataFormat=eDataFormat_Binary;  break;
    default:
#ifdef WARN_NOT_IMPLEMENTED_FUNCTIONS
      printf("eDevSpec_GetDataFormat_m(): data type %d not supported\n",eDataType);
#endif // WARN_NOT_IMPLEMENTED_FUNCTIONS
      eDataFormat=eDataFormat_Unspecified;
  }
  return(eDataFormat);
} // eDevSpec_GetDataFormat_m()


/**
 * Returns the descriptive text for the specified tell-tale
 *
 * @author uidc9013 (01.02.2012)
 * @param uiTellTale number of tell-tale to return the text
 *
 * @return descriptive text for specified tell-tale
 */
char *pcDevSpec_TellTaleGetText_m(UINT uiTellTale)
{
  char *pcTellTaleName;

  switch(uiTellTale)
  {
    //case  0: pcTellTaleName="TellTale 1";  break;
    //case  1: pcTellTaleName="TellTale 2";  break;
    //case  2: pcTellTaleName="TellTale 3";  break;
    //case  3: pcTellTaleName="TellTale 4";  break;
    //case  4: pcTellTaleName="TellTale 5";  break;
    //case  5: pcTellTaleName="TellTale 6";  break;
    //case  6: pcTellTaleName="TellTale 6";  break;
    //case  7: pcTellTaleName="TellTale 7";  break;
    //case  8: pcTellTaleName="TellTale 8";  break;
    //case  9: pcTellTaleName="TellTale 9";  break;
    //case 10: pcTellTaleName="TellTale 10"; break;
    //case 11: pcTellTaleName="TellTale 11"; break;
    //case 12: pcTellTaleName="TellTale 12"; break;
    //case 13: pcTellTaleName="TellTale 13"; break;
    //case 14: pcTellTaleName="TellTale 14"; break;
    //case 15: pcTellTaleName="TellTale 15"; break;
    //case 16: pcTellTaleName="TellTale 16"; break;
    //case 17: pcTellTaleName="TellTale 17"; break;
    //case 18: pcTellTaleName="TellTale 18"; break;
    //case 19: pcTellTaleName="TellTale 19"; break;
    //case 20: pcTellTaleName="TellTale 20"; break;
    //case 21: pcTellTaleName="TellTale 21"; break;
    //case 22: pcTellTaleName="TellTale 22"; break;
    //case 23: pcTellTaleName="TellTale 23"; break;
    //case 24: pcTellTaleName="TellTale 24"; break;
    //case 25: pcTellTaleName="TellTale 25"; break;
    default: pcTellTaleName="TellTale ???";
  }
  return(pcTellTaleName);
} // pcDevSpec_TellTaleGetText_m()


/**
 * Returns the descriptive text for the specified lcd
 *
 * @author uidc9013 (01.02.2012)
 * @param uiLcd number of lcd to return the text
 *
 * @return descriptive text for specified lcd
 */
char *pcDevSpec_LcdGetText_m(UINT uiLcd)
{
  char *pcLcdName;
  switch(uiLcd)
  {
    case 0: pcLcdName="LCD"; break;
    default: pcLcdName="LCD ???";
  }
  return(pcLcdName);
} // pcDevSpec_LcdGetText_m()


/**
 * Returns the descriptive text for the specified pointer
 *
 * @author uidc9013 (01.02.2012)
 * @param uiTellTale number of tell-tale to return the text
 *
 * @return descriptive text for specified pointer
 */
char *pcDevSpec_PointerGetText_m(UINT uiPointer)
{
  char *pcPointerName;
  switch(uiPointer)
  {
    case 0: pcPointerName="Pointer DUT"; break;
    case 1: pcPointerName="Pointer EasyLink"; break;
    //case 2: pcPointerName="Pointer 360°"; break;
    //case 3: pcPointerName="Pointer Temp"; break;
    //case 4: pcPointerName="Pointer 4"; break;   // @@@
    //case 5: pcPointerName="Pointer 5"; break;   // @@@
    default: pcPointerName="Pointer ???";
  }
  return(pcPointerName);
} // pcDevSpec_PointerGetText_m()


/**
 * Returns the descriptive text for the specified backlight
 *
 * @author uidc9013 (08.02.2012)
 * @param uiBacklight number of backlight to return the text
 *
 * @return descriptive text for specified backlight
 */
char *pcDevSpec_BacklightGetText_m(UINT uiBacklight)
{
  char *pcBacklightName;
  switch(uiBacklight)
  {
    case 0: pcBacklightName="Pointer"; break;
    case 1: pcBacklightName="Dial"; break;
    case 2: pcBacklightName="LCD"; break;
    case 3: pcBacklightName="EasyLink"; break;
    default: pcBacklightName="Illumination ???";
  }
  return(pcBacklightName);
} // pcDevSpec_BacklightGetText_m()


/**
 * Returns the descriptive text for the specified digital input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of digital input to return the text
 *
 * @return descriptive text for specified digital input
 */
char *pcDevSpec_DigInputsGetText_m(UINT uiInput)
{
  char *pcInputName;
  switch(uiInput)
  {
    case  0: pcInputName="CPU Idle Time";  break;
    case  1: pcInputName="DIP Switches";  break;
    case  2: pcInputName="reserved";  break;
    //case  3: pcInputName="Trip switch";  break;
    //case  4: pcInputName="dig. Input 5";  break;
    //case  5: pcInputName="dig. Input 6";  break;
    //case  6: pcInputName="dig. Input 7";  break;
    //case  7: pcInputName="dig. Input 8";  break;
    //case  8: pcInputName="dig. Input 9";  break;
    //case  9: pcInputName="dig. Input 10"; break;
    //case 10: pcInputName="dig. Input 11"; break;
    //case 11: pcInputName="dig. Input 12"; break;
    //case 12: pcInputName="dig. Input 13"; break;
    //case 13: pcInputName="dig. Input 14"; break;
    //case 14: pcInputName="dig. Input 15"; break;
    //case 15: pcInputName="dig. Input 16"; break;
    //case 16: pcInputName="dig. Input 17"; break;
    //case 17: pcInputName="dig. Input 18"; break;
    //case 18: pcInputName="dig. Input 19"; break;
    //case 19: pcInputName="dig. Input 20"; break;
    //case 20: pcInputName="dig. Input 21"; break;
    //case 21: pcInputName="dig. Input 22"; break;
    //case 22: pcInputName="dig. Input PS 58"; break;
    //case 23: pcInputName="dig. Input HAZARD"; break;
    default: pcInputName="dig. Input ???";
  }
  return(pcInputName);
} // pcDevSpec_DigInputsGetText_m()

/**
 * Returns the descriptive text for the specified digital output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of digital output to return the text
 *
 * @return descriptive text for specified digital output
 */
char *pcDevSpec_DigOutputsGetText_m(UINT uiOutput)
{
  char *pcOutputName;
  switch(uiOutput)
  {
    //case 0: pcOutputName="Alarm"; break;
    //case 1: pcOutputName="Alarm"; break;
    //case 2: pcOutputName="dig. Output 2"; break;   // @@@
    //case 3: pcOutputName="dig. Output 3"; break;   // @@@
    //case 4: pcOutputName="dig. Output 4"; break;   // @@@
    //case 5: pcOutputName="dig. Output 5"; break;   // @@@
    default: pcOutputName="dig. Output ???";
  }
  return(pcOutputName);
} // pcDevSpec_DigOutputsGetText_m()


/**
 * Returns the descriptive text for the specified analog input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of analog input to return the text
 *
 * @return descriptive text for specified analog input
 */
char *pcDevSpec_AnalogInputsGetText_m(UINT uiInput)
{
  char *pcInputName;
  switch(uiInput)
  {
    case 0: pcInputName="Level [%]"; break;
    //case 1: pcInputName="GPS signal SNR"; break;
    //case 2: pcInputName="ANI3 U-Ign [10mV]"; break;
    //case 3: pcInputName="ANI4 U-Illum [10mV]"; break;
    //case 4: pcInputName="ANI5 R-Pin8 [ohm]"; break;
    //case 5: pcInputName="ANI6 R-Pin9 [ohm]"; break;
    //case 6: pcInputName="GPS SNR"; break;
    //case 7: pcInputName="DUT Ubatt [mV]"; break;
    //case 8: pcInputName="Alternator [mV]"; break;
    default: pcInputName="Analog Input ???";
  }
  return(pcInputName);
} // pcDevSpec_AnalogInputsGetText_m()

/**
 * Returns the descriptive text for the specified analog output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of analog output to return the text
 *
 * @return descriptive text for specified analog output
 */
char *pcDevSpec_AnalogOutputsGetText_m(UINT uiOutput)
{
  char *pcOutputName;
  switch(uiOutput)
  {
    //case 0: pcOutputName="Analog Output 0"; break;
    //case 1: pcOutputName="Analog Output 1"; break;
    //case 2: pcOutputName="Analog Output 2"; break;
    //case 3: pcOutputName="Analog Output 3"; break;
    default: pcOutputName="Analog Output ???";
  }
  return(pcOutputName);
} // pcDevSpec_AnalogOutputsGetText_m()

/**
 * Returns the descriptive text for the specified frequency input
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of frequency input to return the text
 *
 * @return descriptive text for specified frequency input
 */
char *pcDevSpec_FreqInputsGetText_m(UINT uiInput)
{
  char *pcInputName;
  switch(uiInput)
  {
    case 0: pcInputName="Freq. Pin 4"; break;
    //case 1: pcInputName="Freq. Input 2"; break;
    //case 2: pcInputName="Freq. Input 4"; break;
    //case 3: pcInputName="Freq. Input 4"; break;
    default: pcInputName="Freq. Input ???";
  }
  return(pcInputName);
} // pcDevSpec_FreqInputsGetText_m()

/**
 * Returns the descriptive text for the specified analog output
 *
 * @author uidc9013 (02.02.2012)
 * @param uiInput number of analog output to return the text
 *
 * @return descriptive text for specified analog output
 */
char *pcDevSpec_FreqOutputsGetText_m(UINT uiOutput)
{
  char *pcOutputName;
  switch(uiOutput)
  {
    //case 0: pcOutputName="OUT_PWM_01"; break;
    //case 1: pcOutputName="OUT_PWM_02"; break;
    //case 2: pcOutputName="OUT_PWM_03"; break;
    //case 3: pcOutputName="Dials Illumination"; break;
    //case 4: pcOutputName="Dot Matrix Illumination"; break;
    default: pcOutputName="Freq. Output ???";
  }
  return(pcOutputName);
} // pcDevSpec_FreqOutputsGetText_m()

/**
 * Returns the descriptive text for the specified analog output
 *
 * @author uidc9013 (25.10.2012)
 * @param uiInput number of analog output to return the text
 *
 * @return descriptive text for specified analog output
 */
char *pcDevSpec_DeviceSpecificTestGetText_m(UINT uiTestNumber)
{
  char *pcTestName;
  switch(uiTestNumber)
  {
    //case 0: pcTestName="inspection state aktivieren (0x81)"; break;
    //case 1: pcTestName="inspection state deaktivieren (0x01)"; break;
    //case 2: pcTestName="EasyLink-Bus testen"; break;
    //case 3: pcTestName="Lin-Bus testen"; break;
    //case 2: pcTestName="Alternator D+ testen"; break;
    //case 5: pcTestName="Eeprom invalidieren"; break;
    //case 2: pcTestName="Device specific test 2"; break;   // @@@
    default: pcTestName="Device specific test ???";
  }
  return(pcTestName);
} // pcDevSpec_DeviceSpecificTestGetText_m()


/**
 * Returns the description for the specified test and digital input
 *
 * @author uidc9013 (12.12.2012)
 * @param uiTestNumber
 *               Number of the device specific test.
 * @param uiInputNumber
 *               Number of digital output to return the description.
 *
 * @return descriptive text for specified analog output.
 */
char *pcDevSpec_DeviceSpecificTestInputGetText_m(UINT uiTestNumber,
                                                 UINT uiInputNumber)
{
  char *pcInputDescription=NULL;

  switch(uiTestNumber)
  {
    case 0:   // inspection state 0x81
      switch(uiInputNumber)
      {
        //case 0: pcInputDescription="description for digital input 0 for test 0"; break;
        //case 1: pcInputDescription="description for digital input 1 for test 0"; break;
        default: pcInputDescription=NULL; break;
      }
      break;
    case 1:   // inspection state 0x01
      switch(uiInputNumber)
      {
        //case 0: pcInputDescription="description for digital input 0 for test 0"; break;
        //case 1: pcInputDescription="description for digital input 1 for test 0"; break;
        default: pcInputDescription=NULL; break;
      }
      break;
    case 2:   // Alternator D+ testen
      switch(uiInputNumber)
      {
        //case 0: pcInputDescription="description for digital input 0 for test 1"; break;
        //case 1: pcInputDescription="description for digital input 1 for test 1"; break;
        default: pcInputDescription=NULL; break;
      }
      break;
    default: pcInputDescription=NULL;
  }
  return(pcInputDescription);
} // pcDevSpec_DeviceSpecificTestInputGetText_m()

/**
 * Returns the description for the specified test and digital output
 *
 * @author uidc9013 (12.12.2012)
 * @param uiTestNumber
 *               Number of the device specific test.
 * @param uiOutputNumber
 *               Number of digital output to return the description.
 *
 * @return descriptive text for specified analog output
 */
char *pcDevSpec_DeviceSpecificTestOutputGetText_m(UINT uiTestNumber,
                                                  UINT uiOutputNumber)
{
  char *pcOutputDescription=NULL;

  switch(uiTestNumber)
  {
    case 0:   // inspection state 0x81
      switch(uiOutputNumber)
      {
        //case 0: pcOutputDescription="description for digital input 0 for test 0"; break;
        //case 1: pcOutputDescription="description for digital input 1 for test 0"; break;
        default: pcOutputDescription=NULL; break;
      }
      break;
    case 1:   // inspection state 0x01
      switch(uiOutputNumber)
      {
        //case 0: pcOutputDescription="description for digital input 0 for test 0"; break;
        //case 1: pcOutputDescription="description for digital input 1 for test 0"; break;
        default: pcOutputDescription=NULL; break;
      }
      break;
    case 2:   // Alternator D+ testen
      switch(uiOutputNumber)
      {
        case 0: pcOutputDescription="Alternator D+ zu 5V schalten"; break;
        //case 1: pcOutputDescription="description for digital input 1 for test 0"; break;
        default: pcOutputDescription=NULL; break;
      }
      break;
    default: pcOutputDescription=NULL;
  }
  return(pcOutputDescription);
} // pcDevSpec_DeviceSpecificTestOutputGetText_m()


/**
 * Returns the descriptive text for the specified tool
 *
 * @author uidc9013 (06.11.2012)
 * @param uiTool number of tool to return the text (0 based)
 *
 * @return descriptive text for specified tool.
 */
char *pcDevSpec_DataProductionInterlockGetText_m(UINT uiTool)
{
  char *pcToolName;
  switch(uiTool)
  {
    case 0: pcToolName="EOL Test"; break;
    //case 0: pcToolName="Pointer press"; break;
    //case 2: pcToolName="Tool 2"; break;   // @@@
    default: pcToolName="Tool ???";
  }
  return(pcToolName);
} // pcDevSpec_DataProductionInterlockGetText_m()



#include "../../0000/current/DeviceSpecific_0000.c"

/**
 * functions implemented in DeviceSpecific_0000.c
 */
//void vDevSpec_GetCapabilities_m(char *pcCapabilities);
//UINT uiDevSpec_BacklightGetColors_m(UINT uiBacklight);


