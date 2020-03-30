#include <ansi_c.h>
#include <windows.h>

#include "Win32Prt.h"

typedef enum
{
  eStrMatchTrue,
  eStrMatchFalse,
  eStrMatchError
}EStrMatch;

tagWin32PrtData PrinterData_g;
int iPrinterIndex_g;
int iOffset_m = 0;
int iMarginLeft_m = 0;
int iMarginWidth_m = 0;

/**
 * Bereitet einen String darauf vor, damit er vom Drucker ohne abgeschnitten zu werden, ausgegeben werden kann.
 * Der Rückegabewert muss selber freigegeben werden.
 *
 * @param pcString String
 * @param iColumn  Position von Links
 * @param iXMaxPosition
 *                 Maximale Position für Schriftlänge in Pixeln
 *
 * @return Vorbereiteten String mit newlines
 */
char *pcWin32Prt_PrepareString_m(const char *pcString,
                                 int iColumn,
                                 int iXMaxPosition);

/**
 * Teilt ein Wort so auf, dass es nicht abgeschnitten wird
 *
 * @param pcString Aufzuteilendes Wort
 * @param iColumn  Position von Links
 * @param iXMaxPosition
 *                 Maximale Position der Schriftlänge
 *
 * @return Wieviele Zeichen auf die Linie gepasst haben
 */
int iWin32Prt_SplitWord_m(const char *pcString,
                     int iColumn,
                     int iXMaxPosition);

/**
 * Prüft ob der aktuelle String auf die Linie passt
 *
 * @param pcString String
 * @param iColumn  Position von Links
 * @param iXMaxPosition
 *                 Maximale Position der Schriftlänge
 *
 * @return EStrMatch enum
 */
EStrMatch eWin32Prt_StringMatchesOnLine_m(const char *pcString,
                                          int iColumn,
                                          int iXMaxPosition);

/**
 * Setzt die aktuelle vertikale Position auf die nächste Linie
 *
 * @param piRow  Aktuelle vertikale Position
 * @param iFontHeight
 *               Höhe der Schriftart in Pixeln
 */
void vWin32Prt_GoToNewLine_m(int *piRow, int iFontHeight);

