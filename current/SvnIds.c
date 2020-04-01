#if defined(_CVI_DLL_) || defined(_DLL) || defined(_CVI_EXE_)
  #include "stdio.h"
  #include "stdlib.h"

  #ifdef _CVI_EXE_
    #define MYDLL_API
  #else
    #define MYDLL_API __declspec(dllexport)
  #endif // !_CVI_EXE_

  #ifdef _CVI_
    #include <ansi_c.h>
  #endif // _CVI_

  char *pcStripDollars(char *pcString)
  {
    static char caString[200];
    char *pcPos;
    // strip leading '$'
    if(pcString[0]=='$')
      strcpy(caString,&pcString[1]);
    else
      strcpy(caString,pcString);
    // strip trailing '$'
    pcPos=strstr(caString," $");
    if(pcPos!=NULL)
      pcPos[0]=NULL;

    return(caString);
  }

  char *pcExtractRepo(char *pcString)
  {
    char caStringInput[200];
    static char caStringResult[200];
    char *pcPos;

    // HeadURL: https://192.168.20.6/svn/VL_A150A160/SvnIds.c
    //                                   =========== --> Resultat

    strcpy(caStringInput,pcString);
    // strip trailing file name
    pcPos=strrchr(caStringInput,'/');
    if(pcPos!=NULL)
      pcPos[0]=NULL;
    // extract repo name
    pcPos=strrchr(caStringInput,'/');
    if(pcPos)
      strcpy(caStringResult,++pcPos);
    else
      strcpy(caStringResult,caStringInput);

    return(caStringResult);
  }


  // for dll or exe
  MYDLL_API  char *pcSvnGetUrl(void) { return(pcStripDollars("$HeadURL: https://192.168.20.6/svn/LinkUpEol/SvnIds.c $")); }
  MYDLL_API  char *pcSvnGetAuthor(void) { return(pcStripDollars("$Author: WeU $")); }
  MYDLL_API  char *pcSvnGetRevision(void) { return(pcStripDollars("$Revision: 2 $")); }
  MYDLL_API  char *pcSvnGetDate(void) { return(pcStripDollars("$Date: 2020-03-06 14:18:31 +0100 (Fr, 06 Mrz 2020) $")); }

  MYDLL_API  char *pcSvnGetRepoName(void) { return(pcExtractRepo(pcSvnGetUrl())); }

#endif // _CVI_DLL_ || _WINDLL


#if defined(_CVI_LIB_) || defined(_LIB)
  // for libs
  #ifndef TARGET_NAME
    #error TARGET_NAME ist nicht definiert!!
  #endif // !TARGET_NAME

  #define TOKENPASTE(x,y,z) x ## y ## z
  #define TOKENPASTE2(x,y,z) TOKENPASTE(x,y,z)

void TOKENPASTE2(v,TARGET_NAME,_function(void))
{
  static char TOKENPASTE2(*pc,TARGET_NAME,SvnUrl) = "$HeadURL: https://192.168.20.6/svn/LinkUpEol/SvnIds.c $";
  static char TOKENPASTE2(*pc,TARGET_NAME,SvnAuthor) = "$Author: WeU $";
  static char TOKENPASTE2(*pc,TARGET_NAME,SvnRevision) = "$Revision: 2 $";
  static char TOKENPASTE2(*pc,TARGET_NAME,SvnDate) = "$Date: 2020-03-06 14:18:31 +0100 (Fr, 06 Mrz 2020) $";

  // to avoid warnings
  TOKENPASTE2(pc,TARGET_NAME,SvnUrl)[0] = '$';
  TOKENPASTE2(pc,TARGET_NAME,SvnAuthor)[0] = '$';
  TOKENPASTE2(pc,TARGET_NAME,SvnRevision)[0] = '$';
  TOKENPASTE2(pc,TARGET_NAME,SvnDate)[0] = '$';
}

#endif // _CVI_LIB_ || _LIB

/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
/**/ 
