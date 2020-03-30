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
** File         : DDI_USR.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   : J.M.
** Author       : J.M.
**         $Date:   05 Oct 2001 07:55:36  $
**              :
**     $Revision:   1.20  $
**              :
** Description  : Function prototype declarations for DDI functions
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
** 07/22/93    0.9    J.M.          Start working on this file (first version)
**
** 03/07/96    1.0    J.M.          DDI function return values modified. Data
**                                  types modified to improve portability.
** 16.10.00    1.18   Thw           Add constants for bit access
*/


#ifndef INCL_DDI_USR
  #define INCL_DDI_USR


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/* Parameter defines for access control when opening nodes                  */
  #define  DDI_READ               0x0001  /* Read-only access                 */
  #define  DDI_WRITE              0x0002  /* Write-only access                */
  #define  DDI_RW                 0x0003  /* Read and write access            */

/* Defines for accessing various data consistencies */
  #define  DTI_DATA_WORD          0x0000  /* Word data consistency (16 bits)  */
  #define  DTI_DATA_LWORD         0x0001  /* Long word data consist. (32 bits)*/
  #define  DTI_DATA_BYTE          0x0002  /* Byte data consistency (8 bits)   */
  #define  DTI_DATA_64BIT         0x0003  /* 8-byte data consistency (64 bits)*/
  #define  DTI_DATA_8BIT          DTI_DATA_BYTE   /* see above                */
  #define  DTI_DATA_16BIT         DTI_DATA_WORD   /* see above                */
  #define  DTI_DATA_32BIT         DTI_DATA_LWORD  /* see above                */
  #define  DTI_DATA_BIT           0x0008  /* announces bit access, bit-position is selected with lower 3 bits
                                           or below constants */
  #define  DTI_DATA_BIT_ADDR0 (DTI_DATA_BIT | 0) 
  #define  DTI_DATA_BIT_ADDR1 (DTI_DATA_BIT | 1) 
  #define  DTI_DATA_BIT_ADDR2 (DTI_DATA_BIT | 2) 
  #define  DTI_DATA_BIT_ADDR3 (DTI_DATA_BIT | 3) 
  #define  DTI_DATA_BIT_ADDR4 (DTI_DATA_BIT | 4) 
  #define  DTI_DATA_BIT_ADDR5 (DTI_DATA_BIT | 5) 
  #define  DTI_DATA_BIT_ADDR6 (DTI_DATA_BIT | 6) 
  #define  DTI_DATA_BIT_ADDR7 (DTI_DATA_BIT | 7) 

/* 48 bit consistency used by Generation 3 controller boards only */
  #define  DTI_DATA_48BIT         0x0003  /* 6-byte data consistency (48 bits)*/

/*
 * definitions for Generation 3 controller boards only 
 */
  #define  DTA_OFFST_IBS_MASTER   0x0000  /*  */
  #define  DTA_OFFST_COP_TO_PC    0x0400  /*  */
  #define  DTA_OFFST_PC_TO_COP    0x0400  /*  */


/* Constant definitions for the DDI_GetInfo routine */
  #define  DDI_INFO_DDI_VERSION           1   /* Get DDI version info    */
  #define  DDI_INFO_DRV_VERSION           2   /* Get driver version info */

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

/* compiler option: word alignment in structures */
  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack(2)
  #endif

/* Structure for parameters for accessing the data area */
typedef struct
{
  USIGN16     length;     /* Amount of data in bytes                      */
  USIGN16     address;    /* Address in DTI area (byte address)           */
  USIGN16     dataCons;   /* Required data consistency range              */
  USIGN8 IBPTR  *data;    /* Pointer at data (read and/or write)          */
} T_DDI_DTI_ACCESS;

/* Structure for parameters for exchanging messages by mailbox */
typedef struct
{
  USIGN16     msgType;    /* Message type (see DDI description)           */
  USIGN16     msgLength;  /* Message length in bytes                      */
  USIGN16     DDIUserID;  /* Message identifier (see DDI description)     */
  USIGN8 IBPTR  *msgBlk;  /* Pointer at message data                      */
} T_DDI_MXI_ACCESS;


