/*****************************************************************************
**  IBS G4 HLI       : Interbus-S Controller Initialization module          **
**  Application type : Win32 application                                    **
**  Controller-ID    : ISASC1                                               **
******************************************************************************
**  Produced by:                                                            **
**  HLI-Export-Filter Version 2.01 (09/2001)                                **
**  (c) Phoenix Contact 01/1998                                             **
*****************************************************************************/

/*======================================================================*/
/* INCLUDE FILES AND CONSTANT DEFINITION */
/*======================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/***********************************************************
* Include files for the CLIENT library Windows version
************************************************************/
#include "ethwin32.h"
#include "SVC_CODE.H"
#include "ibs_cfg.h"

#define MAX_MSG_LENGTH 20
#define MXI_RCV_TIMEOUT 20




/*
+-------------------------------------------------------------------+
|                         GLOBAL VARIABLES                          |
+-------------------------------------------------------------------+*/

/* ------ Global process data objects ------------------------ */

T_IBS_BOOL          eInp00;
T_IBS_BOOL          eInp01;
T_IBS_BOOL          eInp02;
T_IBS_BOOL          eInp03;
T_IBS_BOOL          eInp04;
T_IBS_BOOL          eInp05;
T_IBS_BOOL          eInp06;
T_IBS_BOOL          eInp07;
//==========================
T_IBS_BOOL          eInp10;
T_IBS_BOOL          eInp11;
T_IBS_BOOL          eInp12;
T_IBS_BOOL          eInp13;
T_IBS_BOOL          eInp14;
T_IBS_BOOL          eInp15;
T_IBS_BOOL          eInp16;
T_IBS_BOOL          eInp17;
//==========================
T_IBS_BOOL          eInp20;
T_IBS_BOOL          eInp21;
T_IBS_BOOL          eInp22;
T_IBS_BOOL          eInp23;
T_IBS_BOOL          eInp24;
T_IBS_BOOL          eInp25;
T_IBS_BOOL          eInp26;
T_IBS_BOOL          eInp27;
//==========================
T_IBS_BOOL          eInp30;
T_IBS_BOOL          eInp31;
T_IBS_BOOL          eInp32;
T_IBS_BOOL          eInp33;
T_IBS_BOOL          eInp34;
T_IBS_BOOL          eInp35;
T_IBS_BOOL          eInp36;
T_IBS_BOOL          eInp37;
//==========================
T_IBS_BOOL          eInp38;
T_IBS_BOOL          eInp39;
T_IBS_BOOL          eInp40;
T_IBS_BOOL          eInp41;
T_IBS_BOOL          eInp42;
T_IBS_BOOL          eInp43;
T_IBS_BOOL          eInp44;
T_IBS_BOOL          eInp45;
T_IBS_BOOL          eInp46;
T_IBS_BOOL          eInp47;
//==========================
T_IBS_BOOL          eInp50;
T_IBS_BOOL          eInp51;
T_IBS_BOOL          eInp52;
T_IBS_BOOL          eInp53;
T_IBS_BOOL          eInp54;
T_IBS_BOOL          eInp55;
T_IBS_BOOL          eInp56;
T_IBS_BOOL          eInp57;
//==========================
T_IBS_BOOL          eInp60;
T_IBS_BOOL          eInp61;
T_IBS_BOOL          eInp62;
T_IBS_BOOL          eInp63;
T_IBS_BOOL          eInp64;
T_IBS_BOOL          eInp65;
T_IBS_BOOL          eInp66;
T_IBS_BOOL          eInp67;
//==========================
T_IBS_BOOL          eInp70;
T_IBS_BOOL          eInp71;
T_IBS_BOOL          eInp72;
T_IBS_BOOL          eInp73;
T_IBS_BOOL          eInp74;
T_IBS_BOOL          eInp75;
T_IBS_BOOL          eInp76;
T_IBS_BOOL          eInp77;
//==========================
T_IBS_BOOL          eInp80;
T_IBS_BOOL          eInp81;
T_IBS_BOOL          eInp82;
T_IBS_BOOL          eInp83;
T_IBS_BOOL          eInp84;
T_IBS_BOOL          eInp85;
T_IBS_BOOL          eInp86;
T_IBS_BOOL          eInp87;
//==========================
T_IBS_BOOL          eInp90;
T_IBS_BOOL          eInp91;
T_IBS_BOOL          eInp92;
T_IBS_BOOL          eInp93;
T_IBS_BOOL          eInp94;
T_IBS_BOOL          eInp95;
T_IBS_BOOL          eInp96;
T_IBS_BOOL          eInp97;
//==========================
T_IBS_BOOL          aOut00;
T_IBS_BOOL          aOut01;
T_IBS_BOOL          aOut02;
T_IBS_BOOL          aOut03;
T_IBS_BOOL          aOut04;
T_IBS_BOOL          aOut05;
T_IBS_BOOL          aOut06;
T_IBS_BOOL          aOut07;
//==========================
T_IBS_BOOL          aOut10;
T_IBS_BOOL          aOut11;
T_IBS_BOOL          aOut12;
T_IBS_BOOL          aOut13;
T_IBS_BOOL          aOut14;
T_IBS_BOOL          aOut15;
T_IBS_BOOL          aOut16;
T_IBS_BOOL          aOut17;
//==========================
T_IBS_BOOL          aOut20;
T_IBS_BOOL          aOut21;
T_IBS_BOOL          aOut22;
T_IBS_BOOL          aOut23;
T_IBS_BOOL          aOut24;
T_IBS_BOOL          aOut25;
T_IBS_BOOL          aOut26;
T_IBS_BOOL          aOut27;
//==========================
T_IBS_BOOL          aOut30;
T_IBS_BOOL          aOut31;
T_IBS_BOOL          aOut32;
T_IBS_BOOL          aOut33;
T_IBS_BOOL          aOut34;
T_IBS_BOOL          aOut35;
T_IBS_BOOL          aOut36;
T_IBS_BOOL          aOut37;
//==========================
T_IBS_BOOL          aOut40;
T_IBS_BOOL          aOut41;
T_IBS_BOOL          aOut42;
T_IBS_BOOL          aOut43;
T_IBS_BOOL          aOut44;
T_IBS_BOOL          aOut45;
T_IBS_BOOL          aOut46;
T_IBS_BOOL          aOut47;
//==========================
T_IBS_BOOL          aOut50;
T_IBS_BOOL          aOut51;
T_IBS_BOOL          aOut52;
T_IBS_BOOL          aOut53;
T_IBS_BOOL          aOut54;
T_IBS_BOOL          aOut55;
T_IBS_BOOL          aOut56;
T_IBS_BOOL          aOut57;
//==========================
T_IBS_BOOL          aOut60;
T_IBS_BOOL          aOut61;
T_IBS_BOOL          aOut62;
T_IBS_BOOL          aOut63;
T_IBS_BOOL          aOut64;
T_IBS_BOOL          aOut65;
T_IBS_BOOL          aOut66;
T_IBS_BOOL          aOut67;
//==========================
T_IBS_BOOL          aOut70;
T_IBS_BOOL          aOut71;
T_IBS_BOOL          aOut72;
T_IBS_BOOL          aOut73;
T_IBS_BOOL          aOut74;
T_IBS_BOOL          aOut75;
T_IBS_BOOL          aOut76;
T_IBS_BOOL          aOut77;
//==========================
T_IBS_BOOL          aOut80;
T_IBS_BOOL          aOut81;
T_IBS_BOOL          aOut82;
T_IBS_BOOL          aOut83;
T_IBS_BOOL          aOut84;
T_IBS_BOOL          aOut85;
T_IBS_BOOL          aOut86;
T_IBS_BOOL          aOut87;
//==========================
T_IBS_BOOL          aOut90;
T_IBS_BOOL          aOut91;
T_IBS_BOOL          aOut92;
T_IBS_BOOL          aOut93;
T_IBS_BOOL          aOut94;
T_IBS_BOOL          aOut95;
T_IBS_BOOL          aOut96;
T_IBS_BOOL          aOut97;
//==========================


