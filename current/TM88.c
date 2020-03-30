#include <windows.h>
#include <WinSpool.h>
#include <ansi_c.h>

#include "TM88.h"
#include "EpsStmApi3.h"
#include "Win32Prt.h"

#define API_DLL_FILE_NAME  "EpsStmApi.dll"

// Funktionsprototypen aus EpsStmApi3.h
DLL_BiOpenMonPrinter    BiOpenMonPrinter = NULL;
DLL_BiGetStatus         BiGetStatus = NULL;
DLL_BiCloseMonPrinter   BiCloseMonPrinter = NULL;

BOOL bTM88_Init_g(char *pcPrinterName)
{
  unsigned int i;

  unsigned long cbNeeded;   // the number of bytes needed for the structure
  unsigned long numLocalPrinterDevices; //number of printer devices on computer
  HMODULE hLibWinSpool=NULL;
  HMODULE hLibEpsStmApi=NULL;

  typedef BOOL (WINAPI *PROC) (DWORD,LPTSTR,DWORD,LPBYTE,DWORD,LPDWORD,LPDWORD);
  PROC fnEnumPrinters=NULL;


  memset((void*)&PrinterData_g,0,sizeof(tagWin32PrtData));

  hLibWinSpool = LoadLibrary("winspool.drv");
  if(!hLibWinSpool)
    return(FALSE);
  fnEnumPrinters = (PROC)GetProcAddress(hLibWinSpool,"EnumPrintersA");
  if(!fnEnumPrinters)
  {
    printf("bTM88_Init_g(): GetProcAddress(EnumPrintersA) failed\n");
    FreeLibrary(hLibWinSpool);
    return(FALSE);
  }

  // get all of the printer drivers installed on the computer
  // get size of memory to allocate
  fnEnumPrinters(PRINTER_ENUM_LOCAL,         // printer type or types
                 NULL,                       // address of server name
                 2,                          // structure level
                 NULL, //(unsigned char*)PrinterData_g.pPrinterInfo2,  // printer info array
                 0,                          // number of bytes in printer info array buffer provided by caller
                 &cbNeeded,                  // address of variable with count of bytes copied (or required)
                 &numLocalPrinterDevices     // address of variable with count of job info. structures copied
                );
  // allocate needed memory and get all local printers
  PrinterData_g.pPrinterInfo2=(PRINTER_INFO_2*)malloc(cbNeeded);
  fnEnumPrinters(PRINTER_ENUM_LOCAL,         // printer type or types
                 NULL,                       // address of server name
                 2,                          // structure level
                 (unsigned char*)PrinterData_g.pPrinterInfo2,  // printer info array
                 cbNeeded,                   // number of bytes in printer info array buffer provided by caller
                 &cbNeeded,                  // address of variable with count of bytes copied (or required)
                 &numLocalPrinterDevices     // address of variable with count of job info. structures copied
                );

  FreeLibrary(hLibWinSpool);

  // if no printer installed output error and exit
  if(numLocalPrinterDevices==0)
  {
    // error no printers installed
    printf("bTM88_Init_g(): kein Drucker gefunden\n");
    free(PrinterData_g.pPrinterInfo2);
    return(FALSE);
  }


  // find THE Epson printer among the local printers
  iPrinterIndex_g=-1;
  for(i=0; i<numLocalPrinterDevices; i++)
  {
    // ignore DM-D Driver.
    if(strncmp(PrinterData_g.pPrinterInfo2[i].pDriverName,pcPrinterName,strlen(pcPrinterName))==0)
    {
      iPrinterIndex_g=i;
      break;
    }
  }

  if(iPrinterIndex_g<0)
  {
    printf("no appropriate printer found:\n");
    for(i=0; i<numLocalPrinterDevices; i++)
    {
      printf("  %u: %s\n",i,PrinterData_g.pPrinterInfo2[i].pDriverName);
    }
    return(FALSE);
  }

  hLibEpsStmApi=LoadLibrary(API_DLL_FILE_NAME);
  if(!hLibEpsStmApi)
  {
    printf("File %s not found\n",API_DLL_FILE_NAME);
    return(FALSE);
  }
  BiOpenMonPrinter=GetProcAddress(hLibEpsStmApi,"BiOpenMonPrinter");
  if(!BiOpenMonPrinter)
  {
    printf("bTM88_Init_g(): GetProcAddress(BiOpenMonPrinter) failed\n");
    FreeLibrary(hLibEpsStmApi);
    return(FALSE);
  }

  BiGetStatus=GetProcAddress(hLibEpsStmApi,"BiGetStatus");
  if(!BiGetStatus)
  {
    printf("bTM88_Init_g(): GetProcAddress(BiGetStatus) failed\n");
    FreeLibrary(hLibEpsStmApi);
    return(FALSE);
  }

  BiCloseMonPrinter=GetProcAddress(hLibEpsStmApi,"BiCloseMonPrinter");
  if(!BiCloseMonPrinter)
  {
    printf("bTM88_Init_g(): GetProcAddress(biCloseMonPrinter) failed\n");
    FreeLibrary(hLibEpsStmApi);
    return(FALSE);
  }

  //PrinterData_g.iPrtMonHandle=BiOpenMonPrinter(TYPE_PORT,"ESDPRT001:");
  PrinterData_g.iPrtMonHandle=BiOpenMonPrinter(TYPE_PRINTER,PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrinterName);
  if(PrinterData_g.iPrtMonHandle<0)
  {
    printf("BiOpenMonPrinter() failed: code=%d\n",PrinterData_g.iPrtMonHandle);
    return(FALSE);
  }
  vTM88_SetLineSpacing_g(1.1); //Initialwert für Line spacing
  vWin32Prt_SetPaperOffset_g(-55); //Papierbreite auf Druckbereich anpassen(512px Breite),

  return(TRUE);
} /* bTM88_Init_g() */

