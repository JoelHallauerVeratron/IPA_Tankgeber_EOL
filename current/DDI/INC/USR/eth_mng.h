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

/* File         : ETH_MNG.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   :
** Author       :
** Date         :
**              :
** Version      : 1.13
** Date         :
** Status       :
**              :
** Description  :
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
** 14/08/98  1.10     J.M.          Changed all definitions including
**                                  ETHA_XX to ETH_XX. For compatibilty the
**                                  old style definitions still available.
*/

#ifndef INCL_ETH_MNG
  #define INCL_ETH_MNG


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Symbolic constants for the physical and logical states of the ETH module.
 */
  #define  ETH_PSTAT_IDLE                 0   /* Not ready                    */
  #define  ETH_PSTAT_FULL_OPERATIONAL     1   /* Ready for operation          */
  #define  ETH_PSTAT_PART_OPERATIONAL     2   /* Part ready                   */
  #define  ETH_PSTAT_INOPERABLE           3   /* Not ready for operation      */
  #define  ETH_PSTAT_NEEDS_MAINTENANCE    4   /* In need of maintenance       */

  #define  ETH_LSTAT_IDLE                 0   /* Not ready                    */
  #define  ETH_LSTAT_COMM_READY           1   /* Ready for communication      */
  #define  ETH_LSTAT_LIMITED_SERV         2   /* Management services only     */

/* Constants for enabling and disabling the hardware resets */
  #define  ETH_DISABLE_HW_RESET           0   /* Disable hardware reset       */
  #define  ETH_ENABLE_HW_RESET            1   /* Enable hardware reset        */

/* Constants for the various TCP modes */
  #define  ETH_FIXED_FAST_TCP_MODE        1   /* TCP retransmit time = 36ms   */
  #define  ETH_VAR_FAST_TCP_MODE          2   /* TCP retransmit time variable */
  #define  ETH_STD_TCP_MODE               3   /* Standard VxWorks time        */


/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/


  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack(2)
  #endif

/*
 * Structure for setting Internet address
 */
typedef struct
{
  USIGN8   addrInternet[4]; /* 32-bit Internet address                    */
  USIGN8   addrRouter[4];   /* 32-bit router address                      */
  USIGN8   subnetMask[4];   /* 32 Bit Subnet Mask                        */
} T_ETH_INTERNET_ADDR;

/*
 * Structure for setting the Ethernet address. This service can only be accessed
 * if the correct password is used.
 */
  #define  ETH_LENGTH_PASSWD             16 /* Max. password length          */

typedef struct
{
  USIGN8  addrEthernet[6];               /* 48-bit Ethernet address       */
  USIGN8  password[ETH_LENGTH_PASSWD];   /* Password (zero terminated)    */
} T_ETH_ETHERNET_ADDR;




/*
 * Structure for setting the DDI services and ETHA management port number
 */
typedef struct
{
  USIGN16  portNumberDDI;        /* DDI port number to be used            */
  USIGN16  portNumberManagement; /* Management port number to be used     */
} T_ETH_PORT_NUM;


  #define  ETH_GET_ACTUAL_CONFIG         0x0001 /* Read current              */
/* configuration             */
  #define  ETH_GET_EEPROM_CONFIG         0x0002 /* Read configuration stored */
/* in EEPROM    */

/*
 * Get Net Configuration requests parameters
 */
typedef struct
{
  USIGN16  tag;
} T_ETH_REQ_NET_CONFIG;

/*
 * Structure for scanning the current ETHA configuration
 */
typedef struct
{
  USIGN8  addrEthernet[6];    /* Ethernet address     */
  USIGN8  addrInternet[4];    /* Internet address     */
  USIGN8  addrRouter[4];      /* Router address       */
  USIGN16 portDDI;            /* DDI services port    */
  USIGN16 portManagement;     /* ETHA management port */
  USIGN8  subnetMask[4];      /* Subnet Mask          */    
} T_ETH_NET_CONFIG;


/*
 * Vector sizes in the T_ETHA_IDENTIFY structure,
 * in bytes
 */
  #define  IDENT_VENDOR_LENGTH        32
  #define  IDENT_MODEL_LENGTH         32
  #define  IDENT_REVISION_LENGTH      32

/*
 * Structure for scanning ETH Identify information
 */
typedef struct
{
  USIGN8  model[IDENT_MODEL_LENGTH];       /* Model name        */
  USIGN8  vendor[IDENT_VENDOR_LENGTH];     /* Manufacturer name */
  USIGN8  revision[IDENT_REVISION_LENGTH]; /* Revision          */
} T_ETH_IDENTIFY;


/*
 * Maximal permissible length of Local Detail input
 */
  #define  LOCAL_DETAIL_LENGTH    8
/*
 * Structure with ETHA status
 */
typedef struct
{
  USIGN16  logicalStatus;     /* Logical status                       */
  USIGN16  physicalStatus;    /* Physical status                      */
  USIGN16  localDetailPres;   /* Specific information present         */
  USIGN16  localDetailLength; /* Local Detail size in bytes           */
  USIGN8   localDetail[LOCAL_DETAIL_LENGTH]; /* Local Detail          */
} T_ETH_STATUS;


/*
 * Structure with Ethernet controller network statistics
 */
typedef struct
{
  USIGN16  collisions;
  /* Other parameters still being specified */
} T_ETH_NET_STAT;


/*
 * Structure for network and protocol stack configuration
 */
typedef struct
{
  USIGN16  dummy;
  /* Parameters still being specified */
} T_ETH_CONFIG;


/*
 * Structure with TCP/IP stack parameters
 */