T_IBS_WORD          eAI1;
T_IBS_WORD          eAI2;
T_IBS_WORD          eAI3;
T_IBS_WORD          eAO1AW;
T_IBS_WORD          eAO2AW;
T_IBS_WORD          aAO1;
T_IBS_WORD          aAO2;

BOOL bIBSRunning;



char caOpen_MXI[20]="IBETH01N1_M";
char caOpen_DTI[20]="IBETH01N1_D";

IBDDIHND mxiHnd, dtiHnd;
T_DDI_MXI_ACCESS mxiAcc;
T_DDI_DTI_ACCESS dti_Write;
T_DDI_DTI_ACCESS dti_Read;

USIGN8 usWriteData[20];
USIGN8 usReadData[20];


void vUpdateSendData_m(void);
void vUpdateReceiveData_m(void);


/**
 * Kommunikation mit ETH-Inline initialisieren
 *
 * @return 0 = Erfolg
 *         <>0 = Fehler
 */
int iIBS_HLI_Init_g(void)
{
  int iRet;
  time_t ltime;
  time_t starttime;
  T_IBS_DIAG infoPtr;
  USIGN16 PlugPlayModus=111;

  /*Mailbox connection*/
  iRet=DDI_DevOpenNode(caOpen_MXI,DDI_RW,&mxiHnd);
  if(iRet!=ERR_OK)
  {
    printf("Error when creating mailbox connection. Error code: 0x%x\n",iRet^ERR_BASE);
    return(-1);
  }

  /*Data channel connection*/
  iRet=DDI_DevOpenNode(caOpen_DTI,DDI_RW,&dtiHnd);
  if(iRet!=ERR_OK)
  {
    printf("Error when creating data channel connection. Error code: %x\n",iRet^ERR_BASE);
    return(-1);
  }




  Sleep(500);
  // Read plug & play mode
  mxiAcc.msgLength=8;
  mxiAcc.msgBlk=usWriteData;
  IB_SetCmdCode(usWriteData,0x0351);
  IB_SetParaCnt(usWriteData,2);
  IB_SetParaN(usWriteData,1,1);
  IB_SetParaN(usWriteData,2,0x2240);
  iRet=DDI_MXI_SndMessage(mxiHnd,&mxiAcc);
  if(iRet!=ERR_OK)
  {
    printf("FAIL Error code %X", iRet^ERR_BASE);
  }
  // Get service confirmation
  mxiAcc.msgLength=128;
  time(&starttime);
  iRet=555;
  do
  {
    iRet=DDI_MXI_RcvMessage(mxiHnd,&mxiAcc);
    time(&ltime);
  }
  while(((ltime - starttime) < MXI_RCV_TIMEOUT) && (iRet!=ERR_OK));
  if(iRet!=ERR_OK)
  {
    printf("Incorrect confirmation received, Error code 0x%04X\n",iRet^ERR_BASE);
  }
  else
  {
    PlugPlayModus=(USHORT)IB_GetParaN(usWriteData,4);
  }
  if(PlugPlayModus!=0)
  {
    // If plug & play mode is active, no data can be written
    // -> End of test
    printf("Plug & play mode is active -> aborted\n");
    return(-1);
  }

  // Activate configuration   (siehe Seite 3-9)
  //printf("Activate configuration\n");
  mxiAcc.msgLength=4;
  mxiAcc.msgBlk=usWriteData;
  IB_SetCmdCode(usWriteData,S_ACTIVATE_CFG_REQ);
  IB_SetParaCnt(usWriteData,0);
  iRet=DDI_MXI_SndMessage(mxiHnd,&mxiAcc);
  if(iRet!=ERR_OK)
  {
    printf("Activate configuration failed with code 0x%x\n",iRet^ERR_BASE);
  }
  Sleep(50);

  // Start configuration   (siehe Seite 3-9)
  //printf("Start configuration\n");
  mxiAcc.msgLength=4;
  mxiAcc.msgBlk=usWriteData;
  IB_SetCmdCode(usWriteData,S_START_DATA_TRANSFER_REQ);
  IB_SetParaCnt(usWriteData,0);
  iRet=DDI_MXI_SndMessage(mxiHnd,&mxiAcc);
  if(iRet!=ERR_OK)
  {
    printf("Start configuration failed with code 0x%x\n",iRet^ERR_BASE);
  }
  Sleep(50);

  //Read IBS status
  iRet=GetIBSDiagnostic(dtiHnd,&infoPtr);
  if(iRet!=ERR_OK)
  {
    printf("Error when reading INTERBUS status. Error code: 0x%04X\n",iRet^ERR_BASE);
  }
  else
  {
    if(infoPtr.state!=(RUN_BIT|ACTIVE_BIT|READY_BIT))
    {
      printf("IBS status: 0x%04X\n",infoPtr.state);
    }
  }
  // Reading and writing only permitted when the bus is running
  if(infoPtr.state!=(RUN_BIT|ACTIVE_BIT|READY_BIT))
  {
    printf("IBS not in RUN state. -> Abort\n");
    return(-1);
  }


  // Set buffer to ZERO
  dti_Write.length=MAX_MSG_LENGTH;
  dti_Write.address=0;
  dti_Write.dataCons=DTI_DATA_WORD; // Specify data consistency
  dti_Write.data=usWriteData;
  memset((void*)usWriteData,0,MAX_MSG_LENGTH*sizeof(USIGN8));

  iRet=DDI_DTI_WriteData(dtiHnd,&dti_Write);
  if(iRet!=ERR_OK)
    printf("Error when resetting buffer. Error code: 0x%04X\n",iRet^ERR_BASE);
  Sleep(100);


  dti_Write.length=MAX_MSG_LENGTH;
  dti_Write.address=0;
  dti_Write.dataCons=DTI_DATA_WORD; //Specify data consistency
  dti_Write.data=usWriteData;

  dti_Read.length=MAX_MSG_LENGTH;
  dti_Read.address=0;
  dti_Read.dataCons=DTI_DATA_WORD; //Specify data consistency
  dti_Read.data=usReadData;

  memset((void*)usReadData,0,sizeof(usReadData));

  bIBSRunning=TRUE;

  return(0);
} // iIBS_HLI_Init_g()


