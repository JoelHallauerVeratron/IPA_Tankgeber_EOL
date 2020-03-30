#if defined(_CVI_LIB_) || defined(_LIB)
  #error SvnIds.h wird für static Libraries nicht benötigt.
#endif // _CVI_LIB_ || _LIB
#ifndef SVN_IDS_H_INCLUDED
  #define SVN_IDS_H_INCLUDED

  #include "stdio.h"
  #include "stdlib.h"


  char *pcSvnGetUrl(void);
  char *pcSvnGetAuthor(void);
  char *pcSvnGetRevision(void);
  char *pcSvnGetDate(void);

  char *pcSvnGetRepoName(void);

#endif // !SVN_IDS_H_INCLUDED

