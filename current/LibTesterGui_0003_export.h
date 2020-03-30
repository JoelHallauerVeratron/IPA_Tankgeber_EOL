#ifndef UNIVERSALGUI_DEFINES_H_INCLUDED  //Muss im Projekt 0000 angepasst werden
	#define UNIVERSALGUI_DEFINES_H_INCLUDED
	
  typedef enum
	{
		eIdle 	 =0,
		eRunning =1,
		ePass		 =2,
		eFail		 =3
	}EPanelStatus;

	typedef struct
	{
		int iHeight;
		int iWidth;
	}TagPnlSize;

	typedef enum
	{
	  eAll           =-1, // Alle Guis ansprechen
		eNone          = 0,
		eLibTesterGui_0001, // WorkOrder
		eLibTesterGui_0002, // IDS uEye USB Camera
		eLibTesterGui_0003, // Strommessung
		eLibTesterGui_0004, // Digital Inp/Out
		eLibTesterGui_0005, // Analog Inp/Out
		eLibTesterGui_0006, // Progress
		eLibTesterGui_0007, // Aktion
		eLibTesterGui_0008, // Produktions Daten
		eLibTesterGui_0009, // Summary
		eLibTesterGui_0010, // Dichtheitsprüfung
		eLibTesterGui_0011, // InvisibleWorkOrder
		eLibTesterGui_0012, // n/a
		eLibTesterGui_0013, // n/a
		eLibTesterGui_0014, // n/a
		eLibTesterGui_0015, // n/a
		eLibTesterGui_0016, // n/a
		eLibTesterGui_0017, // n/a
		eLibTesterGui_0018, // n/a
		eLibTesterGui_0019, // n/a
		eLibTesterGui_0020, // n/a
		eLibTesterGui_XXXX  // Für Template erforderlich
	}ELibTesterGui_Types;

#endif //UNIVERSALGUI_DEFINES_H_INCLUDED
#ifndef LIB_TESTER_GUI_TEST_0003_H_INCLUDED  //Alle Änderungen in der Datei _Funcs.h durchführen
	#define LIB_TESTER_GUI_TEST_0003_H_INCLUDED
	#define LIB_TESTER_GUI_XXXX_INCLUDED

  #include <windows.h>
  typedef struct
  {
    /**
     * Setzt den Titel des Gui's
     * Wird bei jedem Panelstatus durch
     * vLibTesterGui_0002_VisualUpdate() aktualisiert
     *
     * @author Juerg (19.01.2015)
     * @param iIndex  Gui Index
     * @param pcTitle Format für Titel
     * @see printf() formats
     */
    void (* pFvSetTitle)			  (int iIndex, char *pcTitle, ...);

    /**
     * Setzt den Min -und Max Wert
     *
     * @author Juerg (19.01.2015)
     * @param iIndex  Gui Index
     * @param dIMin Minimalwert
     * @param dIMax Maximalwert
     */
    void (* pFvSetMinMax)				(int iIndex, double dIMin,double dIMax);

    /**
     * Setzt den Istwert
     *
     * @author Juerg (19.01.2015)
     * @param iIndex  Gui Index
     * @param dICurrent Istwert
     */
  	void (* pFvSetCurrent)			(int iIndex, double dICurrent);

    /**
     * Setzt die Masseinheit
     *
     * @author Juerg (19.01.2015)
     * @param iIndex  Gui Index
     * @param pcUnit Masseinheit
     */
  	void (* pFvSetUnit)					(int iIndex, char *pcUnit);
  }TagGuiTest0003;

  TagGuiTest0003 *ptagLibTesterGui_0003_Init(int iParentPanel, char *pcTitle);
	void vLibTesterGui_0003_Discard(TagGuiTest0003 *ptagGuiTest0003, BOOL bDiscardAll)	;
  void vLibTesterGui_0003_VisualUpdate(int iIndex);
  void vLibTesterGui_0003_GetInfo(int iIndex,
                             			char *pcTesterGuiInfo,
                             			UINT *puiTesterGuiId,
                             			char *pcVersion,
                             			char *pcBuild);

#endif //LIB_TESTER_GUI_TEST_0003_H_INCLUDED

