/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS Controller Boards                                     **
** Sub-Project  : --                                                        **
**                                                                          **
*****************************************************************************/

/*
** File         : DDI_WIN.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   : J.M.
** Author       : J.M.
**         $Date:   05 Oct 2001 07:57:16  $
**              :
**     $Revision:   1.9  $
**              :
** Description  : Declarations for IBS PC Boards Device Driver
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
** 12/13/93    0.9    J.M.          Start working on this file (first version)
**
** 03/07/96    1.0    J.M.          Changing the return and handle data types
**                                  to symbolic constants. These constants are
**                                  are equal to the data types used before.
**
** 05/30/96    1.1    J.M.          A new notification info structure for
**                                  Windows 95 and NT (T_IBS_WIN32_NOTIFY)
*/


#ifndef INCL_DDI_WIN
  #define INCL_DDI_WIN


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

/* Change to word alignment */
  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack (2)
  #endif


typedef struct
{
  HWND    hWnd;       /* Handle of the dest. windows for the message      */
  UINT    msg;        /* Windows message code to use by the notification  */
  WPARAM  firstParam; /* First parameter                                  */
  LPARAM  secondParam;/* Second parameter                                 */
} T_IBS_WIN_NOTIFY;

typedef struct
{
  DWORD  processId;   /* Process identifier                               */
  DWORD  threadId;    /* Thread identifier                                */
  USIGN32  timeout;   /* Timeout interval in milliseconds                 */
} T_IBS_WIN32_NOTIFY;



  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack ()
  #endif

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

extern IBDDIRET IBDDIFUNC  DDI_SetMsgNotification(IBDDIHND  nodeHd,
                                                  T_IBDDI_NOTIFY IBPTR *notifyInfoPtr);
extern IBDDIRET IBDDIFUNC  DDI_ClrMsgNotification(IBDDIHND  nodeHd,
                                                  T_IBDDI_NOTIFY IBPTR *notifyInfoPtr);


/*****************************************************************************
****  GLOBAL DATA DECLARATIONS (EXTERNAL LINKAGE)                         ****
*****************************************************************************/

/* No global data declarations */

#endif    /* (ifndef INCL_DDI_WIN) */
