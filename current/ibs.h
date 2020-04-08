//
//
//  Headerfile Interbus I/O Funktionen
//

#include "zol.h"

//  ------------------------------------------------------------------------------------------------
// Funktionsdeklarationen
//  ------------------------------------------------------------------------------------------------
extern void vSetDO_g(tagDigOut *tagOut, BOOL bSet);
extern int iReadDI_g(tagDigInp *tagInp);

extern void vSetUbatt_g(float fUbatt);
extern void vSetIvcc_g(float fIvccMa);
extern int iGetIvcc_g(void);
float fGetLiquidLevelReferenz(void);
float fGetCH2(void);
float fGetCH3(void);
float fGetCH4(void);
float fGetCH5(void);
float fGetCH6(void);
