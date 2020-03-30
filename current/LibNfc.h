/*******************************************************************
 *
 *    PROJECT:     Library
 *
 *    EQUIPMENT:   LibNfc
 *
 *    FILE:        LibNfc.h
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
#ifndef LIBNFC_H_INCLUDED
  #define LIBNFC_H_INCLUDED

  #include <windows.h>



#define DllExport __declspec(dllexport)

typedef enum
{
  eNfcFlag_ReadCard  = 0x02,
  eNfcFlag_Inventory = 0x20
} ENfcFlags;


//"Konstruktor"
DllExport BOOL __cdecl bNFC_Init_g(int iComPort, int iBaudrate);


//"Destruktor"
DllExport BOOL __cdecl bNFC_Close_g(void);


DllExport BOOL __cdecl bNFC_IsConnected_g(void);
DllExport char __cdecl *pcNFC_GetErrorMessage_g(void);
DllExport void __cdecl vNFC_SetErrorMessage_g(char *pcErrorMessage, ...);
DllExport void __cdecl vNFC_ClearErrorMessage_g(void);


//DllExport BOOL __cdecl bNFC_SetComPort_g(int iComPort);
DllExport BOOL __cdecl bNFC_SetComBaudrate_g(int iComBaudrate);

DllExport BOOL __cdecl bNFC_ReadCardBuffer_g(char *pcUID, UCHAR ucBlockNrToRead, UCHAR ucNrOfBlocksToRead);

DllExport UCHAR __cdecl *pucNFC_GetDataBlockFromCardBuffer_g(void);

DllExport int __cdecl iNFC_ReadCardsOnReader_g(void);
DllExport int __cdecl iNFC_GetNumberOfCards_g(void);


DllExport char __cdecl *pcNFC_GetAllUids_g(int iNumberOfCards);
DllExport char __cdecl *pcNFC_GetUid_g(int iCardNumber);


DllExport char __cdecl *pcNFC_DumpCardInfos_g(void);
#endif // !LIBNFC_H_INCLUDED
