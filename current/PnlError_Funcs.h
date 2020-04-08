/*******************************************************************
 *
 *    PROJECT:     Library
 *
 *    EQUIPMENT:   Error text display
 *
 *    FILE:        prototypes for PnlError.c
 *
 *    DESCRIPTION: Displays an error message
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

#ifndef PNLERROR_H_INCLUDED
  #define PNLERROR_H_INCLUDED

  #include <windows.h>

void _cdecl vErrorPanel_Show_g(void);
void _cdecl vErrorPanel_Hide_g(void);

void _cdecl vErrorPanel_EnableErrorSlip_g(BOOL bEnable);

void _cdecl vErrorPanel_SetErrorMessage_g(char *pcErrorMessage);
char _cdecl *pcErrorPanel_GetErrorMessage_g(void);

void _cdecl vErrorPanel_SetErrorCode_g(UINT uiErrorCode);
UINT _cdecl uiErrorPanel_GetErrorCode_g(void);

void _cdecl vErrorPanel_SetErrorStep_g(UINT uiErrorStep);
UINT _cdecl vErrorPanel_GetErrorStep_g(void);

BOOL _cdecl bErrorPanel_PrintErrorSlip_g(void);

#endif // PNLERROR_H_INCLUDED
