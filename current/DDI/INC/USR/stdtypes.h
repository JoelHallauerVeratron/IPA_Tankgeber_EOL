/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : -- (global)                                               **
** Sub-Project  : --                                                        **
**                                                                          **
*****************************************************************************/

/*
** File         : stdtypes.h
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   20 Aug 2001 13:27:40  $
**              :
**     $Revision:   1.22  $
**
** Status       : -
**              :
** Description  : unique data type definitions
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
** 12.07.2001  1.22   KKo           Add RTX 5.0 - Features
*/

#ifndef INCL_STDTYPES
  #define INCL_STDTYPES

  #define VOID         void
  #define INT8         signed char
  #define INT16        signed short
  #define INT32        signed long
  #define USIGN8       unsigned char
  #define USIGN16      unsigned short
  #define USIGN32      unsigned long
  #define PHO_FLOAT    float
  #define PHO_DOUBLE   double
  #define FLOAT        float
  #define DOUBLE       double
  #define CHAR         char
  #define PHO_BOOL     unsigned char
  #define MAXENUM      32767

/* Operating system depending constants defintions */
/* Constants used for DOS */
  #ifdef IBS_DOS_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_WIN_NOTIFY
    #define  PASCAL         _pascal
    #define  FAR            _far
    #define  IBDDIRET       INT16
    #define  IBDDIHND       INT16
    #define  IBDDIFUNC      _far
    #define  IBDRVHND       INT16
    #define  IBPTR          _far
    #define  ERR_BASE       0
    #define  IBAPIRET       INT16
    #define  IBAPIHND       INT16
    #define  IBAPIAPPLID    INT16
    #define  IBAPIFUNC      _far
    #define  IBAPIPALIGN    1
  #endif /* IBS_DOS_VERSION */

/* Constants used for Windows 3.x */
  #ifdef IBS_WIN_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_WIN_NOTIFY
    #define  IBDDIRET       INT16
    #define  IBDDIHND       INT16
    #define  IBDDIFUNC      FAR PASCAL
    #define  IBDRVHND       INT16
    #define  IBPTR          FAR
    #define  ERR_BASE       0
    #define  PCP_BOOL       USIGN8
    #define  IBAPIRET       INT16
    #define  IBAPIHND       INT16
    #define  IBAPIAPPLID    INT16
    #define  IBAPIFUNC      FAR PASCAL
    #define  IBAPIPALIGN    1
  #endif /* IBS_WIN_VERSION */

/* Constants used for Windows 95 */
  #ifdef IBS_WIN_95_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_WIN32_NOTIFY
    #define  IBDDIRET       DWORD
    #define  IBDDIHND       DWORD
    #define  IBDDIFUNC      WINAPI
    #define  IBDRVHND       INT16
    #define  IBPTR
    #define  ERR_BASE       0
    #define  PCP_BOOL       USIGN8
    #define  IBAPIRET       DWORD
    #define  IBAPIHND       DWORD
    #define  IBAPIAPPLID    DWORD
    #define  IBAPIFUNC      WINAPI
    #define  IBAPIPALIGN    2
  #endif /* IBS_WIN_95_VERSION */

/* Constants used for Windows NT */
  #ifdef IBS_WIN_NT_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_WIN32_NOTIFY
    #define  IBDDIRET       DWORD
    #define  IBDDIHND       DWORD
    #define  IBDDIFUNC      WINAPI
    #define  IBDRVHND       INT16
    #define  IBPTR
    #define  ERR_BASE       0xE0070000
    #define  PCP_BOOL       USIGN8
    #define  IBAPIRET       DWORD
    #define  IBAPIHND       DWORD
    #define  IBAPIAPPLID    DWORD
    #define  IBAPIFUNC      WINAPI
    #define  IBAPIPALIGN    2
  #endif /* IBS_WIN_NT_VERSION */

/* Constants used for OS/2*/
  #ifdef IBS_OS2_VERSION
    #define  IBDDIRET       INT32
    #define  IBDDIHND       INT32
    #define  IBDDIFUNC
    #define  IBDRVHND       INT32
    #define  IBPTR
    #define  ERR_BASE       0
    #define  IBAPIRET       DWORD
    #define  IBAPIHND       DWORD
    #define  IBAPIAPPLID    DWORD
    #define  IBAPIFUNC
    #define  IBAPIPALIGN    1
  #endif /* IBS_OS/2_VERSION */

/* Constants used for Unix systems */
  #ifdef IBS_UNIX_VERSION
    #define  T_IBDDI_NOTIFY T_ETH_NOTIFY_INFO
    #define  IBDDIRET       int
    #define  IBDDIHND       int
    #define  IBDDIFUNC
    #define  IBDRVHND       unsigned short
    #define  IBPTR
    #define  ERR_BASE       0
  #endif /* IBS_UNIX_VERSION */

  #ifdef IBS_VXWORKS_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_VXWORKS_NOTIFY
    #define  DWORD          USIGN32
    #define  VOID           void
    #define  IBDDIRET       USIGN32
    #define  IBDDIHND       int
    #define  IBDDIFUNC
    #define  IBDRVHND       INT16
    #define  IBPTR
    #define  ERR_BASE       0
    #define  IBAPIRET       USIGN32
    #define  IBAPIHND       int
    #define  IBAPIAPPLID    USIGN32


typedef struct
{
  USIGN8 * tname;
  INT32  tprio;
  INT32  tstsize;
  USIGN8 * tstack;
  USIGN32 ttid;
} T_TASK_DEF;

    #define NEW_EXC_HANDLING only for TEST */
  #endif /* IBS_VXWORKS_VERSION */


  #ifdef IBS_RTX_VERSION
    #define  T_IBDDI_NOTIFY T_IBS_WIN32_NOTIFY
    #define  DWORD          USIGN32
    #define  VOID           void
    #define  IBDDIRET       USIGN32
    #define  IBDDIHND       int
    #define  IBDDIFUNC       
    #define  IBDRVHND       INT16
    #define  IBPTR
    #define  ERR_BASE       0
    #define  IBAPIRET       USIGN32
    #define  IBAPIHND       int
    #define  IBAPIAPPLID    USIGN32
  #endif /* IBS_VXWORKS_VERSION */


#endif /* INCL_STDTYPES */