/**
 * Kommunikation mit ETH-Inline terminieren
 *
 * @return 0 = Erfolg
 *         <>0 = Fehler
 */
int iIBS_HLI_Exit_g(void)
{
  int iRet;

  bIBSRunning=FALSE;

  /* Close mailbox channel */
  iRet=DDI_DevCloseNode(mxiHnd);
  if(iRet!=ERR_OK)
  {
    //printf("Error when closing mailbox channel. Error code: %x\n",iRet^ERR_BASE);
    return(-1);
  }

  /* Close data channel */
  iRet=DDI_DevCloseNode(dtiHnd);
  if(iRet!=ERR_OK)
  {
    //printf("Error when closing data channel. Error code: %x\n",iRet^ERR_BASE);
    return(-1);
  }

  return(0);
} // iIBS_HLI_Exit_g()


/**
 * Abfrage, ob der Bus noch läuft
 *
 * @return TRUE wenn der Bus läuft<BR>
 *         sonst FALSE
 */
BOOL bIBS_HLI_Running_g(void)
{
  return(bIBSRunning);
}


/**
 * Datenaustausch mit ETH-Inline durchführen
 *
 * @return 0 = Erfolg
 *         <>0 = Fehler
 */
int iIBS_HLI_Process_g(void)
{
  IBDDIRET locRet;

  if(!bIBSRunning)
    return(-1);

  vUpdateSendData_m();

  locRet=DDI_DTI_ReadWriteData(dtiHnd,&dti_Write,&dti_Read);
  if(locRet!=0)
  {
    printf("  Read/Write data failed with code 0x%04X\n",ERR_BASE-locRet);
    return(-2);
  }

  vUpdateReceiveData_m();

  return(0);
} // iIBS_HLI_Process_g()


