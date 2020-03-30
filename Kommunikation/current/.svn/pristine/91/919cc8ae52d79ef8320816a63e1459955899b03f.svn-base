/*******************************************************************
 *
 *    PROJECT:     FlexCluster
 *
 *    EQUIPMENT:   KWP2000
 *
 *    FILE:        ProdInterlock.h
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

#ifndef PROD_INTERLOCK_H_INCLUDED
  #define PROD_INTERLOCK_H_INCLUDED

  #include <time.h>


typedef enum
{
  eInterlockFormat_Complete  = 0,   // 10 byte
  eInterlockFormat_1bytes    = 1,   //  1 byte
  eInterlockFormat_2bytes    = 2,   //  2 byte
  eInterlockFormat_6bytes    = 10   //  6 byte
} EInterlockFormat;


int iInt2Bcd_g(int iDec);
int iBcd2Int_g(int iBcd);


BOOL bPIL_SetData_g(EInterlockFormat  eInterlockFormat,  // IN
                    unsigned int      uiVersionMajor,    // IN
                    unsigned int      uiVersionMinor,    // IN
                    unsigned int      uiRunCounter,      // IN
                    unsigned char    *pucDataBuffer,     // OUT
                    unsigned int      iBufferSize);      // IN

BOOL bPIL_GetData_g(EInterlockFormat  eInterlockFormat,  // IN
                    unsigned char    *pucDataBuffer,     // IN
                    unsigned int      iBufferSize,       // IN
                    unsigned int     *puiVersionMajor,   // OUT
                    unsigned int     *puiVersionMinor,   // OUT
                    unsigned int     *puiRunCounter,     // OUT
                    unsigned int     *puiYear,           // OUT
                    unsigned int     *puiMonth,          // OUT
                    unsigned int     *puiDay,            // OUT
                    unsigned int     *puiHour,           // OUT
                    unsigned int     *puiMinute,         // OUT
                    unsigned int     *puiSecond);        // OUT


void vDumpBytes_g(char *pcVariableName,
                  unsigned char *pucBytes,
                  int   iSize);


#endif // !PROD_INTERLOCK_H_INCLUDED
