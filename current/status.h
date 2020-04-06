/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        status.h
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

#ifndef STATUS_H_INCLUDED
  #define STATUS_H_INCLUDED

  #include <userint.h>
  #include "Devicedat.h"   // Für TagDichtTest

  #define STATUS_RUN     100
  #define STATUS_ERROR   200
  #define STATUS_STOP    300

//  ------------------------------------------------------------------------------------------------
//  Typdefinition
//  ------------------------------------------------------------------------------------------------

typedef struct
{
  int       iProcessStatus;
  int       iErrorCode;
  BOOL      bFail;
  BOOL      bPass;

  struct
  {
    BOOL   bSwVersion;
    BOOL   bAnInputs;
  } tagTest;
} TagAppStatus;


typedef struct
{
  int  iCurrentIdleMa;   // gemessener Strom idle
  int  iCurrentLoadMa;   // gemessener Strom load

  char caNfcUid[20];
  int iGpsSnr;
  int iUbatt;

  char caErrorText[500];
  int  iErrorCode;
} TagMesswerte;


extern TagAppStatus   tagAppStatus_g;
extern TagMesswerte   tagMesswerte_g;

#endif // STATUS_H_INCLUDED
