/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2019. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PNLKWP2000                      1       /* callback function: PanelKwp2000CB */
#define  PNLKWP2000_BTN_TRACE_COPY       2       /* callback function: TraceCopyCB */
#define  PNLKWP2000_BTN_TRACE_CLEAR      3       /* callback function: TraceClearCB */
#define  PNLKWP2000_TRACE                4
#define  PNLKWP2000_BTN_TRACE_PAUSE      5       /* callback function: TracePauseCB */
#define  PNLKWP2000_TXT_HEADER           6
#define  PNLKWP2000_NUM_TRACE_BUFFER     7
#define  PNLKWP2000_CHK_TRACE_ENABLE     8       /* callback function: ChkTraceEnableCB */
#define  PNLKWP2000_TMR_UPDATE           9       /* callback function: TmrUpdateCB */
#define  PNLKWP2000_NUM_LIBDUTCOM        10
#define  PNLKWP2000_BTN_LIBDUTCOM_LOAD   11      /* callback function: BtnLibDutComLoad */
#define  PNLKWP2000_TXT_LIBDUTCOM_INFO   12


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK BtnLibDutComLoad(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ChkTraceEnableCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelKwp2000CB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TmrUpdateCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TraceClearCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TraceCopyCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TracePauseCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
