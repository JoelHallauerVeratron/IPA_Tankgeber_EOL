/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        global.c
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
#include <ansi_c.h>
#include <utility.h>

// für pcGetIniErrorDetailed_g():
#include <userint.h>
#include "toolbox.h"
#include "global.h"
#include "strftime.h"


//------------------------------------------------------------------------------------------------
//  Globale Variablen
//------------------------------------------------------------------------------------------------

// Application Status
TagAppStatus   tagAppStatus_g;

TagAppConfig   tagAppConfig_g;

TagMesswerte   tagMesswerte_g;


/**
 * lokalisierte Version von DateStr(). Gibt Datum im Format
 * TT.MM.JJJJ zurueck
 *
 * @return Datum im Format TT.MM.JJJJ
 */
char* cDateStr_g(void)
{
  static char pcDate[11];  // Rückgabe im Format "tt.mm.jjjj"
  char pcDateTmp[11];      //             Format "mm.tt.jjjj"

  strcpy(pcDateTmp,DateStr());

  strncpy(&pcDate[0],&pcDateTmp[3],2);
  pcDate[2]='.';
  strncpy(&pcDate[3],&pcDateTmp[0],2);
  pcDate[5]='.';
  strncpy(&pcDate[6],&pcDateTmp[6],4);
  //pcDateTmp[10]='\0';

  return(pcDate);
} // cDateStr_g()


/**
 * Gibt die aktuelle Zeit mit pcFormat formatiert zur&uuml;ck
 *
 * @author uidc9013 (12.11.2012)
 * @param pcFormat Format f&uuml;r Zeit
 *
 * @return Formatierte Zeit als String
 */
char *pcCurrentDateFormated_g(char *pcFormat)
{
#define DATE_STRING_LEN  50
  static char caCurrentDate[DATE_STRING_LEN+1];
  time_t rawtime;

  time(&rawtime);
  memset(caCurrentDate,0,DATE_STRING_LEN+1);
  strftime_(caCurrentDate,DATE_STRING_LEN,pcFormat,localtime(&rawtime));

  return(caCurrentDate);
}



/**
 * Fehler von Ini_...-Funktionen in Text-Form ausgeben
 *
 * @param iStatus Fehlercode für welchen der Text zurückgegeben werden soll
 *
 * @return Fehlertext
*/
char* pcGetIniErrorDetailed_g(int iStatus)
{
  switch(iStatus)
  {
    case UIEOutOfMemory:
      return("Out of memory");
    case UIETooManyFilesOpen:
      return("Too many files are open");
    case UIEfEOF:
      return("Unexpectedly reached end of file");
    case UIEIOError:
      return("Input/Output error");
    case UIEFileWasNotFound:
      return("File not found");
    case UIEAccessDenied:
      return("File access permission denied");
    case UIEFileAlreadyOpen:
      return("File already open");
    case UIEBadPathnameMsg:
      return("Badly formed pathname");
    case ToolErr_CouldNotOpenFileForReading:
      return("Could not open file for reading");
    case ToolErr_ErrorReadingFile:
      return("Error reading file");
    default: printf("unexpected Error code %d\n",iStatus);
  }
  return(NULL);
} // pcGetIniErrorDetailed_m()

/**
* Gibt die aktuelle Uhrzeit als einzelne Werte zur&uuml;ck.
*
* @author WeU (10.06.2015)
*
* @param puiHour
* @param puiMinutes
* @param puiSeconds
*/
void vGetCurrentTime_g(UINT *puiHour,
                       UINT *puiMinutes,
                       UINT *puiSeconds)
{
  time_t      rawtime;
  struct tm  *timeinfo;
  char caHour[100];

  time(&rawtime);
  timeinfo=localtime(&rawtime);
  strftime(caHour,100,"%H",timeinfo); //Amerikanisch auf europäisches Format konvertieren.
  if(puiHour)
    *puiHour=atoi(caHour);
  if(puiMinutes)
    *puiMinutes=timeinfo->tm_min;
  if(puiSeconds)
    *puiSeconds=timeinfo->tm_sec;
} // vGetCurrentTime_g()

