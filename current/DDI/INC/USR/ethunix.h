/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS ETH                                                   **
** Sub-Project  : IBS ETH DDI                                               **
**                                                                          **
*****************************************************************************/

/*
** File         : ETHUNIX.H
** Component(s) : -
** Device(s)    :
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   05 Oct 2001 12:37:58  $
**              :
**     $Revision:   1.3  $
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
*/

#ifndef INCL_ETHUNIX
  #define INCL_ETHUNIX

/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

  #if defined(SUN)
    #define ETHA_BIG_ENDIAN
    #define DONT_USE_PRAGMA_PACK

  #elif defined(HPUX)
    #define _HPUX_SOURCE
    #define ETHA_BIG_ENDIAN

  #elif defined(NEXT)
    #define ETHA_BIG_ENDIAN

  #elif defined(LINUX)
    #define ETHA_LITTLE_ENDIAN
    #define DONT_USE_PRAGMA_PACK

  #else
    #error NO TARGET SYSTEM DEFINED
  #endif


  #ifndef IBS_UNIX_VERSION
    #define IBS_UNIX_VERSION
  #endif


/*****************************************************************************
****  INCLUDE FILES                                                       ****
*****************************************************************************/

  #ifdef SUN
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <ctype.h>
    #include <sys/ddi.h>
    #include <sys/filio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <thread.h>
  #endif /*endif SUN */

  #ifdef LINUX
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <ctype.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
  #endif /*endif LINUX */

  #include "stdtypes.h"
  #include "ddi_usr.h"
  #include "ddi_macr.h"
  #include "ddi_err.h"
  #include "ddi_eth.h"
  #include "eth_err.h"
  #include "ibs_util.h"
  #include "ioctrl.h"
  #include "svc_code.h"
  #include "ethsecu.h"


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
 * Port number used by the server to call the DDI and management
 * functions.
 */
  #define  ETH_SERV_PORT      1962

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
 * Constant definitions for the socket function return values
 * for compatibility with the WIN socket.
 */
  #define  INVALID_SOCKET     -1
  #define  SOCKET_ERROR       -1

/*
 * Function macro for reading the last socket error message. Using
 * this macro achieves compatibility with the WIN socket.
 */
  #define  GetSocketError()      errno

  #define  ETH_TIMESTAMP         __DATE__

#endif /* INCL_ETHUNIX */


