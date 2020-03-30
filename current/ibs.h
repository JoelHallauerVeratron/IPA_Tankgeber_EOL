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
extern float fGetVoltageCanLowToGnd_g(void);
extern int iGetVoltageCanLowToGnd_g(void);
extern float fGetPLuft_g(void);