void vWin32Prt_Dump_g(void)
{
  printf("vWin32Prt_Dump_g\n");
  printf("  bDocumentStarted =%s\n",(PrinterData_g.bDocumentStarted)?"TRUE":"FALSE");

  printf("  bBold            =%s\n",(PrinterData_g.bBold           )?"TRUE":"FALSE");
  printf("  bItalic          =%s\n",(PrinterData_g.bItalic         )?"TRUE":"FALSE");
  printf("  bUnderline       =%s\n",(PrinterData_g.bUnderline      )?"TRUE":"FALSE");

  if(iPrinterIndex_g<0)
  {
    printf("no printer found\n");
    return;
  }

  printf("  pServerName      =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pServerName)?     PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pServerName:     "NULL");
  printf("  pPrinterName     =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrinterName)?    PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrinterName:    "NULL");
  printf("  pShareName       =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pShareName)?      PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pShareName:      "NULL");
  printf("  pPortName        =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPortName)?       PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPortName:       "NULL");
  printf("  pDriverName      =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDriverName)?     PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDriverName:     "NULL");
  printf("  pComment         =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pComment)?        PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pComment:        "NULL");
  printf("  pLocation        =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pLocation)?       PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pLocation:       "NULL");
  printf("  pSepFile         =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pSepFile)?        PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pSepFile:        "NULL");
  printf("  pPrintProcessor  =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrintProcessor)? PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrintProcessor: "NULL");
  printf("  pDatatype        =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDatatype)?       PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDatatype:       "NULL");
  printf("  pParameters      =\"%s\"\n", (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pParameters)?     PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pParameters:     "NULL");

  printf("  Priority         =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].Priority       );
  printf("  DefaultPriority  =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].DefaultPriority);
  printf("  StartTime        =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].StartTime      );
  printf("  UntilTime        =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].UntilTime      );
  printf("  Status           =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].Status         );
  printf("  cJobs            =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].cJobs          );
  printf("  AveragePPM       =\"%d\"\n", PrinterData_g.pPrinterInfo2[iPrinterIndex_g].AveragePPM     );
} /* bWin32Prt_Dump_g() */


BOOL bWin32Prt_StartDocument_g(void)
{
  if(iPrinterIndex_g<0)
  {
    printf("no printer found\n");
    return(FALSE);
  }

  // create the printer device context
  PrinterData_g.hDC=CreateDC(PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDriverName,     // driver name
                             PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pPrinterName,    // device name
                             NULL,        // not used; should be NULL
                             NULL);       // optional printer data

  memset((void*)&PrinterData_g.di,0,sizeof(DOCINFO));
  PrinterData_g.di.cbSize = sizeof(DOCINFO);
  PrinterData_g.di.lpszDocName = "doc name";

  PrinterData_g.iVPos=0;

  /* Create default font */
  PrinterData_g.hFont=CreateFont(20,0,0,0,
                                 FW_NORMAL,
                                 FALSE,
                                 FALSE,
                                 FALSE,
                                 DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                                 FW_DONTCARE,"Arial");

  PrinterData_g.hOldFont=SelectObject(PrinterData_g.hDC,PrinterData_g.hFont);

  StartDoc(PrinterData_g.hDC,&PrinterData_g.di);
  StartPage(PrinterData_g.hDC);

  PrinterData_g.bDocumentStarted=TRUE;
  return(TRUE);
} /* bWin32Prt_StartDocument_g() */

BOOL bWin32Prt_EndDocument_g(void)
{
  if(!PrinterData_g.bDocumentStarted)
    return(FALSE);

  SelectObject(PrinterData_g.hDC,&PrinterData_g.hOldFont);
  DeleteObject(PrinterData_g.hFont);

  EndPage(PrinterData_g.hDC);
  EndDoc(PrinterData_g.hDC);
  DeleteDC(PrinterData_g.hDC);

  return(TRUE);
} /* bWin32Prt_EndDocument_g() */

BOOL bWin32Prt_SetFont_g(const char *pcFont,int iSize)
{
  if(PrinterData_g.hFont)
    DeleteObject(PrinterData_g.hFont);
  strcpy(PrinterData_g.caFont,pcFont);
  PrinterData_g.iSize=iSize;
  PrinterData_g.hFont=CreateFont(PrinterData_g.iSize,0,0,0,
                                 (PrinterData_g.bBold)?FW_BOLD:FW_NORMAL,
                                 (PrinterData_g.bItalic)?TRUE:FALSE,
                                 (PrinterData_g.bUnderline)?TRUE:FALSE,
                                 FALSE,
                                 DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                                 FW_DONTCARE,
                                 PrinterData_g.caFont);
  if(PrinterData_g.hFont==NULL)
  {
    printf("bWin32Prt_SetFont(%s,%d) has failed with code %d.\n",pcFont,iSize,GetLastError());
    return(FALSE);
  }

  SelectObject(PrinterData_g.hDC,PrinterData_g.hFont);

  return(TRUE);
} /* bWin32Prt_SetFont_g() */

BOOL bWin32Prt_RecreateFont_m(void)
{
  if(PrinterData_g.hFont)
    DeleteObject(PrinterData_g.hFont);
  PrinterData_g.hFont=CreateFont(PrinterData_g.iSize,0,0,0,
                                 (PrinterData_g.bBold)?FW_BOLD:FW_NORMAL,
                                 (PrinterData_g.bItalic)?TRUE:FALSE,
                                 (PrinterData_g.bUnderline)?TRUE:FALSE,
                                 FALSE,
                                 DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                                 FW_DONTCARE,
                                 PrinterData_g.caFont);
  if(PrinterData_g.hFont==NULL)
  {
    printf("bWin32Prt_RecreateFont() has failed with code %d.\n",GetLastError());
    return(FALSE);
  }

  SelectObject(PrinterData_g.hDC,PrinterData_g.hFont);

  return(TRUE);
} /* bWin32Prt_RecreateFont() */

void vWin32Prt_SetLineSpacing_g(float fLineSpacing)
{
  PrinterData_g.fLineSpacing = fLineSpacing;
}

void vWin32Prt_SetBold_g(BOOL bBold)
{
  PrinterData_g.bBold=bBold;
  bWin32Prt_RecreateFont_m();
} // vWin32Prt_SetBold_g()

void vWin32Prt_SetItalic_g(BOOL bItalic)
{
  PrinterData_g.bItalic=bItalic;
  bWin32Prt_RecreateFont_m();
} // vWin32Prt_SetItalic_g()

void vWin32Prt_SetUnderline_g(BOOL bUnderline)
{
  PrinterData_g.bUnderline=bUnderline;
  bWin32Prt_RecreateFont_m();
} // vWin32Prt_SetUnderline_g()

void vWin32Prt_SetMargin_g(int iMarginLeft, int iMarginWidth)
{
  int iMaxPrintingWidth = (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmOrientation == DMORIENT_LANDSCAPE)?iWin32Prt_GetPaperLength_g():iWin32Prt_GetPaperWidth_g();

  if((iMarginLeft == 0) && (iMarginWidth == 0)) //Grenze zurücksetzen
  {
    iMarginLeft_m = 0;
    //Breite auf maximal mögliche Druckposition setzen
    iMarginWidth_m = iMaxPrintingWidth;
    return;
  }
  if(iMarginLeft > iMaxPrintingWidth) //Nicht erlaubt
    return;
  if((iMarginWidth+iMarginLeft) > iMaxPrintingWidth)
  {
    iMarginLeft_m = iMarginLeft;
    iMarginWidth_m = iMaxPrintingWidth-iMarginLeft;
    return;
  }
  iMarginLeft_m = iMarginLeft;
  iMarginWidth_m = iMarginWidth;
}

BOOL bWin32Prt_WriteLine_g(EAlignment eAlignment, const char *pcText, ...)
{
  va_list args;
  int iLinesWritten;
  char caBuffer[MAX_TEXT_LENGTH+1];
  SIZE tagPixel;

  if(!PrinterData_g.bDocumentStarted)
  {
    // bWin32Prt_StartDocument_g() wurde nicht aufgerufen
    return(FALSE);
  }
  if(eAlignment == eAlignmentCenteredOnPoint)
  {
    printf("Error: Can't write Line centered on point\n");
    return(FALSE);
  }
  va_start(args,pcText);
  vsprintf(caBuffer,pcText,args);
  va_end(args);

  if((iLinesWritten = iWin32Prt_WriteAt_g(-1,PrinterData_g.iVPos,eAlignment,caBuffer)) < 0)
    return(FALSE);

  if(!GetTextExtentPoint32A(PrinterData_g.hDC,caBuffer,strlen(caBuffer),&tagPixel))
    return(FALSE);

  PrinterData_g.iVPos += (iLinesWritten * (tagPixel.cy * PrinterData_g.fLineSpacing)); //Neue Position bei Drucker setzen
  //printf("Lines Written: %d Setting vpos to %d\n",iLinesWritten,PrinterData_g.iVPos);

  return(TRUE);
} /* bWin32Prt_WriteLine_g() */

EStrMatch eWin32Prt_StringMatchesOnLine_m(const char *pcString, int iColumn, int iXMaxPosition)
{
  SIZE tagPixelXY;
  if(!GetTextExtentPoint32A(PrinterData_g.hDC,pcString,strlen(pcString),&tagPixelXY)) //Grösse des Texts in Pixel ermitteln
  {
    printf("GetTextExtentPoint32A() failed with Code: %d\n", GetLastError());
    return(eStrMatchError);
  }
  if(iColumn + tagPixelXY.cx > iXMaxPosition)
  {
    return(eStrMatchFalse);
  }
  return(eStrMatchTrue);
}

int iWin32Prt_SplitWord_m(const char *pcString, int iColumn, int iXMaxPosition)
{
  int iIndex;
  char *pcBuffer = NULL;

  pcBuffer = malloc(strlen(pcString)+1); //'\0'

  //printf("Word to split: %s\n", pcString);

  for(iIndex = 0;pcString[iIndex] != '\0';++iIndex)
  {
    strncpy(pcBuffer,pcString,iIndex+1);
    pcBuffer[iIndex+1] = '\0'; //Terminierung muss selber gesetzt werden

    if(eWin32Prt_StringMatchesOnLine_m(pcBuffer,iColumn,iXMaxPosition) == eStrMatchFalse) //Sobald der String nicht mehr auf eine Linie passt, schleife abbrechen
      break;
    if(eWin32Prt_StringMatchesOnLine_m(pcBuffer,iColumn,iXMaxPosition) == eStrMatchError)
    {
      free(pcBuffer);
      return(-1);
    }
  }
  if(pcString[iIndex] == '\0')
  {
    free(pcBuffer);
    printf("String already matches on line\n");
    return(-1);
  }
  free(pcBuffer);
  return(iIndex);
}

void vWin32Prt_GoToNewLine_m(int *piRow, int iFontHeight)
{
  *piRow += (iFontHeight * PrinterData_g.fLineSpacing); //Auf nächste Linie springen
  //printf("Going to new line, new row: %d font height: %d, fLinespacing: %f\n", *piRow,iFontHeight,PrinterData_g.fLineSpacing);
}

char *pcWin32Prt_PrepareString_m(const char *pcString, int iColumn, int iXMaxPosition)
{
  int iIndexStartPosition = 0;
  int iIndexEndPosition = 0;
  int iIndex = 0;
  int iCurrentStringlength;
  int iMaxLength;
  SIZE tagPixelXY;
  char caCurrentString[MAX_TEXT_LENGTH+1];
  char *pcNewString = NULL;
  BOOL bFirstLine = TRUE;

  if(!GetTextExtentPoint32A(PrinterData_g.hDC,&pcString[iIndex],strlen(pcString),&tagPixelXY)) //Grösse des Texts in Pixel ermitteln
  {
    printf("GetTextExtentPoint32A() failed with Code: %d\n", GetLastError());
    return(NULL);
  }
  if(tagPixelXY.cx + iColumn < iXMaxPosition) //Keine Aktion notwendig, nur kopieren
  {
    pcNewString = malloc(strlen(pcString)+1);
    strcpy(pcNewString,pcString);
    return(pcNewString);
  }
  pcNewString = malloc(strlen(pcString) * 2 + 1); //*2 Für Worst Case(Nach jedem Zeichen Newline), +1 für \0
  memset(pcNewString,0,strlen(pcString) * 2 + 1);
  do
  {
    while(pcString[iIndex]!= '\0' && pcString[iIndex]!= ' ' && pcString[iIndex]!= '\n') //Nächstes Leerzeichen, newline oder ende des Strings ermitteln
      ++iIndex;

//  printf("iIndex: %d\n"
//         "iIndexStartPosition: %d\n"
//         "iIndexEndPosition: %d\n"
//         "Text after iIndex(zu bearbeitender Text): %s\n", iIndex, iIndexStartPosition, iIndexEndPosition, &pcString[iIndex]);

    memset(caCurrentString,0,sizeof(caCurrentString));
    strncpy(caCurrentString,&pcString[iIndexStartPosition],iIndex-iIndexStartPosition); //Betreffenden String in Buffer kopieren

    if(!bFirstLine)
    {
      iColumn = iMarginLeft_m;
    }
    //String passt auf Linie
    if(eWin32Prt_StringMatchesOnLine_m(caCurrentString,iColumn,iXMaxPosition)== eStrMatchTrue)
    {
//    printf("--String matches on line!--\n");
      switch(pcString[iIndex])
      {
        case '\0':
          //Letzten Teil noch anfügen, dann Funktion beenden
          strcat(pcNewString,&pcString[iIndexStartPosition]);
          return(pcNewString);
        case ' ':
          //nächstes wort herausparsen
          iIndexEndPosition = iIndex;
          ++iIndex;
          continue;
        case '\n':
          //Aktuellen Teil zu pcNewString hinzufügen, danach neue Zeile beginnen
          strncat(pcNewString,&pcString[iIndexStartPosition],iIndex-iIndexStartPosition);
          iCurrentStringlength = strlen(pcNewString);
          pcNewString[iCurrentStringlength]   = '\n';
          pcNewString[iCurrentStringlength+1] = '\0';

          iIndexEndPosition = 0; //Wieder auf 0 setzen, da eine neue Zeile beginnt
          ++iIndex;
          iIndexStartPosition = iIndex; //Zählen von neu beginnen
          continue;
        default:
          printf("Error in Switch pcString\n");
          free(pcNewString);
          return(NULL);
      }
    }
    else //String passt nicht auf linie
    {
//    printf("--String doesn't matches on line!--\n");
      if(iIndexEndPosition == 0)
      {
        if((iMaxLength = iWin32Prt_SplitWord_m(caCurrentString,iColumn,iXMaxPosition)) < 1) //Wenn weniger als 1 zeichen auf die linie passt, dann abbrechen
        {
          printf("Error in splitting word\n");
          free(pcNewString);
          return(NULL);
        }
        strncat(pcNewString,&pcString[iIndexStartPosition],iMaxLength);
        iCurrentStringlength = strlen(pcNewString);
        pcNewString[iCurrentStringlength]   = '\n';
        pcNewString[iCurrentStringlength+1] = '\0';
        iIndex = iIndexStartPosition + iMaxLength; //index auf pos nach zeilenumbruch
        iIndexStartPosition = iIndex; //startpos zu erstellter newline
        bFirstLine = FALSE;
        continue;
      }
      else
      {
        //Newline bei vorherigem Leerzeichen setzen
        strncat(pcNewString,&pcString[iIndexStartPosition],iIndexEndPosition-iIndexStartPosition);
        iCurrentStringlength = strlen(pcNewString);
        pcNewString[iCurrentStringlength]   = '\n';
        pcNewString[iCurrentStringlength+1] = '\0';
      }
      bFirstLine = FALSE;
      iIndexStartPosition = iIndexEndPosition+1; //- space
      iIndexEndPosition = 0;
    }
  } while(pcString[iIndex]!= '\0' || iIndexEndPosition < strlen(pcString));

  //printf("New String: %s\n",pcNewString);
  return(pcNewString);
}

int iWin32Prt_WriteAt_g(int iColumn,int iRow,
                        EAlignment eAlignment,
                        const char *pcText, ...)
{
  int iNewColumn=0;
  int iNewRow;
  int iIndex;
  int iIndexStartPosition;
  int iLinesWritten;
  int iLines;
  //Maximalen Druckbereich ermitteln nach hoch- oder querformat
  int iMaxHorizontalPosition = (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmOrientation == DMORIENT_LANDSCAPE)?iWin32Prt_GetPaperLength_g():iWin32Prt_GetPaperWidth_g();
  char caBuffer[MAX_TEXT_LENGTH+1];
  char *pcPreparedString = NULL;
  char *pcCurrentString = NULL;
  BOOL bFirstRun = TRUE;
  va_list args;
  SIZE tagPixelXY;

  if(!PrinterData_g.bDocumentStarted)
  {
    // bWin32Prt_StartDocument_g() wurde nicht aufgerufen
    return(-1);
  }
  if(iColumn > iMaxHorizontalPosition)
  {
    //Punkt ausserhalb des Druckbaren bereichs angegeben
    return(-1);
  }

  va_start(args,pcText);
  vsprintf(caBuffer,pcText,args);
  va_end(args);

  if(!GetTextExtentPoint32A(PrinterData_g.hDC,caBuffer,strlen(caBuffer),&tagPixelXY)) //Grösse des Texts in Pixel ermitteln
  {
    printf("GetTextExtentPoint32A() failed with Code: %d\n", GetLastError());
    return(-1);
  }
//printf("Paperdata:\n"
//       "Paper Length(Pixel, with offset): %d Struct: %d\n"
//       "Paper Width(Pixel, with offset): %d Struct: %d\n"
//       "DPI: %d\n"
//       "Paper Orientation: %s\n"
//       "Alignment: %d\n",iWin32Prt_GetPaperLength_g(),PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPaperLength,
//                         iWin32Prt_GetPaperWidth_g(),PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPaperWidth,
//                         PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPrintQuality,
//                         (PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmOrientation == DMORIENT_PORTRAIT)?"Portrait":"Landscape",
//                         eAlignment);

  iLinesWritten = 0;
  if(iMarginWidth_m != 0)
  {
    iMaxHorizontalPosition = iMarginLeft_m+iMarginWidth_m;
    if(iColumn < iMarginLeft_m) //Wenn Column vor margin ist, dann auf margin setzen
    {
      iColumn = iMarginLeft_m;
    }
    else if(iColumn > iMaxHorizontalPosition) //Position ausserhalb des beschreibbaren Bereichs angegeben
    {
      vWin32Prt_GoToNewLine_m(&iRow,tagPixelXY.cy); //Auf nächste Zeile gehen
      ++iLinesWritten;
      iColumn = iMarginLeft_m;
    }
  }
  iNewRow = iRow;
  //Switch für Alignment
  switch(eAlignment)
  {
    case eAlignmentLeft: //Default alignment
      iNewColumn = iColumn;
      break;
    case eAlignmentRight:
      iNewColumn = iColumn;
      break;
    case eAlignmentCenter:
      iNewColumn = iMarginLeft_m;
      break;
    case eAlignmentCenteredOnPoint:
      iNewColumn = iMarginLeft_m;
      break;
    default:
      printf("Invalid Text alignement\n");
      return(-1);
  }
  //printf("iColumn: %d, iNewColumn: %d, iRow: %d, iNewRow: %d, iMarginLeft: %d, iMarginWidth: %d iMaxHorizontalPosition: %d\n",iColumn,iNewColumn,iRow,iNewRow,iMarginLeft_m,iMarginWidth_m,iMaxHorizontalPosition);
  if(!(pcPreparedString = pcWin32Prt_PrepareString_m(caBuffer,iNewColumn,iMaxHorizontalPosition)))
  {
    printf("Error preparing String\n");
    return(-1);
  }

  //Linien zählen für centered on point
  iLines = 1;
  for(iIndex = 0;pcPreparedString[iIndex] != '\0';++iIndex)
  {
    if(pcPreparedString[iIndex] == '\n')
      ++iLines;
  }

  iIndex = 0;
  iIndexStartPosition = 0;
  do
  {
    while(pcPreparedString[iIndex] != '\0' && pcPreparedString[iIndex] != '\n')
      ++iIndex;

    pcCurrentString = malloc(iIndex - iIndexStartPosition + 1);
    memset(pcCurrentString,0,iIndex - iIndexStartPosition + 1);
    strncpy(pcCurrentString,&pcPreparedString[iIndexStartPosition],iIndex-iIndexStartPosition);

    //Aus aktuellem Textteil Abmasse des Texts herauslesen
    if(!GetTextExtentPoint32A(PrinterData_g.hDC,pcCurrentString,strlen(pcCurrentString),&tagPixelXY))
    {
      free(pcCurrentString);
      free(pcPreparedString);
      printf("GetTextExtentPoint32A() failed with Code: %d\n", GetLastError());
      return(-1);
    }
    switch(eAlignment)
    {
      case eAlignmentLeft:
        if(bFirstRun)
          iNewColumn = iColumn;
        else
          iNewColumn = iMarginLeft_m;
        break;
      case eAlignmentRight:
        iNewColumn = iMaxHorizontalPosition - tagPixelXY.cx;
        break;
      case eAlignmentCenter:
        iNewColumn = iMarginLeft_m + (iMarginWidth_m / 2) - (tagPixelXY.cx/2); //Offset + halbe breite des Druckbaren bereichs - hälfte der länge des aktuellen Texts
        break;
      case eAlignmentCenteredOnPoint:
        iNewColumn = iMarginLeft_m + (iMarginWidth_m / 2) - (tagPixelXY.cx/2); //Offset + halbe breite des Druckbaren bereichs - hälfte der länge des aktuellen Texts
        if(bFirstRun == TRUE) //Nur beim ersten durchgang durchführen
        {
          /* Prüfen ob der Textblock von der Höhe nicht über die Obere Kante des Papiers hinauskommt */
          iNewRow = iRow-(tagPixelXY.cy*iLines*PrinterData_g.fLineSpacing)/2;
          if(iNewRow < 0)
          {
            free(pcCurrentString);
            free(pcPreparedString);
            printf("Position of String is too high for paper(missing %d)\n",iNewRow);
            return(-1);
          }
        }
        break;
      default:
        free(pcCurrentString);
        free(pcPreparedString);
        printf("Error in switch eAlignment\n");
        return(-1);
    }
    if(!TextOut(PrinterData_g.hDC,iNewColumn,iNewRow,pcCurrentString,strlen(pcCurrentString)))
    {
      free(pcCurrentString);
      free(pcPreparedString);
      printf("TextOut() failed with code %d\n",GetLastError());
      return(-1);
    }
    free(pcCurrentString);
    ++iLinesWritten;
    bFirstRun = FALSE;
    vWin32Prt_GoToNewLine_m(&iNewRow, tagPixelXY.cy);

    if(pcPreparedString[iIndex] != '\0')
    {
      ++iIndex;
      iIndexStartPosition = iIndex;
    }
  } while(pcPreparedString[iIndex] != '\0');

  free(pcPreparedString);
  return(iLinesWritten);
}/* bWin32Prt_WriteAt_g() */



BOOL bWin32Prt_DrawSetPenWidth_g(int iWidth)
{
  HPEN hPen;
  hPen=CreatePen(PS_SOLID,iWidth,RGB(0,0,0));
  SelectObject(PrinterData_g.hDC,hPen);
  return(TRUE);
}

BOOL bWin32Prt_DrawLine_g(int iX1, int iY1,
                      int iX2, int iY2)
{
  MoveToEx(PrinterData_g.hDC,iX1,iY1,NULL);
  LineTo(PrinterData_g.hDC,iX2,iY2);
  return(TRUE);
} /* bWin32Prt_DrawLine_g() */


BOOL bWin32Prt_DrawRoundRect_g(int iLeft,int iTop,int iRight,int iBottom, int iRadius)
{
  RoundRect(PrinterData_g.hDC,iLeft,iTop,iRight,iBottom,iRadius,iRadius);
  return(TRUE);
}

BOOL bWin32Prt_PrintBitmapFromFile_g(const char *pcFileName,
                                     int iLeft, int iTop,
                                     float fScale)
{
  HANDLE hFile = NULL;          // handle to the bitmap file
  HANDLE hMap = NULL;         // handle to the map
  LPVOID lpvFile = NULL;        // address of the file in memory
  BOOL bReturn;

  // open the bitmap file
  hFile=CreateFile(pcFileName, GENERIC_WRITE|GENERIC_READ,
                   FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile==INVALID_HANDLE_VALUE)
  {
    printf("bWin32Prt_PrintBitmap_g(%s) failed to open file\n",pcFileName);
    return(FALSE);
  }

  hMap=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
  lpvFile=MapViewOfFile(hMap,FILE_MAP_WRITE,0,0,0); // map whole file

  bReturn = bWin32Prt_PrintBitmapFromMemory_g((char *)lpvFile,iLeft,iTop,fScale);

  UnmapViewOfFile(lpvFile);
  CloseHandle(hMap);
  CloseHandle(hFile);

  return(bReturn);
} /* bWin32Prt_PrintBitmapFromFile_g() */

BOOL bWin32Prt_PrintBitmapFromMemory_g(const char *pcBitmap,
                                       int iLeft, int iTop,
                                       float fScale)
{
  LPBYTE lpImage = NULL;        // starting address of DIB bits
  LPBITMAPINFOHEADER lpBMIH = NULL;   // buffer containing the BITMAPINFOHEADER
  LPVOID lpvFile = NULL;        // address of the file in memory
  LPVOID lpvMem;
  HDC    hScreenDC;
  long lPrinterWidth;
  long lPrinterHeight;

  float fScreenTwipsPerPixelX;
  float fScreenTwipsPerPixelY;
  float fPrinterTwipsPerPixelX;
  float fPrinterTwipsPerPixelY;

  lpvFile = pcBitmap;

  lpvMem=(LPBYTE)lpvFile + sizeof(BITMAPFILEHEADER);
  lpBMIH=(LPBITMAPINFOHEADER)lpvMem;

  // address of the bitmap image
  lpImage=(LPBYTE)lpvFile + ((LPBITMAPFILEHEADER)lpvFile)->bfOffBits;

  hScreenDC=CreateDC("Display",NULL,NULL,NULL);
  if(hScreenDC==0)
  {
    printf("hScreenDC=%d\n",hScreenDC);
    return(FALSE);
  }

  // get twips per pixel
  fScreenTwipsPerPixelX = (float)1440/GetDeviceCaps(hScreenDC,LOGPIXELSX);
  fScreenTwipsPerPixelY = (float)1440/GetDeviceCaps(hScreenDC,LOGPIXELSY);
  fPrinterTwipsPerPixelX = (float)1440/GetDeviceCaps(PrinterData_g.hDC,LOGPIXELSX);
  fPrinterTwipsPerPixelY = (float)1440/GetDeviceCaps(PrinterData_g.hDC,LOGPIXELSY);

  // get dimensions of bitmap in printer units
  lPrinterWidth=(long)(lpBMIH->biWidth * fScreenTwipsPerPixelX / fPrinterTwipsPerPixelX);
  lPrinterHeight=(long)(lpBMIH->biHeight * fScreenTwipsPerPixelY / fPrinterTwipsPerPixelY);

  lPrinterWidth=(long)(fScale*(float)lPrinterWidth);
  lPrinterHeight=(long)(fScale*(float)lPrinterHeight);

  //Wenn das Bild nicht aufs Papier passt
  if(((lPrinterWidth + iLeft) > iWin32Prt_GetPaperWidth_g()) ||
     ((lPrinterHeight + iTop) > iWin32Prt_GetPaperLength_g()))
  {
//  printf("Bitmap don't match on Paper: iMaxLeft: %d iMaxTop: %d Paper width: %d Paper height: %d\n",
//         lPrinterWidth+iLeft,
//         lPrinterHeight+iTop,
//         iWin32Prt_GetPaperWidth_g(),
//         iWin32Prt_GetPaperLength_g());
    return(FALSE);
  }
  // set stretch mode and ouput the bitmap
  SetStretchBltMode(PrinterData_g.hDC,COLORONCOLOR);
  StretchDIBits(PrinterData_g.hDC,     // handle of device context
                iLeft,                 // x-coordinate of upper-left corner of dest. rect.
                iTop,                  // y-coordinate of upper-left corner of dest. rect.
                lPrinterWidth,         // width of destination rectangle
                lPrinterHeight,        // height of destination rectangle
                0,                     // x-coordinate of upper-left corner of source rect.
                0,                     // y-coordinate of upper-left corner of source rect.
                lpBMIH->biWidth,       // width of source rectangle
                lpBMIH->biHeight,      // height of source rectangle
                lpImage,               // address of bitmap bits
                (LPBITMAPINFO)lpBMIH,  // address of bitmap data
                DIB_RGB_COLORS,        // usage
                SRCCOPY);              // raster operation code

  return(TRUE);
} /* bWin32Prt_PrintBitmapFromMemory_g() */


int iWin32Prt_GetVPos_g(void)
{
  return(PrinterData_g.iVPos);
}

void vWin32Prt_SetPaperOffset_g(int iOffset)
{
  iOffset_m=iOffset;
}

int iWin32Prt_GetPaperWidth_g(void)
{
  /* Breite des Papiers in Pixel ausrechnen */
  return((int)((float)PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPrintQuality / 25.4 *
         (float)PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPaperWidth / 10 + 0.5) +
         ((PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmOrientation == DMORIENT_LANDSCAPE)?0:iOffset_m)); // /10 weil Zehntel millimeter, +0.5 zum runden, Offset wird bereits einberechnet);
}

int iWin32Prt_GetPaperLength_g(void)
{
  /* Länge des Papiers in Pixel ausrechnen */
  return((int)((float)PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPrintQuality / 25.4 *
         (float)PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmPaperLength / 10 + 0.5) +
         ((PrinterData_g.pPrinterInfo2[iPrinterIndex_g].pDevMode->dmOrientation == DMORIENT_LANDSCAPE)?iOffset_m:0)); // /10 weil Zehntel millimeter, +0.5 zum runden, Offset wird bereits einberechnet);
}
