/*******************************************************************
 *
 *    PROJECT:     Library Trace
 *
 *    EQUIPMENT:   ???
 *
 *    FILE:        Trace.c
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
#else
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
#endif

#include "Trace.h"
#include "Queue.h"


/**
 * modul globale Variablen
 */
Queue_Node_T queueTrace_m=NULL;



/**
 * vTrace_Init_g()
 *
 * Trace initialisieren
 *
 * @author uidc9013 (10.01.2012)
 */
void vTrace_Init_g(void)
{
  queueTrace_m=Queue_New();
} // vTrace_Init_g()


/**
 * vTrace_Clear_g()
 *
 * Trace leeren
 *
 * @author uidc9013 (10.01.2012)
 */
void vTrace_Clear_g(void)
{
  // Queue leeren
  while(Queue_Pop(queueTrace_m,NULL,100,NULL))
  {
    ;
  }
} // vTrace_Clear_g()


/**
 * vTrace_AppendMessageData_g()
 *
 * Trace ausgeben
 *
 * @author uidc9013 (11.06.2009)
 * @param eComDirection
 *               Richtung der Kommunikation
 * @param uiMsgId ID der Meldung
 * @param pcSendData Daten der Kommunkation
 * @param iBytes Anzahl bytes in pcSendData
 * @param pcText Debug text für Trace
 */
void vTrace_AppendMessageData_g(EComDirection   eComDirection,
                                unsigned long   ulMsgId,
                                unsigned char  *pcSendData,
                                int             iBytes,
                                char           *pcText, ...)
{
#define SHOW_TIME_DELTA
#ifdef SHOW_TIME_DELTA
  static double dTimeDiff;
  double dTime;
#endif // SHOW_TIME_DELTA
  char caBuffer[500];
  int iIndex;

  va_list args;
  char caTextTrace[500];

  va_start(args,pcText);
  vsprintf(caTextTrace,pcText,args);

#ifdef _CVI_
  dTime=Timer();
#else // !_CVI_
  dTime=clock()/(double)CLOCKS_PER_SEC;
#endif // !_CVI_

#ifdef SHOW_TIME_DELTA
  sprintf(caBuffer,"%6.2f",min(dTime-dTimeDiff,999.99f));
  dTimeDiff=dTime;
#else
  sprintf(caBuffer,"%6.2f",min(dTime-dTimeDiff,999.99f));
#endif

  if(eComDirection==eComRX)
    sprintf(&caBuffer[strlen(caBuffer)]," Rx ");
  else
    sprintf(&caBuffer[strlen(caBuffer)]," Tx ");

  sprintf(&caBuffer[strlen(caBuffer)],"%04x ",ulMsgId);

  for(iIndex=0;
      iIndex<iBytes;
      iIndex++)
  {
    sprintf(&caBuffer[strlen(caBuffer)],"%02x ",pcSendData[iIndex]);
  }

  // Trace mit Leerzeichen füllen dass pcText untereinander angeordnet ist
  sprintf(&caBuffer[strlen(caBuffer)],"%*s",max(((8-iBytes+1)*3)-2,0)," ");
  sprintf(&caBuffer[strlen(caBuffer)]," %s",caTextTrace);

  if(!queueTrace_m)
    vTrace_Init_g();

  Queue_Push(queueTrace_m,caBuffer,strlen(caBuffer)+1);
} // vTrace_AppendMessageData_g()


/**
 * vTrace_AppendText_g()<p>
 * Formatierter Text in Trace ausgeben
 *
 * @author uidc9013 (11.06.2009)
 * @param pcFormat Format für Text (siehe auch printf())
 *
 * @see printf() formats
 */
void vTrace_AppendText_g(char *pcFormat, ...)
{
  va_list args;
  char caBuffer[200];

  va_start(args,pcFormat);
  vsprintf(caBuffer,pcFormat,args);

  if(!queueTrace_m)
    vTrace_Init_g();

  Queue_Push(queueTrace_m,caBuffer,strlen(caBuffer)+1);
} // vTrace_AppendText_g()


/**
 * iTrace_GetTextLine_g()<p>
 * Gibt eine Linie des Traces zur&uuml;ck
 *
 * @author uidc9013 (10.01.2012)
 * @param pcLine Pointer auf Buffer f&uuml;r Textzeile<br>
 *               (sollte Platz f&uuml;r mind. 100 chars haben)
 * @param piLinesRemaining
 *               pointer auf int Variable f&uuml;r R&uuml;ckgabe der Anzahl<br>
 *               verbleibende Zeilen in Trace.
 *
 * @return Anzahl weiterer Zeilen in Trace noch verbleibend.
 */
int iTrace_GetTextLine_g(char *pcLine,
                         int  *piLinesRemaining)
{
  return(Queue_Pop(queueTrace_m,pcLine,100,piLinesRemaining));
} // iTrace_GetTextLine_g()


