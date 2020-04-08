/*******************************************************************
 *
 *    PROJECT:     Tankfüllstandsgeber EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        global.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      HaJ
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef GLOBAL_H_INCLUDED
  #define GLOBAL_H_INCLUDED

  #include <windows.h>
  #include "status.h"
  #include "config.h"


  #ifndef BOOL
typedef int  BOOL;
  #endif // BOOL

  #ifndef TRUE
    #define TRUE            (1L)
  #endif
  #ifndef FALSE
    #define FALSE           (0L)
  #endif
  #ifndef NULL
    #define NULL            (0L)
  #endif


  #define BIT_SET(x,n) (x=x | (0x01<<n))
  #define BIT_CLEAR(x,n) (x=x & ~(0x01<<n))

  #define BIT_TEST(x,n) ((x & (0x01<<n))!=0)


//  ------------------------------------------------------------------------------------------------
//    Application Status
//  ------------------------------------------------------------------------------------------------
extern TagAppStatus   tagAppStatus_g;
extern TagAppConfig   tagAppConfig_g;


char* pcGetIniErrorDetailed_g(int iStatus);

char* cDateStr_g(void);
char *pcCurrentDateFormated_g(char *pcFormat);

BOOL bGetTime(int *iHour, int *iMin, int *iSec);

void vGetCurrentTime_g(UINT *puiHour,
                       UINT *puiMinutes,
                       UINT *puiSeconds);
#endif // !GLOBAL_H_INCLUDED
