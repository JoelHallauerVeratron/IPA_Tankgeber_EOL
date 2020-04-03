/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        config.c
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
#include <stdio.h>
#include <ansi_c.h>
#include <formatio.h>
#include "inifile.h"

#include "defines.h"
#include "global.h"
#include "config.h"
#include <utility.h>


#define WARN_MISSING_CONFIG_ITEMS


// Item-Keys für Ini-Datei

/**
 * Section Network
 */
#define REG_SECTION_CONFIG_NETWORK                "Network"

#define REG_KEY_CONFIG_NETWORK_USER               "User"
#define REG_KEY_CONFIG_NETWORK_PWD                "Pwd"
#define REG_KEY_CONFIG_NETWORK_MAP_DRIVE          "MapDrive"
#define REG_KEY_CONFIG_NETWORK_SHARE_NAME         "ShareName"

#define DEFAULT_CONFIG_NETWORK_USER               "user"
#define DEFAULT_CONFIG_NETWORK_PWD                "user123"
#define DEFUALT_CONFIG_NETWORK_MAP_DRIVE          "P:"
#define DEFUALT_CONFIG_NETWORK_SHARE_NAME         "\\\\localhost\\c$"


/**
 * Section Path
 */
#define REG_SECTION_CONFIG_PATH                   "Path"
#define REG_KEY_CONFIG_PATH_RESULTS               "PathResults"
#define DEFAULT_CONFIG_PATH_RESULTS               "Results"

#define REG_KEY_CONFIG_PATH_WORKORDERS            "PathWorkorders"
#define DEFAULT_CONFIG_PATH_WORKORDERS            "Workorders"

#define REG_KEY_CONFIG_PATH_SERIAL_NUMBERS        "PathSerialNumbers"
#define DEFAULT_CONFIG_PATH_SERIAL_NUMBERS        "SerialNumbers"

/**
 * Section Files
 */
#define REG_SECTION_CONFIG_FILES                  "Files"
#define REG_KEY_CONFIG_PATH_DUT_CONFIG_FILE       "DeviceData"
#define DEFAULT_CONFIG_PATH_DUT_CONFIG_FILE       "DeviceDat.ini"


char* pcRot13_m(char *s);


void vAppConfig_Dump_g(void)
{
  printf("tagAppConfig:\n");
  printf("  %s = \"%s\"\n","pcProjectName       ",tagAppConfig_g.pcProjectName       );
  printf("  %s = \"%s\"\n","pcApplicationName   ",tagAppConfig_g.pcApplicationName   );
  printf("  %s = \"%s\"\n","caDutDatIniFileName ",tagAppConfig_g.caDutDatIniFileName );
  printf("  %s = \"%s\"\n","caPathResults       ",tagAppConfig_g.caPathResults       );
  printf("  %s = \"%s\"\n","caPathWorkOrders    ",tagAppConfig_g.caPathWorkOrders    );
  printf("  %s = \"%s\"\n","caPathSerialNumbers ",tagAppConfig_g.caPathSerialNumbers );
  printf("tagNetwork:\n");
  printf("  %s = \"%s\"\n","caUser              ",tagAppConfig_g.tagNetwork.caUser);
  printf("  %s = \"%s\"\n","caPwd               ",tagAppConfig_g.tagNetwork.caPwd);
  printf("  %s = \"%s\"\n","caMapDrive          ",tagAppConfig_g.tagNetwork.caMapDrive);
  printf("  %s = \"%s\"\n","caShareName         ",tagAppConfig_g.tagNetwork.caShareName);
  printf("tagNfcReader:\n");
  printf("  %s = %d\n",    "uiComPort           ",tagAppConfig_g.tagNfcReader.uiComPort);
  printf("  %s = %d\n",    "uiBaudrate          ",tagAppConfig_g.tagNfcReader.uiBaudrate);
}

