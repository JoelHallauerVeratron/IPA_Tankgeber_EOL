#ifndef ALPHAPANEL_FUNCS_H_INCLUDED
  #define ALPHAPANEL_FUNCS_H_INCLUDED

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
  eLowerCase      = 1,   //Nur Kleinbuchstaben
  eUpperCase      = 2,   //Nur Grossbuchstaben
  eNumbers        = 4,   //Nur Zahlen
  eDataName       = 8,   //Nur Zeichen für gülltige Dateinamen
  eSpecialChar    = 16,  //Nur Alle Sonderzeichen
  eStringSelected = 32   //String wird Selektiert -> Erste Eingabe ersetzt String
}EAlphaPanel_Options;

/**
 *  fCallbackFunktion: Wenn hier ein Pointer auf eine Callbackfunktion angegeben wird, wird
 *                     der String welchern man eigegibt bei jeder Änderung mit dem MainControl
 *                     aktualisiert. Bei jeder Aktualisierung wird ein EVENT_VAL_CHANGED event
 *                     simuliert. Ein NULL Pointer wenn erst bei OK aktualisiert werden soll.
 *  eOptions:          Der Parameter kann mit Bitweise | mit den EnumVariablen gefüllt werden.
 *                     Bei 0 werden keine Einschränkungen vorgenommen. eStringSelected ist
 *                     Defaultmässig ausgeschaltet
 *  ixAchse:           PanelPosition ausgehend von links.
 *  iyAchse:           PanelPosition ausgehend von oben.
 */

// Alpha Panel schliessen
BOOL __cdecl bAlphaPanel_Close_g(EXxxPanel_ReturnCode eRc);
// Prüft, ob Alpha Panel offen ist
BOOL __cdecl bAlphaPanel_IsPanelOpen_g(void);

EXxxPanel_ReturnCode __cdecl eAlphaPanel_Open_g(int iPanel,
                                                int iControl,
                                                EAlphaPanel_Options eOptions,
                                                int (*fCallbackFunktion)(),
                                                int iLeft,
                                                int iTop
                                                );

#endif // !ALPHAPANEL_FUNCS_H_INCLUDED

