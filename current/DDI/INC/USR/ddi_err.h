/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 32819 Blomberg                  **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS Controller Boards                                     **
** Sub-Project  : --                                                        **
**                                                                          **
*****************************************************************************/

/*
** File         : DDI_ERR.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   : J.M.
** Author       : J.M.
**         $Date:   15 Jun 2000 10:45:06  $
**              :
**     $Revision:   1.15  $
** Status       : -
**              :
** Description  : Driver error codes for DOS, WINDOWS and OS/2
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
** 07/26/93    0.9    J.M.          Start working on this file (first version)
**
** 23/03/95    0.91   J.M.          Extended to include error messages for
**                                  driver version 0.91.
**
** 08/07/97           T.W.          SubNodes, Sync. Modes
** 
** 01/14/99           HVo           added error codes for security handling  
** 
*/


#ifndef INCL_DDI_ERR
  #define INCL_DDI_ERR


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

  #define  ERR_OK                  0x0000 /* result positiv, no error occurs  */

  #define  ERR_INVLD_BOARD_NUM    (0x0080 | ERR_BASE)
/* invalid boardnumber              */
  #define  ERR_INVLD_IO_ADDR      (0x0081 | ERR_BASE)
/* invalid I/O address              */
  #define  ERR_INVLD_MPM_ADDR     (0x0082 | ERR_BASE)
/* invalid memory address           */
  #define  ERR_INVLD_INTR_NUM     (0x0083 | ERR_BASE)
/* invalid interrupt number         */
  #define  ERR_INVLD_CARD_CODE    (0x0084 | ERR_BASE)
/* incorrect board code             */

  #define  ERR_INVLD_NODE_HD      (0x0085 | ERR_BASE)
/* invalid node handle              */
  #define  ERR_INVLD_NODE_STATE   (0x0086 | ERR_BASE)
/* state prohibits action           */
  #define  ERR_NODE_NOT_READY     (0x0087 | ERR_BASE)
/* node is not ready                */
  #define  ERR_WRONG_DEV_TYPE     (0x0088 | ERR_BASE)
  #define  ERR_WRONG_DEV_TYP      (0x0088 | ERR_BASE)
/* MXI and DTI confused             */
  #define  ERR_DEV_NOT_READY      (0x0089 | ERR_BASE)
/* board not ready                  */
  #define  ERR_INVLD_PERM         (0x008A | ERR_BASE)
/* no access rights                 */
  #define  ERR_TSR_NOT_LOADED     (0x008B | ERR_BASE)
/* no TSR installed                 */
  #define  ERR_INVLD_CMD          (0x008C | ERR_BASE)
/* unknown command                  */
  #define  ERR_INVLD_PARAM        (0x008D | ERR_BASE)
/* command contains invalid param.  */


  #define  ERR_NODE_NOT_PRES      (0x0090 | ERR_BASE)
/* node not physically present      */
  #define  ERR_INVLD_DEV_NAME     (0x0091 | ERR_BASE)
/* invalid device name              */
  #define  ERR_NO_MORE_HNDL       (0x0092 | ERR_BASE)
/* no more handles available        */
  #define  ERR_NO_MORE_SUBNODE    (0x0093 | ERR_BASE)
/* no more subnode handles available*/
  #define  ERR_SUBNODE_IN_USE     (0x0094 | ERR_BASE)
/* subnode handle in use            */


  #define  ERR_PARA_OUT_OF_RANGE  (0x0095 | ERR_BASE)
/* parameters exceed range          */
  #define  ERR_AREA_EXCDED        (0x0096 | ERR_BASE)
/* DTA exceeded                     */
  #define  ERR_INVLD_DATA_CONS    (0x0097 | ERR_BASE)
/* invalid consistency range        */
  #define  ERR_DTA_NOT_PRESENT    (0x0098 | ERR_BASE)
/* data area does not exist         */
  #define  ERR_MPM_NOT_AVALBL     (0x0099 | ERR_BASE)
/* MPM not available                */

  #define  ERR_MSG_TO_LONG        (0x009A | ERR_BASE)
  #define  ERR_MSG_TOO_LONG       (0x009A | ERR_BASE)
/* messsage will not fit in buffer  */
  #define  ERR_NO_MSG             (0x009B | ERR_BASE)
/* no message available             */
  #define  ERR_NO_MORE_MAILBOX    (0x009C | ERR_BASE)
/* no free mailbox available        */
  #define  ERR_SVR_IN_USE         (0x009D | ERR_BASE)
/* SV register still in use         */
  #define  ERR_SVR_TIMEOUT        (0x009E | ERR_BASE)
/* SV register blocked              */
  #define  ERR_AVR_TIMEOUT        (0x009F | ERR_BASE)
/* AV register blocked              */
  #define  ERR_COP_USES_MA        (0x00A0 | ERR_BASE)
/* IBS master working with COP      */
  #define  ERR_PC_USES_MA         (0x00A1 | ERR_BASE)
/* IBS master working with PC       */
  #define  ERR_SYSFAIL            (0x00A2 | ERR_BASE)
/* SysFail is active                */

  #define ERR_MSG_EVENT_IN_USE    (0x00A3 | ERR_BASE)
/* Event is already used */
  #define ERR_MSG_EVENT_ADDRESS   (0x00A4 | ERR_BASE)
/* used address not in a propper area */
  #define ERR_MSG_EVENT_LENGTH    (0x00A5 | ERR_BASE)
/* used length not allowed (0 or greater area */
  #define ERR_MSG_EVENT_AREA      (0x00A6 | ERR_BASE) 