/**
 * Fügt den Laufwerksbuchstaben dem Pfad zu und bereinigt ihn
 *
 * @author Juerg (18.12.2014)
 *
 * @param pcMapDrive Laufwerksbuchstaben
 * @param pcMapPath  Pfad mit Dateiname
 *
 * @return char* bereinigter Pfad
 */
char *pcCorrectPath(char *pcMapDrive, char *pcMapPath)
{
  static char caPath[MAX_PATH];
  char caDriveName[10];
  char caDirName[MAX_PATH];
  char caFileName[MAX_FILENAME_LEN];

  memset(caDriveName,0,sizeof(caDriveName));
  memset(caDirName,0,sizeof(caDirName));
  memset(caFileName,0,sizeof(caFileName));
  memset(caPath,0,sizeof(caPath));

  SplitPath (pcMapPath, caDriveName, caDirName, caFileName);

  //Backslash muss am Anfang sein
  if(caDirName[0]!='\\')
  {
    memmove(&caDirName[1],caDirName,strlen(caDirName));
    caDirName[0]='\\';
  }
  sprintf(caPath,"%s%s%s",pcMapDrive,caDirName,caFileName);

  //Letzter Backslash eliminiert
  if(caPath[strlen(caPath)-1]=='\\')
    caPath[strlen(caPath)-1]=0;

  return(caPath);
}



/**
 * Liest Programm-Settings aus Ini-File ein
 *
 * @author uidc9013 (17.04.2012)
 * @param pcConfigFileName
 *               Dateiname der zu lesenden Datei (ggf. mit Pfad)
 *
 * @return TRUE wenn Daten gelesen werden konnten<br>
 *         FALSE bei Fehler
 */
BOOL bAppConfig_Read_g(char *pcConfigFileName)
{
  IniText iniHandle_m;
  int iStatus;
  char *pcSection;
  char *pcItem;
  char *pcDefault;
  long lFileSize;
  BOOL bFaultsFound=FALSE;
  //char caBuffer[MAX_PATH];

#if 0
  //Test for pcRot13_m()

  char caTest[100];

  strcpy(caTest,"user123");
  printf("rot13(\"%s\") = ",caTest);
  printf("\"%s\"\n",pcRot13_m(caTest));
#endif

  // Ini-Datei öffnen
  iniHandle_m=Ini_New(0);

  if(GetFileInfo(pcConfigFileName,&lFileSize)==0)
  {
    printf("vReadDevicesFromIniFile_g(): Ini-File \"%s\" nicht gefunden\n",
           pcConfigFileName);
    return(FALSE);
  }

  iStatus=Ini_ReadFromFile(iniHandle_m,pcConfigFileName);
  if(iStatus!=0)
  {
    printf("vReadDevicesFromIniFile_g(): Ini_ReadFromFile() Error=%d\n",iStatus);
    return(FALSE);
  }

  pcSection=REG_SECTION_CONFIG_NETWORK;
  pcItem=REG_KEY_CONFIG_NETWORK_USER;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.tagNetwork.caUser,MAX_USERNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_NETWORK_USER;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.tagNetwork.caUser,pcDefault);
  }

  pcItem=REG_KEY_CONFIG_NETWORK_PWD;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.tagNetwork.caPwd,MAX_USERNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_NETWORK_PWD;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.tagNetwork.caPwd,pcDefault);
  }
#ifdef MANGLE_PASSWORDS
  strcpy(tagAppConfig_g.tagNetwork.caPwd,pcRot13_m(tagAppConfig_g.tagNetwork.caPwd));
