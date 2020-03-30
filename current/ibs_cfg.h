/*****************************************************************************
**  IBS G4 HLI       : Interbus-S Controller Initialization header file     **
**  Application type : Win32 application                                    **
**  Controller-ID    : ISASC1                                               **
******************************************************************************
**  Produced by:                                                            **
**  HLI-Export-Filter Version 2.01 (09/2001)                                  **
**  (c) Phoenix Contact 01/1998                                             **
*****************************************************************************/

#ifndef IBS_CFG_H_INCLUDED
  #define IBS_CFG_H_INCLUDED

//  ------------------------------------------------------------------------------------------------
//  Typendefinitionen
//  ------------------------------------------------------------------------------------------------

  #define T_IBS_BOOL                     unsigned short
  #define T_IBS_BITSTRING                unsigned short
  #define T_IBS_BYTE                     unsigned char
  #define T_IBS_WORD                     unsigned short
  #define T_IBS_DWORD                    unsigned long

  #define HLIRET                         unsigned short


/* ------ Global vars (external linkage ) */

/* Process data object vars */

extern T_IBS_WORD          eAI1;
extern T_IBS_WORD          eAI2;
extern T_IBS_WORD          eAI3;
extern T_IBS_WORD          eAO1AW;
extern T_IBS_WORD          eAO2AW;
extern T_IBS_WORD          aAO1;
extern T_IBS_WORD          aAO2;
extern T_IBS_BOOL          eInp00;
extern T_IBS_BOOL          eInp01;
extern T_IBS_BOOL          eInp02;
extern T_IBS_BOOL          eInp03;
extern T_IBS_BOOL          eInp04;
extern T_IBS_BOOL          eInp05;
extern T_IBS_BOOL          eInp06;
extern T_IBS_BOOL          eInp07;
extern T_IBS_BOOL          eInp10;
extern T_IBS_BOOL          eInp11;
extern T_IBS_BOOL          eInp12;
extern T_IBS_BOOL          eInp13;
extern T_IBS_BOOL          eInp14;
extern T_IBS_BOOL          eInp15;
extern T_IBS_BOOL          eInp16;
extern T_IBS_BOOL          eInp17;
extern T_IBS_BOOL          eInp20;
extern T_IBS_BOOL          eInp21;
extern T_IBS_BOOL          eInp22;
extern T_IBS_BOOL          eInp23;
extern T_IBS_BOOL          eInp24;
extern T_IBS_BOOL          eInp25;
extern T_IBS_BOOL          eInp26;
extern T_IBS_BOOL          eInp27;
extern T_IBS_BOOL          eInp30;
extern T_IBS_BOOL          eInp31;
extern T_IBS_BOOL          eInp32;
extern T_IBS_BOOL          eInp33;
extern T_IBS_BOOL          eInp34;
extern T_IBS_BOOL          eInp35;
extern T_IBS_BOOL          eInp36;
extern T_IBS_BOOL          eInp37;
extern T_IBS_BOOL          eInp40;
extern T_IBS_BOOL          eInp41;
extern T_IBS_BOOL          eInp42;
extern T_IBS_BOOL          eInp43;
extern T_IBS_BOOL          eInp44;
extern T_IBS_BOOL          eInp45;
extern T_IBS_BOOL          eInp46;
extern T_IBS_BOOL          eInp47;
extern T_IBS_BOOL          eInp50;
extern T_IBS_BOOL          eInp51;
extern T_IBS_BOOL          eInp52;
extern T_IBS_BOOL          eInp53;
extern T_IBS_BOOL          eInp54;
extern T_IBS_BOOL          eInp55;
extern T_IBS_BOOL          eInp56;
extern T_IBS_BOOL          eInp57;
extern T_IBS_BOOL          eInp60;
extern T_IBS_BOOL          eInp61;
extern T_IBS_BOOL          eInp62;
extern T_IBS_BOOL          eInp63;
extern T_IBS_BOOL          eInp64;
extern T_IBS_BOOL          eInp65;
extern T_IBS_BOOL          eInp66;
extern T_IBS_BOOL          eInp67;
extern T_IBS_BOOL          eInp70;
extern T_IBS_BOOL          eInp71;
extern T_IBS_BOOL          eInp72;
extern T_IBS_BOOL          eInp73;
extern T_IBS_BOOL          eInp74;
extern T_IBS_BOOL          eInp75;
extern T_IBS_BOOL          eInp76;
extern T_IBS_BOOL          eInp77;
extern T_IBS_BOOL          eInp80;
extern T_IBS_BOOL          eInp81;
extern T_IBS_BOOL          eInp82;
extern T_IBS_BOOL          eInp83;
extern T_IBS_BOOL          eInp84;
extern T_IBS_BOOL          eInp85;
extern T_IBS_BOOL          eInp86;
extern T_IBS_BOOL          eInp87;
extern T_IBS_BOOL          eInp90;
extern T_IBS_BOOL          eInp91;
extern T_IBS_BOOL          eInp92;
extern T_IBS_BOOL          eInp93;
extern T_IBS_BOOL          eInp94;
extern T_IBS_BOOL          eInp95;
extern T_IBS_BOOL          eInp96;
extern T_IBS_BOOL          eInp97;
extern T_IBS_BOOL          aOut00;
extern T_IBS_BOOL          aOut01;
extern T_IBS_BOOL          aOut02;
extern T_IBS_BOOL          aOut03;
extern T_IBS_BOOL          aOut04;
extern T_IBS_BOOL          aOut05;
extern T_IBS_BOOL          aOut06;
extern T_IBS_BOOL          aOut07;
extern T_IBS_BOOL          aOut10;
extern T_IBS_BOOL          aOut11;
extern T_IBS_BOOL          aOut12;
extern T_IBS_BOOL          aOut13;
extern T_IBS_BOOL          aOut14;
extern T_IBS_BOOL          aOut15;
extern T_IBS_BOOL          aOut16;
extern T_IBS_BOOL          aOut17;
extern T_IBS_BOOL          aOut20;
extern T_IBS_BOOL          aOut21;
extern T_IBS_BOOL          aOut22;
extern T_IBS_BOOL          aOut23;
extern T_IBS_BOOL          aOut24;
extern T_IBS_BOOL          aOut25;
extern T_IBS_BOOL          aOut26;
extern T_IBS_BOOL          aOut27;
extern T_IBS_BOOL          aOut30;
extern T_IBS_BOOL          aOut31;
extern T_IBS_BOOL          aOut32;
extern T_IBS_BOOL          aOut33;
extern T_IBS_BOOL          aOut34;
extern T_IBS_BOOL          aOut35;
extern T_IBS_BOOL          aOut36;
extern T_IBS_BOOL          aOut37;
extern T_IBS_BOOL          aOut40;
extern T_IBS_BOOL          aOut41;
extern T_IBS_BOOL          aOut42;
extern T_IBS_BOOL          aOut43;
extern T_IBS_BOOL          aOut44;
extern T_IBS_BOOL          aOut45;
extern T_IBS_BOOL          aOut46;
extern T_IBS_BOOL          aOut47;
extern T_IBS_BOOL          aOut50;
extern T_IBS_BOOL          aOut51;
extern T_IBS_BOOL          aOut52;
extern T_IBS_BOOL          aOut53;
extern T_IBS_BOOL          aOut54;
extern T_IBS_BOOL          aOut55;
extern T_IBS_BOOL          aOut56;
extern T_IBS_BOOL          aOut57;
extern T_IBS_BOOL          aOut60;
extern T_IBS_BOOL          aOut61;
extern T_IBS_BOOL          aOut62;
extern T_IBS_BOOL          aOut63;
extern T_IBS_BOOL          aOut64;
extern T_IBS_BOOL          aOut65;
extern T_IBS_BOOL          aOut66;
extern T_IBS_BOOL          aOut67;
extern T_IBS_BOOL          aOut70;
extern T_IBS_BOOL          aOut71;
extern T_IBS_BOOL          aOut72;
extern T_IBS_BOOL          aOut73;
extern T_IBS_BOOL          aOut74;
extern T_IBS_BOOL          aOut75;
extern T_IBS_BOOL          aOut76;
extern T_IBS_BOOL          aOut77;
extern T_IBS_BOOL          aOut78;
extern T_IBS_BOOL          aOut81;
extern T_IBS_BOOL          aOut82;
extern T_IBS_BOOL          aOut83;
extern T_IBS_BOOL          aOut84;
extern T_IBS_BOOL          aOut85;
extern T_IBS_BOOL          aOut86;
extern T_IBS_BOOL          aOut87;
extern T_IBS_BOOL          aOut90;
extern T_IBS_BOOL          aOut91;
extern T_IBS_BOOL          aOut92;
extern T_IBS_BOOL          aOut93;
extern T_IBS_BOOL          aOut94;
extern T_IBS_BOOL          aOut95;
extern T_IBS_BOOL          aOut96;
extern T_IBS_BOOL          aOut97;

/* ------ Controller initialization function */
extern int iIBS_HLI_Init_g(void);

/* ------ Controller de-initialization function */
extern int iIBS_HLI_Exit_g(void);

/* ------ HLI state machine and process data refresh */
extern int iIBS_HLI_Process_g(void);

extern BOOL bIBS_HLI_Running_g(void);


#endif // IBS_CFG_H_INCLUDED
