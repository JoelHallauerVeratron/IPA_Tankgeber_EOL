//
//  I/O Zuordnungsliste
//

#include <windows.h>
#include <stdlib.h>

#include "IODiag.h"
#include "zol.h"
#include "ibs_cfg.h"
#include "ibs.h"


#define MODUL(byte,bit)  ((byte*8)+bit)

//  ------------------------------------------------------------------------------------------------
//  Definition der digitalen Eingänge
//  ------------------------------------------------------------------------------------------------
//                                 +----------- .iModulNr (IBS-Modulnummer)
//                                 |  +----------- .iControl (Control-Id bei Diagnose)
//                                 |  |           +----------- .IBS_Node (Eingangsvariable)
//                                 |  |           |        +----------- .pcTxt (Beschreibung)
//                                 |  |           |        |
tagDigInp  eNotausOk            = {1, MODUL(0,0), &eInp00, "Notaus quittiert"};
tagDigInp  eTasteStart          = {1, MODUL(0,1), &eInp01, "Taste: Start"};
tagDigInp  eTasteQuittierung    = {1, MODUL(0,2), &eInp02, "Taste: Quittierung"};
tagDigInp  eNotausEntriegelt    = {1, MODUL(0,3), &eInp03, "Notaustaste entriegelt"};
// E0.4 Reserve
// E0.5 Reserve
tagDigInp  eTeilEingelegt       = {1, MODUL(0,6), &eInp06, "Teil eingelegt"};
// E0.7 Reserve
//==========================
//tagDigInp  eVerriegelungRS      = {1, MODUL(1,0), &eInp10, "Verriegelung offen"};
//tagDigInp  eVerriegelungAS      = {1, MODUL(1,1), &eInp11, "Verriegelung zu"};
//tagDigInp  eKontaktAS           = {1, MODUL(1,2), &eInp12, "Kontakt. Arbeitspos."};
//tagDigInp  eKontaktRS           = {1, MODUL(1,3), &eInp13, "Kontakt. Ruhepos."};
tagDigInp  eHaubeInAS             = {1, MODUL(1,4), &eInp14, "Haube Arbeitspos."};
// E1.5 Reserve
// E1.6 Reserve
// E1.7 Reserve
//==========================
tagDigInp  eTasteIo             = {1, MODUL(2,0), &eInp20, "Taste IO"};
tagDigInp  eTasteNio            = {1, MODUL(2,1), &eInp21, "Taste NIO"};
tagDigInp  eTasteEnter          = {1, MODUL(2,2), &eInp22, "Taste Enter"};
tagDigInp  eTastePrev           = {1, MODUL(2,3), &eInp23, "Taste <<"};
tagDigInp  eTasteNext           = {1, MODUL(2,4), &eInp24, "Taste >>"};
// E2.5 Reserve
// E2.6 Reserve
//tagDigInp  eTTOilPress          = {1, MODUL(2,7), &eInp27, "Oil Pressure"};
//==========================
//tagDigInp  eTachoOutput         = {1, MODUL(3,0), &eInp30, "Tacho Output"};
//tagDigInp  eTTHighBeam          = {1, MODUL(3,1), &eInp31, "TT High beam"};
//tagDigInp  eTTNeutral           = {1, MODUL(3,2), &eInp32, "TT Neutral"};
//tagDigInp  eTTLeft              = {1, MODUL(3,3), &eInp33, "TT Left"};
//tagDigInp  eInp_14              = {1, MODUL(3,4), &eInp34, "TT Analog PWM"};
//tagDigInp  eTTRight             = {1, MODUL(3,5), &eInp35, "TT Right"};
//tagDigInp  eKontInput1          = {1, MODUL(3,6), &eInp36, "Kontakt. Input 1"};
// E3.7 Reserve
//==========================



