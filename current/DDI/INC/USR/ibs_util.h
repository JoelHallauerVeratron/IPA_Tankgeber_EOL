/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg, Germany         **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : Utility Routines                                          **
** Sub-Project  : --                                                        **
**                                                                          **
*****************************************************************************/

/*
** File         : IBS_UTIL.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   05 Oct 2001 08:36:22  $
**              :
**     $Revision:   1.25  $
** Status       :
**              :
** Description  : Declarations and function prototypes for help functions
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
** 05/19/93    0.9    J.M.          Start working on this file (first version)
**
*/



#ifndef INCL_IBS_UTIL
  #define INCL_IBS_UTIL


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

  #define WD_RELEASED        1 /* Watchdog triggered before last reset */
  #define WD_NOT_RELEASED    0 /* Watchdog not triggered before last reset */
/* Constants to use with SetWatchdogTimeout() */
  #define  WDT_TIMEOUT_8      0x000    /* Timeout = 8.2 ms    */
  #define  WDT_TIMEOUT_16     0x004    /* Timeout = 16.4 ms   */
  #define  WDT_TIMEOUT_32     0x008    /* Timeout = 32.8 ms   */
  #define  WDT_TIMEOUT_65     0x00C    /* Timeout = 65.5 ms   */
  #define  WDT_TIMEOUT_131    0x010    /* Timeout = 131.1 ms  */
  #define  WDT_TIMEOUT_262    0x014    /* Timeout = 262.1 ms  */
  #define  WDT_TIMEOUT_524    0x018    /* Timeout = 524.3 ms  */
  #define  WDT_TIMEOUT_1048   0x01C    /* Timeout = 1048.6 ms */


/* Constants for the function for reading diagnostic words in the MPM */
  #define USER_BIT                 0x0001 /* User error detected              */
  #define PF_BIT                   0x0002 /* I/O error detected               */
  #define BUS_BIT                  0x0004 /* Bus error detected               */
  #define CTRL_BIT                 0x0008 /* Controller error detected        */
  #define DETECT_BIT               0x0010 /* Searching for bus error          */
  #define RUN_BIT                  0x0020 /* Set if data cycles are running   */
  #define ACTIVE_BIT               0x0040 /* Set if the firmware is in        */
                                          /* ACTIVE or RUN                    */
                                          /* Reset on READY                   */
  #define READY_BIT                0x0080 /* Ready bit: set once the          */
                                          /* firmware has started up          */
                                          /* successfully and the EEPROM      */
                                          /* boot sequence has been processed.*/
                                          /* Reset on priority 3 errors       */
  #define BSA_BIT                  0x0100 /* Set if the current               */
                                          /* configuration is different       */
                                          /* from the active configuration    */
  #define BASP_BIT                 0x0200 /* Set if the host is in Stop       */
  #define RESULT_BIT               0x0400 /* Result bit for default actions   */
  #define SYNCHRON_RESULT_BIT      0x0800 /* Synchronous result bit is set,   */
                                          /* if synchronization with the      */
                                          /* user program has failed in       */
                                          /* operating mode 2.                */
  #define DATA_CYCLE_RESULT_BIT    0x1000 /* Data cycle result bit is set     */
                                          /* if a data cycle was faulty       */
                                          /* (OM2 and OM3).                   */
  #define WARN_BIT                 0x2000 /* Warning bit: set if the bus      */
                                          /* warning time elapses without     */
                                          /* a valid data cycle               */
  #define QUALITY_BIT              0x4000 /* Statistical error evaluation     */
                                          /* of layer 2. Set if 20 out of 1   */
                                          /* million data cycles are faulty,  */
                                          /* otherwise reset                  */
  #define SS_INFO_BIT              0x8000 /* Standard signal interface info   */
                                          /* bit is set, provided that some   */
                                          /* type of confirmation or          */
                                          /* indication is available for the  */
                                          /* host in SSG.                     */


/*
Constants for slave diagnostic register
*/
  #define SD_SLAVE_DATA_TRANSFER   0x0001 /* data transfer to/from slave is active */
  #define SD_FAIL                  0x0002 /* no data transfer, Interbus is in reset state */
  #define SD_SLAVE_INITIALIZED     0x0004 /* slave is initialized, Interbus not in run state */
  #define SD_POWER_ON              0x0008 /* power supply of slave hardware is on */
  #define SD_READY                 0x0010 /* slave firmware is ready, diagnostic-reg. is initialized */
  #define SD_PROCESS_GUIDE         0x0020 /* see manual */


