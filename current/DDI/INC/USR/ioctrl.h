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
** File         : IOCTRL.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   : J.M.
** Author       : J.M.
**         $Date:   05 Oct 2001 08:37:30  $
**              :
**     $Revision:   1.6  $
**
** Status       :
**              :
** Description  : Additional DD commands which can be executed using the
**              : device IO Ctrl function. These commands apply to
**              : IBS ETH-specific services.
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
*/

#ifndef INCL_IOCTRL
  #define INCL_IOCTRL


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Constant definitions for the DevIOCtrl function sub-command codes
 */
  #define  DDI_READ_ETH_MPM           0x0801 /* Read MPM                      */
  #define  DDI_WRITE_ETH_MPM          0x0802 /* Write MPM                     */
  #define  DDI_ACTIVATE_HOST_CTRL     0x0803 /* Activate host monitoring      */
  #define  DDI_DEACTIVATE_HOST_CTRL   0x0804 /* Deactivate host monitoring    */
  #define  DDI_ACTIVATE_DTI_CTRL      0x0805 /* Activate DTI monitoring       */
  #define  DDI_DEACTIVATE_DTI_CTRL    0x0806 /* Deactivate DTI monitoring     */
  #define  DDI_SET_PD_IN_CHK          0x0807 /* Activate PD In monitoring     */
  #define  DDI_CLR_PD_IN_CHK          0x0808 /* Deactivate PD In monitoring   */
  #define  DDI_GET_ETH_WD_STATE       0x0809 /* Read watchdog status          */
  #define  DDI_CLR_ETH_WD_STATE       0x080A /* Reset watchdog status         */
  #define  DDI_GET_NET_FAIL_STAT      0x080B /* Read NetFail status           */
  #define  DDI_CLR_NET_FAIL_STAT      0x080C /* Reset NetFails status         */
  #define  DDI_SET_NET_FAIL           0x080D /* Trigger NetFail               */
  #define  DDI_SET_NET_FAIL_MODE      0x080E /* Set NetFail mode              */
  #define  DDI_GET_NET_FAIL_MODE      0x080F /* Read NetFail mode             */

/*
 * NetFail status constants
 */
  #define  ETH_NET_FAIL_ACTIVE        0xFFFF /* NetFail signal triggered      */
  #define  ETH_NET_FAIL_INACTIVE      0x0000 /* NetFail signal not triggered  */

/*
 * Constant definitions for possible NetFail causes
 * (NF = NetFail) Some of the error causes cannot be read
 * over the network.
 */
  #define  ETH_NF_NO_ERR              0x0000 /* No error                      */
  #define  ETH_NF_TASK_CREAT_ERR      0x0001 /* Error starting a task         */
  #define  ETH_NF_LISTENER_ERR        0x0002 /* Listener task error           */
  #define  ETH_NF_RECEIVER_ERR        0x0003 /* Receiver task error           */
  #define  ETH_NF_ACCEPT_ERR          0x0004 /* Accept call error             */
  #define  ETH_NF_ECHO_SERVER_ERR     0x0005 /* Echo server task error        */
  #define  ETH_NF_HOST_CONTROL_ERR    0x0006 /* Host controller task error    */
  #define  ETH_NF_DTI_TIMEOUT         0x0007 /* DTI timeout                   */
  #define  ETH_NF_HOST_TIMEOUT        0x0008 /* Host timeout                  */
  #define  ETH_NF_USER_TEST           0x0009 /* Set by the user               */
  #define  ETH_NF_CONN_ABORT          0x000A /* Connection aborted            */
  #define  ETH_NF_INIT_ERR            0x000B /* Initialization error          */

/*
 * Various NetFail mode constants
 */
  #define  ETH_NF_STD_MODE           0
  #define  ETH_NF_ALARMSTOP_MODE     1

