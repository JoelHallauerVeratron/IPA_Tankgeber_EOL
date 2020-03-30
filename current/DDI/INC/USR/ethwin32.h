/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS ETH                                                   **
** Sub-Project  : IBS ETH  DDI                                              **
**                                                                          **
*****************************************************************************/

/*
** File         : ETHWIN32.H
** Component(s) : -
** Device(s)    :
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   05 Oct 2001 08:02:30  $
**              :
**     $Revision:   1.5  $
**
** Status       :
**              :
** Description  : All required header files are included by this file.
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
**
*/

#ifndef INCL_ETHWIN32
  #define INCL_ETHWIN32

/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Constant definitions for the return value, node handle and
 * function-specific code words
 */

/* Switch to INTEL format */
  #define ETHA_LITTLE_ENDIAN

  #define  IBS_WIN_NT_VERSION

  #ifndef ETH_WIN32
    #define  ETH_WIN32
  #endif

/*****************************************************************************
****  INCLUDE FILES                                                       ****
*****************************************************************************/

/* WIN socket includes */
  #include <windows.h>
  #include <winsock.h>
  #include <stdio.h>
  #include <time.h>

  #pragma pack (2)

  #include "stdtypes.h"
  #include "ddi_usr.h"
  #include "ddi_macr.h"
  #include "ddi_err.h"
  #include "ddi_win.h"
  #include "eth_err.h"
  #include "ibs_util.h"
  #include "ioctrl.h"
/*#include "msg_code.h"*/

  #include "eth_mng.h"
  #include "ethsecu.h"

  #pragma pack ()

/*
 * Macros for adapting the data representation on
 * different processors
 */
  #ifdef ETHA_LITTLE_ENDIAN
    #ifndef HIBYTE
      #define  HIBYTE(w)      (unsigned char)(w >> 8)
    #endif /* endif HIBYTE */
    #ifndef LOBYTE
      #define  LOBYTE(w)      (unsigned char)(w)
    #endif /* endif LOBYTE */
    #define  SWAPWORD(w)    (HIBYTE(w) | (LOBYTE(w) << 8))

    #define  SET_WORD(a)    SWAPWORD(a)
    #define  GET_WORD(a)    SWAPWORD(a)
  #else /* if not, then BIG Endian */
    #define  SET_WORD(a)    a
    #define  GET_WORD(a)    a
  #endif /* endif ETAH_LITTLE_ENDIAN */

/*
 * WIN socket-specific error constants
 */
  #define  EINTR          WSAEINTR
  #define  ECONNRESET     WSAECONNRESET
  #define  EINPROGRESS    WSAEINPROGRESS

  #define  IPPORT_USERRESERVED     IPPORT_RESERVED

/*
 * Port number used by the server to call the DDI and management
 * functions.
 */
  #define  ETH_SERV_PORT      1962

/*
 * IBSETHA file path name when used under Windows. This
 * path name can be adapted to individual requirements.
 */
  #define  ETHA_WIN_IBSETHA_PATH  ".\\"

  #ifndef TRUE
    #define  TRUE    1
  #endif /* endif TRUE */
  #ifndef FALSE
    #define  FALSE   0
  #endif /* endif FALSE */

  #ifndef INADDR_NONE
    #define  INADDR_NONE    0xFFFFFFFF
  #endif /* endif INADDR_NONE */

/*
 * The bzero and bcopy functions are replaced with functions
 * which can be found on any computer.
 */
  #ifndef bzero
    #define  bzero(m, n)    memset(m, 0, n)
  #endif /* endif bzero */
  #ifndef bcopy
    #define  bcopy(m, n, o) memcpy(n, m, o)
  #endif /* endif bcopy */

/*
 * Function macro for reading the last socket error message. Using
 * this macro achieves compatibility with the WIN socket.
 */
  #define  GetSocketError()      WSAGetLastError()

/*
 * The close UNIX function, which is used to close a socket,
 * is mapped onto the closesocket WIN socket function.
 */
  #define  close(a)              closesocket(a)

/*
 * The ioctl UNIX function for setting non-blocking mode
 * is mapped onto the ioctlsocket WIN socket function.
 */
  #define  ioctl(a, b, c)         ioctlsocket(a, b, c)

  #define  ETH_TIMESTAMP         __TIMESTAMP__

#endif /* INCL_ETHWIN32 */