/*
enhanced constants for read/write process data
*/
  #define IB_EX_DTA                0x2000 /* R/W from/to ExDTA instead of DTA */
  #define IB_TO_REMOTE_DTA         0x1000 /* Read from DTA to Remote Node     */
  #define IB_NODE_0                0x0000 /* Select Node 0                    */
  #define IB_NODE_1                0x4000 /* Select Node 1                    */
  #define IB_NODE_2                0x8000 /* Select Node 2                    */
  #define IB_NODE_3                0xC000 /* Select Node 3                    */


  #define IB_EX_DTA_ETH_RFC        0x0800 /* Internal Use Only !              */

/* pattern to select boards reset */
  #define IB_RST_CON_KER      0xCA  /* reset controller kernel */
  #define IB_RST_COP_CFG      0xCC  /* reset COP, boot with CONFIG.SYS */
  #define IB_RST_COP_NO_CFG   0xCD  /* reset COP, boot without CONFIG.SYS */
  #define IB_RST_CON_COP_CFG  0xCE  /* reset controller kernel and COP, boot with CONFIG.SYS */


/* event handling */
  #define MAX_DATA_EVT_ENTRIES  200

  #define SET_EVENT_CLEAR_LIST   0
  #define SET_EVENT_MSG_RECEIVED 1
  #define SET_EVENT_DIAG_CHANGE  2
  #define SET_EVENT_DATA_CHANGE  3
  #define SET_EVENT_READ_RELEASE 4


  #define SET_EVENT_ACTION_SIGNAL_EVENT  1
  #define SET_EVENT_ACTION_SET_SEMAPHORE 2

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

/* compiler option: word alignment in structures */
  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack (2)
  #endif 


/* Structure for scanning the diagnostic words in the MPM */
typedef struct
{
  USIGN16  state;    /* State of InterBus  */

  #ifdef FW3X
  USIGN16  errType;  /* type of error ( Generation 3 only) */
  #endif

  USIGN16  diagPara; /* Diagnostic parameters */

} T_IBS_DIAG;

/* structure to read out diagonstic parameters */
typedef struct
{
  USIGN16  state;    /* diagnostic bit register */
  USIGN16  diagPara; /* diagnostic parameter register */
  USIGN16  addInfo;  /* diagnostic register Add_Error_Info */
} T_IBS_DIAG_EX;

/* structure to read out diagonstic parameters of coprocessor board */
typedef struct
{
  USIGN16  state;    /* diagnostic bit register of co-processor */
  USIGN16  code;     /* error code register, related with bit register */
  USIGN16  addCode;  /* additional error code register, related with above reg. */
} T_COP_DIAG;

/* Parameter fuer den Zugriff auf das (gesamte) MPM */
typedef struct
{
  USIGN32    length;     /* Anzahl der Daten in Bytes                    */
  USIGN32    address;    /* Adresse im MPM (auf Anfangsadresse bezogen)  */
  USIGN8 IBPTR  *data;      /* Zeiger auf den Datenpuffer                   */
} T_DDI_MPM_ACCESS;


  #ifdef NEW_EXC_HANDLING
typedef VOID (T_EXC_HANDLER) (USIGN32 argument);
  #else  /* NEW_EXC_HANDLING */
typedef VOID (T_EXC_HANDLER) (USIGN16 reason, USIGN32 usrValue);
  #endif /* NEW_EXC_HANDLING */

  #ifdef NEW_EXC_HANDLING
/* Struktur fuer den Exception Handler Mechanismus */
typedef struct
{
  USIGN16 excNumber;
  T_EXC_HANDLER IBPTR *exceptHandlerPtr;
} T_EXC_HANDLER_INFO;
  #else  /* NEW_EXC_HANDLING */
/* Struktur fuer den Exception Handler Mechanismus */
typedef struct
{
  USIGN16 mask;
  USIGN32 usrValue;
  T_EXC_HANDLER IBPTR *exceptHandlerPtr;
} T_EXC_HANDLER_INFO;
  #endif /* NEW_EXC_HANDLING */


/* structure to set mailbox parameters */           
typedef struct
{
  USIGN16    numOfMailboxes;
  USIGN16    freeMailboxes;
  USIGN16    maxMxiPerSubnode;
} T_SET_MXI_PARA, *PT_SET_MXI_PARA;