BOOL bTM88_CloseLib_g(void)
{
  //Wenn der MonPrinter nicht geschlossen werden kann, wird ein Fehler zurückgegeben
  if(PrinterData_g.pPrinterInfo2)
  {
    free(PrinterData_g.pPrinterInfo2);
  }
  return((BiCloseMonPrinter(PrinterData_g.iPrtMonHandle) == ERR_HANDLE)?FALSE:TRUE);
}


void vTM88_Dump_g(void)
{
  vWin32Prt_Dump_g();
} /* bTM88_Dump_g() */

BOOL bTM88_StartDocument_g(void)
{
  return(bWin32Prt_StartDocument_g());
} /* bTM88_StartDocument_g() */

BOOL bTM88_EndDocument_g(void)
{
  return(bWin32Prt_EndDocument_g());
} /* bTM88_EndDocument_g() */

BOOL bTM88_SetFont_g(const char *pcFont,int iSize)
{
  return(bWin32Prt_SetFont_g(pcFont,iSize));
} /* bTM88_SetFont_g() */

BOOL bTM88_GetPrtFontName_g(int iWidth,int iHeight,BOOL bBold,
                            int *piHeightPixel, char *pcFont)
{
  switch(iWidth)
  {
    case 1:
    case 2:
    case 4:
    case 8:
      break;
    default:
      printf("Width %d nicht zulässig! ",iWidth);
      iWidth=2;
      printf("korrigiert auf %d.\n",iWidth);
  }

  switch(iWidth)
  {
    case 1:
      switch(iHeight)
      {
        case 1:
        case 2:
          break;
        default:
          printf("Height %d für Width %d nicht zulässig! ",iHeight,iWidth);
          iHeight=2;
          printf("korrigiert auf %d.\n",iHeight);
      }
      break;
    case 2:
      switch(iHeight)
      {
        case 1:
        case 2:
        case 4:
          break;
        default:
          printf("Height %d für Width %d nicht zulässig! ",iHeight,iWidth);
          iHeight=2;
          printf("korrigiert auf %d.\n",iHeight);
      }
      break;
    case 4:
      switch(iHeight)
      {
        case 2:
        case 4:
        case 8:
          break;
        default:
          printf("Height %d für Width %d nicht zulässig! ",iHeight,iWidth);
          iHeight=2;
          printf("korrigiert auf %d.\n",iHeight);
      }
      break;
    case 8:
      switch(iHeight)
      {
        case 4:
        case 8:
          break;
        default:
          printf("Height %d für Width %d nicht zulässig! ",iHeight,iWidth);
          iHeight=2;
          printf("korrigiert auf %d.\n",iHeight);
      }
      break;
  }

  switch(iHeight)
  {
    case 1: *piHeightPixel=0;  break;
    case 2: *piHeightPixel=30; break;
    case 4: *piHeightPixel=40; break;
    case 8: *piHeightPixel=80; break;
  }

  sprintf(pcFont,"Font%c%d%d",
          (bBold)?'B':'A',
          iWidth,
          iHeight);
  return(TRUE);
} /* bTM88_GetPrtFontName_g() */

