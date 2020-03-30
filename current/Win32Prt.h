#ifndef WIN32_PRT_H_INCLUDED
  #define WIN32_PRT_H_INCLUDED

#include <windows.h>
#include <WinSpool.h>

#define MAX_TEXT_LENGTH 500

typedef struct
{
  BOOL  bDocumentStarted;
  HDC   hDC;            // Handle f�r Device Context
  int   iPrtMonHandle;  // Handle f�r Printer f�r Bi...-Funktionen aus EpsStmApi3.h

  char  caFont[100];    // aktueller Font-Name
  int   iSize;          // aktuelle Font-Gr�sse
  HFONT hFont;          // Handle auf aktuellen Font
  HFONT hOldFont;       // Handle auf urspr�nglichen Font
  float fLineSpacing;
  BOOL  bBold;
  BOOL  bItalic;
  BOOL  bUnderline;

  int   iVPos;          // vertikale Position
  DOCINFO di;
  PRINTER_INFO_2 *pPrinterInfo2;
} tagWin32PrtData;

typedef enum
{
  eAlignmentLeft,
  eAlignmentRight,
  eAlignmentCenter,
  eAlignmentCenteredOnPoint
}EAlignment;

extern tagWin32PrtData PrinterData_g;
extern int iPrinterIndex_g;

/**
 * Dump f�r Informationsausgabe der Druckerstucktur
 */
void vWin32Prt_Dump_g(void);

/**
 * Startet ein neues Dokument, muss immer zuerst aufgerufen werden
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_StartDocument_g(void);
/**
 * Beendet das aktuelle bearbeiten und druckt das Dokument
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_EndDocument_g(void);

/**
 * Setzt eine neue Schriftart
 *
 * @param pcFont Schriftart
 * @param iSize  Schriftgr�sse
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_SetFont_g(const char *pcFont,int iSize);

/**
 * Gr�sse des Stifts f�r Zeichnen festlegen
 *
 * @param iWidth Gr�sse
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_DrawSetPenWidth_g(int iWidth);

/**
 * Den Linienabstand f�r newlines setzen in Abh�ngigkeit der Schriftgr�sse(Default: 1.1)
 *
 * @param fLineSpacing
 *               Linienabstandsfaktor
 */
void vWin32Prt_SetLineSpacing_g(float fLineSpacing);

/**
 * Option Fett der Schriftart setzen
 *
 * @param bBold  Fett setzen
 */
void vWin32Prt_SetBold_g(BOOL bBold);
/**
 * Option Kursiv f�r Schriftart setzen
 *
 * @param bItalic Kursiv setzen
 */
void vWin32Prt_SetItalic_g(BOOL bItalic);
/**
 * Option unterstrichen f�r Schriftart setzen
 *
 * @param bUnderline Unterstrichen setzen
 */
void vWin32Prt_SetUnderline_g(BOOL bUnderline);


/**
 * Grenzt den Druckbereich ein, wenn beide Parameter 0 sind wird die Grenze zur�ckgesetzt auf die Papiergr�sse.
 * Standardm�ssig werden die beiden R�nder der Papiergr�sse als Grenzen genommen.
 *
 * @param iMarginLeft
 *               Offset von Linker Seite
 * @param iMarginWidth
 *               Breite der Grenze
 */
void vWin32Prt_SetMargin_g(int iMarginLeft, int iMarginWidth);

/**
 * Schreibt eine Linie
 *
 * @param eAlignment Textausrichtung
 * @param pcText     Zu schreibenden Text
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_WriteLine_g(EAlignment eAlignment, const char *pcText, ...);

/**
 * An bestimmte Position mit spezifischer Textausrichtung schreiben.
 * Bei Alginment Center wird die Variable iColumn ignoriert und alles wird in die Mitte zentriert geschrieben.
 *
 * @param iColumn    Position von Links
 * @param iRow       Position von Oben
 * @param eAlignment Textausrichtung
 * @param pcText     zu schreibenden Text
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL iWin32Prt_WriteAt_g(int iColumn, int iRow,
                         EAlignment eAlignment,
                         const char *pcText, ...);

/**
 * Gibt die aktuelle, Vertikale Position zur�ck
 *
 * @return Vertikale Position
 */
int iWin32Prt_GetVPos_g(void);


/**
 * Eine Linie zeichnen
 *
 * @param iX1    Anfangsposition X
 * @param iY1    Anfangsposition Y
 * @param iX2    Zielposition X
 * @param iY2    Zielposition Y
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_DrawLine_g(int iX1, int iY1,
                          int iX2, int iY2);

/**
 * Rechteck mit abgerundeten Ecken zeichnen
 *
 * @param iLeft   Position von links
 * @param iTop    Position von oben
 * @param iRight  Position der rechten seite
 * @param iBottom Position der unteren Seite
 * @param iRadius Eckenradius
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_DrawRoundRect_g(int iLeft,int iTop,
                               int iRight,int iBottom,
                               int iRadius);

/**
 * Druckt ein Bitmap aus einer Datei
 *
 * @param pcFileName Filename
 * @param iLeft      Position von Links
 * @param iTop       Position von Oben
 * @param fScale     Skallierung
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_PrintBitmapFromFile_g(const char *pcFileName,
                                     int iLeft, int iTop,
                                     float fScale);

/**
 * Druckt ein Bitmap direkt aus dem Speicher
 *
 * @param pcBitmap Pointer auf Bitmap
 * @param iLeft    Position von Links
 * @param iTop     Position von Oben
 * @param fScale   Skallierung
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bWin32Prt_PrintBitmapFromMemory_g(const char *pcBitmap,
                                       int iLeft, int iTop,
                                       float fScale);

/**
 * Setzt einen Offset f�r die Papiergr�sse, verhindert dass der Text ev. abgeschnitten wird.
 *
 * @param iOffset Offset(positiv oder negativ, wird zur Papiergr�sse addiert)
 */
void vWin32Prt_SetPaperOffset_g(int iOffset);

/**
 * Gibt die Papierbreite zur�ck
 *
 * @return Papierbreite
 */
int iWin32Prt_GetPaperWidth_g(void);

/**
 * Gibt die Papierl�nge zur�ck
 *
 * @return Papierl�nge
 */
int iWin32Prt_GetPaperLength_g(void);



#endif //WIN32_PRT_H_INCLUDED
