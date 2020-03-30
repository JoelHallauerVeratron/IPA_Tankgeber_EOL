/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., 4933 Blomberg, Germany          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS ETH                                                   **
** Sub-Project  : IBS ETH DDI                                               **
**                                                                          **
*****************************************************************************/

/*
** File         : ETH_ERR.H
** Component(s) : -
** Device(s)    : -
** Terminology  : -
**              :
** Definition   :
** Author       :
**         $Date:   Feb 09 2001 13:58:24  $
**              :
**     $Revision:   1.2  $
**
** Status       : -
**              :
** Description  : Special error codes for ETH controller boards
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
** 03/07/95    0.9    J.M.          Start working on this file (first version)
**
** 14/08/98    1.10   J.M.          Changed all definitions including
**                                  ETHA_XX to ETH_XX. For compatibilty the
**                                  old style definitions still available.
**                                  Added an error base value (only needed by
**                                  Windows NT)
*/


#ifndef INCL_ETH_ERR
  #define INCL_ETH_ERR


/*****************************************************************************
****  CONSTANT DEFINITIONS                                                ****
*****************************************************************************/

/*
 * Special error codes for the TCP/IP device driver interface. These
 * error codes are generated on the client side.
 */
  #define  ERR_ETH_SOCKET_ERROR   (0x1000 | ERR_BASE)
/* Socket function error            */
  #define  ERR_ETH_RCV_TIMEOUT    (0x1001 | ERR_BASE)
/* A data telegram was not received */
/* by the client within the         */
/* specified time.                  */
  #define  ERR_ETH_MALLOC         (0x1002 | ERR_BASE)
/* No memory could be allocated for */
/* sending and/or receiving         */
  #define  ERR_ETH_DG_SIZE        (0x1003 | ERR_BASE)
/* The data telegram is too large.  */
/* The number of parameters in the  */
/* request must be modified.        */
  #define  ERR_ETH_INIT           (0x1004 | ERR_BASE)
/* Error occurred during            */
/* initialization of the            */
/* ETH library                      */

/*
 * ERR_IBSETHA_XXX error messages can only occur if the host name is specified
 * from the device name. The exact error cause can be found in the global error
 * variables.
 */
  #define  ERR_IBSETH_OPEN        (0x1010 | ERR_BASE)
/* IBSETHA file cannot be opened.   */
/* Check that the path is correct   */
/* and the file is available.       */
  #define  ERR_IBSETH_STAT        (0x1011 | ERR_BASE)
/* The file status could not be     */
/* determined. Check that the file  */
/* is available.                    */
  #define  ERR_IBSETH_MALLOC      (0x1012 | ERR_BASE)
/* The memory for reading the file  */
/* could not be allocated. Check    */
/* if the file is too large.        */
  #define  ERR_IBSETH_READ        (0x1013 | ERR_BASE)
/* The IBSETHA file could not be    */
/* read. Check whether the file     */
/* actually contains any data.      */
  #define  ERR_IBSETH_NAME        (0x1014 | ERR_BASE)
/* The device name specified cannot */
/* be found in the file. Check      */
/* entries.                         */
  #define  ERR_IBSETH_SIZE        (0x1015 | ERR_BASE)
/* The host name found will not fit */
/* in the buffer available.         */
  #define  ERR_IBSETH_INTERNET    (0x1016 | ERR_BASE)
/* The system cannot recognize the  */
/* host name or Internet address.   */
/* Check the host file.             */

  #define  ERR_IBSETH_REG_ACCESS  (0x1020 | ERR_BASE)
/* The registry path cannot be      */
/* accessed. Check the registry     */
/* path.                            */
  #define  ERR_IBSETH_REG_VALUE   (0x1021 | ERR_BASE)
/* A required value cannot be found.*/
/* Check the registry inputs.       */
  #define  ERR_IBSETH_REG_INUSE   (0x1022 | ERR_BASE)
/* The InUse input does not equal   */
/* YES. Modify the input            */

/*
 * Special error constants for the ETH controller board. These error
 * messages affect special controller board services which cannot occur
 * in other systems. These error messages are generated on the ETH
 * controller board and are to be regarded as additional to the error
 * messages defined in the DDI_ERR.H file.
 */
  #define  ERR_STATE_CONFLICT     (0x0100 | ERR_BASE)
/* The service to be executed does   */
/* not match the state of the        */
/* controller board                  */
  #define  ERR_INVLD_CONN_TYPE    (0x0101 | ERR_BASE)
/* An attempt is being made to use a */
/* service over an unsuitable        */
/* connection.                       */
  #define  ERR_ACTIVATE_PD_CHK    (0x0102 | ERR_BASE)
/* The PD In monitoring task could   */
/* not be activated.                 */
  #define  ERR_DATA_SIZE          (0x0103 | ERR_BASE)
