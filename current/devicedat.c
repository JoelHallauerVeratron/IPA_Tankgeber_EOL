/*******************************************************************
 *
 *    PROJECT:     Tankfüllstandsgeber EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        devicedat.c
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

#include <windows.h>
#include <stdio.h>
#include <formatio.h>
#include <ansi_c.h>
#include "inifile.h"

#include "global.h"
#include "devicedat.h"
#include <utility.h>


// Item-Keys für Ini-Datei
#define REG_KEY_DEVICE_DESCRIPTION         "Description"          // Description="Mustergerät"
#define REG_KEY_DEVICE_LIB_DUT_COM_NR      "LibDutComNr"          // LibDutComNr=11
#define REG_KEY_DEVICE_PROD_NR_CUSTOMER    "ProdNrCustomer"       //
#define REG_KEY_DEVICE_SW_VERSION          "SwVersion"            // SwVersion="xyz???"
#define REG_KEY_DEVICE_BOOTTIME            "BootTime"             // BootTime=2.0
#define REG_KEY_DEVICE_VOLTAGE             "Voltage"              // Voltage=14.0
#define REG_KEY_DEVICE_IVCC_IDLE_MIN       "IvccIdleMin"          // IvccIdleMin=100.0
#define REG_KEY_DEVICE_IVCC_IDLE_MAX       "IvccIdleMax"          // IvccIdleMax=250.0
#define REG_KEY_DEVICE_IVCC_LOAD_MIN       "IvccLoadMin"          // IvccLoadMin=100.0
#define REG_KEY_DEVICE_IVCC_LOAD_MAX       "IvccLoadMax"          // IvccLoadMax=250.0

#define REG_KEY_DEVICE_NMEA_CODE           "NMEACode"             // NMEACode=0xffff

#define REG_KEY_SIGNAL_TYPE                "SignalType"           // SignalType="R"
#define REG_KEY_SIGNAL_VALUE               "SignalValue"          // SignalValue=511
#define REG_KEY_SIGNAL_ADC                 "SignalADC"            // SignalADC=480
#define REG_KEY_SIGNAL_TOL                 "SignalTol"            // SignalTol=30

#define REG_KEY_DEVICE_LABEL_TYPE          "LabelType"            // LabelType=1
#define REG_KEY_DEVICE_LABEL_TEXT          "LabelText"            // LabelText="VDO Tachometer 85mm BK"
#define REG_KEY_DEVICE_LABEL_DATE_FORMAT   "LabelDateFormat"      // LabelDateFormat="KW%V/%y"
#define REG_KEY_DEVICE_LABEL_SAMPLE_TEXT   "SampleText"           // SampleText="C1 sample"

#define REG_KEY_DEVICE_SOURCEMODE          "SourceMode"           // SampleText="C1 sample"
#define REG_KEY_DEVICE_SINKMODE            "Stecker"           // SampleText="C1 sample"
#define REG_KEY_DEVICE_STECKER             "IvccIdleMin"          // IvccIdleMin=100.0
#define REG_KEY_DEVICE_WATERTYPE           "WaterType"           // SampleText="C1 sample"


#define DEFAULT_BOOT_TIME                  1.0f                   // default für BootTime=2.0
#define DEFAULT_VOLTAGE                   14.0f                   // default für Voltage=14.0
#define DEFAULT_LIB_DUT_COM_NR            56                      // default für LibDutComNr=56
#define DEFAULT_NMEA_CODE                 0x0000   // TODO: set NMEA Code
#define DEFAULT_LABEL_DATE_FORMAT         "KW%V/%y"

// analoge Inputs
#define REG_KEY_DEVICE_AN_INPUT_X_VALUE    "AnalogInputOhmic%d"   // AnalogInputOhmic1=200
#define REG_KEY_DEVICE_AN_INPUT_THRESHOLD  "AnalogInputThreshold" // AnalogInputThreshold=10

#define DEFAULT_DEVICE_AN_INPUT_1_VALUE     30
#define DEFAULT_DEVICE_AN_INPUT_2_VALUE     99
#define DEFAULT_DEVICE_AN_INPUT_THRESHOLD   10

// Freq.Inputs
#define REG_KEY_DEVICE_DO_FREQINPUTTEST    "DoFreqInputTest"      // DoFreqInputTest=FALSE
#define REG_KEY_DEVICE_FQ_INPUT_VALUE      "FreqInputFrequency"   // FreqInputFrequency=1000
#define REG_KEY_DEVICE_FQ_INPUT_THRESHOLD  "FreqInputThreshold"   // FreqInputThreshold=50

#define DEFAULT_DEVICE_FQ_INPUT_VALUE      1000
#define DEFAULT_DEVICE_FQ_INPUT_THRESHOLD    50

// GPS SNR
#define REG_KEY_DEVICE_GPS_MIN_SNR         "GpsMinSNR"            // GpsMinSNR=20
#define DEFAULT_DEVICE_GPS_MIN_SNR         20

//#define DEFAULT_DEVICE_AN_INPUT_2_VALUE   1000
//#define DEFAULT_DEVICE_AN_INPUT_THRESHOLD   50



// Dichtheitstest
#define REG_KEY_DEVICE_DT_T_FILL           "T_FillTO"       //T_FillTO=30
#define REG_KEY_DEVICE_DT_T_RELAX          "T_Relax"        //T_Relax=3.5
#define REG_KEY_DEVICE_DT_T_PRF            "T_Prf"          //T_Prf=7
#define REG_KEY_DEVICE_DT_P_PRF            "P_Prf"          //P_Prf=50
#define REG_KEY_DEVICE_DT_P_OVERSHOT       "P_Overshot"     //P_Overshot=20
#define REG_KEY_DEVICE_DT_P_DROP           "P_Drop"         //P_Drop=100

#define DEFAULT_DT_T_FILL            30     // [s] max. zul. Füllzeit
#define DEFAULT_DT_T_RELAX            3     // [s] Beruhigungszeit
#define DEFAULT_DT_T_PRF              7     // [s] Prüfzeit
#define DEFAULT_DT_P_PRF             50     // [mB] Prüfdruck
#define DEFAULT_DT_P_OVERSHOT        20     // [mB] Overshoot
#define DEFAULT_DT_P_DROP             5     // [mB] max. zul. Druckabfall


char caIniFileDeviceName_m[300]={ 0 };
BOOL bIniFileDevice_FileIsSet_m=FALSE;
BOOL bIniFileDevice_ReadDataValid_m=FALSE;


TagDevice tagCurrentDeviceTyp_g;


BOOL bIniFileDevice_SetFile_g(char *pcFilePath)
{
  long lFileSize;

  if(GetFileInfo(pcFilePath,&lFileSize)==0)
  {
    printf("bIniFileDevice_SetFile_g(): Ini-File \"%s\" nicht gefunden\n",
           pcFilePath);
    return(FALSE);
  }

  strcpy(caIniFileDeviceName_m,pcFilePath);
  bIniFileDevice_FileIsSet_m=TRUE;

  return(TRUE);
} // bIniFileDevice_SetFile_g()



/**
 * F&uuml;llt den Ring-Control mit den Artikelnummern aus der Ini-Datei
 *
 * @author uidc9013 (10.04.2012)
 * @param iPanel Handle auf Panel
 * @param iRingControl
 *               Handle auf Ring-Control
 * @param bWithPrompt
 *               <li>TRUE: Als erster Eintrag wird der Text "Typ ausw&auml;hlen" eingefügt
 *               und danach werden alle Artikelnummern eingef&uuml;gt</li>
 *               <li>FALSE: Es werden nur alle Artikelnummern eingef&uuml;gt</li>
 *
 * @return TRUE bei Erfolg<br>
 *         sonst FALSE bei Fehler
 */
