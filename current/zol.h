//
//  zol.h
//
//  1.10.04 /LoU
//
//

#ifndef ZOL_H_INCLUDED
  #define ZOL_H_INCLUDED

#include "ibs_cfg.h"

//
// digitaler Input
typedef struct
{
  int          iModulNr;
  int          iControl;
  T_IBS_BOOL  *IBS_Node;                  // Input-Wert
  char        *pcTxt;
}tagDigInp;

// digitaler Output
typedef struct
{
  int          iModulNr;
  int          iControl;
  T_IBS_BOOL  *IBS_Node;                  // Output-Wert
  char        *pcTxt;
}tagDigOut;

// analoger Input
typedef struct
{
  int          iModulNr;
  int          iControl;
  float        fConv;
  T_IBS_WORD  *IBS_Node;                  // Input-Wert
  char        *pcTxt;
}tagAInp;

// analoger Output
typedef struct
{
  int          iModulNr;
  int          iControl;                  // Control-ID für Sollwert
  int          iControlAW;                // Control-ID für Istwert
  float        fConv;                     //
  T_IBS_WORD  *IBS_Node;                  // Istwert von Output
  T_IBS_WORD  *IBS_NodeAW;                // Sollwert von Output
  char        *pcTxt;
}tagAOut;


extern tagDigInp  eNotausOk;
extern tagDigInp  eTasteStart;
extern tagDigInp  eTasteQuittierung;
extern tagDigInp  eNotausEntriegelt;
// E0.4 Reserve
// E0.5 Reserve
extern tagDigInp  eTeilEingelegt;
// E0.7 Reserve
//==========================
//extern tagDigInp  eVerriegelungRS;
//extern tagDigInp  eVerriegelungAS;
//extern tagDigInp  eKontaktAS;
//extern tagDigInp  eKontaktRS;
extern tagDigInp  eHaubeInAS;
// E1.5 Reserve
// E1.6 Reserve
// E1.7 Reserve
//==========================
extern tagDigInp  eTasteIo;
extern tagDigInp  eTasteNio;
extern tagDigInp  eTasteEnter;
extern tagDigInp  eTastePrev;
extern tagDigInp  eTasteNext;
// E2.5 Reserve
// E2.6 Reserve
//extern tagDigInp  eTTOilPress;
//==========================
//extern tagDigInp  eTachoOutput;
//extern tagDigInp  eTTHighBeam;
//extern tagDigInp  eTTNeutral;
//extern tagDigInp  eTTLeft;
//extern tagDigInp  eInp_14;
//extern tagDigInp  eTTRight;
//extern tagDigInp  eKontInput1;
// E3.7 Reserve
//==========================



//-----------------------------------------
extern tagDigOut  aLampeStart;
extern tagDigOut  aLampeStart;
extern tagDigOut  aLampeFehler;
extern tagDigOut  aLampePass;
extern tagDigOut  aLampeFail;
extern tagDigOut  aSteuerungBereit;
// A0.5 Reserve
// A0.6 Reserve
// A0.7 Reserve
//==========================
//extern tagDigOut  aVentilVerrieglungZu;
//extern tagDigOut  aVentilKontakt;
//extern tagDigOut  aSignierung;
//extern tagDigOut  aVentilFill;
//extern tagDigOut  aVentilDichten;
extern tagDigOut  aK506Reserve;
extern tagDigOut  aK507Reserve;
// A1.7 Reserve
//==========================
//extern tagDigOut  aUmschVGND;
extern tagDigOut  aDutVccBatt;
//extern tagDigOut  aDutIgnition;
//extern tagDigOut  aDutFuel;
//extern tagDigOut  aDUT_Pin4_high;
//extern tagDigOut  aDutTripSwitch;
//extern tagDigOut  aDUT_Pin10_on;
//extern tagDigOut  aDUT_Pin12_on;
//==========================
//extern tagDigOut  aDutAccessory;
//extern tagDigOut  aDUT_Pin8_high;
//extern tagDigOut  aSwitchToJ1850;
//extern tagDigOut  aTripSwToGnd;
extern tagDigOut  aK502DutR500;
extern tagDigOut  aK503Reserve;
extern tagDigOut  aK504Reserve;
extern tagDigOut  aK505Reserve;
//==========================



extern  tagAInp eADC1;
extern  tagAInp eADC2;
extern  tagAInp eADC3;


//extern  tagAOut aDAC1;
//extern  tagAOut aDAC2;

extern  tagDigInp *apDigInpList_g[];
extern  tagDigOut *apDigOutList_g[];
extern  tagAInp   *apAInpList_g[];
extern  tagAOut   *apAOutList_g[];


/**
 * Funtionsprototypen
 */
extern void  vResAllOutputs_g(void);

#endif // ZOL_H_INCLUDED
