/*******************************************************************
 *
 *    PROJECT:     FlexCluster
 *
 *    EQUIPMENT:   KWP2000
 *
 *    FILE:        ProdInterlock.c
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
#include <time.h>

#ifdef _CVI_
  #include <ansi_c.h>
  #include <utility.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
#endif


#include "ProdInterlock.h"


/**
 * convert from int to bcd
 *
 * @author uidc9013 (23.03.2010)
 *
 * @param iDec dec. Value to convert
 *
 * @return int converted value in bcd
 */
int iInt2Bcd_g(int iDec)
{
  return(((iDec/10)<<4)+(iDec%10));
} // iInt2Bcd_g()


/**
 * convert from bcd to int
 *
 * @author uidc9013 (23.03.2010)
 *
 * @param iDec bcd Value to convert
 *
 * @return int converted value in int
 */
int iBcd2Int_g(int iBcd)
{
  return(((iBcd>>4)*10)+iBcd%16);
} // iBcd2Int_g()




/**
 * Setzt mit den übergebenen Parameter den Buffer zusammen
 *
 * @param eInterlockFormat
 *               Format von Interlock
 * @param uiVersionMajor
 *               Versionsnummer, Wertigkeit: 0..9
 * @param uiVersionMinor
 *               Sub-Versionsnummer, Wertigkeit: 0..999
 * @param uiRunCounter
 *               Durchlaufszähler 0..255
 * @param pucDataBuffer
 *               Datenbuffer mit zusammengesetzten Interlockdaten als Rückgabewert
 * @param iBufferSize
 *               Grösse des übergebenen Datenbuffers
 *
 * @return TRUE bei Erfolg<BR>
 *         FALSE bei Fehler
 */
BOOL bPIL_SetData_g(EInterlockFormat  eInterlockFormat,
                    unsigned int      uiVersionMajor,
                    unsigned int      uiVersionMinor,
                    unsigned int      uiRunCounter,
                    unsigned char    *pucDataBuffer,
                    unsigned int      iBufferSize)
{
  time_t timeLocalTime;
  struct tm *ptmLocalTime;

  time(&timeLocalTime);
  ptmLocalTime=localtime(&timeLocalTime);

  if(eInterlockFormat==eInterlockFormat_Complete)
  {
    pucDataBuffer[0]=(UCHAR)(min(uiVersionMajor,9)<<4);
    pucDataBuffer[0]+=iInt2Bcd_g(uiVersionMinor/100);      // 4 upper byte Version Major + 4 MSB Version Minor
    pucDataBuffer[1]=(UCHAR)iInt2Bcd_g(uiVersionMinor%100);       // 8 byte LSB Version Minor

    pucDataBuffer[2]=(UCHAR)iInt2Bcd_g((ptmLocalTime->tm_year+1900)/100);  // Year XX__
    pucDataBuffer[3]=(UCHAR)iInt2Bcd_g((ptmLocalTime->tm_year+1900)%100);  // Year __XX
    pucDataBuffer[4]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_mon+1);     // Month
    pucDataBuffer[5]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_mday);      // Day

    pucDataBuffer[6]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_hour);   // Hour
    pucDataBuffer[7]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_min);    // Minute
    pucDataBuffer[8]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_sec);    // Second

    pucDataBuffer[9]=uiRunCounter&0xFF;   // Counter
  }
  else if(eInterlockFormat==eInterlockFormat_1bytes)
  {
    pucDataBuffer[0]=uiRunCounter&0xFF;   // Counter
  }
  else if(eInterlockFormat==eInterlockFormat_2bytes)
  {
    pucDataBuffer[0]=(UCHAR)min(uiVersionMinor,255);
    pucDataBuffer[1]=uiRunCounter&0xFF;   // Counter
  }
  else if(eInterlockFormat==eInterlockFormat_6bytes)
  {
    pucDataBuffer[0]=(UCHAR)min(uiVersionMinor,255);
    pucDataBuffer[1]=(UCHAR)iInt2Bcd_g((ptmLocalTime->tm_year+1900)%100);  // Year __XX
    pucDataBuffer[2]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_mon+1);     // Month
    pucDataBuffer[3]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_mday);      // Day
    pucDataBuffer[4]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_hour);   // Hour
    pucDataBuffer[5]=(UCHAR)iInt2Bcd_g(ptmLocalTime->tm_min);    // Minute
  }
  else
  {
    return(FALSE);
  }

  return(TRUE);
} // bPIL_SetData_g()


