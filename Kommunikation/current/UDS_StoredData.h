/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        Uds_StoredData.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author$
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef UDS_STOREDDATA_H_INCLUDED
  #define UDS_STOREDDATA_H_INCLUDED

  #include <windows.h>
  #ifdef _CVI_
    #include <toolbox.h>
    #include "cvidef.h"
  #else // !_CVI_
    #include <stdlib.h>
    #include <stdio.h>
  #endif // !_CVI_

/**
 * Achtung:
 *
 * Reihenfolge der struct-Member muss mit der Initialisierung von atagStoredData_m[] übereinstimmen!
 *
 * @author uidc9013 (16.03.2010)
 */
typedef struct
{
  int   iId;
  int   iAccessLevelRead;
  int   iAccessLevelWrite;
  int   iSize;
  BOOL  bValid;
  char *pcLidValue;
  char *pcDescription;
} TagStoredDataInfo;


/**
 * DIDs
 */
  #define DID_SOFTWARE_VERSION           0x0120   // Software Version
  #define DID_SERIAL_NUMBER              0x0130
  #define DID_A2C_NR_UNPACKED            0x0132
  #define DID_NMEA_PROD_CODE             0x0133
  #define DID_DIAL_SCALE_VALUES          0x0140
  #define DID_GAUGE_TYPE                 0x0141
  #define DID_GAUGE_OFFSET               0x0156
  #define DID_CALIBRATION                0x0121
  #define DID_PROD_INTRLOCK              0x01FE
  #define DID_FACTORY_DEFAULT            0x1234   // Factory defaults

  #define DID_SOFTWARE_VERSION_LEN            4
  #define DID_SERIAL_NUMBER_LEN               4
  #define DID_A2C_NR_UNPACKED_LEN            11
  #define DID_NMEA_PROD_CODE_LEN              2
  #define DID_DIAL_SCALE_VALUES_LEN           4
  #define DID_GAUGE_TYPE_LEN                  1
  #define DID_GAUGE_OFFSET_LEN                2
  #define DID_CALIBRATION_LEN                64
  #define DID_PROD_INTRLOCK_LEN              32
  #define DID_FACTORY_DEFAULT_LEN             4


  // restliche DIDs nicht verwendet
  #define DID_ECU_MANUFACTURING_DATE     0xF18B
  #define DID_VM_ECU_HARDWARE_NUMBER     0xF191   // 5.1.2.8 Vehicle Manufacturer ECU Hardware Number
  #define DID_SS_ECU_HARDWARE_NUMBER     0xF192   // 5.1.2.9 System Supplier ECU Hardware Number
  #define DID_ECU_HARDWARE_VERSION       0xF193
  #define DID_ECU_SOFTWARE_NUMBER        0xF194
  #define DID_ECU_SOFTWARE_VERSION       0xF195
  #define DID_ECU_PROGRAMMING_DATE       0xF199
  #define DID_INSPECTION_STATUS          0xFEE7
  #define DID_PRODUCT_INSPECTION_DATE    0xFEEA

  #define DID_ECU_MANUFACTURING_DATE_LEN      4
  #define DID_VM_ECU_HARDWARE_NUMBER_LEN     13
  #define DID_SS_ECU_HARDWARE_NUMBER_LEN     17
  #define DID_ECU_HARDWARE_VERSION_LEN       14
  #define DID_ECU_SOFTWARE_NUMBER_LEN       106
  #define DID_ECU_SOFTWARE_VERSION_LEN       50
  #define DID_ECU_PROGRAMMING_DATE_LEN        4
  #define DID_INSPECTION_STATUS_LEN           1
  #define DID_PRODUCT_INSPECTION_DATE_LEN     4


#endif // !UDS_STOREDDATA_H_INCLUDED

