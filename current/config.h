/*******************************************************************
 *
 *    PROJECT:     SDF
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        config.h
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

#ifndef CONFIG_H_INCLUDED
  #define CONFIG_H_INCLUDED

  #include <windows.h>


  #define MAX_IP_ADDRESS_LEN  15  // "255.255.255.255"
  #define MAX_USERNAME_LEN  100
  #define MAX_NETWORK_SHARE_NAME_LEN  260

typedef struct
{
  // Programm-Konstanten
  char *pcCompanyName;
  char *pcProjectName;
  char *pcApplicationName;

  // Aus Konfig-Datei
  char caDutDatIniFileName[MAX_PATHNAME_LEN+1];
  char caPathWorkOrders[MAX_PATHNAME_LEN+1];
  char caPathResults[MAX_PATHNAME_LEN+1];
  char caPathSerialNumbers[MAX_PATHNAME_LEN+1];

  struct
  {
    char caUser[MAX_USERNAME_LEN+1];
    char caPwd[MAX_USERNAME_LEN+1];
    char caMapDrive[2+1];   // drive letter such as "k:"
    char caShareName[MAX_NETWORK_SHARE_NAME_LEN+1];
  } tagNetwork;

  struct
  {
    char caIpAddress[MAX_IP_ADDRESS_LEN+1];
  } tagLabelPrinter;

  struct
  {
    UINT uiComPort;
    UINT uiBaudrate;
  } tagNfcReader;

} TagAppConfig;


BOOL bAppConfig_Read_g(char *pcConfigFileName);
void vAppConfig_Dump_g(void);


#endif // !CONFIG_H_INCLUDED