BOOL bIniFileDevice_FillSelectorCtrl_g(int iPanel, int iRingControl, BOOL bWithPrompt)
{
  IniText iniHandle;
  int iDeviceIndex;
  int iPrevIndex;
  int iStatus;
  char *pcSection=NULL;

  iniHandle=Ini_New(1);   // Ini-Datei öffnen
  SetCtrlAttribute(iPanel,iRingControl,ATTR_DIMMED,1);

  if(!bIniFileDevice_FileIsSet_m)
  {
    printf("bIniFileDevice_FillSelectorCtrl_g(): Name von Ini-File nicht gesetzt\n");
    return(FALSE);
  }

  iStatus=Ini_ReadFromFile(iniHandle,caIniFileDeviceName_m);
  if(iStatus!=0)
  {
    printf("bIniFileDevice_FillSelectorCtrl_g(): Ini_ReadFromFile() Error=\"%s\"\n",
           pcGetIniErrorDetailed_g(iStatus));
    return(FALSE);
  }

  if(bWithPrompt)
    iPrevIndex=0;
  else
    GetCtrlVal(iPanel,iRingControl,&iPrevIndex);

  ClearListCtrl(iPanel,iRingControl);
  if(bWithPrompt)
  {
    InsertListItem(iPanel,iRingControl,-1,DEVICE_DAT_SELECT_PROMPT,0);
  }

  /**
   * Alle Sections einzeln einlesen
   */
  for(iDeviceIndex=0;
      ;
      iDeviceIndex++)
  {
    // Aktueller Section-Name lesen
    if(Ini_NthSectionName(iniHandle,iDeviceIndex+1,&pcSection)!=1)
    {
      // Section nicht gefunden
      break;
    }

    InsertListItem(iPanel,iRingControl,-1,pcSection,iDeviceIndex);
  }

  Ini_Dispose(iniHandle);
  SetCtrlAttribute(iPanel,iRingControl,ATTR_DIMMED,0);
  SetCtrlVal(iPanel,iRingControl,iPrevIndex);
  return(TRUE);
} // bIniFileDevice_FillSelectorCtrl_g()