typedef struct
{
  CHAR  vendor[32];   /* Name of the DDI vendor                           */
  CHAR  name[48];     /* Name of the DDI (vendor specific)                */
  CHAR  revision[8];  /* Revision information as string                   */
  CHAR  dateTime[32]; /* Time and date of compilation                     */
  INT16 revNumber;    /* Revision as integer number                       */
} T_DDI_VERSION_INFO;


typedef struct
{
  CHAR  vendor[32];   /* Name of the driver vendor                        */
  CHAR  name[48];     /* Name of the driver (vendor-specific)             */
  CHAR  revision[8];  /* Revision information as string                   */
  CHAR  dateTime[32]; /* Time and date of compilation                     */
  INT16 revNumber;    /* Revision as integer number                       */
} T_DRIVER_VERSION_INFO;


  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack()
  #endif


  #define MAX_VERSION_CHAR 200
typedef struct
{
  CHAR IBPTR    *versionStringPointer;   /* points to 200 byte buffer */
  USIGN16     version;                   /* version of device driver (1.00 -> 100 */
} T_DRIVER_VERSION;

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

/*
 * Information about functions can be found in the DDI description
 */
extern IBDDIRET IBDDIFUNC  DDI_DevOpenNode(CHAR IBPTR *devName, INT16 perm,
                                           IBDDIHND IBPTR *nodeHd);
extern IBDDIRET IBDDIFUNC  DDI_DevCloseNode(IBDDIHND nodeHd);

extern IBDDIRET IBDDIFUNC  DDI_DTI_WriteData(IBDDIHND nodeHd,
                                             T_DDI_DTI_ACCESS IBPTR *dtiAcc);
extern IBDDIRET IBDDIFUNC  DDI_DTI_ReadData(IBDDIHND nodeHd,
                                            T_DDI_DTI_ACCESS IBPTR *dtiAcc);

extern IBDDIRET IBDDIFUNC  DDI_MXI_SndMessage(IBDDIHND nodeHd,
                                              T_DDI_MXI_ACCESS IBPTR *mxiAcc);
extern IBDDIRET IBDDIFUNC  DDI_MXI_RcvMessage(IBDDIHND nodeHd,
                                              T_DDI_MXI_ACCESS IBPTR *mxiAcc);

/* available only for the IBS ETH controllers */                                              
extern IBDDIRET IBDDIFUNC  DDI_DTI_ReadWriteData(IBDDIHND nodeHd,
                                                 T_DDI_DTI_ACCESS IBPTR *dtiWriteAcc,
                                                 T_DDI_DTI_ACCESS IBPTR *dtiReadAcc);

/* available only for the IBS ETH controllers */                                              
extern IBDDIRET IBDDIFUNC  DDI_MXI_SndMultipleMessages(IBDDIHND nodeHd,
                                                       T_DDI_MXI_ACCESS *mxiAcc,
                                                       int numOfMessages);



extern IBDDIRET IBDDIFUNC  DDI_GetInfo(IBDDIHND  nodeHd, USIGN16 cmd, VOID IBPTR *infoPtr);

extern INT32 IBDDIFUNC WaitForPDInIndication(IBDDIHND nodeHd, T_DDI_DTI_ACCESS  *dtiAcc); /* cv 5/2000 */

  #if !defined(ETH_WIN32) & !defined(ETH_WIN)

    #ifdef IBS_WIN_NT_VERSION
extern IBDDIRET IBDDIFUNC  DDI_DTI_SetOperatingMode(IBDDIHND nodeHd,
                                                    T_DDI_DTI_MODE IBPTR *mode);
extern IBDDIRET IBDDIFUNC  DDI_DTI_ClrOperatingMode(IBDDIHND nodeHd);
    #endif

  #endif /* !defined(ETH_WIN32) & !defined(ETH_WIN) */ 

  #ifdef IBS_DOS_VERSION
extern IBDDIRET IBDDIFUNC DDI_SetMsgNotification(INT16  nodeHd,
                                                 VOID (FAR *notifyFunc)(INT16 nodeHd));
extern IBDDIRET IBDDIFUNC DDI_ClrMsgNotification(INT16  nodeHd);
  #endif
/*****************************************************************************
****  GLOBAL DATA DECLARATIONS (EXTERNAL LINKAGE)                         ****
*****************************************************************************/

/* No global data declarations */


#endif    /* (ifndef INCL_DDI_USR) */