/* event handling */
typedef struct
{
  INT32 sNumberEvents;
  USIGN32 uEventId[MAX_DATA_EVT_ENTRIES+2];
} T_EVENT_ID_LIST, *PT_EVENT_ID_LIST;


  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack ()
  #endif

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

extern IBDDIRET IBDDIFUNC  GetIBSDiagnostic(IBDDIHND nodeHd,
                                            T_IBS_DIAG IBPTR *infoPtr);
extern IBDDIRET IBDDIFUNC  GetIBSDiagnosticEx(IBDDIHND nodeHd,
                                              T_IBS_DIAG_EX IBPTR *infoPtr);
extern IBDDIRET IBDDIFUNC  GetCOPDiagnostic(IBDDIHND nodeHd,
                                            T_COP_DIAG IBPTR *infoPtr);
extern IBDDIRET IBDDIFUNC  GetSlaveDiagnostic(IBDDIHND nodeHd,
                                              T_IBS_DIAG_EX IBPTR *infoPtr);
extern IBDDIRET IBDDIFUNC  GetDIPSwitch(IBDDIHND nodeHd,
                                        USIGN16 IBPTR *dataPtr);
extern IBDDIRET IBDDIFUNC  GetSysFailRegister(IBDDIHND nodeHd,
                                              USIGN16 IBPTR  *sysFailRegPtr);
extern IBDDIRET IBDDIFUNC  DDI_MPM_WriteData(IBDDIHND hnd,
                                             T_DDI_MPM_ACCESS IBPTR *mpmAcc);
extern IBDDIRET IBDDIFUNC  DDI_MPM_ReadData(IBDDIHND hnd,
                                            T_DDI_MPM_ACCESS IBPTR *mpmAcc);

extern IBDDIRET IBDDIFUNC  EnableWatchDog(IBDDIHND hnd);
extern IBDDIRET IBDDIFUNC  TriggerWatchDog(IBDDIHND hnd);
extern IBDDIRET IBDDIFUNC  GetWatchDogState(IBDDIHND hnd);
extern IBDDIRET IBDDIFUNC  ClearWatchDog(IBDDIHND hnd);
extern IBDDIRET IBDDIFUNC  SetWatchDogTimeout(IBDDIHND hnd, USIGN16 IBPTR *timeout);
extern IBDDIRET IBDDIFUNC  GetWatchDogTimeout(IBDDIHND hnd, USIGN16 IBPTR *timeout);
extern IBDDIRET IBDDIFUNC  DDI_WaitForSysFail(IBDDIHND hnd);
extern IBDDIRET IBDDIFUNC  SetSysFailSignal(IBDDIHND boardNumber);
extern IBDDIRET IBDDIFUNC  ClearSysFailSignal(IBDDIHND boardNumber);
extern IBDDIRET IBDDIFUNC  EnableWatchDogEx(IBDDIHND hnd, USIGN16 timeout);
extern IBDDIRET IBDDIFUNC  SetResetControlRegister(IBDDIHND hnd, USIGN8 pattern);
extern IBDDIRET IBDDIFUNC  ReadResetCounter(IBDDIHND hnd, INT32 IBPTR *pattern);

extern IBDDIRET IBDDIFUNC  SetEventSubscription(IBDDIHND nodeHd, USIGN16 uSelect, USIGN32 uEventId, USIGN16 para1, USIGN16 para2);
extern IBDDIRET IBDDIFUNC  CheckEventList(IBDDIHND nodeHd, PT_EVENT_ID_LIST evtList);
extern IBDDIRET IBDDIFUNC  ClearEventSubscription(IBDDIHND nodeHd, USIGN32 uEventId);
extern IBDDIRET IBDDIFUNC  SetEventAction(IBDDIHND nodeHd, USIGN16 uSelect, USIGN32 uPara);

  #ifdef IBS_DOS_VERSION
extern IBDDIRET IBDDIFUNC SetMxiParameter(IBDDIHND hnd, USIGN16 numOfMxi, USIGN16 freeMxi, USIGN16 maxMxiPerSubnode);
extern IBDDIRET IBDDIFUNC SetAddControl(IBDDIHND hnd, USIGN16 addControl);
  #endif /* endif IBS_DOS_VERSION */

#endif    /* (ifndef INCL_PC_UTIL) */