/**
 * Erweiterte Funktionalit&auml; zu CVI Ini_GetInt()<br>
 * Erweiterungen:<br>
 * <li>pcTagName wird als sprintf-Format angegeben (z.B.: "Value%02dTest") und iIndex
 * wird dann in den Platzhalter eingef&uuml;gt.<br>
 * <li>Default-Wert, wenn Tag-Value-Paar nicht gefunden wird, kann angegeben werden.
 *
 * @param iniHandle A handle returned from Ini_New().
 * @param pcSectionName
 *                  The section name under which to find the tag/value pair.
 * @param pcTagName The tag name in the tag/value pair.<br>
 *                  <b>Erweiterung:</b> pcTagName kann ein %d Format enthalten um<br>
 *                  den Wert von iIndex aufzunehmen. Wird kein Format-Platzhalter verwendet<br>
 *                  so muss für iIndex ein negativer Wert &uuml;bergeben werden.
 * @param iIndex    <b>Erweitung:</b> Z&auml;hl-Variable als Parameter zu pcTagName.
 * @param piValue   Aus Ini-File gelesener int-Wert.
 * @param bOptional <b>Erweiterung:</b> Muss der Eintrag vorhanden sein?<br>
 *                  TRUE: Der Eintrag muss vorhanden sein. Wenn er fehlt, wird *piErrorCounter<br>
 *                  um 1 erh&ouml; und eine Meldung auf stdio ausgegeben.
 * @param iDefaultValue
 *                  <b>Erweiterung:</b> Standard-Wert falls Eintrag in Ini-File nicht vorhanden ist.
 * @param piErrorCounter
 *                  Pointer auf int Variable, welche bei einem Fehler +1 gerechnet zur&uuml;ckgegeben<br>
 *                  wird oder NULL wenn der Fehler nicht gez&auml;hlt werden soll.
 *
 * @return Fehler-Code von Ini_GetInt()<br>
 *         &gt;0: Wert konnte gelesen werden<br>
 *         =0: Wert konnte nicht gelesen werden<br>
 *         &lt;0: anderer Fehler aufgetreten<br>
 */
