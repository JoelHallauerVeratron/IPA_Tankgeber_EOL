/**
 * UDS_Security.c
 *
 * Projekt: FlexCluster
 *
 * Die Funktion vUDS_GetKey_m() wird aus der Datei UDS_private.c herausgehalten
 * für eine einfachere Portierung
 *
 * *********************************************************
 * Diese Funkion wird nicht gelinkt, sondern nur included!!!
 *
 * => Darf nicht dem CVI-Projekt hinzugefügt werden.
 *
 * *********************************************************
 *
 *    AUTHOR:      $Author$
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 */

/**
 * Berechnet den Key zum empfangenen Seed
 *
 * @param eSecurityAccess
 *               gewuenschter Security-Level
 * @param pucKey Buffer für Key als Rueckgabewert
 * @param piSize Groesse von Key in bytes
 */
void vUds_CalcKey_m(EUdsSecurityAccess  eSecurityAccess,
                    unsigned char      *pucKey,
                    int                *piSize)
{
  DWORD dwSeed1;
  DWORD dwSeed2;
  DWORD dwSeed3;
  DWORD dwSeed4;

  DWORD dwKey1;
  DWORD dwKey2;
  DWORD dwKey3;
  DWORD dwKey4;
  DWORD dwNewKey;

  DWORD dwA=0;
  DWORD dwB=0;
  DWORD dwC=0;
  DWORD dwD=0;

  dwSeed1=(DWORD)ucaSeed_m[0];
  dwSeed2=(DWORD)ucaSeed_m[1];
  dwSeed3=(DWORD)ucaSeed_m[2];
  dwSeed4=(DWORD)ucaSeed_m[3];

  switch(eSecurityAccess)
  {
    case eUdsSecurityAccessVmsEol:  // 1 = AccessModeVehicleManufacturerSpecificEndOfLine
      dwA=0x3d;
      dwB=0x51;
      dwC=0x0b;
      dwD=0xd4;
      break;
    case eUdsSecurityAccessVmsProg: // 3 = AccessModeVehicleManufacturerSpecificProgramming
      dwA=0x9e;
      dwB=0x32;
      dwC=0xc6;
      dwD=0x52;
      break;
    case eUdsSecurityAccessVmsDev:  // 5 = AccessModeVehicleManufacturerSpecificDevelopment
      dwA=0x86;
      dwB=0xa4;
      dwC=0xcf;
      dwD=0x77;
      break;
    case eUdsSecurityAccessSssEol:  // 0x63 = AccessSystemSupplierSpecificEndOfLine
      dwA=0x90;
      dwB=0x5c;
      dwC=0x32;
      dwD=0x6a;
      break;
    case eUdsSecurityAccessSssDev:  // 0x65 = AccessSystemSupplierSpecificDevelopment
      dwA=0x85;
      dwB=0xcc;
      dwC=0x31;
      dwD=0xed;
      break;
  }

  dwKey1 = (dwA*(dwSeed1*dwSeed1)) + (dwB*(dwSeed2*dwSeed2)) + (dwC*(dwSeed1*dwSeed2));
  dwKey2 = (dwA         *dwSeed1 ) + (dwB         *dwSeed2 ) + (dwD*(dwSeed1*dwSeed2));
  dwKey3 = (dwA*(dwSeed3*dwSeed4)) + (dwB*(dwSeed4*dwSeed4)) + (dwC*(dwSeed3*dwSeed4));
  dwKey4 = (dwA*(dwSeed3*dwSeed4)) + (dwB*         dwSeed4 ) + (dwD*(dwSeed3*dwSeed4));

#if 0
  printf("Key1=0x%02x\n",dwKey1);
  printf("Key2=0x%02x\n",dwKey2);
  #if SEED_KEY_SIZE==4
  printf("Key3: 0x%06x  0x%06x  0x%06x\n",(dwA*(dwSeed3*dwSeed4)),(dwB*(dwSeed4*dwSeed4)),(dwC*(dwSeed3*dwSeed4))  );
  printf("Key3=0x%02x\n",dwKey3);
  printf("Key4: 0x%06x  0x%06x  0x%06x\n",(dwA*(dwSeed3*dwSeed4)),(dwB*         dwSeed4 ),(dwD*(dwSeed3*dwSeed4))  );
  printf("Key4=0x%02x\n",dwKey4);
  #endif // SEED_KEY_SIZE==4
#endif

  dwNewKey = ((dwKey1 & 0xFF) << 24)
             + ((dwKey2 & 0xFF) << 16)
             + ((dwKey3 & 0xFF) <<  8)
             + (dwKey4 & 0xFF)
             ;

  pucKey[0] = (dwKey1 & 0xFF);
  pucKey[1] = (dwKey2 & 0xFF);
  pucKey[2] = (dwKey3 & 0xFF);
  pucKey[3] = (dwKey4 & 0xFF);

#if 0
  printf("pucKey[0]=0x%02x\n",pucKey[0]);
  printf("pucKey[1]=0x%02x\n",pucKey[1]);
  #if SEED_KEY_SIZE==4
  printf("pucKey[2]=0x%02x\n",pucKey[2]);
  printf("pucKey[3]=0x%02x\n",pucKey[3]);
  #endif // SEED_KEY_SIZE==4
#endif

  if(piSize)
  {
    *piSize=SEED_KEY_SIZE;
  }
} // vUds_CalcKey_m()

