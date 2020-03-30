/*******************************************************************
 *
 *    PROJECT:     Libraries
 *
 *    EQUIPMENT:   SerialNumber
 *
 *    FILE:        SerialNumber.h
 *
 *    DESCRIPTION: Interface zu SerialNumber
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

#ifndef SERIAL_NUMBER_H_INCLUDED
  #define SERIAL_NUMBER_H_INCLUDED

  #include <windows.h>

typedef enum
{
  eDaily   = 0,
  eWeekly  = 1,
  eMonthly = 2
} ESNrFileInterval;

#define SNR_ERROR_NO_ERROR                0
#define SNR_ERROR_WRONG_PARAMETER        -1
#define SNR_ERROR_ACCESS_DENIED          -2
#define SNR_ERROR_NO_INITIAL_SNR         -3
#define SNR_ERROR_OPEN_FILE_FAILED       -4
#define SNR_ERROR_NO_NETWORK_AVAILABLE   -5
#define SNR_ERROR_NO_OPEN_TRANSCATION    -6
#define SNR_ERROR_WRONG_SERIALNUMBER     -7



int iSNr_Init_g(char              *pcPath,
                ESNrFileInterval   eSNrFileInterval);

UINT uiSNr_NextFreeBegin_g(void);

int iSNr_NextFreeCommit_g(UINT uiSNr);

void vSNr_Rollback_g(void);

BOOL bSNrUnique_Init_g(char  *pcResultsPath, unsigned int iColumnIndex);
BOOL bSNrUnique_CheckIfUnique_g(UINT uiSnr);
BOOL bSNrUnique_SetCheck_m(unsigned int uiSerialNumber, BOOL bCheck);

int iSerialNumberAlreadyUsed_g(UINT uiSnr,
                               char *pcResultsPath,
                               UINT uiColumnIndex);


void vSNrUnique_Dump_g(FILE *pfp);


#endif // !SERIAL_NUMBER_H_INCLUDED