int iIni2_GetInt_m(IniText       iniHandle,
                   const char    pcSectionName[],   // "A2C1234567890"
                   const char    pcTagName[],       // "FreqOutput%02d_SigNr"
                   const int     iIndex,          // Index zu tagName
                   int          *piValue,
                   BOOL          bOptional,
                   int           iDefaultValue,
                   int          *piErrorCounter)
{
  char  caItem[100];
  int   iRc;

  if(iIndex>-1)
  {
    if(strchr(pcTagName,'%')==0)
    {
      printf("iIni2_GetInt_m(): %s enthält keinen Platzhalter --> iIndex muss<0 sein.\n",pcTagName);
      strcpy(caItem,pcTagName);
    }
    else
      sprintf(caItem,pcTagName,iIndex);
  }
  else
    strcpy(caItem,pcTagName);

  iRc=Ini_GetInt(iniHandle,pcSectionName,caItem,piValue);
  if(iRc<1)
  {
    // Tag-Value-Paar nicht gefunden
    *piValue=iDefaultValue;
    if(!bOptional)
    {
      if(piErrorCounter)
        *piErrorCounter+=1;
      printf("Device %s: Item \"%s\" nicht gefunden\n",
             pcSectionName,caItem);
      return(iRc);
    }
  }
  return(iRc);
} // iIni2_GetInt_m()


/**
 * Erweiterte Funktionalit&auml; zu CVI Ini_GetInt()<br>
 * Erweiterungen:<br>
 * <li>Zahlwert wird als Hex-String erwartet. Bsp.: Value="0x12A". Prefix "0x" wird erwartet
 * <li>pcTagName wird als sprintf-Format angegeben (z.B.: "Value%02dTest") und iIndex
 * wird dann in den Platzhalter eingef&uuml;gt.<br>
 * <li>Default-Wert, wenn Tag-Value-Paar nicht gefunden wird, kann angegeben werden.
 *
 * @param iniHandle A handle returned from Ini_New().
 * @param pcSectionName
 *                  The section name under which to find the tag/value pair.
 * @param pcTagName The tag name in the tag/value pair.<br>
 *                  <b>Erweiterung:</b> pcTagName kann ein %d Format enthalten um<br>
 *                  den Wert von iIndex aufzunehmen. Wird kein Format-Platzhalter verwendet<br>
 *                  so muss für iIndex ein negativer Wert &uuml;bergeben werden.
 * @param iIndex    <b>Erweitung:</b> Z&auml;hl-Variable als Parameter zu pcTagName.
 * @param puiValue   Aus Ini-File gelesener int-Wert.
 * @param bOptional <b>Erweiterung:</b> Muss der Eintrag vorhanden sein?<br>
 *                  TRUE: Der Eintrag muss vorhanden sein. Wenn er fehlt, wird *piErrorCounter<br>
 *                  um 1 erh&ouml; und eine Meldung auf stdio ausgegeben.
 * @param iDefaultValue
 *                  <b>Erweiterung:</b> Standard-Wert falls Eintrag in Ini-File nicht vorhanden ist.
 * @param piErrorCounter
 *                  Pointer auf int Variable, welche bei einem Fehler +1 gerechnet zur&uuml;ckgegeben<br>
 *                  wird oder NULL wenn der Fehler nicht gez&auml;hlt werden soll.
 *
 * @return Fehler-Code von Ini_GetInt()<br>
 *         &gt;0: Wert konnte gelesen werden<br>
 *         =0: Wert konnte nicht gelesen werden<br>
 *         &lt;0: anderer Fehler aufgetreten<br>
 */
int iIni2_GetIntHex_m(IniText iniHandle,
                      const char pcSectionName[],   // "A2C1234567890"
                      const char pcTagName[],       // "TagName"
                      const int iIndex,          // Index zu tagName
                      UINT *puiValue,
                      BOOL bOptional,
                      UINT uiDefaultValue,
                      int *piErrorCounter)
{
  char caItem[100];
  char caValue[10];
  int iRc;

  if(iIndex>-1)
  {
    if(strchr(pcTagName,'%')==0)
    {
      printf("iIni2_GetHexInt_m(): %s enthält keinen Platzhalter --> iIndex muss<0 sein.\n",pcTagName);
      strcpy(caItem,pcTagName);
    }
    else
      sprintf(caItem,pcTagName,iIndex);
  }
  else
    strcpy(caItem,pcTagName);

  iRc=Ini_GetStringIntoBuffer(iniHandle,pcSectionName,caItem,caValue,sizeof(caValue));
  if(iRc>0)
  {
    // Tag-Value-Paar gefunden
    if(strnicmp("0x",caValue,2)!=0)
    {
      // Prefix "0x" nicht vorhanden
      if(!bOptional)
      {
        if(piErrorCounter)
          *piErrorCounter+=1;
        printf("Device %s: Item \"%s\" erwarteter Präfix '0x' nicht vorhanden\n",
               pcSectionName,caItem);
      }
      return(0);
    }
    sscanf(caValue,"%x",puiValue);
  }
  if(iRc<1)
  {
    // Tag-Value-Paar nicht gefunden
    *puiValue=uiDefaultValue;
    if(!bOptional)
    {
      if(piErrorCounter)
        *piErrorCounter+=1;
      printf("Device %s: Item \"%s\" nicht gefunden\n",
             pcSectionName,caItem);
      return(iRc);
    }
  }
  return(iRc);
} // iIni2_GetIntHex_m()


