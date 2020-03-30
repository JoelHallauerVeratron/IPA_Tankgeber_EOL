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
#ifndef LIB_TESTER_GUI_TEST_0001_H_INCLUDED  //Alle Änderungen in der Datei _Funcs.h durchführen
  #define LIB_TESTER_GUI_TEST_0001_H_INCLUDED
  #define LIB_TESTER_GUI_XXXX_INCLUDED

  #include <windows.h>

  typedef enum
  {
    eSizeToImage,     // Make the control conform to the size of the image.
    eSizeToPicture,   // Make the picture conform to the size of the control.
    ePictCorner,      // Place the picture image at the top-left corner of the control.
    ePictCenter,      // Center the picture image in the control.
    ePictTitle,       // Tile or repeat the image so that it fills up the control.
  }EFixMode;

  typedef struct
  {
    int iLeft;
    int iTop;
    int iWidth;
    int iHeight;
  }TagPictureSize;

  typedef struct
  {
    /**
     * Setzt den Titel des Gui's
     * Wird bei jedem Panel Status durch
     * vLibTesterGui_0002_VisualUpdate() aktualisiert
     *
     * @author Juerg (19.01.2015)
     * @param iIndex  Gui Index
     * @param pcTitle Format für Titel
     * @see printf() formats
     */
    void    (* pFvSetTitle)     (int iIndex, char *pcTitle, ...);

    /**
     * Setzt ein WorkOrder
     *
     * @author Juerg (19.01.2015)
     * @param iIndex Gui Index
     * @param pcPath Dateiname welches Bild geladen werden soll
     * @return TRUE bei Erfolg,<br>
     *         sonst FALSE
     */
    BOOL    (* pFbSetWorkOrder) (int iIndex, char *pcPath);

    /**
     * Skaliert das Bild im Control
     *
     * @author Juerg (01.04.2015)
     * @param iIndex Gui Index
     * @param eFixMode FixMode Attribute
     * @return TRUE bei Erfolg,<br>
     *         sonst FALSE
     */
    BOOL    (* pFbSetFixMode)   (int iIndex,EFixMode eFixMode);

    /**
     * Fügt ein Bild hinzu
     *
     * @author GaS (17.08.2015)
     * @param iIndex Gui Index
     * @param pcPath Pfad zu einzufügendem Bild
     * @param ptagImageSize Pointer auf Struktur, die Daten zur Bildgrösse und Position enthält.<br>
     * Es kann als Höhe/Breite 0 angegeben werden für die originale Grösse der Bilddatei,<br>
     * -1 führt dazu, dass die Seite proportional zur anderen mitskaliert wird.<br>
     * Nur eine Seite darf als Parameter -1 haben.<br>
     * F&uuml;r einen transparenter Hintergrund muss eine .ico Datei (max. 8bpp, 1-bit Alpha,
     * 256 Slot Palette) gewählt werden.
     * @return Index der aktuell eingefügten Grafik <br>
     * oder -1 bei Fehler
     */
    int     (* pfiInsertPicture)  (int iIndex, const char *pcPath, TagPictureSize *ptagImagesize);

    /**
     * Entfernt ein eingefügtes Bild
     *
     * @author GaS (18.08.2015)
     * @param iIndex Gui Index
     * @param iPictureID ID des zu entfernenden Bildes, bei -1 werden alle eingefügten Bilder entfernt
     * @return TRUE bei Erfolg, sonst FALSE
     */
    BOOL    (* pfbRemovePicture)  (int iIndex, int iPictureID);

    /**
     * Ersetzt ein bereits bestehendes Bild mit einem anderen, Position und Grösse bleiben erhalten.
     * Mit dem iPictureID Parameterwert =-1 werden alle Bilder angesprochen
     *
     * @author GaS(24.08.2015)
     * @param iIndex Gui index
     * @param pcPath Pfad zum neuen Bild
     * @param iPictureID ID des zu ersetzenden Bildes
     * @return TRUE bei Erfolg, sonst FALSE
     *
     */
    BOOL    (* pfbReplacePicture) (int iIndex, const char *pcPath, int iPictureID);

    /**
     * Schreibt einen Text in die Grafik.
     *
     * @author GaS(29.06.2017)
     * @param iIndex Gui index
     * @param pcPath Pfad zum neuen Bild
     * @param iPictureID ID des zu ersetzenden Bildes
     * @return TRUE bei Erfolg, sonst FALSE
     *
     */
    BOOL    (* pfbWriteText) (int iIndex, const char *pcText);
  }TagGuiTest0001;

  TagGuiTest0001 *ptagLibTesterGui_0001_Init(int iParentPanel, char *pcTitle);
  void vLibTesterGui_0001_Discard(TagGuiTest0001 *ptagGuiTest0001, BOOL bDiscardAll);
  void vLibTesterGui_0001_VisualUpdate(int iIndex);
  void vLibTesterGui_0001_GetInfo(int iIndex,
                                  char *pcTesterGuiInfo,
                                  UINT *puiTesterGuiId,
                                  char *pcVersion,
                                  char *pcBuild);

#endif //LIB_TESTER_GUI_TEST_0001_H_INCLUDED