/*
 * Constants for the various PD In Data monitoring modes
 */
  #define  ETH_PD_IN_CHK_INACTIVE    0x0000 /* PD In Check is not activated  */
  #define  ETH_PD_IN_CHK_MODE_TCP    0x0001 /* Send data over TCP connection */
  #define  ETH_PD_IN_CHK_MODE_UDP    0x0002 /* Send data over UDP port       */

/*
 * Constants for the various Data monitoring modes
 */
  #define ETH_DATA_CHK_MODE_INACTIVE 0x0000 /* data check mechanism is inactive */
  #define ETH_DATA_CHK_MODE_UDP      0x0002 /* Update mode based on UDP         */

/* data indication modes */
  #define ETH_DATA_IND_ERROR         0x0010 /* error indication                 */
  #define ETH_DATA_IND_REFERENCE     0x0011 /* data change event                */
  #define ETH_DATA_IND_DATA_CHANGE   0x0012 /* data change event                */
  #define ETH_DATA_IND_REFRESH       0x0013 /* refresh data event               */

/*
 * Constants for the client options
 */
  #define  ETH_OPT_RCV_TIMEOUT   0x0001 /* Receive timeout                   */
  #define  ETH_OPT_CON_TIMEOUT   0x0002 /* Connect timeout                   */
  #define  ETH_OPT_IBSETHA_PATH  0x0003 /* IBSETH file path                  */

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack(2)
  #endif

/*
 * Structure of the parameter blocks for Read MPM and Write MPM services
 */
typedef struct
{
  USIGN32  address;   /* MPM read start address                      */
  USIGN32  numOfBytes;/* Number of bytes to be read                  */
  USIGN8   *data;     /* Pointer at data buffer                      */
} T_ETH_IOCTRL_RD_MPM;

  #define  T_ETH_IOCTRL_WR_MPM       T_ETH_IOCTRL_RD_MPM


/*
 * Data telegram structure for Activate Host Control service
 */
typedef struct
{
  USIGN16   time; /* Time, in ms, during which the host can be addressed  */
                  /* by the ETHA module. The time is rounded up to        */
                  /* intervals of 11ms                                    */
                  /* The smallest possible value is 330ms.                */
} T_ETH_IOCTRL_HOST_CTRL;


/*
 * No data telegram is required to deactivate host monitoring,
 * as no parameters are required for request and confirmation
 * with this service
 */

/*
 * Data telegram structure for Activate DTI Control service
 */
typedef struct
{
  USIGN16  time;  /* Time, in ms, during which the server data telegram   */
                  /* waits for the DTI from the host. The time is rounded */
                  /* up to intervals of 11ms                              */
                  /* The smallest possible value is 55ms.                 */
} T_ETH_IOCTRL_DTI_CTRL;


/*
 * No special data telegram is required to deactivate DTI monitoring,
 * as no parameters are required for request and confirmation
 * with this service
 */

/*
 * Data structure for scanning the NetFail status
 */
typedef struct
{
  USIGN16  status;  /* NetFail status         */
  USIGN16  reason;  /* Reason for NetFail */
} T_ETH_NET_FAIL;

/*
 * Data structure for setting and reading the various NetFail modes
 */
typedef struct
{
  USIGN16  mode;        /* NetFail mode                               */
  USIGN16  numOfBytes;  /* Parameter block size in bytes              */
  VOID  *miscParamPtr;  /* Parameters for relevant NetFail mode */
} T_ETH_NET_FAIL_MODE;

/*
 * Data structure for transmitting PD In monitoring parameters
 */
typedef struct
{
  USIGN16  mode;       /* Selects monitoring mode                         */
  USIGN16  address;    /* Start address of area to be monitored           */
  USIGN16  numOfBytes; /* Size of area to be monitored in bytes           */
  USIGN8  *maskData;   /* Pointer at masking data buffer                  */
                       /* Buffer size corresponds to numOfBytes           */
  VOID (*notifyFuncPtr)(IBDDIHND nodeHd, T_DDI_DTI_ACCESS *dtiAcc);
  /* Pointer at a function which was called when     */
  /* the PD In data was modified.                    */
  USIGN32  timeout;    /* Timeout in ms                                   */
} T_ETH_PD_IN_MON;

