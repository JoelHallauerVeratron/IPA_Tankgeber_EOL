/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                  Copyright Phoenix Contact 1998                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS ETH                                                   **
** Sub-Project  : IBS ETH DDI                                               **
**                                                                          **
*****************************************************************************/

/*
** File         : ETHSECU.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   : 
** Author       : 
**         $Date:   05 Oct 2001 07:58:16  $
**              :
**     $Revision:   1.7  $
** Status       :
**              :
** Description  : Function and structure declarations for IBS ETH access
**              : management.
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
**
*/

#ifndef INCL_ETHSECU
  #define INCL_ETHSECU


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Constants for the different security levels
 */

  #define SEC_LEVEL_NO_SEC               0x0000  /* No security               */
  #define SEC_LEVEL_PASSWORD             0x0001  /* Password protection       */
  #define SEC_LEVEL_IP                   0x0002  /* Access only for selected  */ 
/* IP addresses
              */
  #define SEC_LEVEL_PASSWORD_AND_IP      (SEC_LEVEL_PASSWORD | SEC_LEVEL_IP )
/* Password protection and   */
/* IP addresses              */

/*
 * other constants
 */
  #define SEC_MIN_LENGTH_OF_PASSWORD     8         /*  8 bytes minimum          */
  #define SEC_MAX_LENGTH_OF_PASSWORD    16         /* 16 bytes maximum          */
  #define SEC_LENGTH_OF_PASSWORD        SEC_MAX_LENGTH_OF_PASSWORD

  #define SEC_MAX_NUM_OF_IP_ADDR      32         /* number of IP addresses    */ 


  #define OPTION_ENCRYPT           0x0100
  #define OPTION_CLEAR_CRYPT       0x0200



/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack(2)
  #endif

typedef struct
{
  USIGN16 options;                /* options                              */
  USIGN16 lengthOfPassword;       /* length of password                   */
  CHAR*   password;               /* pointer to password                  */
} T_ETH_SEC_LOGIN;



typedef struct
{
  USIGN16  options;               /* options                              */
  USIGN16  securityLevel;         /* security level                       */
  USIGN16  lengthOfPassword;      /* length of password                   */
  CHAR*    password;              /* pointer to password                  */
  USIGN16  numOfEntries;          /* number of following IP addresses     */
  struct in_addr *listOfIPAddr;   /* pointer to IP address buffer         */
} T_ETH_SEC_INFO;


  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack()
  #endif

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/


extern IBDDIRET IBDDIFUNC  ETH_SecurityLogin(IBDDIHND nodeHd, CHAR *password,
                                             USIGN16 length, 
                                             USIGN16 options);

extern IBDDIRET IBDDIFUNC  ETH_SetSecurity(IBDDIHND nodeHd,
                                           T_ETH_SEC_INFO *securityInfoPtr);

extern IBDDIRET IBDDIFUNC  ETH_GetSecurity(IBDDIHND nodeHd,
                                           T_ETH_SEC_INFO *securityInfoPtr );


/*****************************************************************************
****  GLOBAL DATA DECLARATIONS (EXTERNAL LINKAGE)                         ****
*****************************************************************************/


#endif    /* (ifndef INCL_ETHSECU) */