/* the address memory area exceeded */
  #define ERR_MSG_EVENT_MAX_ITEM  (0x00A7 | ERR_BASE)
/* the number of used event items exceeded */


  #define  ERR_NODE_IN_USE        (0x00B0 | ERR_BASE)
/* node ready in notify mode        */


  #define ERR_INVLD_PID           (0x00C0 | ERR_BASE)
/* invalid process identifier       */
  #define ERR_BLK_MODE_IS_ENBLD   (0x00C1 | ERR_BASE)
/* blocked mode active              */
  #define ERR_THREAD_IS_WAITING   (0x00C2 | ERR_BASE)
/* a thread is waiting              */


  #define ERR_INVLD_NODE          (0x00C8 | ERR_BASE)
/* invalid node number (IDC)        */
  #define ERR_INVLD_MEMORY        (0x00C9 | ERR_BASE)
/* memory not "valid"               */
  #define ERR_INVLD_NOTIFY_MODE   (0x00CA | ERR_BASE)
/* invalid notification mode        */
  #define ERR_BLOCK_TIMEOUT       (0x00CB | ERR_BASE)
/* timeout in blocked mode          */
  #define ERR_INVLD_NOTIFY_STATE  (0x00CC | ERR_BASE)
/* invalid notify state             */

  #define ERR_INVLD_HWND          (0x00D1 | ERR_BASE)
/* invalid window handle            */
  #define ERR_BOARD_NOT_PRES      (0x00D2 | ERR_BASE)
/* board not available/enabled      */
  #define ERR_INVLD_INIT_PARAM    (0x00D3 | ERR_BASE)
/* incorrect INI file parameters    */

  #define ERR_WRONG_BOARD_REV     (0x00E0 | ERR_BASE)
/* controlller board does not       */
/* support the command              */
  #define ERR_DRV_INVLD_FUNC      (0x00E1 | ERR_BASE)
/* function not supported by        */
/* the driver                       */
  #define ERR_SEC_PERM_DENIED     (0x00E3 | ERR_BASE)
/* access not permitted. (incorrect */
/* password)                        */



  #define ERR_DATA_TIMEOUT        (0x00E8 | ERR_BASE)
/* max. duration time to get valid  */
/* process data exceeded            */
  #define ERR_DATA_DC_RESULT      (0x00E9 | ERR_BASE)
/* DC-Result bit of diagnostic state*/
/* register reports faulty cycle    */
  #define ERR_DATA_SY_RESULT      (0x00EA | ERR_BASE)
/* SY-Result bit of diagnostic state*/
/* register reports synchronisation */
/* error                            */
  #define ERR_DTI_IN_USE          (0x00EB | ERR_BASE)
/* USC4 data interface is not ready */
/* to perform read/write with synch.*/
/* mode                             */
  #define ERR_DTI_NOT_RDY         (0x00EC | ERR_BASE)
/* USC4 data interface is not ready */
/* to perform read/write with synch.*/
/* mode                             */
  #define ERR_DATA_BUSSTOP        (0x00ED | ERR_BASE)
/* Interbus is not in RUN state     */



  #define ERR_SEND_TO_PIC         (0x00F0 | ERR_BASE)
/* fatal error, PIC not ready       */


  #define ERR_ACCESS_TO_DEVICE_FAILED  (0x0100 | ERR_BASE)
/* get device object pointer failed*/

  #define ERR_NUMBER_OF_DRIVERS_EXCEEDED  (0x0110 | ERR_BASE) 
/* too many used drivers           */
  #define ERR_LOAD_DLL_FAILED             (0x0111 | ERR_BASE) 
/* file does not exist             */



  #define ERR_SEC_NO_SECURITY     (0x0131 | ERR_BASE)
/* security login but no            */
/* security level                   */ 
  #define ERR_SEC_INVLD_PASSWORD  (0x0132 | ERR_BASE)
/* security login:                  */ 
/* invalid password                 */
  #define ERR_SEC_INVLD_IP_ADDR   (0x0133 | ERR_BASE)
/* security Login:                  */ 
/* invalid IP address of remote host*/
  #define ERR_SEC_INVLD_PASSWORD_AND_IP_ADDR  (0x0134 | ERR_BASE)
/* Security Login:                  */ 
/* invalid password and IP address  */
/* of remote host                   */
  #define ERR_SEC_INVLD_OPTION    (0x0135 | ERR_BASE)
/* security Login:                  */
/* invalid option                   */
  #define ERR_SEC_INVLD_PARAM     (0x0136 | ERR_BASE)
/* security Login:                  */
/* invalid option or wrong length of*/
/* password                         */    
  #define ERR_SEC_NUMBER_OF_IP_ADDR (0x0137 | ERR_BASE )
/* set security level:              */
/* wrong number of IP addresses     */


  #define ERR_SEC_SAVE_SETTINGS    (0x0138 | ERR_BASE)
/* set security level:              */
/* can't save settings              */



/*
 * Additional WINDOWS NT error codes
 */
  #define ERR_WINDOWS_NT_ERROR    (0x0800 | ERR_BASE)
/* An operating system specific     */
/* error code was returned. Use the */
/* function GetLastError() to get   */
/* extended error information       */

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/

/* No type defintions */

/*****************************************************************************
****  GLOBAL FUNCTION DECLARATIONS (EXTERNAL LINKAGE)                     ****
*****************************************************************************/

/* No global function declarations */

/*****************************************************************************
****  GLOBAL DATA DECLARATIONS (EXTERNAL LINKAGE)                         ****
*****************************************************************************/

/* No global data declarations */


#endif    /* (ifndef INCL_DDI_ERR) */