#endif // MANGLE_PASSWORDS

  pcItem=REG_KEY_CONFIG_NETWORK_MAP_DRIVE;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.tagNetwork.caMapDrive,2+1)<1)
  {
    pcDefault=DEFUALT_CONFIG_NETWORK_MAP_DRIVE;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.tagNetwork.caMapDrive,pcDefault);
  }

  pcItem=REG_KEY_CONFIG_NETWORK_SHARE_NAME;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.tagNetwork.caShareName,MAX_NETWORK_SHARE_NAME_LEN+1)<1)
  {
    pcDefault=DEFUALT_CONFIG_NETWORK_SHARE_NAME;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.tagNetwork.caShareName,pcDefault);
  }

  pcSection=REG_SECTION_CONFIG_PATH;
  pcItem=REG_KEY_CONFIG_PATH_RESULTS;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.caPathResults,MAX_PATHNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_PATH_RESULTS;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.caPathWorkOrders,pcDefault);
  }
  strcpy(tagAppConfig_g.caPathResults,pcCorrectPath(tagAppConfig_g.tagNetwork.caMapDrive,tagAppConfig_g.caPathResults));


  pcSection=REG_SECTION_CONFIG_PATH;
  pcItem=REG_KEY_CONFIG_PATH_WORKORDERS;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.caPathWorkOrders,MAX_PATHNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_PATH_WORKORDERS;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.caPathWorkOrders,pcDefault);
  }
  strcpy(tagAppConfig_g.caPathWorkOrders,pcCorrectPath(tagAppConfig_g.tagNetwork.caMapDrive,tagAppConfig_g.caPathWorkOrders));

  pcSection=REG_SECTION_CONFIG_PATH;
  pcItem=REG_KEY_CONFIG_PATH_SERIAL_NUMBERS;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.caPathSerialNumbers,MAX_PATHNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_PATH_SERIAL_NUMBERS;
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.caPathSerialNumbers,pcDefault);
  }
  strcpy(tagAppConfig_g.caPathSerialNumbers,pcCorrectPath(tagAppConfig_g.tagNetwork.caMapDrive,tagAppConfig_g.caPathSerialNumbers));


#if 0
  pcItem=REG_KEY_CONFIG_PATH_DEV_IMAGES;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.caPathDevImages,MAX_PATHNAME_LEN+1)<1)
  {
    pcDefault=DEFAULT_CONFIG_PATH_DEV_IMAGES;
  #ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
  #endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.caPathDevImages,pcDefault);
  }
#endif

  pcSection=REG_SECTION_CONFIG_FILES;
  pcItem=REG_KEY_CONFIG_PATH_DUT_CONFIG_FILE;
  pcDefault=DEFAULT_CONFIG_PATH_DUT_CONFIG_FILE;
  if(Ini_GetStringIntoBuffer(iniHandle_m,pcSection,pcItem,tagAppConfig_g.caDutDatIniFileName,MAX_PATHNAME_LEN+1)<1)
  {
#ifdef WARN_MISSING_CONFIG_ITEMS
    printf("Item \"%s/%s\" not found!"
           "   using default  \"%s\" instead\n",pcSection,pcItem,pcDefault);
#endif // WARN_MISSING_CONFIG_ITEMS
    strcpy(tagAppConfig_g.caDutDatIniFileName,DEFAULT_CONFIG_PATH_DUT_CONFIG_FILE);
  }
  strcpy(tagAppConfig_g.caDutDatIniFileName,pcCorrectPath(tagAppConfig_g.tagNetwork.caMapDrive,tagAppConfig_g.caDutDatIniFileName));


  //Baudrate=19200
  pcItem=REG_KEY_CONFIG_NFC_READER_BAUDRATE;
  if(Ini_GetUInt(iniHandle_m,pcSection,pcItem,&(tagAppConfig_g.tagNfcReader.uiBaudrate))<1)
  {
    printf("Item \"%s/%s\" not found!",pcSection,pcItem);
    bFaultsFound=TRUE;
  }




  Ini_Dispose(iniHandle_m);

  return(!bFaultsFound);
} // bReadAppConfig_g


char* pcRot13_m(char *s)
{
  char *p=s;
  int upper;

  while(*p)
  {
    upper=toupper(*p);
    if(upper>='A'&&upper<='M')
      *p+=13;
    else if(upper>='N'&&upper<='Z')
      *p-=13;
    ++p;
  }
  return(s);
} // pcRot13_m()