/**
 * Setzt mit den übergebenen Parameter den Buffer zusammen
 *
 * @param eInterlockFormat
 *               Format von Interlock
 * @param uiVersionMajor
 *               Versionsnummer, Wertigkeit: 0..9
 * @param uiVersionMinor
 *               Sub-Versionsnummer, Wertigkeit: 0..999
 * @param uiRunCounter
 *               Durchlaufszähler 0..255
 * @param pucDataBuffer
 *               Datenbuffer mit zusammengesetzten Interlockdaten als Rückgabewert
 * @param iBufferSize
 *               Grösse des übergebenen Datenbuffers
 *
 * @return TRUE bei Erfolg<BR>
 *         FALSE bei Fehler
 */
BOOL bPIL_GetData_g(EInterlockFormat  eInterlockFormat,    // IN
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
                    unsigned int     *puiSecond          // OUT
                   )
{
  unsigned char aucDataBuffer[20];
  int iIndex;

  memset(aucDataBuffer,0,sizeof(aucDataBuffer));

  // 0xFF aus Eeprom in 0x0 umwandeln
  for(iIndex=0;
       iIndex<iBufferSize;
       iIndex++)
  {
    if(pucDataBuffer[iIndex]==0xFF)
    {
      // Data von Eeprom invalid
      aucDataBuffer[iIndex]=0;
    }
    else
      aucDataBuffer[iIndex]=pucDataBuffer[iIndex];
  }

  if(eInterlockFormat==eInterlockFormat_Complete)
  {
    if(puiVersionMajor)
      *puiVersionMajor=aucDataBuffer[0]>>4;
    if(puiVersionMinor)
    {
      *puiVersionMinor=iBcd2Int_g(aucDataBuffer[0]&0xF)*100;
      *puiVersionMinor+=iBcd2Int_g(aucDataBuffer[1]);
    }

    if(puiRunCounter)
      *puiRunCounter=aucDataBuffer[9];

    if(puiYear)
    {
      *puiYear=iBcd2Int_g(aucDataBuffer[2])*100;
      *puiYear+=iBcd2Int_g(aucDataBuffer[3]);
    }
    if(puiMonth)
      *puiMonth=iBcd2Int_g(aucDataBuffer[4]);
    if(puiDay)
      *puiDay=iBcd2Int_g(aucDataBuffer[5]);

    if(puiHour)
      *puiHour=iBcd2Int_g(aucDataBuffer[6]);
    if(puiMinute)
      *puiMinute=iBcd2Int_g(aucDataBuffer[7]);
    if(puiSecond)
      *puiSecond=iBcd2Int_g(aucDataBuffer[8]);
  }
  else if(eInterlockFormat==eInterlockFormat_6bytes)
  {
    if(puiVersionMinor)
      *puiVersionMinor=aucDataBuffer[0];

    if(puiYear)
      *puiYear+=iBcd2Int_g(aucDataBuffer[1]);
    if(puiMonth)
      *puiMonth=iBcd2Int_g(aucDataBuffer[2]);
    if(puiDay)
      *puiDay=iBcd2Int_g(aucDataBuffer[3]);

    if(puiHour)
      *puiHour=iBcd2Int_g(aucDataBuffer[4]);
    if(puiMinute)
      *puiMinute=iBcd2Int_g(aucDataBuffer[5]);

    if(puiRunCounter)
      *puiRunCounter=aucDataBuffer[6];
  }
  else if(eInterlockFormat==eInterlockFormat_1bytes)
  {
    if(puiRunCounter)
      *puiRunCounter=aucDataBuffer[0];
  }
  else if(eInterlockFormat==eInterlockFormat_2bytes)
  {
    if(puiRunCounter)
      *puiRunCounter=aucDataBuffer[1];
    if(puiVersionMinor)
      *puiVersionMinor=aucDataBuffer[0];
  }
  else
  {
    return(FALSE);
  }

  return(TRUE);
} // bPIL_GetData_g()




void vDumpBytes_g(char *pcVariableName,
                  unsigned char *pucBytes,
                  int   iSize)
{
  int iIndex;

  printf("%s =",pcVariableName);
  for(iIndex=0;
     iIndex<iSize;
     iIndex++)
  {
    printf(" %02x",pucBytes[iIndex]);
  }
  printf("\n");
} // vDumpBytes_g()
