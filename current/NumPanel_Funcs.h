#ifndef NUMPANEL_FUNCS_H_INCLUDED
	#define NUMPANEL_FUNCS_H_INCLUDED

/**
 *	Anwendungsbeispiel:
 *
 *	eNumPanelOpen_g(panel,control,eShowAll);
 *
 *  Anwendbar für Integer, Float und Double
 *	Bei Float und Double ist der Maximale Wert 10Mio -1
 *  Es wird der Maximum-Wert des Control als Limite gesetzt 
 *  Es wird der Range Checking berücksichtigt   
 *
 *	ENumPanel_DisplayOptions eShowLimits     = Blendet die Limite des Controls ein
 *													 eShowRangeCheck = Blendet den Range Check des Controls ein
 *													 eValueSelected  = Wert wird Markiert, erste Eingabe ersetzt Wert
 *
 *  Panel, Control und AnzeigeOption des gewünschten Eingabefeld angeben 
 *  Rc = eEnter   Neuer Wert geschrieben, 
 *			 eCancel  Kein neuer Wert geschrieben
 *       ePnlOpen Keine neue Instanz gestartet --> Panel ist schon geöffnet
 */


#ifndef EXXXPANEL_RETURN_CODE
	#define EXXXPANEL_RETURN_CODE

#ifndef BOOL
typedef int BOOL;
#endif // !BOOL

#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE !TRUE
#endif

typedef enum
{
	eEnter   = 1,
	eCancel  = 0,
	ePnlOpen = -1
} EXxxPanel_ReturnCode;
#endif //!EXXXPANEL_RETURN_CODE

typedef enum
{
	eShowLimits      = 1,
	eShowRangeCheck  = 2,
	eValueSelected	 = 4
} ENumPanel_DisplayOptions;

EXxxPanel_ReturnCode __cdecl eNumPanel_Open_g(int iPanel,												//__in PanelHandel des Eingabefeldes
																							int iControl, 										//__in Control des Eingabefeldes
																							ENumPanel_DisplayOptions eOptions //__in Welche Attribute auf dem Panel angezeigt werden sollen
																							);
																			
/**
 *	vNumPanel_Close_g()
 *
 *  Panel schliessen, Änderungen werden nicht übernommen
 */
void __cdecl vNumPanel_Close_g(void);

/**
 *	bNumPanel_Open_g()
 *
 *  @return TRUE  Panel ist geöffent
 *					FALSE Panel ist geschlossen
 */
BOOL __cdecl bNumPanel_Open_g(void);

#endif // !NUMPANEL_FUNCS_H_INCLUDED
 