tagDigInp *apDigInpList_g[] =
{
  &eNotausOk,
  &eTasteStart,
  &eTasteQuittierung,
  &eNotausEntriegelt,
// E0.4 Reserve
// E0.5 Reserve
  &eTeilEingelegt,
// E0.7 Reserve
//==========================
  //&eVerriegelungRS,
  //&eVerriegelungAS,
  //&eKontaktAS,
  //&eKontaktRS,
  &eHaubeInAS,
// E1.5 Reserve
// E1.6 Reserve
// E1.7 Reserve
//==========================
  &eTasteIo,
  &eTasteNio,
  &eTasteEnter,
  &eTastePrev,
  &eTasteNext,
// E2.5 Reserve
// E2.6 Reserve
  //&eTTOilPress,
//==========================
  //&eTachoOutput,
  //&eTTHighBeam,
  //&eTTNeutral,
  //&eTTLeft,
//eInp_14
  //&eTTRight,
  //&eKontInput1,
// E3.7 Reserve
  NULL
};



//  ------------------------------------------------------------------------------------------------
//  Definition der digitalen Ausgänge
//  ------------------------------------------------------------------------------------------------
//                                     +----------- .iModulNr (IBS-Modulnummer)
//                                     |  +----------- .iControl (Control-Id bei Diagnosepanel)
//                                     |  |           +----------- .IBS_Node (Eingangsvariable)
//                                     |  |           |        +----------- .pcTxt (Beschreibung)
//                                     |  |           |        |
tagDigOut  aLampeStart              = {1, MODUL(0,0), &aOut00, "Lampe: Start"};
tagDigOut  aLampeFehler             = {1, MODUL(0,1), &aOut01, "Lampe: Störung"};
tagDigOut  aLampePass               = {1, MODUL(0,2), &aOut02, "Lampe: PASS"};
tagDigOut  aLampeFail               = {1, MODUL(0,3), &aOut03, "Lampe: FAIL"};
tagDigOut  aSteuerungBereit         = {1, MODUL(0,4), &aOut04, "Steuerung bereit"};
// A0.5 Reserve
// A0.6 Reserve
// A0.7 Reserve
//==========================
//tagDigOut  aVentilVerrieglungZu     = {1, MODUL(1,0), &aOut10, "-Y312 Verriegelung zu"};
//tagDigOut  aVentilKontakt           = {1, MODUL(1,1), &aOut11, "-Y322 Kontaktierung"};
//tagDigOut  aSignierung              = {1, MODUL(1,2), &aOut12, "-K133 Signierung"};
//tagDigOut  aVentilFill              = {1, MODUL(1,3), &aOut13, "-Y344 Füll-Ventil"};
//tagDigOut  aVentilDichten           = {1, MODUL(1,4), &aOut14, "-Y346 Dicht-Ventil"};
tagDigOut  aK506Reserve               = {1, MODUL(1,5), &aOut15, "-K506 (Reserve)"};
tagDigOut  aK507Reserve               = {1, MODUL(1,6), &aOut16, "-K507 (Reserve)"};
// A1.7 Reserve
//==========================
//tagDigOut  aUmschVGND               = {1, MODUL(2,0), &aOut20, "-K412 Umschaltung GND -> +V"};
tagDigOut  aDutVccBatt              = {1, MODUL(2,1), &aOut21, "-K413/-K414 Batterie"};
//tagDigOut  aDutIgnition             = {1, MODUL(2,2), &aOut22, "-K415 Ignition"};
//tagDigOut  aDutFuel                 = {1, MODUL(2,3), &aOut23, "-K416 Fuel"};
//tagDigOut  aDUT_Pin4_high           = {1, MODUL(2,4), &aOut24, "-K417 Pin 4 high (ABS)"};
//tagDigOut  aDutTripSwitch           = {1, MODUL(2,5), &aOut25, "-K422 Trip switch"};
//tagDigOut  aDUT_Pin10_on            = {1, MODUL(2,6), &aOut26, "-K423 Pin 10 on (Cruise set)"};
//tagDigOut  aDUT_Pin12_on            = {1, MODUL(2,7), &aOut27, "-K424 Pin 12 on (Cruise enable)"};
//==========================
//tagDigOut  aDutAccessory            = {1, MODUL(3,0), &aOut30, "-K425 Accessory"};
//tagDigOut  aDUT_Pin8_high           = {1, MODUL(3,1), &aOut31, "-K426 Pin 4 high"};
//tagDigOut  aSwitchToJ1850           = {1, MODUL(3,2), &aOut32, "-K134 Umschaltung auf J1850"};
//tagDigOut  aTripSwToGnd             = {1, MODUL(3,3), &aOut33, "-K427 TripSwitch auf GND"};
tagDigOut  aK502DutR500             = {1, MODUL(3,4), &aOut34, "-K502 Rxx an DUT"};
tagDigOut  aK503Reserve             = {1, MODUL(3,5), &aOut35, "-K503 (Reserve)"};
tagDigOut  aK504Reserve             = {1, MODUL(3,6), &aOut36, "-K504 (Reserve)"};
tagDigOut  aK505Reserve             = {1, MODUL(3,7), &aOut37, "-K505 (Reserve)"};
//==========================