typedef struct
{
  USIGN16  numOfSockets; /* Max. no. of sockets                */
  USIGN16  numOfTCPConnections; /* Max. no. of TCP connections */
  USIGN16  numOfFrames;  /* No. of frame buffers               */
  USIGN16  maxFrameSize; /* Frame buffer size                  */
} T_ETH_NET_PARAM;

  #if !defined(DONT_USE_PRAGMA_PACK)
    #pragma pack()
  #endif

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

extern INT32 IBDDIFUNC  ETH_InitiateManagement(CHAR  *server, IBDDIHND  *hnd);
extern INT32 IBDDIFUNC  ETH_AbortManagement(IBDDIHND hnd);

extern INT32 IBDDIFUNC  ETH_SetInternetAddress(IBDDIHND hnd,
                                               USIGN8  *internetAddrBuf);
extern INT32 IBDDIFUNC  ETH_SetEthernetAddress(IBDDIHND hnd,
                                               USIGN8  *ethernetAddrBuf,
                                               CHAR  *password);
extern INT32 IBDDIFUNC  ETH_SetPortNumbers(IBDDIHND hnd,
                                           USIGN16  portNumberDDI,
                                           USIGN16  portNumberManagement);
extern INT32 IBDDIFUNC  ETH_GetNetConfiguration(IBDDIHND hnd, USIGN16  tag,
                                                T_ETH_NET_CONFIG *configPtr);

extern INT32 IBDDIFUNC  ETH_Identify(IBDDIHND hnd,
                                     T_ETH_IDENTIFY *identPtr);
extern INT32 IBDDIFUNC  ETH_Status(IBDDIHND hnd, T_ETH_STATUS *statusPtr);

extern INT32 IBDDIFUNC  ETH_HardwareReset(IBDDIHND hnd);

extern INT32 IBDDIFUNC  ETH_SetPSIDomainInfo(IBDDIHND  hnd,
                                             USIGN8  *inetAddrBuf,
                                             USIGN16 flagIsDomain);
extern INT32 IBDDIFUNC  ETH_GetPSIDomainInfo(IBDDIHND  hnd,
                                             USIGN8  *inetAddrBuf,
                                             USIGN16 *flagIsDomainPtr);

extern INT32 IBDDIFUNC  ETH_EnableHardwareReset(IBDDIHND  hnd);
extern INT32 IBDDIFUNC  ETH_DisableHardwareReset(IBDDIHND  hnd);
extern INT32 IBDDIFUNC  ETH_GetHardwareResetMode(IBDDIHND  hnd,
                                                 USIGN16  *modePtr);

extern INT32 IBDDIFUNC  ETH_SetTCPMode(IBDDIHND hnd, USIGN16 mode,
                                       USIGN16 value);
extern INT32 IBDDIFUNC  ETH_GetTCPMode(IBDDIHND hnd, USIGN16 *modePtr,
                                       USIGN16 *valuePtr);

/*****************************************************************************
****  CONSTANT DEFINITIONS FOR COMPATIBILITY                              ****
*****************************************************************************/

  #define  ETHA_PSTAT_IDLE                ETH_PSTAT_IDLE
  #define  ETHA_PSTAT_FULL_OPERATIONAL    ETH_PSTAT_FULL_OPERATIONAL
  #define  ETHA_PSTAT_PART_OPERATIONAL    ETH_PSTAT_PART_OPERATIONAL
  #define  ETHA_PSTAT_INOPERABLE          ETH_PSTAT_INOPERABLE
  #define  ETHA_PSTAT_NEEDS_MAINTENANCE   ETH_PSTAT_NEEDS_MAINTENANCE
  #define  ETHA_LSTAT_IDLE                ETH_LSTAT_IDLE
  #define  ETHA_LSTAT_COMM_READY          ETH_LSTAT_COMM_READY
  #define  ETHA_LSTAT_LIMITED_SERV        ETH_LSTAT_LIMITED_SERV
  #define  ETHA_DISABLE_HW_RESET          ETH_DISABLE_HW_RESET
  #define  ETHA_ENABLE_HW_RESET           ETH_ENABLE_HW_RESET
  #define  ETHA_FIXED_FAST_TCP_MODE       ETH_FIXED_FAST_TCP_MODE
  #define  ETHA_VAR_FAST_TCP_MODE         ETH_VAR_FAST_TCP_MODE
  #define  ETHA_STD_TCP_MODE              ETH_STD_TCP_MODE

  #define  T_ETHA_INTERNET_ADDR           T_ETH_INTERNET_ADDR
  #define  ETHA_LENGTH_PASSWD             ETH_LENGTH_PASSWD
  #define  T_ETHA_ETHERNET_ADDR           T_ETH_ETHERNET_ADDR
  #define  T_ETHA_PORT_NUM                T_ETH_PORT_NUM
  #define  ETHA_GET_ACTUAL_CONFIG         ETH_GET_ACTUAL_CONFIG
  #define  ETHA_GET_EEPROM_CONFIG         ETH_GET_EEPROM_CONFIG
  #define  T_ETHA_REQ_NET_CONFIG          T_ETH_REQ_NET_CONFIG
  #define  T_ETHA_NET_CONFIG              T_ETH_NET_CONFIG
  #define  T_ETHA_IDENTIFY                T_ETH_IDENTIFY
  #define  T_ETHA_STATUS                  T_ETH_STATUS
  #define  T_ETHA_NET_STAT                T_ETH_NET_STAT
  #define  T_ETHA_CONFIG                  T_ETH_CONFIG
  #define  T_ETHA_NET_PARAM               T_ETH_NET_PARAM

#endif    /* (ifndef INCL_ETH_MNG) */