/*
 * Data structure for transmitting Data monitoring parameters
 */
typedef struct
{
  USIGN16     mode;       /* not used at the moment                       */
  USIGN32     refreshTime; /* time for data update even if data unchanged */
  USIGN32     cycleTime;  /* cycle time for compare function              */
  USIGN32     minTBT;     /* min time between transmission                */
} T_ETH_DATA_MON_SETTINGS;

typedef struct
{
  USIGN32     clientHd;   /* application defined handle                   */
  USIGN32     dataAreaHd; /* server specific handle                      */
  T_DDI_DTI_ACCESS *dtiAcc; /* definition of watch area with AND mask     */
} T_ETH_DATA_AREA;

typedef struct
{
  USIGN32     clientHd;  /* application defined handle                   */
  USIGN32     timeout;   /* socket timeout                               */
  USIGN16     mode;      /* Refresh, Datachange or Reference                */
  T_DDI_DTI_ACCESS *dtiAcc; /* return data id received                    */
} T_ETH_DATA_INDICATION;

  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack()
  #endif

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

/*
 * Functions for reading and writing within the whole MPM. This routine
 * can be used for unlimited access to every MPM address within the
 * virtual address area.
 * Note : In the current version, no more than 400 bytes
 * can be read or written in one call.
 */
extern IBDDIRET IBDDIFUNC  ETH_ReadMPM(IBDDIHND nodeHd, USIGN32 address,
                                       USIGN32 numOfBytes, USIGN8 *data);
extern IBDDIRET IBDDIFUNC  ETH_WriteMPM(IBDDIHND nodeHd, USIGN32 address,
                                        USIGN32 numOfBytes, USIGN8 *data);

/*
 * These functions activate and deactivate the cyclical "addressing"
 * of a host computer (client).
 */
extern IBDDIRET IBDDIFUNC  ETH_SetHostChecking(IBDDIHND nodeHd,
                                               USIGN16  *time);
extern IBDDIRET IBDDIFUNC  ETH_ClearHostChecking(IBDDIHND nodeHd);

/*
 * These functions activate and deactivate the DTI monitoring.
 * This means that the DTI node must be addressed within the prescribed
 * time (timeout period). Otherwise, the gateway SysFail signal will be
 * activated and output of process data halted. Monitoring is enabled after
 * activation of the first service over the service to be monitored.
 * Therefore, process data traffic must begin immediately after the
 * activation.
 */
extern IBDDIRET IBDDIFUNC  ETH_SetDTITimeoutCtrl(IBDDIHND nodeHd,
                                                 USIGN16  *time);
extern IBDDIRET IBDDIFUNC  ETH_ClearDTITimeoutCtrl(IBDDIHND nodeHd);


/*
 * This function is primarily used for test purposes, to test the function of
 * the NetFail signal.
 */
extern IBDDIRET IBDDIFUNC  ETH_SetNetFail(IBDDIHND  nodeHd);

/*
 * The ETH_GetNetFailStatus function notifies the system of the NetFail status
 * of the controller board. This means that, in the case of an error, the
 * reason for triggering the NetFail signal can be precisely determined. The NetFail
 * status is entered in the status-referenced variable. Any valid DDI node handle
 * can be used as a node handle.
 */
extern IBDDIRET IBDDIFUNC  ETH_GetNetFailStatus(IBDDIHND  nodeHd,
                                                T_ETH_NET_FAIL *netFailInfo);

/*
 * This function resets the NetFail signal on the controller board
 * specified by the node handle. Any valid DDI node handle
 * can be used as a node handle.
 */
extern IBDDIRET IBDDIFUNC  ETH_ClrNetFailStatus(IBDDIHND  nodeHd);

/*
 * This function is used to set the behavior of the controller board
 * in the case of a NetFail. This is specified in parameter mode.
 * Depending on the mode, other parameters can - or may have to be -
 * transmitted.
 */