#if 0
tagDigOut  aDUT_Pin4_on             = {1, MODUL(x,y), &aOut20, "-K111: Pin 4 on"};
tagDigOut  aDUT_Pin8_on             = {1, MODUL(x,y), &aOut21, "-K112: Pin 8 on"};
tagDigOut  aDUT_Pin10_on            = {1, MODUL(x,y), &aOut22, "-K113: Pin 10 on"};
tagDigOut  aDUT_Pin12_on            = {1, MODUL(x,y), &aOut23, "-K114: Pin 12 on"};
tagDigOut  aDUT_Pin4_high           = {1, MODUL(x,y), &aOut24, "-K101: Pin 4 high"};
tagDigOut  aDUT_Pin8_high           = {1, MODUL(x,y), &aOut25, "-K102: Pin 8 high"};
tagDigOut  aDUT_Pin10_high          = {1, MODUL(x,y), &aOut26, "-K103: Pin 10 high"};
tagDigOut  aDUT_Pin12_high          = {1, MODUL(x,y), &aOut27, "-K104: Pin 12 high"};
#endif


tagDigOut *apDigOutList_g[] =
{
  &aLampeStart,
  &aLampeFehler,
  &aLampePass,
  &aLampeFail,
  &aSteuerungBereit,
// A0.5 Reserve
// A0.6 Reserve
// A0.7 Reserve
//==========================
  //&aVentilVerrieglungZu,
  //&aVentilKontakt,
  //&aSignierung,
  //&aVentilFill,
  //&aVentilDichten,
  &aK506Reserve,
  &aK507Reserve,
// A1.7 Reserve
//==========================
  //&aUmschVGND,
  &aDutVccBatt,
  //&aDutIgnition,
  //&aDutFuel,
  //&aDUT_Pin4_high,
  //&aDutTripSwitch,
  //&aDUT_Pin10_on,
  //&aDUT_Pin12_on,
//==========================
  //&aDutAccessory,
  //&aDUT_Pin8_high,
  //&aSwitchToJ1850,
  //&aTripSwToGnd,
  &aK502DutR500,
  &aK503Reserve,
  &aK504Reserve,
  &aK505Reserve,
//==========================
  NULL
};


//  ------------------------------------------------------------------------------------------------
//  Definition der analogen Eingänge
//  ------------------------------------------------------------------------------------------------
//               +----------- .iModulNr (IBS-Modulnummer)
//               |  +----------- .iControl (Control-Id bei Diagnose)
//               |  |               +----------- .fConv (Faktor)
//               |  |               |         +----------- .IBS_Node (Eingangsvariable)
//               |  |               |         |      +----------- .pcTxt (Beschreibung)
//               |  |               |         |      |
tagAInp eADC1 = {1, PNL_DIAGIO_AI1, 0.033333, &eAI1, "Ivcc [mA]"};   // 30000 entspr. 1A (10V)
//tagAInp eADC2 = {1, PNL_DIAGIO_AI2, 0.333333, &eAI2, "U-CAN [mV]"};   // 3000 entspr. 1000mV (1V)
//tagAInp eADC3 = {2, PNL_DIAGIO_AI3, 0.065769, &eAI3, "Druck [mB]"};   // 3429 entspr. 227.8mb