/* The max. amount of data in a      */
/* command has been exceeded, so     */
/* the Req. or Cnf will not fit in   */
/* the send or receive buffer.       */
  #define  ERR_OUTOFMEMORY        (0x0104 | ERR_BASE)
/* a malloc function failed          */
  #define  ERR_ACTIVATE_DATA_CHK  (0x0105 | ERR_BASE)
/* activation of data monitoring     */
/* failed                            */                                       
  #define  ERR_NO_MORE_AREAS      (0x0107 | ERR_BASE)
/* number of areas is limited        */

/*
 * Special error constants for ETH management services. These error
 * messages can only occur with ETH management services.
 */
  #define  ERR_ETH_MNGMNT_IN_USE          (0x0100 | ERR_BASE)
/* Mng. already in use     */
  #define  ERR_ETH_PERM_DENIED            (0x0101 | ERR_BASE)
/* Access denied           */
  #define  ERR_ETH_INVLD_MNGMNT_ID        (0x0102 | ERR_BASE)
/* Invalid Mng. ID         */
  #define  ERR_ETH_SERV_ERR               (0x0103 | ERR_BASE)
/* Service execution error */
  #define  ERR_ETH_INVLD_PASSWD           (0x0104 | ERR_BASE)
/* Incorrect password      */
  #define  ERR_ETH_HW_RST_DISBLD          (0x0105 | ERR_BASE)
/* Hardware reset disabled */
  #define  ERR_ETH_INVLD_PARAM            (0x0106 | ERR_BASE)
/* Invalid parameters      */

  #define  ERR_MNGMNT_INVLD_HNDL          (0x0040 | ERR_BASE)
  #define  ERR_MNGMNT_NOT_CONNECTED       (0x0041 | ERR_BASE)
  #define  ERR_MNGMNT_SERV_REQ            (0x0042 | ERR_BASE)
  #define  ERR_MNGMNT_SERV_CNF            (0x0043 | ERR_BASE)
  #define  ERR_MNGMNT_PASSWD_LENGTH       (0x0044 | ERR_BASE)
  #define  ERR_MNGMNT_UNKNOWN_SERVER      (0x0045 | ERR_BASE)



/*
 * Error constants for the ETHA_SetClientOption and ETHA_SetClientOption
 * functions. Both functions are only operated on the host side and do
 * not access the network.
 */
  #define  ERR_OPT_INVLD_CMD      (0x0200 | ERR_BASE)
/* Unknown command code             */
  #define  ERR_OPT_INVLD_PARAM    (0x0201 | ERR_BASE)
/* Invalid parameters               */
  #define  ERR_OPT_INVLD_SOCKET   (0x0202 | ERR_BASE)
/* Invalid socket descriptor        */
  #define  ERR_OPT_SIZE           (0x0203 | ERR_BASE)
/* Not enough space for the result  */

/*****************************************************************************
****  TYPE DEFINITIONS                                                    ****
*****************************************************************************/


/*****************************************************************************
****  CONSTANT DEFINITIONS FOR COMPATIBILITY                              ****
*****************************************************************************/

  #define  ERR_ETHA_SOCKET_ERROR              ERR_ETH_SOCKET_ERROR
  #define  ERR_ETHA_RCV_TIMEOUT               ERR_ETH_RCV_TIMEOUT
  #define  ERR_ETHA_MALLOC                    ERR_ETH_MALLOC
  #define  ERR_ETHA_DG_SIZE                   ERR_ETH_DG_SIZE
  #define  ERR_ETHA_INIT                      ERR_ETH_INIT

  #define  ERR_IBSETHA_OPEN                   ERR_IBSETH_OPEN
  #define  ERR_IBSETHA_STAT                   ERR_IBSETH_STAT
  #define  ERR_IBSETHA_MALLOC                 ERR_IBSETH_MALLOC
  #define  ERR_IBSETHA_READ                   ERR_IBSETH_READ
  #define  ERR_IBSETHA_NAME                   ERR_IBSETH_NAME
  #define  ERR_IBSETHA_SIZE                   ERR_IBSETH_SIZE
  #define  ERR_IBSETHA_INTERNET               ERR_IBSETH_INTERNET

  #define  ERR_ETHA_MNGMNT_IN_USE             ERR_ETH_MNGMNT_IN_USE
  #define  ERR_ETHA_PERM_DENIED               ERR_ETH_PERM_DENIED
  #define  ERR_ETHA_INVLD_MNGMNT_ID           ERR_ETH_INVLD_MNGMNT_ID
  #define  ERR_ETHA_SERV_ERR                  ERR_ETH_SERV_ERR
  #define  ERR_ETHA_INVLD_PASSWD              ERR_ETH_INVLD_PASSWD
  #define  ERR_ETHA_HW_RST_DISBLD             ERR_ETH_HW_RST_DISBLD
  #define  ERR_ETHA_INVLD_PARAM               ERR_ETH_INVLD_PARAM

#endif    /* (ifndef INCL_ETH_ERR) */


