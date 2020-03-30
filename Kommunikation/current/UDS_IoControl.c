/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   UDS
 *
 *    FILE:        Uds_IoControl.c
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


#include <windows.h>
#ifdef _CVI_
  #include <formatio.h>
  #include <utility.h>
  #include <ansi_c.h>
#else // !_CVI_
  #include <stdlib.h>
  #include <stdio.h>
#endif // !_CVI_

#include "Uds_IoControl.h"

/**
 * Dynamische Daten:
 * Alle Inputs und Outputs
 */
TagIoCtrlInfo atagIoCtrlInfo_m[]=
{
  /*
  +--- iId
  |                                        +--- bWrite (Output)
  |                                        |      +--- bRead (Input)
  |                                        |      |      +--- iSize
  |                                        |      |      |                            +--- bValid
  |                                        |      |      |                            |      +--- pcIoCtrlValue
  |                                        |      |      |                            |      |     +--- pcDescription
  |                                        |      |      |                            |      |     |                  */
  //CID_PWM_ILLUMINATION,        TRUE,  FALSE, CID_PWM_ILLUMINATION_LEN,    FALSE, 0,    "PWM_Illumination",
  //CID_POINTER_OVERRIDE,        TRUE,  FALSE, CID_POINTER_OVERRIDE_LEN,    FALSE, 0,    "Pointer Override",
  //CID_LCD_OVERRIDE,            TRUE,  FALSE, CID_LCD_OVERRIDE_LEN,        FALSE, 0,    "LCD Override",

  CID_OUT_PWM_DC,              TRUE,  FALSE, 1,                           FALSE, 0,    "OUT_DigOutp.OUT_PWM_01_DC",
  CID_OUT_DIG_OUT,             TRUE,  FALSE, 2,                           FALSE, 0,    "Digital Out",

  CID_IN_ANA,                  FALSE, TRUE,  CID_IN_ANA_LEN,              FALSE, 0,    "IN_AnaInp",
  //CID_IN_FREQ,                 FALSE, TRUE,  2,                           FALSE, 0,    "IN_FreqInp",
  //CID_IN_DIG,                  FALSE, TRUE,  2,                           FALSE, 0,    "IN_DigInp",
  //CID_IN_ANA_STEPPER_MOTOR,    FALSE, TRUE,  1,                           FALSE, 0,    "stepper motor state",

  CID_LIN_210,                 FALSE, TRUE,  CID_LIN_210_LEN,             FALSE, 0,    "LIN 210",   // TODO: implement this
  CID_LIN_211,                 FALSE, TRUE,  CID_LIN_211_LEN,             FALSE, 0,    "LIN 210",
  CID_LIN_212,                 FALSE, TRUE,  CID_LIN_212_LEN,             FALSE, 0,    "LIN 210",
  CID_LIN_213,                 FALSE, TRUE,  CID_LIN_213_LEN,             FALSE, 0,    "LIN 210",
  CID_LIN_214,                 FALSE, TRUE,  CID_LIN_214_LEN,             FALSE, 0,    "LIN 210",

  CID_IN_LOCAL_TIME,           FALSE, TRUE, 12,                           FALSE, 0,    "IN_Time_Date.LocalTime",

  0,                                       0,     0,     0,                           0,     NULL, NULL
};