/**
 * Lokale Ausgangsdaten in Sendebuffer übertragen
 */
void vUpdateSendData_m(void)
{
  WORD wData;
//#define AO_TEST   // zum Testen Wert von analogen Signal alle 2 Sekunden erhöhen (~1.5%)
#ifdef AO_TEST
  static int iDelay;
  static WORD wTest;
#endif // !AO_TEST

  // Daten als WORD setzen
  wData=(WORD)(((aOut00)?1:0)<<0
              |((aOut01)?1:0)<<1
              |((aOut02)?1:0)<<2
              |((aOut03)?1:0)<<3
              |((aOut04)?1:0)<<4
              |((aOut05)?1:0)<<5
              |((aOut06)?1:0)<<6
              |((aOut07)?1:0)<<7
              |((aOut10)?1:0)<<8
              |((aOut11)?1:0)<<9
              |((aOut12)?1:0)<<10
              |((aOut13)?1:0)<<11
              |((aOut14)?1:0)<<12
              |((aOut15)?1:0)<<13
              |((aOut16)?1:0)<<14
              |((aOut17)?1:0)<<15);
  IB_PD_SetDataN(usWriteData,0,wData);

  wData=(WORD)(((aOut20)?1:0)<<0
              |((aOut21)?1:0)<<1
              |((aOut22)?1:0)<<2
              |((aOut23)?1:0)<<3
              |((aOut24)?1:0)<<4
              |((aOut25)?1:0)<<5
              |((aOut26)?1:0)<<6
              |((aOut27)?1:0)<<7
              |((aOut30)?1:0)<<8
              |((aOut31)?1:0)<<9
              |((aOut32)?1:0)<<10
              |((aOut33)?1:0)<<11
              |((aOut34)?1:0)<<12
              |((aOut35)?1:0)<<13
              |((aOut36)?1:0)<<14
              |((aOut37)?1:0)<<15);
  IB_PD_SetDataN(usWriteData,1,wData);

  wData=(WORD)(((aOut40)?1:0)<<0
              |((aOut41)?1:0)<<1
              |((aOut42)?1:0)<<2
              |((aOut43)?1:0)<<3
              |((aOut44)?1:0)<<4
              |((aOut45)?1:0)<<5
              |((aOut46)?1:0)<<6
              |((aOut47)?1:0)<<7
              |((aOut50)?1:0)<<8
              |((aOut51)?1:0)<<9
              |((aOut52)?1:0)<<10
              |((aOut53)?1:0)<<11
              |((aOut54)?1:0)<<12
              |((aOut55)?1:0)<<13
              |((aOut56)?1:0)<<14
              |((aOut57)?1:0)<<15);
  IB_PD_SetDataN(usWriteData,2,wData);

  wData=(WORD)(((aOut60)?1:0)<<0
              |((aOut61)?1:0)<<1
              |((aOut62)?1:0)<<2
              |((aOut63)?1:0)<<3
              |((aOut64)?1:0)<<4
              |((aOut65)?1:0)<<5
              |((aOut66)?1:0)<<6
              |((aOut67)?1:0)<<7
              |((aOut70)?1:0)<<8
              |((aOut71)?1:0)<<9
              |((aOut72)?1:0)<<10
              |((aOut73)?1:0)<<11
              |((aOut74)?1:0)<<12
              |((aOut75)?1:0)<<13
              |((aOut76)?1:0)<<14
              |((aOut77)?1:0)<<15);
  IB_PD_SetDataN(usWriteData,3,wData);

#ifdef AO_TEST
  if(++iDelay>20)
  {
    // Alle ca. 2 Sekunden Wert von wTest erhöhen
    iDelay=0;
    wTest+=500;
  }
  if(wTest>0xffff)
  {
    wTest=0;
  }
  //IB_PD_SetDataN(usWriteData,4,wTest);
  //IB_PD_SetDataN(usWriteData,5,wTest);
  IB_PD_SetDataN(usWriteData,6,wTest);
  IB_PD_SetDataN(usWriteData,7,abs(0xffff-wTest));
  //IB_PD_SetDataN(usWriteData,8,wTest);
#else // !AO_TEST
  IB_PD_SetDataN(usWriteData,6,aAO1);
  IB_PD_SetDataN(usWriteData,7,aAO2);
#endif // !AO_TEST
} // vUpdateSendData_m()