/**
 * Erweiterte Funktionalit&auml; zu CVI Ini_GetStringIntoBuffer()
 *
 * @param iniHandle A handle returned from Ini_New().
 * @param pcSectionName
 *                  The section name under which to find the tag/value pair.
 * @param pcTagName The tag name in the tag/value pair.<br>
 *                  <b>Erweiterung:</b> pcTagName kann ein %d Format enthalten um<br>
 *                  den Wert von iIndex aufzunehmen.
 * @param iIndex    <b>Erweitung:</b> Z&auml;hl-Variable als Parameter zu pcTagName.
 * @param pcValue   Aus Ini-File gelesener String.
 * @param iSize     The maximum number of bytes (including the ASCII NUL byte) to be written into buffer.
 * @param bOptional <b>Erweiterung:</b> Muss der Eintrag vorhanden sein?<br>
 *                  TRUE: Der Eintrag muss vorhanden sein. Wenn er fehlt, wird *piErrorCounter<br>
 *                  um 1 erh&ouml; und eine Meldung auf stdio ausgegeben.
 * @param pcDefaultValue
 *                  <b>Erweiterung:</b> Standard-Wert falls Eintrag in Ini-File nicht vorhanden ist.
 * @param piErrorCounter
 *                  pointer auf int Variable um gefundene Fehler zu<br>
 *                  zaehlen. Wenn bOptional==TRUE dann wird diese Variable<br>
 *                  nicht ge&auml;ndert.<br>
 *                  Bei Nichtverwendung kann NULL angegeben werden.
 *
 * @return Fehler-Code von Ini_GetInt()<br>
 *         &gt;0: Wert konnte gelesen werden<br>
 *         =0: Wert konnte nicht gelesen werden<br>
 *         &lt;0: anderer Fehler aufgetreten<br>
 */
int iIni2_GetStringIntoBuffer_m(IniText       iniHandle,
                                const char    pcSectionName[],
                                const char    pcTagName[],
                                const int     iIndex,
                                char         *pcValue,
                                int           iSize,
                                BOOL          bOptional,
                                char         *pcDefaultValue,
                                int          *piErrorCounter)
{
  char  caItem[100];
  int   iRc;

  if(iIndex>-1)
    sprintf(caItem,pcTagName,iIndex);
  else
    strcpy(caItem,pcTagName);


  iRc=Ini_GetStringIntoBuffer(iniHandle,pcSectionName,pcTagName,pcValue,iSize);
  if(iRc<1)
  {
    // Tag-Value-Paar nicht gefunden
    if(bOptional)
    {
      strcpy(pcValue,pcDefaultValue);
    }
    else
    {
      if(piErrorCounter)
        *piErrorCounter+=1;
      printf("Device %s: Item \"%s\" nicht gefunden\n",
             pcSectionName,caItem);
      return(iRc);
    }
  }
  return(iRc);
} // iIni2_GetStringIntoBuffer_m()



/**
 * Gibt zurück, ob die Daten zum ausgewählten Gerät gültig sind
 *
 * @author WeU (08.01.2015)
 * @return TRUE: Daten sind gültig<br>
 *         FALSE: Daten enthalten Fehler
 */
BOOL bIniFileDevice_ReadDataValid_g(void)
{
  return(bIniFileDevice_ReadDataValid_m);
} // bIniFileDevice_ReadDataValid_g()