extern IBDDIRET IBDDIFUNC  ETH_SetNetFailMode(IBDDIHND nodeHd,
                                              T_ETH_NET_FAIL_MODE *netFailModeInfo);

/*
 * This function reads the current NetFail mode on the module specified
 * by the node handle.
 */
extern IBDDIRET IBDDIFUNC  ETH_GetNetFailMode(IBDDIHND  nodeHd,
                                              T_ETH_NET_FAIL_MODE *netFailModeInfo);

/*
 * Both functions described below are used to scan and
 * reset the ETH controller board watchdog.
 */
extern IBDDIRET IBDDIFUNC  ETH_GetWatchdogStatus(IBDDIHND  nodeHd,
                                                 USIGN16 *status);
extern IBDDIRET IBDDIFUNC  ETH_ClrWatchdogStatus(IBDDIHND  nodeHd);

/*
 * Functions for the automatic monitoring of the process IN data area.
 * These functions enable the automatic detection of data changes in the
 * PD In area. A mask can be specified to differentiate between relevant
 * and irrelevant data.
 */
extern IBDDIRET IBDDIFUNC  ETH_ActivatePDInMonitoring(IBDDIHND nodeHd,
                                                      T_ETH_PD_IN_MON *infoPtr);
extern IBDDIRET IBDDIFUNC  ETH_DeactivatePDInMonitoring(IBDDIHND nodeHd);

/* HVo */
extern IBDDIRET IBDDIFUNC  ETH_WaitForIndication(IBDDIHND nodeHd,
                                                 VOID *infoPtr);

/*
 * Functions for entering and deleting the exception handler for a
 * node. The exception handler indicates an exceptional situation on
 * the ETH controller board. These occur if the NetFail, MA SysFail,
 * Watchdog reset and/or Option bits are set in the status word of a
 * data telegram.
 */
extern IBDDIRET IBDDIFUNC  ETH_SetExceptionHandler(IBDDIHND nodeHd,
                                                   VOID (*funcPtr)(IBDDIHND nodeHd,
                                                                   INT16 boardNumber, USIGN16 reason));
extern IBDDIRET IBDDIFUNC  ETH_ClrExceptionHandler(IBDDIHND nodeHd);

/*
 * Functions for modifying local (client) parameters
 */
extern IBDDIRET IBDDIFUNC  ETH_SetClientOptions(int  cmd, char  *arg,
                                                int  length);

extern IBDDIRET IBDDIFUNC  ETH_GetClientOptions(int  cmd, char  *arg,
                                                int  *length);

extern IBDDIRET IBDDIFUNC  ETH_GetNetworkError(IBDDIHND  nodeHd);


/*
 * Functions for the automatic monitoring of the MPM data areas.
 * These functions enable the automatic detection of data changes in the
 * different MPM areas for the given nodeHd. The WatchArea function specify
 * the areas to be checked.
 */
extern IBDDIRET IBDDIFUNC  ETH_ActivateDataIndication(IBDDIHND nodeHd,
                                                      T_ETH_DATA_MON_SETTINGS  *dataMonSettingsPtr);

extern IBDDIRET IBDDIFUNC  ETH_DeactivateDataIndication(IBDDIHND nodeHd);

extern IBDDIRET IBDDIFUNC  ETH_SetDataWatchArea(IBDDIHND nodeHd,
                                                T_ETH_DATA_AREA  *dataAreaPtr);

extern IBDDIRET IBDDIFUNC  ETH_ClearDataWatchArea(IBDDIHND nodeHd, USIGN32  dataAreaHd);

extern IBDDIRET IBDDIFUNC  ETH_WaitForDataIndication(IBDDIHND nodeHd,
                                                     T_ETH_DATA_INDICATION  *dataIndicationPtr);

extern IBDDIRET IBDDIFUNC  ETH_ClearAllDataWatchAreas(IBDDIHND nodeHd);


