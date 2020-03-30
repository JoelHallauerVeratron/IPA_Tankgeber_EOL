#ifndef SELECTORPANEL_FUNCS_H_INCLUDED
	#define SELECTORPANEL_FUNCS_H_INCLUDED
	#include "AlphaPanel_Funcs.h"

/*
	Die Header SelectorPanel_M.h und SelectorPanel_T.h werden nicht benötigt. Der Inhalt der defines und der Callbacks muss in den Headern
	SelectorPanel_T.h
	SelectorPanel_M.h
	SelectorPanel.h
	identisch sein!!!
*/

/**
 *	bUseTouchPanel: 	 Der SelectorPanel enthält zwei uir's. Eine Version für Touchdisplay, die Andere
 *										 für Computer mit Maus (mit oder ohne Tastatur). Die virtuelle Tastatur erscheint bei 
 *										 bUseTouchPanel=TRUE bei Links- sowie Rechtklick. Bei False nur bei Rechsklick
 *
 *	iXPanelPos:				 PanelPosition ausgehend von links.  
 *										 Bei iXPanelPos<0 = iXPanelPos=Height+Top des Main Controls.									 
 *	iYPanelPos:				 PanelPosition ausgehend von oben.  
 *										 Bei iYPanelPos<0 iXPanelPos=Left des Main Controls.									 
 *
 */
EXxxPanel_ReturnCode  __cdecl eSelectorPanel_Open_g(int iPanel, int iControl, BOOL bUseTouchPanel, int iXPanelPos,int iYPanelPos);

#endif // !SELECTORPANEL_FUNCS_H_INCLUDED
 
