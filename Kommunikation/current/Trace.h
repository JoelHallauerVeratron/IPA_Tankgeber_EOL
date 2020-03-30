/*******************************************************************
 *
 *    PROJECT:     Library Trace
 *
 *    EQUIPMENT:   ???
 *
 *    FILE:        Trace.h
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

#ifndef TRACE_H_INCLUDED
  #define TRACE_H_INCLUDED

  #include <windows.h>
  #include "Queue.h"


typedef enum
{
  eComRX=1,
  eComTX=2
} EComDirection;



void vTrace_Init_g(void);
void vTrace_Clear_g(void);


void vTrace_AppendMessageData_g(EComDirection eComDirection,
                                unsigned long ulMsgId,
                                unsigned char *pcData,
                                int  iBytes,
                                char *pcText, ...);

void vTrace_AppendText_g(char *pcFormat, ...);


int iTrace_GetTextLine_g(char *pcLine,
                         int  *piLinesRemaining);


#endif