/**
 * Liest alle Geräte-Specifikationen aus der Init-Datei ein
 * und füllt sie in die globale Tabelle pptagDeviceList_g
 *
 * Beim lesen der Ini-Datei wird sie auf ihre Richtigkeit geprüft.
 *
 * @param pcDeviceNr Ger&auml;tenummer f&uuml;r welche die Daten gelesen werden sollen
 *
 * @return TRUE bei Erfolg<br>
 *         sonst FALSE bei Fehler
 */
BOOL bIniFileDevice_ReadData_g(char *pcDeviceNr)
{
  IniText iniHandle;
  int iSectionCount;
  int iDeviceIndex;
  int iStatus;
  char *pcSection;
  char *pcItem;
  long lFileSize;
  double dValue;
  char caString[100];
  int iValue;
  int iErrorFound=0;

  bIniFileDevice_ReadDataValid_m=FALSE;

  // Ini-Datei öffnen
  iniHandle=Ini_New(1);

  if(GetFileInfo(tagAppConfig_g.caDutDatIniFileName,&lFileSize)==0)
  {
    printf("bIniFileDevices_ReadFile_g(): Ini-File \"%s\" nicht gefunden\n",
           tagAppConfig_g.caDutDatIniFileName);
    return(FALSE);
  }

  iStatus=Ini_ReadFromFile(iniHandle,tagAppConfig_g.caDutDatIniFileName);
  if(iStatus!=0)
  {
    printf("bIniFileDevices_ReadFile_g(): Ini_ReadFromFile() Error=\"%s\"\n",
           pcGetIniErrorDetailed_g(iStatus));
    return(FALSE);
  }

  // Tabelle genügend gross allozieren (letzter Index als Terminator)
  iSectionCount=Ini_NumberOfSections(iniHandle);


  /**
   * pcDeviceNr in Sections suchen
   */
  for(iDeviceIndex=0;
      iDeviceIndex<iSectionCount;
      iDeviceIndex++)
  {
    // Aktueller Section-Name lesen
    if(Ini_NthSectionName(iniHandle,iDeviceIndex+1,&pcSection)!=1)
    {
      // Section doch nicht gefunden?!?
      printf("bReadDevicesFromIniFile_g(): Ini_ReadFromFile() Error=\"%s\"\n",
             pcGetIniErrorDetailed_g(iStatus));
      Ini_Dispose(iniHandle);
      return(FALSE);
    }

    if(strcmp(pcDeviceNr,pcSection)==0)
    {
      // Richtige Section gefunden
      break;
    }
  }

  if(iDeviceIndex>=iSectionCount)
  {
    // pcDeviceNr in Sections nicht gefunden
    printf("bReadDevicesFromIniFile_g(): Daten für %s nicht gefunden\n",pcDeviceNr);
    Ini_Dispose(iniHandle);
    return(FALSE);
  }


  // Aktueller Section-Name lesen
  if(Ini_NthSectionName(iniHandle,iDeviceIndex+1,&pcSection)!=1)
  {
    // Section doch nicht gefunden?!?
    printf("bReadDevicesFromIniFile_g(): Ini_ReadFromFile() Error=\"%s\"\n",
           pcGetIniErrorDetailed_g(iStatus));
    Ini_Dispose(iniHandle);
    return(FALSE);
  }


  strcpy(tagCurrentDeviceTyp_g.caProdNrUnpacked,pcSection);

  // Description="Gerät xyz"
  pcItem=REG_KEY_DEVICE_DESCRIPTION;
  if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,tagCurrentDeviceTyp_g.caDescription,DEVICE_DESCRIPTION_LEN+1)<1)
  {
    // keine Beschreibung gefunden
    tagCurrentDeviceTyp_g.caDescription[0]=0;
  }

  // LibDutComNr=55
  iIni2_GetInt_m(iniHandle,pcSection,REG_KEY_DEVICE_LIB_DUT_COM_NR,-1,&(tagCurrentDeviceTyp_g.uiLibDutComNr),TRUE,DEFAULT_LIB_DUT_COM_NR,&iErrorFound);

  //  SwVersion="2.0"
  pcItem=REG_KEY_DEVICE_SW_VERSION;
  tagCurrentDeviceTyp_g.bCheckSwVersion=TRUE;
  if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,tagCurrentDeviceTyp_g.caSwVersion,DEVICE_SW_VERSION_LEN+1)<1)
  {
    // Keine SW-Version angegeben --> nicht prüfen
    tagCurrentDeviceTyp_g.caSwVersion[0]=0;
    tagCurrentDeviceTyp_g.bCheckSwVersion=FALSE;
  }


  // BootTime=2.0
  pcItem=REG_KEY_DEVICE_BOOTTIME;
  if(Ini_GetDouble(iniHandle,pcSection,pcItem,&dValue)<1)
  {
    // kein Eintrag --> default verwenden
    dValue=DEFAULT_BOOT_TIME;
  }
  tagCurrentDeviceTyp_g.fBoottime=dValue;

  // Voltage=14.0
  pcItem=REG_KEY_DEVICE_VOLTAGE;
  if(Ini_GetDouble(iniHandle,pcSection,pcItem,&dValue)<1)
  {
    // kein Eintrag --> default verwenden
    dValue=DEFAULT_VOLTAGE;
  }
  tagCurrentDeviceTyp_g.fUbatt=dValue;

  // IvccIdleMin=100.0
  pcItem=REG_KEY_DEVICE_IVCC_IDLE_MIN;
  if(Ini_GetInt(iniHandle,pcSection,pcItem,&iValue)<1)
  {
    printf("Device %s: Item \"%s\" nicht gefunden\n",
           pcSection,pcItem);
    iErrorFound++;
  }
  else
    tagCurrentDeviceTyp_g.iIvccIdleMin=iValue;

  // IvccIdleMax=250.0
  pcItem=REG_KEY_DEVICE_IVCC_IDLE_MAX;
  if(Ini_GetInt(iniHandle,pcSection,pcItem,&iValue)<1)
  {
    printf("Device %s: Item \"%s\" nicht gefunden\n",
           pcSection,pcItem);
    iErrorFound++;
  }
  else
    tagCurrentDeviceTyp_g.iIvccIdleMax=iValue;

  // NMEACode=0xffff
  pcItem=REG_KEY_DEVICE_NMEA_CODE;
  iIni2_GetIntHex_m(iniHandle,pcSection,pcItem,-1,&tagCurrentDeviceTyp_g.tagProdData.uiNMEACode,TRUE,DEFAULT_NMEA_CODE,NULL);



  // WaterType="F"
  pcItem=REG_KEY_DEVICE_WATERTYPE;
  if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,caString,DEVICE_LABEL_TEXT_LEN+1)<1)
  {
    // keine Angabe gefunden
    printf("Device %s: Item \"%s\" nicht gefunden\n",
           pcSection,pcItem);
    iErrorFound++;
  }
  else
  {
    if(strnicmp(caString,"F",1)==0)
      tagCurrentDeviceTyp_g.eWaterType =eWaterType_Fresh;
    if(strnicmp(caString,"W",1)==0)
      tagCurrentDeviceTyp_g.eWaterType =eWaterType_Wasted;
    //if(strnicmp(caString,"LinBus",6)==0)
    //  tagCurrentDeviceTyp_g.tagSignalData.eType=eSignalType_LinBus;
  }

    // SignalType="A"
  pcItem=REG_KEY_SIGNAL_TYPE;
  if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,caString,DEVICE_LABEL_TEXT_LEN+1)<1)
  {
    // keine Angabe gefunden
    printf("Device %s: Item \"%s\" nicht gefunden\n",
           pcSection,pcItem);
    iErrorFound++;
  }
  else
  {
    if(strnicmp(caString,"A",1)==0)
      tagCurrentDeviceTyp_g.bAnalogSignal = TRUE;
    if(strnicmp(caString,"D",1)==0)
      tagCurrentDeviceTyp_g.bAnalogSignal = FALSE;
  }

  // Stecker=2
  pcItem=REG_KEY_DEVICE_SINKMODE;
  if(Ini_GetInt(iniHandle,pcSection,pcItem,&iValue)<1)
  {
    printf("Device %s: Item \"%s\" nicht gefunden\n",
           pcSection,pcItem);
    iErrorFound++;
  }
  else
    tagCurrentDeviceTyp_g.iStecker=iValue;