/**
 * Daten aus Readbuffer in lokale Eingangsdaten übertragen
 */
void vUpdateReceiveData_m(void)
{
  // Daten auslesen
  eInp00 = (IB_PD_GetDataN(usReadData,0)& (1<<0)) >>0;
  eInp01 = (IB_PD_GetDataN(usReadData,0)& (1<<1)) >>1;
  eInp02 = (IB_PD_GetDataN(usReadData,0)& (1<<2)) >>2;
  eInp03 = (IB_PD_GetDataN(usReadData,0)& (1<<3)) >>3;
  eInp04 = (IB_PD_GetDataN(usReadData,0)& (1<<4)) >>4;
  eInp05 = (IB_PD_GetDataN(usReadData,0)& (1<<5)) >>5;
  eInp06 = (IB_PD_GetDataN(usReadData,0)& (1<<6)) >>6;
  eInp07 = (IB_PD_GetDataN(usReadData,0)& (1<<7)) >>7;

  eInp10 = (IB_PD_GetDataN(usReadData,0)& (1<<8)) >>8;
  eInp11 = (IB_PD_GetDataN(usReadData,0)& (1<<9)) >>9;
  eInp12 = (IB_PD_GetDataN(usReadData,0)& (1<<10))>>10;
  eInp13 = (IB_PD_GetDataN(usReadData,0)& (1<<11))>>11;
  eInp14 = (IB_PD_GetDataN(usReadData,0)& (1<<12))>>12;
  eInp15 = (IB_PD_GetDataN(usReadData,0)& (1<<13))>>13;
  eInp16 = (IB_PD_GetDataN(usReadData,0)& (1<<14))>>14;
  eInp17 = (IB_PD_GetDataN(usReadData,0)& (1<<15))>>15;

  eInp20 = (IB_PD_GetDataN(usReadData,1)& (1<<0)) >>0;
  eInp21 = (IB_PD_GetDataN(usReadData,1)& (1<<1)) >>1;
  eInp22 = (IB_PD_GetDataN(usReadData,1)& (1<<2)) >>2;
  eInp23 = (IB_PD_GetDataN(usReadData,1)& (1<<3)) >>3;
  eInp24 = (IB_PD_GetDataN(usReadData,1)& (1<<4)) >>4;
  eInp25 = (IB_PD_GetDataN(usReadData,1)& (1<<5)) >>5;
  eInp26 = (IB_PD_GetDataN(usReadData,1)& (1<<6)) >>6;
  eInp27 = (IB_PD_GetDataN(usReadData,1)& (1<<7)) >>7;

  eInp30 = (IB_PD_GetDataN(usReadData,1)& (1<<8)) >>8;
  eInp31 = (IB_PD_GetDataN(usReadData,1)& (1<<9)) >>9;
  eInp32 = (IB_PD_GetDataN(usReadData,1)& (1<<10))>>10;
  eInp33 = (IB_PD_GetDataN(usReadData,1)& (1<<11))>>11;
  eInp34 = (IB_PD_GetDataN(usReadData,1)& (1<<12))>>12;
  eInp35 = (IB_PD_GetDataN(usReadData,1)& (1<<13))>>13;
  eInp36 = (IB_PD_GetDataN(usReadData,1)& (1<<14))>>14;
  eInp37 = (IB_PD_GetDataN(usReadData,1)& (1<<15))>>15;

  eAI1 = (USHORT)IB_PD_GetDataN(usReadData,2);
  eAI2 = (USHORT)IB_PD_GetDataN(usReadData,3);
  eAI3 = (USHORT)IB_PD_GetDataN(usReadData,4);

} // vUpdateReceiveData_m()

