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
#ifndef LIB_TESTER_GUI_MGR_H_INCLUDED
	#define LIB_TESTER_GUI_MGR_H_INCLUDED
	#ifdef LIB_TESTER_GUI_XXXX_INCLUDED
	 	#error LibTesterGui_Mgr_export.h muss vor allen LibTesterGui_nnnn_export.h included werden!!!
	#endif

	#include <windows.h>

	BOOL bLibTesterGuiMgr_Configure(int iPanel, int iControl, BOOL bEnableAnimation, BOOL bStepTimeVisible, BOOL bUseSummaryPanel);
	void bLibTesterGuiMgr_Discard(void);

	int  iLibTesterGuiMgr_AddGui(ELibTesterGui_Types eLibTesterGui_Type);
	BOOL bLibTesterGuiMgr_DeleteGui(ELibTesterGui_Types eLibTesterGui_Type, int iIndex);
  BOOL bLibTesterGuiMgr_DeleteAllGuis(void);

  void vLibTesterGuiMgr_ProcessStart(void);
  void vLibTesterGuiMgr_ProcessStop(BOOL bPass);

	BOOL bLibTesterGuiMgr_DisplayAllGuis(BOOL bDisplay);

	int  iLibTesterGuiMgr_SetPanelStatus(ELibTesterGui_Types eLibTesterGui_Type, int iIndex, EPanelStatus ePanelStatus, BOOL bVisible);

  void vLibTesterGuiMgr_SetAnimationTime(double dAnimationTime);

  void vLibTesterGuiMgr_SetPrintCmd(BOOL bPrintCmd);
#endif //LIB_TESTER_GUI_MGR_H_INCLUDED

