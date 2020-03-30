#ifndef TM88_H_INCLUDED
  #define TM88_H_INCLUDED

#include <windows.h>
#include <WinSpool.h>

#include "EpsStmApi3.h"
#include "Win32Prt.h"


/**
 * Drucker initialisieren
 *
 * @param pcPrinterName
 *               Name des Druckers
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_Init_g(char *pcPrinterName);

/**
 * Schliesst die Bibliothek und gibt Speicher frei
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_CloseLib_g(void);

/**
 * Dump f�r Informationsausgabe der Druckerstucktur
 */
void vTM88_Dump_g(void);

/**
 * Startet ein neues Dokument, muss immer zuerst aufgerufen werden
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_StartDocument_g(void);
/**
 * Beendet das aktuelle bearbeiten und druckt das Dokument
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_EndDocument_g(void);

/**
 * Setzt eine neue Schriftart.<BR>
 * Als Font kann ein Drucker-Font oder ein Windows-Font
 * angegeben werden. Ein g�ltiger Drucker-Font kann mit der
 * Funktion bTM88_GetPrtFontName_g() ermittelt werden.
 *
 * @param pcFont Schriftart
 * @param iSize  Schriftgr�sse
 *
 * @return TRUE bei Erfolg, sonst FALSE
 * @see bTM88_GetPrtFontName_g
 */
BOOL bTM88_SetFont_g(const char *pcFont,int iSize);

/**
 * Gr�sse des Stifts f�r Zeichnen festlegen
 *
 * @param iWidth Gr�sse
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_DrawSetPenWidth_g(int iWidth);

/**
 * Den Linienabstand f�r newlines setzen in Abh�ngigkeit der Schriftgr�sse(Default: 1.1)
 *
 * @param fLineSpacing
 *               Linienabstandsfaktor
 */
void vTM88_SetLineSpacing_g(float fLineSpacing);

/**
 * Setzt Bold.
 * Diese Funktion IMMER vor bTM88_SetFont() aufrufen
 *
 * @param bBold  Bold
 * @see bTM88_SetFont
 */
void vTM88_SetBold_g(BOOL bBold);

/**
 * Setzt Italic.
 * Diese Funktion IMMER vor bTM88_SetFont() aufrufen
 *
 * @param bItalic Italic
 * @see bTM88_SetFont
 */
void vTM88_SetItalic_g(BOOL bItalic);

/**
 * Setzt Underline.
 * Diese Funktion IMMER vor bTM88_SetFont() aufrufen
 *
 * @param bUnderline Underline
 * @see bTM88_SetFont
 */
void vTM88_SetUnderline_g(BOOL bUnderline);

/**
 * Grenzt den Druckbereich ein, wenn beide Parameter 0 sind wird die Grenze zur�ckgesetzt.
 *
 * @param iMarginLeft
 *               Position von Links
 * @param iMarginWidth
 *               Breite der Grenze
 */
void vTM88_SetMargin_g(int iMarginLeft, int iMarginWidth);

/**
 * Schreibt eine Linie
 *
 * @param eAlignment Textausrichtung
 * @param pcText     Zu schreibenden Text
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_WriteLine_g(EAlignment eAlignment, const char *pcText, ...);

/**
 * An bestimmte Position mit spezifischer Textausrichtung schreiben
 *
 * @param iColumn    Position von Links
 * @param iRow       Position von Oben
 * @param eAlignment Textausrichtung
 * @param pcText     zu schreibenden Text
 *
 * @return Anz. geschriebener Linien
 */
int iTM88_WriteAt_g(int iColumn, int iRow,
                    EAlignment eAlignment,
                    const char *pcText, ...);

/**
 * Gibt die aktuelle, Vertikale Position zur�ck
 *
 * @return Vertikale Position
 */
int iTM88_GetVPos_g(void);


/**
 * Gibt den g�ltigen Namen eines Drucker-Fonts zur�ck<BR>
 *
 * Der Drucker-Font-Name hat folgendes Format:
 *
 * Font&lt;A|B&gt;&lt;width&gt;&lt;height&gt;
 *
 * folgende Fonts sind zul�ssig:<BR>
 * <PRE>
 *             +-----------+----------+----------+----------+
 *             | std size  | 2* width | 4* width | 8* width |
 * +-----------+-----------+----------+----------+----------+
 * | std size  |  FontA11  | FontA12  |          |          |
 * +-----------+-----------+----------+----------+----------+
 * | 2* height |  FontA21  | FontA22  | FontA24  |          |
 * +-----------+-----------+----------+----------+----------+
 * | 4* height |           | FontA42  | FontA44  |  FontA48 |
 * +-----------+-----------+----------+----------+----------+
 * | 8* height |           |          | FontA84  |  FontA88 |
 * +-----------+-----------+----------+----------+----------+
 * </PRE>
 *
 * @param iWidth  Breite des Fonts (1,2,4,8)
 * @param iHeight H�he des Fonts (1,2,4,8)
 * @param iStyle
 * @param pcFont
 *
 * @return TRUE wenn Fontname gefunden wurde oder FALSE wenn
 *         Fontname erst mit korrigierten Parametern gefunden
 *         werden konnte
 */
BOOL bTM88_GetPrtFontName_g(int iWidth,int iHeight,BOOL bBold,
                            int *piHeightPixel, char *pcFont);

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
BOOL bTM88_DrawLine_g(int iX1, int iY1,
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
BOOL bTM88_DrawRoundRect_g(int iLeft,int iTop,
                          int iRight,int iBottom,
                          int iRadius);

/**
 * Bitmap an Koordinate x,y ausdrucken
 *
 * @param pcFileName Dateiname der auszudruckenden Bitmap
 * @param iColumn    horizontale Koordinate
 * @param iRow       vertikale Koordinate
 * @param fScale     Skalierungsfaktor f�r Grafik (1.0=unver�ndert; 0.5=halbe Gr�sse)
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_PrintBitmapFromFile_g(const char *pcFileName,
                                int iLeft, int iTop,
                                float fScale);

/**
 * Bitmap an Koordinate x,y ausdrucken
 *
 * @param pcFileName Dateiname der auszudruckenden Bitmap
 * @param iColumn    horizontale Koordinate
 * @param iRow       vertikale Koordinate
 * @param fScale     Skalierungsfaktor f�r Grafik (1.0=unver�ndert; 0.5=halbe Gr�sse)
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
BOOL bTM88_PrintBitmapFromMemory_g(const char *pcBitmap,
                                  int iLeft, int iTop,
                                  float fScale);

/**
 * Gibt die Papierbreite zur�ck
 *
 * @return Papierbreite
 */
int iTM88_GetPaperWidth_g(void);

/**
 * Gibt die Papierl�nge zur�ck
 *
 * @return Papierl�nge
 */
int iTM88_GetPaperLength_g(void);


#endif // TM88_H_INCLUDED