//// LabelType=9
//pcItem=REG_KEY_DEVICE_LABEL_TYPE;
//iIni2_GetInt_m(iniHandle,pcSection,pcItem,-1,&iValue,FALSE,0,&iErrorFound);
//switch(iValue)
//{
//  case eLabelType_LinkUp:
//    tagCurrentDeviceTyp_g.tagProdData.eLabelType=(ELabelType)iValue;
//    break;
//  default:
//    printf("Device %s: Item \"%s\" hat ungültigen Wert %d\n",
//           pcSection,pcItem,iValue);
//    iErrorFound++;
//}
//
//// LabelText="LinkUp"
//pcItem=REG_KEY_DEVICE_LABEL_TEXT;
//if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,tagCurrentDeviceTyp_g.tagProdData.caLabelText,DEVICE_LABEL_TEXT_LEN+1)<1)
//{
//  // keine Angabe gefunden
//  printf("Device %s: Item \"%s\" nicht gefunden\n",
//         pcSection,pcItem);
//  iErrorFound++;
//}
//
//// LabelDateFormat="KW%V/%y"
//iIni2_GetStringIntoBuffer_m(iniHandle,pcSection,REG_KEY_DEVICE_LABEL_DATE_FORMAT,-1,tagCurrentDeviceTyp_g.tagProdData.caLabelDateFormat,DEVICE_LABEL_DATE_FORMAT_LEN+1,TRUE,DEFAULT_LABEL_DATE_FORMAT,NULL);
//
//// SampleText="C1 sample"
//pcItem=REG_KEY_DEVICE_LABEL_SAMPLE_TEXT;
//if(Ini_GetStringIntoBuffer(iniHandle,pcSection,pcItem,tagCurrentDeviceTyp_g.tagProdData.caSampleText,DEVICE_LABEL_TEXT_LEN+1)<1)
//{
//  // keine Angabe gefunden
//  tagCurrentDeviceTyp_g.tagProdData.caSampleText[0]=0;
//}
//
//



  if(iErrorFound>0)
  {
    printf("================================================\n");
    printf(" %d Fehler in Ini-File \"%s\" gefunden\n",iErrorFound,tagAppConfig_g.caDutDatIniFileName);
    printf("================================================\n\n");
  }
  else
  {
    Cls();
    SetStdioWindowVisibility(0);
  }


  Ini_Dispose(iniHandle);
  bIniFileDevice_ReadDataValid_m=(iErrorFound==0);
  return(bIniFileDevice_ReadDataValid_m);
} // bIniFileDevices_ReadFile_g()


