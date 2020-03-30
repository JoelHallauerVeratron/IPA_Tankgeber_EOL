/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2020. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PNL_MAIN                        1       /* callback function: MainPanelCB */
#define  PNL_MAIN_LED_STOERUNG           2
#define  PNL_MAIN_LED_STOP               3
#define  PNL_MAIN_LED_RUN                4
#define  PNL_MAIN_RING_DEVICE            5       /* callback function: SelectDeviceCB */
#define  PNL_MAIN_DECORATION_2           6
#define  PNL_MAIN_TEXTMSG_2              7
#define  PNL_MAIN_TMR_SCHEDULER          8       /* callback function: SchedulerCB */
#define  PNL_MAIN_TXT_DESCR              9
#define  PNL_MAIN_BTN_NIO                10      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_IO                 11      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_PREV               12      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_ENTER              13      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_RES2               14      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_RES1               15      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_NEXT               16      /* callback function: BoxCB */
#define  PNL_MAIN_LED_PASS               17
#define  PNL_MAIN_LED_FAIL               18
#define  PNL_MAIN_SIM_BOX2               19
#define  PNL_MAIN_SIM_BOX                20
#define  PNL_MAIN_BTN_QUIT               21      /* callback function: BoxCB */
#define  PNL_MAIN_BTN_START              22      /* callback function: BoxCB */
#define  PNL_MAIN_NUM_STEP               23
#define  PNL_MAIN_BTN_MENU               24      /* callback function: TouchMainMenuCB */
#define  PNL_MAIN_TEXTMSG_4              25
#define  PNL_MAIN_TEXTMSG                26
#define  PNL_MAIN_GUI                    27
#define  PNL_MAIN_TXT_USER               28
#define  PNL_MAIN_NUM_SERIAL             29
#define  PNL_MAIN_PICTURE                30
#define  PNL_MAIN_TEXTMSG_3              31
#define  PNL_MAIN_LED_NET_STATE          32

#define  PNL_MENU                        2
#define  PNL_MENU_BTN_ABOUT              2       /* callback function: TouchMenuCB */
#define  PNL_MENU_BTN_EXIT               3       /* callback function: TouchMenuCB */
#define  PNL_MENU_BTN_DIAG_PRT9          4       /* callback function: TouchMenuCB */
#define  PNL_MENU_BTN_DIAG_IO            5       /* callback function: TouchMenuCB */
#define  PNL_MENU_BTN_TEST_FKT           6       /* callback function: TouchMenuCB */
#define  PNL_MENU_BTN_DIAG_CAN           7       /* callback function: TouchMenuCB */
#define  PNL_MENU_TEXTMSG                8

#define  PNL_TESTS                       3
#define  PNL_TESTS_SW_TEST_LED           2       /* callback function: SwitchCB */
#define  PNL_TESTS_SW_TEST_NFC           3       /* callback function: SwitchCB */
#define  PNL_TESTS_SW_TEST_AI            4       /* callback function: SwitchCB */
#define  PNL_TESTS_SW_TEST_SW_VERSION    5       /* callback function: SwitchCB */
#define  PNL_TESTS_BTN_OK                6       /* callback function: SwitchCB */
#define  PNL_TESTS_TEXTMSG_4             7
#define  PNL_TESTS_DECORATION_4          8
#define  PNL_TESTS_DECORATION_2          9
#define  PNL_TESTS_TEXTMSG_2             10


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK BoxCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SchedulerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SelectDeviceCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SwitchCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TouchMainMenuCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TouchMenuCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