BOOL bTM88_DrawSetPenWidth_g(int iWidth)
{
  return(bWin32Prt_DrawSetPenWidth_g(iWidth));
}

void vTM88_SetLineSpacing_g(float fLineSpacing)
{
  vWin32Prt_SetLineSpacing_g(fLineSpacing);
}

void vTM88_SetBold_g(BOOL bBold)
{
  vWin32Prt_SetBold_g(bBold);
} // vTM88_SetBold_g()

void vTM88_SetItalic_g(BOOL bItalic)
{
  vWin32Prt_SetItalic_g(bItalic);
} // vTM88_SetItalic_g()

void vTM88_SetUnderline_g(BOOL bUnderline)
{
  vWin32Prt_SetUnderline_g(bUnderline);
} // vTM88_SetUnderline_g()

void vTM88_SetMargin_g(int iMarginLeft, int iMarginWidth)
{
  vWin32Prt_SetMargin_g(iMarginLeft,iMarginWidth);
}

BOOL bTM88_DrawLine_g(int iX1, int iY1,
                      int iX2, int iY2)
{
  return(bWin32Prt_DrawLine_g(iX1,iY1,iX2,iY2));
} /* bTM88_DrawLine_g() */

BOOL bTM88_DrawRoundRect_g(int iLeft,int iTop,
                           int iRight,int iBottom,
                           int iRadius)
{
  return(bWin32Prt_DrawRoundRect_g(iLeft,iTop,iRight,iBottom,iRadius));
}

BOOL bTM88_WriteLine_g(EAlignment eAlignment, const char *pcText, ...)
{
  va_list args;
  char caBuffer[MAX_TEXT_LENGTH+1];

  va_start(args,pcText);
  vsprintf(caBuffer,pcText,args);
  va_end(args);

  return(bWin32Prt_WriteLine_g(eAlignment,caBuffer));
} /* bTM88_WriteLine_g() */

int iTM88_WriteAt_g(int iColumn,
                    int iRow,
                    EAlignment eAlignment,
                    const char *pcText, ...)
{
  va_list args;
  char caBuffer[MAX_TEXT_LENGTH+1];

  va_start(args,pcText);
  vsprintf(caBuffer,pcText,args);
  va_end(args);

  return(iWin32Prt_WriteAt_g(iColumn,iRow,eAlignment,caBuffer));
}

BOOL bTM88_PrintBitmapFromFile_g(const char *pcFileName,
                                 int iLeft, int iTop,
                                 float fScale)
{
  return(bWin32Prt_PrintBitmapFromFile_g(pcFileName,iLeft,iTop,fScale));
} /* bTM88_PrintBitmapFromFile_g() */

BOOL bTM88_PrintBitmapFromMemory_g(const char *pcBitmap,
                                   int iLeft, int iTop,
                                   float fScale)
{
  return(bWin32Prt_PrintBitmapFromMemory_g(pcBitmap,iLeft,iTop,fScale));
} /* bTM88_PrintBitmapFromFile_g() */

int iTM88_GetVPos_g(void)
{
  return(iWin32Prt_GetVPos_g());
}

int iTM88_GetPaperWidth_g(void)
{
  return(iWin32Prt_GetPaperWidth_g());
}

int iTM88_GetPaperLength_g(void)
{
  return(iWin32Prt_GetPaperLength_g());
}