void vIniFileDevices_Dump_g(TagDevice *ptagDeviceTyp)
{
  printf("DeviceData dump:\n");

  printf("Description    = \"%s\"\n",ptagDeviceTyp->caDescription);
  printf("ProdNrUnpacked = \"%s\"\n",ptagDeviceTyp->caProdNrUnpacked);
  printf("Sw-Version = \"%s\"\n",ptagDeviceTyp->caSwVersion);
  printf("   %s\n",(ptagDeviceTyp->bCheckSwVersion)?"checked":"not checked");
  printf("Description = \"%s\"\n",ptagDeviceTyp->caDescription);
  putchar('\n');
  //TagProdData    tagProdData;
  printf("NMEA Code = 0x%04x\n",ptagDeviceTyp->tagProdData.uiNMEACode);
  putchar('\n');
  printf("boot time = %.1f\n",ptagDeviceTyp->fBoottime);
  printf("Ubatt = %.1f\n",ptagDeviceTyp->fUbatt);
  printf("Ivcc min. = %dmA\n",ptagDeviceTyp->iIvccIdleMin);
  printf("Ivcc max. = %dmA\n",ptagDeviceTyp->iIvccIdleMax);
  putchar('\n');
  //TagSignalData  tagSignalData;
  printf("Signal data:");
  printf("  type = ");
} // vIniFileDevices_Dump_g()