tagAInp *apAInpList_g[] =
{
  &eADC1,
  //&eADC2,
  //&eADC3,
  NULL
};


//  ------------------------------------------------------------------------------------------------
//  Definition der analogen Ausgänge
//  ------------------------------------------------------------------------------------------------
//               +----------- .iModulNr (IBS-Modulnummer)
//               |  +----------- .iControl (Control-Id bei Diagnose)
//               |  |                     +----------- .iControl (Control-Id bei Diagnose)
//               |  |                     |                    +----------- .fConv (Faktor)
//               |  |                     |                    |       +----------- .IBS_Node (Eingangsvariable)
//               |  |                     |                    |       |      +----------- .IBS_Node (Eingangsvariable aktueller Wert)
//               |  |                     |                    |       |      |        +----------- .pcTxt (Beschreibung)
//               |  |                     |                    |       |      |        |
//tagAOut aDAC1 = {1, PNL_DIAGIO_DAC1_SOLL, PNL_DIAGIO_DAC1_IST, 144.0,  &aAO1, &eAO1AW, "Spannung Prüfling"};
//tagAOut aDAC2 = {1, PNL_DIAGIO_DAC2_SOLL, PNL_DIAGIO_DAC2_IST, 1.0,    &aAO2, &eAO2AW, "Analog Input 1 4-20mA"};   // Faktorumrechnung findet in vSetIvcc_g() statt

tagAOut *apAOutList_g[] =
{
  //&aDAC1,
  //&aDAC2,
  NULL
};





//  ------------------------------------------------------------------------------------------------
void vResAllOutputs_g(void)
//  ------------------------------------------------------------------------------------------------
{
  vSetDO_g(&aLampeStart,FALSE);
  vSetDO_g(&aLampeStart,FALSE);
  vSetDO_g(&aLampeStart,FALSE);
  vSetDO_g(&aLampeFehler,FALSE);
  vSetDO_g(&aLampePass,FALSE);
  vSetDO_g(&aLampeFail,FALSE);
  //vSetDO_g(&aSteuerungBereit,FALSE);
// A0.5 Reserve
// A0.6 Reserve
// A0.7 Reserve
//==========================
  //vSetDO_g(&aVentilVerrieglungZu,FALSE);
  //vSetDO_g(&aVentilKontakt,FALSE);
  //vSetDO_g(&aSignierung,FALSE);
  //vSetDO_g(&aVentilFill,FALSE);
  //vSetDO_g(&aVentilDichten,FALSE);
  vSetDO_g(&aK506Reserve,FALSE);
  vSetDO_g(&aK507Reserve,FALSE);
// A1.7 Reserve
//==========================
  //vSetDO_g(&aUmschVGND,FALSE);
  vSetDO_g(&aDutVccBatt,FALSE);
  //vSetDO_g(&aDutIgnition,FALSE);
  //vSetDO_g(&aDutFuel,FALSE);
  //vSetDO_g(&aDUT_Pin4_high,FALSE);
  //vSetDO_g(&aDutTripSwitch,FALSE);
  //vSetDO_g(&aDUT_Pin10_on,FALSE);
  //vSetDO_g(&aDUT_Pin12_on,FALSE);
//==========================
  //vSetDO_g(&aDutAccessory,FALSE);
  //vSetDO_g(&aDUT_Pin8_high,FALSE);
  //vSetDO_g(&aSwitchToJ1850,FALSE);
  //vSetDO_g(&aTripSwToGnd,FALSE);
  vSetDO_g(&aK502DutR500,FALSE);
  vSetDO_g(&aK503Reserve,FALSE);
  vSetDO_g(&aK504Reserve,FALSE);
  vSetDO_g(&aK505Reserve,FALSE);
//==========================
} // vResAllOutputs_g()








