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

#define  PNL_LPRT09                      1
#define  PNL_LPRT09_TXT_DESCR            2
#define  PNL_LPRT09_TXT_DATE             3
#define  PNL_LPRT09_BTN_PRINT            4       /* callback function: BtnPrint09CB */
#define  PNL_LPRT09_TXT_SNR              5
#define  PNL_LPRT09_TXT_RHI_NR           6
#define  PNL_LPRT09_BTN_CLOSE            7       /* callback function: BtnClose09CB */
#define  PNL_LPRT09_PIC_LABEL            8
#define  PNL_LPRT09_LED_CONNECTED        9
#define  PNL_LPRT09_TMR_STATUS           10      /* callback function: TmrStatus09CB */
#define  PNL_LPRT09_TEXTMSG_2            11


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK BtnClose09CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BtnPrint09CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TmrStatus09CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
