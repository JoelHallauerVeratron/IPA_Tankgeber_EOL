//
//  Interbus I/O Funktionen
//

#include "global.h"
#include "zol.h"
#include "ibs_cfg.h"

#include "ibs.h"


//  ------------------------------------------------------------------------------------------------
void vSetDO_g(tagDigOut *tagOut, BOOL bSet)
//  ------------------------------------------------------------------------------------------------
{
  *tagOut->IBS_Node=(T_IBS_BOOL)bSet;
} // vSetDO_g()


//  ------------------------------------------------------------------------------------------------
int iReadDI_g(tagDigInp *tagInp)
//  ------------------------------------------------------------------------------------------------
{
  return(*(tagInp->IBS_Node));
} // iReadDI_g()


#if 0
/**
 * Setzt die Spannung
 *
 * @author uidc9013 (18.02.2010)
 * @param fUbatt  Spannung in V
 */
void vSetUbatt_g(float fUbatt)
{
  *aDAC1.IBS_Node=fUbatt*aDAC1.fConv;
} // vSetUbatt_g()
#endif


#if 0
/**
 * Setzt den Strom
 *
 * @author uidc9013 (18.02.2010)
 * @param fIvcc  Strom in mA
 */
void vSetIvcc_g(float fIvccMa)
{
  if(fIvccMa > 20.0)
    fIvccMa=20.0;
  if(fIvccMa < 4.0)
    fIvccMa=4.0;
  /**
   *
   * 0     inc = 4 mA
   * 30000 inc = 20 mA
   *
   * 0     inc = 0 mA
   * 30000 inc = 16 mA
   *
   *                      65535
   * inc = fIvccMa-4mA * -------
   *                      16 mA
   */

  *aDAC2.IBS_Node=(fIvccMa-4.0)*30000.0/16.0;
} // vSetIvcc_g()
#endif


/**
 * Gibt aktuelle Stromaufnahme in mA zurück
 *
 * @author uidc9013 (18.02.2010)
 *
 * @return Stromaufnnahme in mA
 */
int iGetIvcc_g(void)
{
  return((int)(apAInpList_g[0]->fConv * *apAInpList_g[0]->IBS_Node));
} // iGetIvcc_g()


float fGetLiquidLevelReferenz(void)
{
  return(((*apAInpList_g[5]->IBS_Node << 8) | *apAInpList_g[4]->IBS_Node) * apAInpList_g[4]->fConv);
} // fGetPLuft_g()

float fGetCH2(void)
{
  return(apAInpList_g[1]->fConv * *apAInpList_g[1]->IBS_Node);
}

float fGetCH3(void)
{
  return(apAInpList_g[2]->fConv * *apAInpList_g[2]->IBS_Node);
}

float fGetCH4(void)
{
  return(apAInpList_g[3]->fConv * *apAInpList_g[3]->IBS_Node);
}

float fGetCH5(void)
{
  return(apAInpList_g[4]->fConv * *apAInpList_g[4]->IBS_Node);
}

float fGetCH6(void)
{
  return(apAInpList_g[5]->fConv * *apAInpList_g[5]->IBS_Node);
}

