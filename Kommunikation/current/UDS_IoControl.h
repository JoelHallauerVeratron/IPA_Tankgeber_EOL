/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        Uds_IoControl.h
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

#ifndef UDS_IOCONTROL_H_INCLUDED
  #define UDS_IOCONTROL_H_INCLUDED

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
 * Reihenfolge der struct-Member muss mit der Initialisierung von atagIoCtrlInfo_m[] übereinstimmen!
 *
 * @author uidc9013 (15.03.2010)
 */
typedef struct
{
  int   iId;
  BOOL  bWrite;
  BOOL  bRead;
  int   iSize;
  BOOL  bValid;
  char *pcIoCtrlValue;
  char *pcDescription;
} TagIoCtrlInfo;


/**
 * I/O Control specifiers
 */
  //#define CID_PWM_ILLUMINATION            0x0502
  //#define CID_POINTER_OVERRIDE            0x0500
  //#define CID_LCD_OVERRIDE                0x0503

  #define CID_OUT_PWM_DC                  0x0154
  #define CID_OUT_DIG_OUT                 0x0501

  #define CID_IN_ANA                      0x0201
  #define CID_IN_FREQ                     0x0202
  #define CID_IN_DIG                      0x0204
  #define CID_IN_ANA_STEPPER_MOTOR        0x0207

  #define CID_LIN_210                     0x0210   // TODO: implement this
  #define CID_LIN_211                     0x0211
  #define CID_LIN_212                     0x0212
  #define CID_LIN_213                     0x0213
  #define CID_LIN_214                     0x0214


  //#define CID_PWM_ILLUMINATION_LEN        4
  //#define CID_POINTER_OVERRIDE_LEN        4
  //#define CID_LCD_OVERRIDE_LEN            1
  #define CID_IN_ANA_LEN                  4

  #define CID_LIN_210_LEN                 4
  #define CID_LIN_211_LEN                 4
  #define CID_LIN_212_LEN                 4
  #define CID_LIN_213_LEN                 4
  #define CID_LIN_214_LEN                 4



  #define CID_IN_LOCAL_TIME               0x017C

#endif // !UDS_IOCONTROL_H_INCLUDED