/*****************************************************************************
****  GLOBAL DATA DECLARATIONS (EXTERNAL LINKAGE)                         ****
*****************************************************************************/

  #define  ETH_NET_FAIL_AVCTIVE               ETH_NET_FAIL_ACTIVE

  #define  DDI_READ_ETHA_MPM                  DDI_READ_ETH_MPM
  #define  DDI_WRITE_ETHA_MPM                 DDI_WRITE_ETH_MPM
  #define  DDI_GET_ETHA_WD_STATE              DDI_GET_ETH_WD_STATE
  #define  DDI_CLR_ETHA_WD_STATE              DDI_CLR_ETH_WD_STATE

  #define  ETHA_NET_FAIL_AVCTIVE              ETH_NET_FAIL_AVCTIVE
  #define  ETHA_NET_FAIL_ACTIVE               ETH_NET_FAIL_AVCTIVE
  #define  ETHA_NET_FAIL_INACTIVE             ETH_NET_FAIL_INACTIVE

  #define  ETHA_NF_NO_ERR                     ETH_NF_NO_ERR
  #define  ETHA_NF_TASK_CREAT_ERR             ETH_NF_TASK_CREAT_ERR
  #define  ETHA_NF_LISTENER_ERR               ETH_NF_LISTENER_ERR
  #define  ETHA_NF_RECEIVER_ERR               ETH_NF_RECEIVER_ERR
  #define  ETHA_NF_ACCEPT_ERR                 ETH_NF_ACCEPT_ERR
  #define  ETHA_NF_ECHO_SERVER_ERR            ETH_NF_ECHO_SERVER_ERR
  #define  ETHA_NF_HOST_CONTROL_ERR           ETH_NF_HOST_CONTROL_ERR
  #define  ETHA_NF_DTI_TIMEOUT                ETH_NF_DTI_TIMEOUT
  #define  ETHA_NF_HOST_TIMEOUT               ETH_NF_HOST_TIMEOUT
  #define  ETHA_NF_USER_TEST                  ETH_NF_USER_TEST
  #define  ETHA_NF_CONN_ABORT                 ETH_NF_CONN_ABORT
  #define  ETHA_NF_INIT_ERR                   ETH_NF_INIT_ERR
  #define  ETHA_NF_STD_MODE                   ETH_NF_STD_MODE
  #define  ETHA_NF_ALARMSTOP_MODE             ETH_NF_ALARMSTOP_MODE

  #define  ETHA_PD_IN_CHK_INACTIVE            ETH_PD_IN_CHK_INACTIVE
  #define  ETHA_PD_IN_CHK_MODE_TCP            ETH_PD_IN_CHK_MODE_TCP
  #define  ETHA_PD_IN_CHK_MODE_UDP            ETH_PD_IN_CHK_MODE_UDP

  #define  ETHA_OPT_RCV_TIMEOUT               ETH_OPT_RCV_TIMEOUT
  #define  ETHA_OPT_CON_TIMEOUT               ETH_OPT_CON_TIMEOUT
  #define  ETHA_OPT_IBSETHA_PATH              ETH_OPT_IBSETH_PATH

  #define  T_ETHA_IOCTRL_RD_MPM               T_ETH_IOCTRL_RD_MPM
  #define  T_ETHA_IOCTRL_WR_MPM               T_ETH_IOCTRL_RD_MPM
  #define  T_ETHA_IOCTRL_HOST_CTRL            T_ETH_IOCTRL_HOST_CTRL
  #define  T_ETHA_IOCTRL_HOST_CTRL            T_ETH_IOCTRL_HOST_CTRL
  #define  T_ETHA_NET_FAIL                    T_ETH_NET_FAIL
  #define  T_ETHA_NET_FAIL_MODE               T_ETH_NET_FAIL_MODE
  #define  T_ETHA_PD_IN_MON                   T_ETH_PD_IN_MON

#endif    /* (ifndef INCL_IOCTRL) */



