/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS ETH                                                   **
** Sub-Project  : IBS ETH DDI                                               **
**                                                                          **
*****************************************************************************/

/*
** File         : DDI_ETH.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   04 Oct 2001 15:40:10  $
**              :
**     $Revision:   1.1  $
** Date         :
**              :
** Description  :
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
*/


#ifndef INCL_DDI_ETH
  #define INCL_DDI_ETH


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Defines for the various notification modes
 */
  #define  ETH_NOTIFY_MODE_1      1       /* Notification mode 1. In this     */
/* mode, a DDI_MXI_RcvMessage       */
/* request is held on the server    */
/* until a message appears          */
/* the timeout period has elapsed.  */
  #define  ETH_NOTIFY_MODE_2      2       /* In this mode, the server sends   */
/* messages independently to the    */
/* client, who only interrogates    */
/* them locally. No requests are    */
/* sent to the server.              */
  #define  ETH_NOTIFY_ABORT       0xFEDC  /* Terminates notification mode     */
/* by means of an MXI data channel  */
/* (not the one used for            */
/* activation).                     */


/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

/* compiler option: word alignment in structures */
  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack(2)
  #endif

/* Notification structure for the IBS ETH controller */
typedef struct
{
  USIGN32     mode;       /* Selects notification mode                    */
  USIGN32     threadId;   /* Thread identifier                            */
  USIGN32     processId;  /* Process identifier                           */
  USIGN32     timeout;    /* Timeout value in milliseconds                */
} T_ETH_NOTIFY_INFO;




  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack()
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

#endif    /* (ifndef INCL_DDI_ETH) */
