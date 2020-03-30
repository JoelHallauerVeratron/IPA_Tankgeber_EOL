/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1996.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       inifile.c                                                     */
/* Purpose:     provides functions to read and write .ini style files         */
/*                                                                            */
/*============================================================================*/


#include <ansi_c.h>
#include <userint.h>
#include <utility.h>
#include <formatio.h>
#include "toolbox.h"
#include "inifile.h"

#ifdef _MSC_VER
  #define INISTRICMP  stricmp
#else
  #define INISTRICMP  StrICmp
#endif


/******************/
/* Macros & Types */
/******************/

#define FastTokenCmp(string, tokenString) ((*string != *tokenString) ? (*string - *tokenString) : strncmp(string, tokenString, strlen(tokenString)))
#define kMaxSpecialTokenLength          8           /* way longer than reasonably needed */

#define kMaxIniLineSize                 255   /* long lines should be split into multi-line entries */

#define kLineTag                        " Line"
#define INI_NUM_CHARS_PER_LINE          80

#define kStringCacheChunkSize           1024 /* the cache size is always the smallest multiple of this value that will hold the requested number of bytes */

typedef enum
{
  kIni_CommentLine,
  kIni_SectionLine,
  kIni_TagAndValueLine,
  kIni_NotAnIniLine
} IniLineType;


typedef struct _iniEntryStruct
{
  char    *name;
  char    *value;
  int     sourceLine;
  int     addMarkerQuotes;    /* add quotes when writing to file, because item was put as a string */
} *IniEntry;


typedef struct
{
  char        *sectionName;
  ListType    entryList;
  char        sorted;
  int         sourceLine;
} *IniSection;


struct IniStructForwardReferenceTag /* needed to allow declaring IniText in the header file */
{
  ListType                sectionList;
  IniSection              lastSectionAccessed;  /* optimization */
  char                    sorted;
  int                     lastLineAccessed;
  int                     nameValueInputSeparatorLength;
  char                    nameValueInputSeparatorToken[kMaxSpecialTokenLength];
  int                     nameValueOutputSeparatorLength;
  char                    nameValueOutputSeparatorToken[kMaxSpecialTokenLength + 2];
  char                    sectionNameStartChar;
  char                    sectionNameEndChar;
  char                    commentMarker[kMaxSpecialTokenLength];    
  int                     checkForDuplicatesWhenPutting;
  Ini_SectionFilterFunc   sectionFilter;
  void                    *sectionFilterCallbackData;
  char                    **stringCache;
};

/*******************************/
/* Static Functions Prototypes */
/*******************************/

static int          Ini_FindSectionIndex(IniText theIniText, const char *sectionName);
static IniSection   Ini_FindSection(IniText theIniText, const char *sectionName);
static int          Ini_InsertSection(IniText theIniText, IniSection section);
static void         Ini_SetDefaultTokens(IniText iniText);


/*  IniSection members */
static IniSection   Ini_NewSection(const char *sectionName, int sorted);
static void         Ini_DisposeSection(IniSection section);
static char         *Ini_SectionName(IniSection section);
static int          CVICALLBACK Ini_CompareSectionNames(void *ptrToSection1, void *ptrToSection2);
static int          CVICALLBACK Ini_CompareSectionToName(void *sectionName, void *ptrToSection);
static IniEntry     Ini_FindEntry(IniSection section, const char *itemName);
static int          Ini_InsertEntry(IniSection section, IniEntry entry);
static int          Ini_WriteSection(IniText iniText, IniSection theSection, Ini_OutputFunc outputFunc, void *outputDest);
static int          Ini_RemoveFromSection(IniText theIniText, IniSection section, const char *itemName);


/*  IniEntry members */
static IniEntry     Ini_NewEntry(const char *name, const char *value, int stripMarkerQuotes);
static void         Ini_DisposeEntry(IniEntry theEntry);
static char         *Ini_EntryName(IniEntry entry);
static int          CVICALLBACK Ini_CompareEntryNames(void *ptrToEntry1, void *ptrToEntry2);
static int          CVICALLBACK Ini_CompareEntryToName(void *entryName, void *ptrToEntry);
static int          Ini_WriteEntry(IniText iniText, IniEntry theEntry, Ini_OutputFunc outputFunc, void *outputDest);
static int          Ini_SetEntryValue(IniEntry entry, const char *value, int stripMarkerQuotes);


/*  Helper functions */
static void         Ini_DisposeSectionList(ListType sectionList);
static void         Ini_DisposeEntryList(ListType entryList);
static int          ParseTagTextSectionTitle(IniText iniText, char *firstCharAfterOpenBracket, char *sectionTitle);
static int          ParseTagTextLine(IniText iniText, char buf[], char *section, char *item, char *value);
static int          CVICALLBACK Ini_LineFileInputFunction(void *inputSource, char *inputBuffer, int maxLineLength, int *endOfSource, int *lineNumRead);
static int          CVICALLBACK Ini_FileOutputFunc(void *file, char *outputString);
static int          SetStringCacheSize(char **strHandle, int size);
static int          Ini_PutSingleLineString(IniText theIniText, const char *section, const char *itemName, const char *value);
static int          CoalesceMultilineItemsInSection(IniSection  section);
static int          CoalesceMultiLineItems(IniSection section, int startIndex, int endIndex);
static int          FindMultiLineItem(IniSection section, int startOfSearchIndex, int *startIndex, int *endIndex);
static int          CoalesceMultilineItemsAfterRead(IniText iniText);

static int          Ini_Put(IniText theIniText, const char *section, const char *itemName, const char *value, int addMarkerQuotes);
static int          Ini_Get(IniText theIniText, const char *section, const char *itemName, char **value);
static void         Ini_AnalyzeItemName(const char *itemName, int *isMultiLine, int *whichLine, char singleLineItemName[]);

static int          EscapeText(const char *srcTextPtr, char **destTextPtr);   
static int          UnescapeText(char *srcTextPtr, char *destTextPtr);

static void         CVIANSI IniFileTerminationFunction(void);

static int          GetHexCharValue (int hexChar);

static char *       StrDupWithoutQuotes(const char *string, int *hadQuotes);

/*********************************************************************/

/****************************/
/* Function Implementations */
/****************************/

IniText CVIFUNC Ini_New(int sorted)
{
  IniText newIniText = NULL;
  int error = 0;

  nullChk(newIniText = calloc(sizeof(*newIniText), 1));
  nullChk(newIniText->sectionList = ListCreate(sizeof(IniSection)));
  newIniText->sorted = (char)sorted;
  newIniText->lastLineAccessed = -1;
  newIniText->lastSectionAccessed = NULL;
  newIniText->sectionFilter = NULL;
  Ini_SetDefaultTokens(newIniText);

  newIniText->checkForDuplicatesWhenPutting = TRUE;
  nullChk(newIniText->stringCache = (char **)NewHandle(0));

Error:
  if(error < 0)
  {
    Ini_Dispose(newIniText);
    newIniText = NULL;
  }

  return(newIniText);
}

/*********************************************************************/

void    CVIFUNC Ini_Dispose(IniText iniText)
{
  if(iniText)
  {
    Ini_DisposeSectionList(iniText->sectionList);
    DisposeHandle((Handle)iniText->stringCache);
    free(iniText);
  }
}

/*********************************************************************/

int CVIFUNC Ini_SectionExists(IniText iniText, const char *sectionName)
{
  return(Ini_FindSection(iniText, sectionName) != NULL);
}

/*********************************************************************/

/* returns 0 if the section doesn't exist */
int  CVIFUNC Ini_NumberOfItems(IniText theIniText, const char *sectionName)
{
  IniSection  section;

  section = Ini_FindSection(theIniText, sectionName);

  if(section)
    return(ListNumItems(section->entryList));
  else
    return(0);
}

/*********************************************************************/

/* returns FALSE if the section or item doesn't exist */
int  CVIFUNC Ini_NthItemName(IniText theIniText, const char *sectionName, int index, char **itemName)
{
  IniSection  section;
  IniEntry    entry = NULL;
  int         numItems;
  int         result = FALSE;

  section = Ini_FindSection(theIniText, sectionName);

  if(section)
  {
    numItems = ListNumItems(section->entryList);
    if(index > 0 && index <= numItems)
    {
      ListGetItem(section->entryList, &entry, index);
      *itemName = entry->name;
      result = TRUE;
    }
  }

  return(result);
}

/*********************************************************************/

int  CVIFUNC Ini_NumberOfSections(IniText theIniText)
{
  return(ListNumItems(theIniText->sectionList));
}

/*********************************************************************/

int CVIFUNC Ini_NthSectionName(IniText theIniText, int index, char **sectionName)
{
  IniSection  section;
  int     numItems;
  int     result = FALSE;

  numItems = ListNumItems(theIniText->sectionList);
  if(index > 0 && index <= numItems)
  {
    ListGetItem(theIniText->sectionList, &section, index);
    *sectionName = section->sectionName;
    result = TRUE;
  }

  return(result);
}

/*********************************************************************/

int CVIFUNC Ini_LineOfLastAccess(IniText theIniText)
{
  return(theIniText->lastLineAccessed);
}

/*********************************************************************/

void    CVIFUNC Ini_Sort(IniText theIniText)
{
  int index;
  int numSections;

  numSections = ListNumItems(theIniText->sectionList);

  for(index = 1; index <= numSections; index++)
  {
    IniSection  currentSection;

    ListGetItem(theIniText->sectionList, &currentSection, index);
    ListQuickSort(currentSection->entryList, Ini_CompareEntryNames);
    currentSection->sorted = TRUE;
  }

  ListQuickSort(theIniText->sectionList, Ini_CompareSectionNames);
  theIniText->sorted = TRUE;
}

/*********************************************************************/

void    CVIFUNC Ini_DisableSorting (IniText theIniText)
{
  int index;
  int numSections;

  numSections = ListNumItems(theIniText->sectionList);

  for(index = 1; index <= numSections; index++)
  {
    IniSection  currentSection;

    ListGetItem(theIniText->sectionList, &currentSection, index);
    currentSection->sorted = FALSE;
  }
  theIniText->sorted = FALSE;
}

/*********************************************************************/

static void Ini_DisposeSectionList(ListType sectionList)
{
  int index;

  if(sectionList)
  {
    for(index = 1; index <= ListNumItems(sectionList); index++)
      Ini_DisposeSection(*(IniSection *)ListGetPtrToItem(sectionList, index));

    ListDispose(sectionList);
  }
}

/*********************************************************************/

/*  After an section is inserted into a IniText, the IniText owns it,
    is responsible for disposing it, and should be the only one to access it.
*/
static int Ini_InsertSection(IniText theIniText, IniSection section)
{
  if(theIniText->sorted)
    return(ListInsertInOrder(theIniText->sectionList, &section, Ini_CompareSectionNames));
  else
    return(ListInsertItem(theIniText->sectionList, &section, END_OF_LIST));
}

/*********************************************************************/

static IniSection Ini_NewSection(const char *sectionName, int sorted)
{
  IniSection  newSection = NULL;
  int     error = 0;

  nullChk(newSection = calloc(sizeof(*newSection), 1));
  nullChk(newSection->entryList = ListCreate(sizeof(IniEntry)));
  nullChk(newSection->sectionName = StrDup((char *)sectionName));
  newSection->sorted = (char)sorted;

Error:
  if(error < 0)
  {
    Ini_DisposeSection(newSection);
    newSection = NULL;
  }

  return(newSection);
}

/*********************************************************************/

static void Ini_DisposeSection(IniSection section)
{
  if(section)
  {
    Ini_DisposeEntryList(section->entryList);
    free(section->sectionName);
    free(section);
  }
}

/*********************************************************************/

static int Ini_FindSectionIndex(IniText theIniText, const char *sectionName)
{
  int index = 0;

  if(theIniText)
  {
    if(theIniText->sorted)
      index = ListBinSearch(theIniText->sectionList, (void *)sectionName, Ini_CompareSectionToName);
    else
      index = ListFindItem(theIniText->sectionList, (void *)sectionName, FRONT_OF_LIST, Ini_CompareSectionToName);
  }

  return(index);
}

/*********************************************************************/

int CVIFUNC Ini_RemoveSection(IniText theIniText, const char *sectionName)
{
  int         index;
  IniSection  section;
  int         removed = FALSE;

  index = Ini_FindSectionIndex(theIniText, sectionName);

  if(index != 0)
  {
    ListRemoveItem(theIniText->sectionList, &section, index);
    if(theIniText->lastSectionAccessed == section)
      theIniText->lastSectionAccessed = NULL;

    Ini_DisposeSection(section);
    removed = TRUE;
  }

  return(removed);
}

/*********************************************************************/

static void Ini_DisposeEntryList(ListType entryList)
{
  int index;

  if(entryList)
  {
    for(index = 1; index <= ListNumItems(entryList); index++)
      Ini_DisposeEntry(*(IniEntry *)ListGetPtrToItem(entryList, index));

    ListDispose(entryList);
  }
}

/*********************************************************************/

static char *Ini_SectionName(IniSection section)
{
  return(section->sectionName);
}

/*********************************************************************/

static int CVICALLBACK Ini_CompareSectionToName(void *sectionName, void *ptrToSection)
{
  return(INISTRICMP((char *)sectionName, Ini_SectionName(*(IniSection*)ptrToSection)));
}

/*********************************************************************/

static int CVICALLBACK Ini_CompareSectionNames(void *ptrToSection1, void *ptrToSection2)
{
  return(INISTRICMP(Ini_SectionName(*(IniSection*)ptrToSection1), Ini_SectionName(*(IniSection*)ptrToSection2)));
}

/*********************************************************************/

/*
Creates a dynamically allocated copy of a string while stripping
off matching quotes if present.  Returns whether quotes were present.
*/
static char * StrDupWithoutQuotes(const char *string, int *hadQuotes)
{
  char        *copy;
  int         len;

  if(hadQuotes)
    *hadQuotes = FALSE;

  len = strlen(string);

  /* look for and remove matching quotes */
  if((len > 1) && (string[0] == '\"') && (string[len - 1] == '\"'))
  {
    string++;
    len -= 2;
    if(hadQuotes)
      *hadQuotes = TRUE;
  }

  copy = malloc (len+1);
  if(copy)
  {
    memcpy (copy, string, len);
    copy[len] = '\0';
  }

  return(copy);
}

/*********************************************************************/

static int Ini_SetEntryValue(IniEntry entry, const char *value, int stripMarkerQuotes)   
{
  int     error = 0;
  char    *newValue = 0;
  int     hadMarkerQuotes;

  if(stripMarkerQuotes)
  {
    nullChk(newValue = StrDupWithoutQuotes(value, &hadMarkerQuotes));
  }
  else
  {
    hadMarkerQuotes = FALSE;
    nullChk(newValue = StrDup((char *)value));
  }

  free(entry->value);
  entry->value = newValue;
  entry->addMarkerQuotes = hadMarkerQuotes;   

Error:
  return(error);
}

/*********************************************************************/

static IniEntry Ini_NewEntry(const char *name, const char *value, int stripMarkerQuotes)
{
  IniEntry    newEntry = NULL;
  int         error = 0;

  nullChk(newEntry = calloc(sizeof(*newEntry), 1));
  nullChk(newEntry->name = StrDupWithoutSurrWhiteSpace((char *)name));
  errChk( Ini_SetEntryValue(newEntry, value, stripMarkerQuotes));

Error:
  if(error < 0)
  {
    Ini_DisposeEntry(newEntry);
    newEntry = NULL;
  }

  return(newEntry);
}

/*********************************************************************/

static void Ini_DisposeEntry(IniEntry theEntry)
{
  if(theEntry)
  {
    free(theEntry->name);
    free(theEntry->value);
    free(theEntry);
  }
}

/*********************************************************************/

static int CVICALLBACK Ini_CompareEntryNames(void *ptrToEntry1, void *ptrToEntry2)
{
  return(INISTRICMP(Ini_EntryName(*(IniEntry*)ptrToEntry1), Ini_EntryName(*(IniEntry*)ptrToEntry2)));
}

/*********************************************************************/

static int CVICALLBACK Ini_CompareEntryToName(void *entryName, void *ptrToEntry)
{
  return(INISTRICMP((char *)entryName, Ini_EntryName(*(IniEntry*)ptrToEntry)));
}

/*********************************************************************/

static char *Ini_EntryName(IniEntry entry)
{
  return(entry->name);
}

/*********************************************************************/

/*  After an entry is inserted into a section, the section owns it, is responsible
    for disposing it, and should be the only one to access it.
*/
static int Ini_InsertEntry(IniSection section, IniEntry entry)
{
  if(section->sorted)
    return(ListInsertInOrder(section->entryList, &entry, Ini_CompareEntryNames));
  else
    return(ListInsertItem(section->entryList, &entry, END_OF_LIST));
}

/*********************************************************************/

static IniSection Ini_FindSection(IniText theIniText, const char *sectionName)
{
  int         index;
  IniSection  section = NULL;

  /* optimization */
  if(theIniText->lastSectionAccessed
     && (INISTRICMP((char *)sectionName,
                    theIniText->lastSectionAccessed->sectionName) == 0))
    return(theIniText->lastSectionAccessed);

  index = Ini_FindSectionIndex(theIniText, sectionName);

  if(index != 0)
  {
    ListGetItem(theIniText->sectionList, &section, index);
    theIniText->lastSectionAccessed = section;
  }

  return(section);
}

/*********************************************************************/

static int Ini_FindEntryIndex(IniSection section, const char *itemName)
{
  int index = 0;

  if(section)
  {
    if(section->sorted)
      index = ListBinSearch(section->entryList, (void *)itemName, Ini_CompareEntryToName);
    else
      index = ListFindItem(section->entryList, (void *)itemName, FRONT_OF_LIST, Ini_CompareEntryToName);
  }

  return(index);
}

/*********************************************************************/

static IniEntry Ini_FindEntry(IniSection section, const char *itemName)
{
  int         index;
  IniEntry    entry = NULL;

  index = Ini_FindEntryIndex(section, itemName);

  if(index != 0)
    ListGetItem(section->entryList, &entry, index);

  return(entry);
}

/*********************************************************************/

int CVIFUNC Ini_RemoveItem(IniText theIniText, const char *sectionName, const char *itemName)
{
  IniSection  section;
  int         removed;

  section = Ini_FindSection(theIniText, sectionName);

  removed = Ini_RemoveFromSection(theIniText, section, itemName);

  if(removed)
    while(Ini_RemoveFromSection(theIniText, section, itemName));   /* make sure to remove any duplicates as well */

  return(removed);
}

/*********************************************************************/

static int Ini_RemoveFromSection(IniText theIniText, IniSection section, const char *itemName)
{
  int         index;
  IniEntry    entry;
  int         removed = FALSE;

  if(section)
  {
    index = Ini_FindEntryIndex(section, itemName);

    if(index != 0)
    {
      ListRemoveItem(section->entryList, &entry, index);
      Ini_DisposeEntry(entry);
      removed = TRUE;
    }
  }

  return(removed);
}

/*********************************************************************/

static int Ini_WriteSection(IniText iniText, IniSection theSection, Ini_OutputFunc outputFunc, void *outputDest)
{
  int     error = 0;
  char    *outputBuf = NULL;
  int     outputLength;
  int     index;

  outputLength = strlen(theSection->sectionName) + 2 + 1;
  /* +2 for section name brackets, +1 for the nul byte */
  nullChk (outputBuf = malloc(outputLength));
  sprintf (outputBuf, "%c%s%c", iniText->sectionNameStartChar, theSection->sectionName, 
           iniText->sectionNameEndChar);
  errChk ((*outputFunc) (outputDest, outputBuf));

  if(theSection->entryList)
    for(index=1; index <= ListNumItems(theSection->entryList); index++)
      errChk (Ini_WriteEntry (iniText, *(IniEntry*)ListGetPtrToItem(theSection->entryList, index),
                              outputFunc, outputDest));

Error:
  free(outputBuf);
  return(error);
}

/*********************************************************************/

static int Ini_WriteEntry(IniText iniText, IniEntry entry, Ini_OutputFunc outputFunc, void *outputDest) 
{
  int     error = 0;
  char    lineBuffer[INI_NUM_CHARS_PER_LINE + 1];
  char    itemNameBuff[kMaxIniLineSize + 1];
  char    outputBuffer[kMaxIniLineSize + 1];
  int     len, numLines, i, remainder;
  char    *srcTextPtr;

  len = strlen(entry->value);
  numLines = len / INI_NUM_CHARS_PER_LINE;
  remainder = len % INI_NUM_CHARS_PER_LINE;
  if(remainder)
    numLines++;

  if(numLines <= 1)
  {
    sprintf (outputBuffer, entry->addMarkerQuotes ? "%s%s\"%s\"" : "%s%s%s", entry->name, iniText->nameValueOutputSeparatorToken, 
             entry->value);
    errChk ((*outputFunc)(outputDest, outputBuffer));
  }
  else
  {
    srcTextPtr = entry->value;

    for(i = 1; i <= numLines; i++)
    {
      sprintf(itemNameBuff, "%s%s%04d", entry->name, kLineTag, i);
      strncpy(lineBuffer, srcTextPtr, INI_NUM_CHARS_PER_LINE);
      srcTextPtr += INI_NUM_CHARS_PER_LINE;
      lineBuffer[INI_NUM_CHARS_PER_LINE] = '\0';
      /* always add quotes to multiline items */
      sprintf (outputBuffer, entry->addMarkerQuotes ? "%s%s\"%s\"" : "%s%s%s", itemNameBuff, iniText->nameValueOutputSeparatorToken, 
               lineBuffer);
      errChk ((*outputFunc)(outputDest, outputBuffer));
    }
  }

Error:
  return(error);
}

/*********************************************************************/

static int  Ini_Put(IniText theIniText, const char *sectionName, const char *itemName, const char *value, int addMarkerQuotes)
{
  IniSection  section = NULL;
  IniSection  newSection = NULL;
  IniEntry    newEntry = NULL;
  int         error = 0;

  nullChk(newEntry = Ini_NewEntry(itemName, value, FALSE));
  newEntry->addMarkerQuotes = addMarkerQuotes;

  section = Ini_FindSection(theIniText, sectionName);

  if(!section) /* if no current section with this name, create one and use it */
  {
    nullChk(newSection = Ini_NewSection(sectionName, theIniText->sorted));
    nullChk(Ini_InsertEntry(newSection, newEntry));
    nullChk(Ini_InsertSection(theIniText, newSection));
  }
  else
  {
    if(theIniText->checkForDuplicatesWhenPutting)
      while(Ini_RemoveFromSection(theIniText, section, itemName)); /* remove any items with the same name */
    nullChk(Ini_InsertEntry(section, newEntry));
  }

Error:
  if(error < 0)
  {
    Ini_DisposeSection(newSection);
    Ini_DisposeEntry(newEntry);
  }

  return(error);
}

/*********************************************************************/

/*  If not found, sets *value = NULL and returns FALSE if not found.
    If found, sets *value = ptr to value string and returns TRUE
    (the caller must not modify the returned string).
    The string in *value is valid until the next operation on theIniText.
*/
static int  Ini_Get(IniText theIniText, const char *sectionName, const char *itemName, char **value)
{
  IniSection  section = NULL;
  IniEntry    entry = NULL;
  int         found = FALSE;

  if(value)
    *value = NULL;

  section = Ini_FindSection(theIniText, sectionName);

  if(section)
  {
    entry = Ini_FindEntry(section, itemName);
    if(entry)
    {
      if(value)
        *value = entry->value;
      found = TRUE;
      theIniText->lastLineAccessed = entry->sourceLine;
    }
  }

  return(found);
}

/*********************************************************************/

int CVIFUNC Ini_WriteGeneric(IniText theIniText, Ini_OutputFunc outputFunc, void *outputDest)
{
  int error = 0;
  int index;

  if(theIniText->sectionList)
  {
    for(index = 1; index <= ListNumItems(theIniText->sectionList); index++)
    {
      errChk(Ini_WriteSection(theIniText, *(IniSection *)ListGetPtrToItem(theIniText->sectionList, index), outputFunc, outputDest));
      errChk((*outputFunc)(outputDest, ""));
    }
  }
Error:
  return(error);
}

/*********************************************************************/

static int CVICALLBACK Ini_FileOutputFunc(void *file, char *outputString)
{
  int error = 0;

  errChk (WriteStringToFile ((FILE *)file, outputString));
  errChk (WriteStringToFile ((FILE *)file, "\n"));

Error:
  return(error);
}

/*********************************************************************/
/*********************************************************************/

int CVIFUNC Ini_WriteToFile(IniText theIniText, const char pathName[])
{
  char tempPathName[MAX_PATHNAME_LEN];
  FILE *tempFile=NULL;    /* a non-NULL value indicates the file is open */
  int tempFileCreated=FALSE;
  int isWritable;
  int error = 0;

  sprintf(tempPathName,"");
  
  if(pathName)
  {
    /*******************************************************************/
    /* Check the file identified by pathName for writeability.         */
    /* Create a temporary file in the same directory.                  */
    /*******************************************************************/
    errChk (GetFileWritability ((char *)pathName, &isWritable));
    if(!isWritable)
      errChk (UIEAccessDenied);
    errChk (CreateAndOpenTemporaryFile ((char *)pathName, "ini$$", "TMP", "wb",
                                        tempPathName, &tempFile))
    tempFileCreated = TRUE;

    /*******************************************************************/
    /* Write to the temporary file so that the original file will not  */
    /* be wiped out in case an error occurs.                           */
    /* Remember to flush the file before closing it.                   */
    /*******************************************************************/
    errChk (Ini_WriteGeneric (theIniText, Ini_FileOutputFunc, (void *)tempFile));
    if(fflush (tempFile) != 0)
      errChk (UIEDiskFull);
    fclose (tempFile);
    tempFile = NULL;     /* so won't close it twice */

    /**********************************************************************/
    /* Remove the original file, and rename the temporary file.           */
    /* Since the two files are in the same directory, we should not       */
    /* get a disk full error.                                             */
    /* We should not get a persmission error, unless the original file    */
    /* is writable but not deletable.                                     */
    /**********************************************************************/
    errChk (DeleteAndRename ((char *)tempPathName, (char *)pathName));
  }
  else
  {
    errChk (Ini_WriteGeneric (theIniText, Ini_FileOutputFunc, (void *)stdout));
    fflush(stdout);
  }

Error:
  if(error < 0)
    if(tempFileCreated)
    {
      if(tempFile)
        fclose (tempFile);
      RemoveFileIfExists (tempPathName);
    }

  return(error);
}

/*********************************************************************/

int CVIFUNC Ini_ReadFromFile(IniText theIniText, const char pathName[])
{
  int         error = 0;
  FILE        *file = NULL;
  LineFile    lineFile = NULL;

  file = fopen (pathName, "rb");
  if(file == NULL)
    errChk (ToolErr_CouldNotOpenFileForReading);

  nullChk (lineFile = LineFile_New(LineFile_ReadFromFile, (void *)file));

  errChk (Ini_ReadGeneric (theIniText, Ini_LineFileInputFunction, (void *)lineFile));

Error:
  LineFile_Dispose(lineFile);
  if(file)
    fclose (file);
  return(error);
}

/*********************************************************************/

static int CVICALLBACK Ini_LineFileInputFunction(void *inputSource, char *inputBuffer, int maxLineLength, int *endOfSource, int *lineNumRead)
{
  int error = 0;

  errChk (LineFile_ReadNonEmpty((LineFile)inputSource, inputBuffer,
                                maxLineLength, NULL, endOfSource));
  if(lineNumRead)
    *lineNumRead = LineFile_NumLinesRead((LineFile)inputSource);

Error:
  return(error);
}

/***********************************************************************************/

int  CVIFUNC Ini_ReadGeneric(IniText theIniText, Ini_InputFunc inputFunc, void *inputSource)
{
  char        lineBuf[kMaxIniLineSize+1];
  char        sectionBuf[kMaxIniLineSize+1];
  char        itemNameBuf[kMaxIniLineSize+1];
  char        valueBuf[kMaxIniLineSize+1];
  int         error = 0;
  int         lineType;
  IniSection  currentSection = NULL, newSection = NULL;
  int         endOfSource = FALSE;
  IniEntry    newEntry = NULL;
  int         lineRead;

  while(!endOfSource)
  {
    errChk((*inputFunc)(inputSource, lineBuf, kMaxIniLineSize, &endOfSource, &lineRead));

    if(!endOfSource)
    {
      sectionBuf[0] = 0;
      itemNameBuf[0] = 0;
      valueBuf[0] = 0;

      errChk(lineType = ParseTagTextLine(theIniText, lineBuf, sectionBuf, itemNameBuf, valueBuf));

      switch(lineType)
      {
        case kIni_SectionLine:
          if(theIniText->sectionFilter && !theIniText->sectionFilter(theIniText, theIniText->sectionFilterCallbackData, sectionBuf))
            currentSection = NULL;
          else
          {
            nullChk(newSection = Ini_NewSection(sectionBuf, theIniText->sorted));
            newSection->sourceLine = lineRead;
            errChk(ListInsertItem(theIniText->sectionList, &newSection, END_OF_LIST));
            currentSection = newSection;
            newSection = NULL;
          }
          break;
        case kIni_TagAndValueLine:
          if(currentSection)
          {
            nullChk(newEntry = Ini_NewEntry(itemNameBuf, valueBuf, TRUE));
            newEntry->sourceLine = lineRead;
            errChk(ListInsertItem(currentSection->entryList, &newEntry, END_OF_LIST));
            newEntry = NULL;
          }
          break;
        default:
          break;
      }
    }
  }

  errChk( CoalesceMultilineItemsAfterRead(theIniText));

  if(theIniText->sorted)
    Ini_Sort(theIniText);

Error:
  Ini_DisposeSection(newSection);
  Ini_DisposeEntry(newEntry);
  return(error);
}

/*********************************************************************/

static int CoalesceMultilineItemsAfterRead(IniText iniText)
{
  int         error = 0;
  int         numSections;
  int         index;
  IniSection  section;

  numSections = ListNumItems(iniText->sectionList);

  for(index = 1; index <= numSections; index++)
  {
    ListGetItem(iniText->sectionList, &section, index);
    errChk( CoalesceMultilineItemsInSection(section));
  }

Error:
  return(error);
}

/*********************************************************************/

/*  Searches for a multiline entry starting from <startOfSearchIndex>.  Returns the 
    start and ending indices of the multiline entry if found.  Function returns TRUE 
    if a multiline item was found, FALSE otherwise.
*/

static int FindMultiLineItem(IniSection section, int startOfSearchIndex, int *startIndex, int *endIndex)
{
  int         numItems;
  int         index;
  IniEntry    entry; 
  int         lookingForEndOfMultiLine = FALSE;
  int         isMultiLineItem;
  int         whichLine;

  numItems = ListNumItems(section->entryList);

  for(index = startOfSearchIndex; index <= numItems; index++)
  {
    ListGetItem(section->entryList, &entry, index);
    Ini_AnalyzeItemName(entry->name, &isMultiLineItem, &whichLine, 0);

    /* if we found the first line of a multiline entry */
    if(isMultiLineItem && !lookingForEndOfMultiLine && whichLine == 1)
    {
      *startIndex = index;
      *endIndex = index;
      lookingForEndOfMultiLine = TRUE;
    }
    else
      if(lookingForEndOfMultiLine)
    {
      if(!isMultiLineItem || whichLine == 1)  /* found the end of this multiline entry */
        break;
      else
        *endIndex = index;  /* found another item in this multiline entry */
    }
  }

  return(lookingForEndOfMultiLine);
}

/*********************************************************************/

static int CoalesceMultiLineItems(IniSection section, int startIndex, int endIndex)
{
  IniEntry    entry;
  char        singleLineItemNameBuf[kMaxIniLineSize+1];
  char        *newName = 0;
  int         error = 0;
  int         length = 0;
  char        *coalescedValue = 0;
  char        *appendPtr;
  int         index;

  for(index = startIndex; index <= endIndex; index++)
  {
    ListGetItem(section->entryList, &entry, index);
    length += strlen(entry->value);         
  }

  nullChk( coalescedValue = (char *)malloc(length+1));

  appendPtr = coalescedValue;

  /* coalesce the values of all multiline entries */
  for(index = startIndex; index <= endIndex; index++)
  {
    ListGetItem(section->entryList, &entry, index);
    strcpy(appendPtr, entry->value);
    appendPtr += strlen(entry->value);
  }       

  ListGetItem(section->entryList, &entry, startIndex);
  Ini_AnalyzeItemName(entry->name, 0, 0, singleLineItemNameBuf); /* get item base name */
  nullChk( newName = StrDup(singleLineItemNameBuf));

  /* alter first entry to contain the coalesced value */
  free(entry->value);
  entry->value = coalescedValue;
  free(entry->name);
  entry->name = newName;
  /* keep the same value of entry->addMarkerQuotes */

  /* dispose all multiline entries except the first */
  for(index = startIndex + 1; index <= endIndex; index++)
  {
    ListRemoveItem(section->entryList, &entry, startIndex + 1);
    Ini_DisposeEntry(entry);
  }

Error:
  if(error < 0)
  {
    free(newName);
    free(coalescedValue);
  }

  return(error);
}

/*********************************************************************/

static int CoalesceMultilineItemsInSection(IniSection   section)
{
  int         error = 0;
  int         index;
  int         start;
  int         end;

  for(index = 1; FindMultiLineItem(section, index, &start, &end); index = start + 1)
  {
    errChk( CoalesceMultiLineItems(section, start, end));
  }

Error:
  return(error);
}

/*********************************************************************/


/* returns an IniLineType enum or negative int error,
   buf must be NULL terminated
*/
static int ParseTagTextLine(IniText iniText, char buf[], char *section, char *item, char *value)
{
  char    *tagStart, *tagEnd, *valStart, *valEnd, *sep, *p;
  int     tagLen, valLen;

  /********************************************************************/
  /* NOTE:  Comment lines begin with ';'.                             */
  /*        Section titles are in square brackets.                    */
  /*        Extraneous characters after "<tag>=<val>" are ignored.    */
  /*        If the value can include embedded blanks (particularly    */
  /*        trailing blanks) then it should be surrounded by double   */
  /*        quotation marks.  If the value can begin and end with     */
  /*        double qoutation marks, then it should be surrounded      */
  /*        with an extra set of double quotation marks.              */
  /*        We will consider an all blank value as no value at all.   */
  /*        However, "" and "   " will be considered as significant   */
  /*        values.                                                   */
  /*        NOTE:  Leading and trailing blanks (outside of the double */
  /*               quotes) are always ignored.                        */
  /********************************************************************/

  tagStart = SkipWhiteSpace(buf);

  if(FastTokenCmp(tagStart,iniText->commentMarker) == 0)
    return(kIni_CommentLine);                /* is a comment */

  if(*tagStart == iniText->sectionNameStartChar)
    return(ParseTagTextSectionTitle(iniText, tagStart+1, section));

  for(p=tagStart; FastTokenCmp(p, iniText->nameValueInputSeparatorToken); p++)
    if(!*p)
      return(kIni_NotAnIniLine);               /* no separator */

  sep = p;

  for(tagEnd = sep - 1; tagEnd > tagStart && isspace(*tagEnd); tagEnd--);      /* find end of tag, removing white space from trailing end of the tag */
  tagEnd++;               /* point to first character after the last character in the tag */

  if(tagEnd <= tagStart)
    return(kIni_NotAnIniLine);               /* no tag */

  valStart = SkipWhiteSpace(sep + iniText->nameValueInputSeparatorLength);

  p = valStart + strlen(valStart) - 1;
  while(isspace (*p))                /* look backwards for last non white space */
    p--;
  valEnd = p + 1;                         /* Note:  valEnd may equal valStart */

  if(valEnd <= valStart)
    return(kIni_NotAnIniLine);               /* no value string at all */


  p = SkipNonWhiteSpace(valStart);
  /* nValCharsBeforeFirstWhiteSpace = p - valStart; */

  /*****************************************************************/
  /* At this point, we know that there is a tag, a separator       */
  /* and a value.  So we can dynamically allocate the strings.     */
  /*****************************************************************/

  tagLen = tagEnd - tagStart;

  memcpy (item, tagStart, tagLen);
  (item)[tagLen] = 0;

  valLen = valEnd - valStart;
  memcpy (value, valStart, valLen);
  (value)[valLen] = 0;

  return(kIni_TagAndValueLine);
}

/*********************************************************************/

/*
    Returns an IniLineType enum or negative int error
    modified to still ignore leading and trailing white space after the start
    bracket and before the close bracket while keeping white space between multiple
    words in the section title (before it stopped after the first word.
*/
static int ParseTagTextSectionTitle(IniText iniText, char *firstCharAfterOpenBracket, char *sectionTitle)
{
  char    *begChar, *closeBracket, *stopChar;
  int     nChars;

  begChar = SkipWhiteSpace (firstCharAfterOpenBracket);

  if((closeBracket = strrchr (begChar, iniText->sectionNameEndChar)) == NULL)     /* JAG 8/23/96 PATCH 4.0.1 */
    return(kIni_NotAnIniLine);

  /* make sure the end bracket is the last non-whitespace character on the line */
  if(*SkipWhiteSpace(closeBracket + 1) != '\0')    /* JAG 8/23/96 PATCH 4.0.1 */
    return(kIni_NotAnIniLine);

  /* find end of section title (minus trailing whitespace) */
  for(stopChar = closeBracket; stopChar > begChar && isspace(*(stopChar-1)); stopChar--);

  nChars = stopChar - begChar;
  if(nChars <= 0)
    return(kIni_NotAnIniLine);

  memcpy (sectionTitle, begChar, nChars);
  (sectionTitle)[nChars] = 0;

  return(kIni_SectionLine);
}

/*********************************************************************/

#define kBooleanTrueValueString   "True"
#define kBooleanFalseValueString  "False"

/**********************************************************************/

int CVIFUNC Ini_PutInt(IniText dest, const char *section, const char *itemName, int value)
{
  char buffer[256];

  sprintf(buffer, "%ld", value);
  return(Ini_Put(dest, section, itemName, buffer, FALSE));
}

/**********************************************************************/

int CVIFUNC Ini_PutUInt(IniText dest, const char *section, const char *itemName, unsigned int value)
{
  char buffer[256];

  sprintf(buffer, "%lu", value);
  return(Ini_Put(dest, section, itemName, buffer, FALSE));
}

/**********************************************************************************/

int CVIFUNC Ini_PutDouble(IniText dest, const char *section, const char *itemName, double value)
{
  char buffer[256];

  sprintf(buffer, "%.16g", value);   /* JAG 8/13/96 PATCH 4.0.1 */
  return(Ini_Put(dest, section, itemName, buffer, FALSE));
}

/**********************************************************************************/

int CVIFUNC Ini_PutBoolean(IniText dest, const char *section, const char *itemName, int boolValue)
{
  return(Ini_Put (dest, section, itemName, boolValue
                  ? kBooleanTrueValueString
                  : kBooleanFalseValueString,
                  FALSE));
}

/**********************************************************************/

int CVIFUNC Ini_PutString(IniText dest, const char *section, const char *itemName, const char *srcText)
{
  int     error = 0;
  char    *escText = NULL;

  if(!srcText)
    srcText = "";

  errChk( EscapeText(srcText, &escText));
  errChk( Ini_PutRawString(dest, section, itemName, escText));

Error:
  free(escText);
  return(error);
}

/**********************************************************************/

int CVIFUNC Ini_PutRawString(IniText dest, const char *section, const char *itemName, const char *srcText)
{
  return(Ini_PutSingleLineString(dest, section, itemName, srcText));  /* add quotes around string */
}

/*****************************************************************************/

/*  Ini_GetPointerToString:
    If itemName is NOT found, sets *value to NULL and returns FALSE.
    If itemName IS found, sets *value to the pointer to string, and returns TRUE.
    The string in *value is valid until the next call to Ini_GetPointerToString,
    Ini_GetStringCopy, or Ini_GetStringIntoBuffer..  (or their "Raw" equivalents).
    If an error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetPointerToString(IniText dest, const char *section, const char *itemName, char **destStr)
{
  int     error = 0;
  int     found;
  char    *rawString;

  *destStr = NULL;

  errChk( found = Ini_GetPointerToRawString(dest, section, itemName, &rawString));

  if(found)
  {
    errChk(SetStringCacheSize(dest->stringCache, strlen(rawString)+1));
    errChk(UnescapeText(rawString, *dest->stringCache));
    *destStr = *dest->stringCache;
  }

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/*****************************************************************************/

/*  Ini_GetPointerToRawString:
    If itemName is NOT found, sets *value to NULL and returns FALSE.
    If itemName IS found, sets *value to the pointer to string, and returns TRUE.
    The string in *value is the string in the in-memory list.  Do not modify its contents.
    Ini_GetStringCopy, or Ini_GetStringIntoBuffer (or their "raw" equivalents)
    If an error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetPointerToRawString(IniText dest, const char *section, const char *itemName, char **destStr)
{
  return(Ini_Get(dest, section, itemName, destStr));
}

/************************************************************************************/

/* Allocated in chunk sized increments to minimize the need to reallocate when the cache grows or shrinks */
static int SetStringCacheSize(char **strHandle, int sizeNeeded)
{
  int error = 0;
  int numCacheChunks;

  numCacheChunks  = (sizeNeeded / kStringCacheChunkSize) + 1;  /* minumum number of chunks needed to hold <sizeNeeded> bytes */
  if(!SetHandleSize((void **)strHandle, numCacheChunks * kStringCacheChunkSize))
    errChk (UIEOutOfMemory);

Error:
  return(error);
}

/*****************************************************************************/

static int  Ini_PutSingleLineString(IniText theIniText, const char *section, const char *itemName, const char *value)
{
  int     error = 0;

  if(value == NULL)
    value = "";

  errChk(Ini_Put(theIniText, section, itemName, value, TRUE));

Error:
  return(error);
}

/************************************************************************************/

/*
    Ini_GetStringCopy
    If itemName is NOT found, sets *value to NULL and returns FALSE.
    If itemName IS found, dynamically allocates a copy of the string,
    places the pointer to the copy in *value, and returns TRUE.
    The user is responsible for freeing the string.
    If an error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetStringCopy(IniText theIniText, const char *section, const char *itemName, char **value)
{
  int     error = 0;
  char    *string;
  int     found;

  *value = NULL;
  errChk(found = Ini_GetPointerToString(theIniText, section, itemName, &string));
  if(found)
    nullChk(*value = StrDup(string));

Error:
  if(error < 0)
  {
    free(*value);
    *value = NULL;
    return(error);
  }
  else
    return(found);
}

/************************************************************************************/

/*
    Ini_GetRawStringCopy
    If itemName is NOT found, sets *value to NULL and returns FALSE.
    If itemName IS found, dynamically allocates a copy of the string,
    places the pointer to the copy in *value, and returns TRUE.
    The user is responsible for freeing the string.
    If an error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetRawStringCopy(IniText theIniText, const char *section, const char *itemName, char **value)
{
  int     error = 0;
  char    *string;
  int     found;

  *value = NULL;
  errChk(found = Ini_GetPointerToRawString(theIniText, section, itemName, &string));
  if(found)
    nullChk(*value = StrDup(string));

Error:
  if(error < 0)
  {
    free(*value);
    *value = NULL;
    return(error);
  }
  else
    return(found);
}

/************************************************************************************/

/*
    Ini_GetStringIntoBuffer
    If itemName is NOT found, sets buffer[0] to 0, and returns FALSE.
    If itemName IS found, copies the string into buffer, and returns TRUE.
    If an error occurs, returns a negative error code.
    (Note: bufSize includes the space for the nul byte.)
*/
int CVIFUNC Ini_GetStringIntoBuffer(IniText theIniText, const char *section, const char *itemName,
                                    char buffer[], int bufSize)
{
  int     error = 0;
  char    *temp;
  int     found;

  buffer[0] = 0;
  errChk(found = Ini_GetPointerToString(theIniText, section, itemName, &temp));
  if(found)
  {
    strncpy (buffer, temp, bufSize-1);
    buffer[bufSize-1] = 0;
  }

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/************************************************************************************/

/*
    Ini_GetRawStringIntoBuffer
    If itemName is NOT found, sets buffer[0] to 0, and returns FALSE.
    If itemName IS found, copies the string into buffer, and returns TRUE.
    If an error occurs, returns a negative error code.
    (Note: bufSize includes the space for the nul byte.)
*/
int CVIFUNC Ini_GetRawStringIntoBuffer(IniText theIniText, const char *section, const char *itemName,
                                       char buffer[], int bufSize)
{
  int     error = 0;
  char    *temp;
  int     found;

  buffer[0] = 0;
  errChk(found = Ini_GetPointerToRawString(theIniText, section, itemName, &temp));
  if(found)
  {
    strncpy (buffer, temp, bufSize-1);
    buffer[bufSize-1] = 0;
  }

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/**********************************************************************/

/*
    Ini_GetInt:
    If the itemName is NOT found or has an empty value, returns FALSE.
    If itemName IS found and has a valid ASCII representation of an integer,
    sets *value to the integer value, and returns TRUE.
    If the value is not a valid ASCII representation of an integer, or any
    other error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetInt(IniText src, const char *section, const char *itemName, int *value)
/* returns negative error, or FALSE (tag or value is missing), or TRUE */
{
  char *temp=0;
  int found;
  int error = 0;

  errChk (found = Ini_Get(src,section,itemName, &temp));
  found = found && HasNonWhiteSpace(temp);
  if(found)
    if(!StrToInt (temp, value))
      errChk (ToolErr_InvalidIntNumber);

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/***************************************************************************/

/*
    Ini_GetUInt:
    If the itemName is NOT found or has an empty value, returns FALSE.
    If itemName IS found and has a valid ASCII representation of an unsigned
    integer, sets *value to the integer value, and returns TRUE.
    If the value is not a valid ASCII representation of an unsigned integer,
    or any other error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetUInt(IniText src, const char *section, const char *itemName, unsigned int *value)
/* returns negative error, or FALSE (tag or value is missing), or TRUE */
{
  char *temp;
  int found;
  int error = 0;

  errChk (found = Ini_Get (src, section, itemName, &temp));
  found = found && HasNonWhiteSpace (temp);
  if(found)
    if(!StrToUInt (temp, value))
      errChk (ToolErr_InvalidUIntNumber);

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/***************************************************************************/

/*
    Ini_GetDouble:
    If the itemName is NOT found or has an empty value, returns FALSE.
    If itemName IS found and has a valid ASCII representation of a double,
    sets *value to the double value, and returns TRUE.
    If the value is not a valid ASCII representation of a double, or any
    other error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetDouble(IniText src, const char *section, const char *itemName, double *value)
/* returns negative error, or FALSE (tag or value is missing), or TRUE */
{
  char *temp = NULL, *endPtr = NULL;
  int found, len;
  int error = 0;

  errChk (found = Ini_Get(src,section,itemName, &temp));
  found = found && HasNonWhiteSpace (temp);
  if(found)
  {
    if((INISTRICMP(temp, "+Inf") == 0) 
       ||  (INISTRICMP(temp, "Inf") == 0))
      *value = HUGE_VAL;
    else
    {
      if(INISTRICMP(temp, "-Inf") == 0)
        *value = -HUGE_VAL;
      else
      {
        errno = 0;
        len = strlen(temp);
        *value = strtod(temp, (char **)&endPtr);
        if(errno || (endPtr != temp + len))
          errChk (ToolErr_InvalidDoubleNumber);
      }
    }
  }

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/***************************************************************************/

/*
    Ini_GetBoolean:
    If the itemName is NOT found or has an empty value, returns FALSE.
    If itemName IS found and has a valid ASCII representation of a boolean,
    sets *value to TRUE or FALSE, and returns TRUE.
    If the value is not a valid ASCII representation of a boolean,
    or any other error occurs, returns a negative error code.
*/
int CVIFUNC Ini_GetBoolean(IniText src, const char *section, const char *itemName, int *boolValue)
{
  char     *temp;
  int     error=0;
  int     found;

  errChk (found = Ini_Get(src,section,itemName, &temp));
  found = found && HasNonWhiteSpace (temp);
  if(found)
  {
    if(!INISTRICMP (temp, kBooleanTrueValueString))
      *boolValue = TRUE;
    else if(!INISTRICMP (temp, kBooleanFalseValueString))
      *boolValue = FALSE;
    else
      errChk (ToolErr_InvalidBooleanValue);
  }

Error:
  if(error < 0)
    return(error);
  else
    return(found);
}

/*********************************************************************/

int CVIFUNC Ini_ItemOrMultiLineItemExists(IniText theIniText, const char *section, const char *itemName)
{
  return(Ini_ItemExists(theIniText, section, itemName));
}

/*********************************************************************/

int CVIFUNC Ini_ItemExists(IniText theIniText, const char *section, const char *itemName)
{
  /******************************************************************/
  /* NOTE:  This function only indicates that there was a tag,      */
  /*        not that there was a value.  Functions like             */
  /*        Ini_GetInt may return FALSE (not found) even though    */
  /*        this function returns TRUE.                             */
  /******************************************************************/

  return(Ini_Get(theIniText, section, itemName, 0));
}

/*********************************************************************/

static int EscapeText (const char *srcTextPtr, char **destTextPtr)
{
  int         error = 0;
  const unsigned char  *s = (unsigned char *) srcTextPtr;
  char        *d;
  int         srcLen = 0;
  char        buffer[3];
  int         i;

  *destTextPtr = NULL;
  srcLen = strlen (srcTextPtr);

  /***********************************************************************/
  /* since a single character can become as large as \xHH, we want the   */
  /* destination string to be 4 times as large as the original plus NULL */
  /***********************************************************************/
  nullChk (*destTextPtr = malloc (4*srcLen + 1));
  d = *destTextPtr;
  while(*s)
  {
    switch(*s)
    {
      case '\n':
        *d++ = '\\';
        *d++ = 'n';
        break;
      case '\t':
        *d++ = '\\';
        *d++ = 't';
        break;
      case '"':
        *d++ = '\\';
        *d++ = '"';
        break;
      case '\\':
        *d++ = '\\';
        *d++ = '\\';
        break;
      default:
        if(iscntrl(*s))
        {
          *d++ = '\\';
          *d++ = 'x';
          sprintf (buffer, "%02x", *s);
          for(i=0; buffer[i]; i++)
            *d++ = buffer[i];
        }
        else
          *d++ = *s;
        break;
    }
    s++;
  }

  *d = '\0';

Error:
  return(error);
}

/***************************************************************************/
/*
   UnescapeText expects the source string to come from EscapeText, and as
   expects there will be no '"', only '\"'.                               
*/
static int UnescapeText (char *srcTextPtr, char *destTextPtr)
{
  int     error = 0;
  char    *s = srcTextPtr;
  char    *d = destTextPtr;
  int     hexVal1, hexVal2, slash = FALSE;

  while(*s)
  {
    if(slash)
    {
      switch(*s)
      {
        case 'n':
          *d++ = '\n';
          break;
        case 't':
          *d++ = '\t';
          break;
        case 'x':
          hexVal1 = GetHexCharValue(*++s);
          hexVal2 = GetHexCharValue(*++s);
          *d++ = (char)(16 * hexVal1 + hexVal2);
          break;
        default:
          *d++ = *s;
          break;
      }
      slash = FALSE;
    }
    else
    {
      if(*s == '\\')
        slash = TRUE;       /* hasEscapeText = TRUE */
      else
        *d++ = *s;
    }
    s++;
  }

  *d = '\0';

  /*Error:*/
  return(error);
}

/**************************************************************************/

static int GetHexCharValue (int hexChar)
{
  char    buffer[2];
  int     retVal=0;

  if(isxdigit(hexChar))
  {
    if(isdigit(hexChar))
    {
      buffer[0] = (char)hexChar;
      buffer[1] = '\0';
      retVal = atoi(buffer);
    }
    else
    {
      switch(tolower(hexChar))
      {
        case 'a':
          retVal = 10;
        case 'b':
          retVal = 11;
        case 'c':
          retVal = 12;
        case 'd':
          retVal = 13;
        case 'e':
          retVal = 14;
        case 'f':
          retVal = 15;
      }
    }
  }
  else
  {
    retVal = -1;
  }

  return(retVal);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

#define kLineFileIOBufSize  2084

struct LineFileForwardReferenceTag
{
  void  *inputSource;
  LineFileReadFunc inputFunc;
  char  *fileBuf;
  int   numBytesLeftInBuf;
  char  *currBufLoc;
  int   numLinesRead;
  int   numBytesRead;
  int   prevLineTermChar;
  int   offsetOfLastLine;    /* byte offset within file of the line most recently read */
  int   numExtraLineTermChars;   /* number of extra line term chars thrown away at beginning of most recently read line */
  int   numCharsInEmptyLines;    /* includes whitespace chars and line term chars in empty lines thrown away by LineFileReadNonEmpty */
};


static void     LineFile_ResetStateVariables (LineFile lineFile);
static int      LineFile_RefillBuf(LineFile lineFile);

/*************************************************************************************************/

/*  Creates a new LineFile object which is useful for line oriented file io.
    file should be an open file.
*/
LineFile CVIFUNC LineFile_New(LineFileReadFunc inputFunc, void *inputSource)
{
  LineFile newLineFile = NULL;
  int      error = 0;

  nullChk(newLineFile = calloc(sizeof(*newLineFile), 1));
  nullChk(newLineFile->fileBuf = malloc(kLineFileIOBufSize));
  newLineFile->inputSource = inputSource;
  if(!inputFunc)
    inputFunc = LineFile_ReadFromFile;      /* default to reading from a file */

  newLineFile->inputFunc = inputFunc;
  LineFile_ResetStateVariables(newLineFile);

Error:
  if(error < 0)
  {
    LineFile_Dispose(newLineFile);
    newLineFile = NULL;
  }

  return(newLineFile);
}

/*******************************************************************************/

static void LineFile_ResetStateVariables (LineFile lineFile)
{
  lineFile->currBufLoc = lineFile->fileBuf;
  lineFile->numBytesLeftInBuf = 0;
  lineFile->prevLineTermChar = 0;
  lineFile->numLinesRead = 0;
  lineFile->numBytesRead = 0;
  lineFile->offsetOfLastLine = -1L;   /* no line has yet been read */
  lineFile->numExtraLineTermChars = 0;
  lineFile->numCharsInEmptyLines = 0;
}

/*******************************************************************************/

/*  Disposes a LineFile object, does not close the file.
*/
void CVIFUNC LineFile_Dispose(LineFile lineFile)
{
  if(lineFile)
  {
    free(lineFile->fileBuf);
    free(lineFile);
  }
}

/*******************************************************************************/

/*  Reads a line from a LineFile object into destbuf.
    maxNumBytes is the max number of bytes to read into destBuf (excluding the NULL byte).
    NULL may be passed for bytesRead if the value isn't needed
    NOTE:  if *endOfFile==TRUE, then *bytesRead==0.
*/
int CVIFUNC LineFile_Read (LineFile lineFile, char *destBuf, int maxNumBytes,
                           int *bytesRead, int *endOfFile)
{
  int   endOfLine = FALSE;
  int   firstByteForLine = TRUE;
  int   tempBytesRead = 0;
  int   ch;
  int   error = 0;

  tempBytesRead = 0;
  *endOfFile = FALSE;

  lineFile->offsetOfLastLine = lineFile->numBytesRead - lineFile->numBytesLeftInBuf;
  lineFile->numExtraLineTermChars = 0;

  while(!*endOfFile && !endOfLine)
  {
    if(lineFile->numBytesLeftInBuf == 0)
      errChk(LineFile_RefillBuf(lineFile));

    if(lineFile->numBytesLeftInBuf)
    {
      ch = *lineFile->currBufLoc;
      if((ch == '\n') || (ch == '\r') || (ch == '\0'))
      {
        if(firstByteForLine && (ch != lineFile->prevLineTermChar))
        {
          lineFile->offsetOfLastLine++;
          lineFile->numExtraLineTermChars++;
        }
        else
        {
          lineFile->prevLineTermChar = ch;
          endOfLine = TRUE;
        }
      }
      else
      {
        if(tempBytesRead < maxNumBytes)
        {
          destBuf[tempBytesRead] = (char)ch;        /* fill user buffer if not full yet */
          tempBytesRead++;
        }
      }

      lineFile->numBytesLeftInBuf--;
      lineFile->currBufLoc++;
      firstByteForLine = FALSE;
    }
    else
    {
      if(tempBytesRead > 0)
        endOfLine = TRUE;    /* to handle case where last line does not have a newline or carriage return */
      else
        *endOfFile = TRUE;
    }
  }

Error:
  destBuf[tempBytesRead] = 0;
  if(bytesRead)
    *bytesRead = tempBytesRead;

  if(endOfLine)
    lineFile->numLinesRead++;

  return(error);
}

/*********************************************************************/

/*  Exactly like LineFile_Read(), except it skips empty lines.
    Either *endOfFile==TRUE && *bytesRead==0
        or *endOfFile==FALSE && *bytesRead > 0
*/
int CVIFUNC LineFile_ReadNonEmpty (LineFile lineFile, char *destBuf, int maxNumBytes,
                                   int *bytesRead, int *endOfFile)
{
  int   numCharsInEmptyLines=0;
  int   error = 0;

  for(;;)
  {
    errChk(LineFile_Read(lineFile, destBuf, maxNumBytes, bytesRead, endOfFile));
    if(*endOfFile || HasNonWhiteSpace(destBuf))
      break;
    numCharsInEmptyLines += strlen(destBuf) + lineFile->numExtraLineTermChars + 1;   /* add 1 for the regular line term char */
  }

Error:
  lineFile->numCharsInEmptyLines = numCharsInEmptyLines;
  return(error);
}

/*******************************************************************************/

int CVIFUNC LineFile_NumLinesRead (LineFile lineFile)
{
  return(lineFile->numLinesRead);
}

/*******************************************************************************/

int CVIFUNC LineFile_OffsetOfLastLine (LineFile lineFile)
{
  return(lineFile->offsetOfLastLine);
}

/*******************************************************************************/

int CVIFUNC LineFile_NumExtraLineTermChars (LineFile lineFile)
{
  return(lineFile->numExtraLineTermChars);
}

/*******************************************************************************/

int CVIFUNC LineFile_NumCharsInEmptyLines (LineFile lineFile)
{
  return(lineFile->numCharsInEmptyLines);
}

/*******************************************************************************/

/*  Refills the contents of a LineFile objects internal IO buffer if it has no more
    unused characters.
*/
static int LineFile_RefillBuf(LineFile lineFile)
{
  int   numRead;
  int   error = 0;

  if(lineFile->numBytesLeftInBuf == 0)
  {
    errChk( lineFile->inputFunc(lineFile->inputSource, kLineFileIOBufSize, &numRead, lineFile->fileBuf));

    lineFile->currBufLoc = lineFile->fileBuf;
    lineFile->numBytesLeftInBuf = numRead;
    lineFile->numBytesRead += numRead;
  }

Error:
  return(error);
}

/*******************************************************************************/

/*  Can be passed to LineFile_New() to read lines from a FILE * */
int CVICALLBACK LineFile_ReadFromFile(void *inputSource, int numBytesToRead, int *numBytesRead, void *dest)
{
  *numBytesRead = fread(dest, 1, numBytesToRead, (FILE *)inputSource);

  if(ferror ((FILE *)inputSource))
    return(ToolErr_ErrorReadingFile);
  else
    return(0);
}

/*******************************************************************************/

#define kNameValueInputSeparatorToken   "="
#define kSectionNameStartChar           '['
#define kSectionNameEndChar             ']'
#define kCommentMarker                  ";"

static void Ini_SetDefaultTokens(IniText iniText)
{
  Ini_SetTokens(iniText, kSectionNameStartChar, kSectionNameEndChar, kNameValueInputSeparatorToken, kCommentMarker);  
}

/*******************************************************************************/

int CVIFUNC Ini_SetTokens(IniText iniText, char sectionNameStartCharacter, char sectionNameEndCharacter, const char *valueSeparator, const char *commentMarker)
{
  if(sectionNameStartCharacter)
    iniText->sectionNameStartChar = sectionNameStartCharacter;

  if(sectionNameEndCharacter)
    iniText->sectionNameEndChar = sectionNameEndCharacter;

  if(valueSeparator && valueSeparator[0])
  {
    StringCopyMax(iniText->nameValueInputSeparatorToken, (char *)valueSeparator, kMaxSpecialTokenLength);
    sprintf(iniText->nameValueOutputSeparatorToken, " %s ", iniText->nameValueInputSeparatorToken);
    iniText->nameValueInputSeparatorLength = strlen(iniText->nameValueInputSeparatorToken);
    iniText->nameValueOutputSeparatorLength = strlen(iniText->nameValueOutputSeparatorToken);
  }

  if(commentMarker && commentMarker[0])
    StringCopyMax(iniText->commentMarker, (char *)commentMarker, kMaxSpecialTokenLength);

  return(0);
}

/*******************************************************************************/

int CVIFUNC Ini_SetSectionFilter(IniText iniText, Ini_SectionFilterFunc sectionFilter, void *callbackData)
{
  iniText->sectionFilter = sectionFilter;
  iniText->sectionFilterCallbackData = callbackData;
  return(0);
}

/*******************************************************************************/

int CVIFUNC Ini_SetDuplicateChecking(IniText iniText, int checkForDuplicates)
{
  int oldState = iniText->checkForDuplicatesWhenPutting;

  iniText->checkForDuplicatesWhenPutting = (checkForDuplicates != 0);

  return(oldState);
}

/*******************************************************************************/

int CVIFUNC Ini_PutData(IniText iniText, const char *section, const char *itemName, const unsigned char *data, long dataSize)
{
  unsigned char   *textPtr=0;
  int             error=0;
  int             result=0;

  nullChk( ConvertDataToText((unsigned char *)data, dataSize, &textPtr));
  errChk( result = Ini_PutRawString(iniText, section, itemName, (const char *)textPtr));  

Error:
  free(textPtr);
  if(error < 0)
    return(error);
  else
    return(result);
}

/*******************************************************************************/

int CVIFUNC Ini_GetData(IniText iniText, const char *section, const char *itemName, unsigned char **data, long *dataSize)
{
  int             error = 0;
  unsigned char   *rawString;
  int             found;
  long            size = 0;

  *data = 0;  
  errChk(found = Ini_GetPointerToRawString(iniText, section, itemName, (char **)&rawString));
  if(found)
    nullChk( ConvertTextToData(rawString, &size, (unsigned char **)data));

Error:
  if(dataSize)
    *dataSize = size;

  if(error < 0)
  {
    free(*data);
    return(error);
  }
  else
    return(found);
}

/*******************************************************************************/

int CVIFUNC Ini_CopySection(IniText source, const char sectionToCopy[], IniText destination, int overwriteDuplicateSections)
{
  int     sectionIndex;
  int     numSectionsToMerge;
  char    *sectionName;
  int     itemIndex;
  int     numItemsToMerge;
  char    *itemName;
  char    *valuePtr;
  int     error = 0;

  if(source == destination)
    goto Error;

  numSectionsToMerge  = Ini_NumberOfSections(source);

  for(sectionIndex = 1; sectionIndex <= numSectionsToMerge; sectionIndex++)
  {
    Ini_NthSectionName (source, sectionIndex, &sectionName);

    if((sectionToCopy == 0) || (INISTRICMP((char *)sectionToCopy, sectionName) == 0))
    {
      if(overwriteDuplicateSections && Ini_SectionExists (destination, sectionName))
        Ini_RemoveSection (destination, sectionName);

      numItemsToMerge = Ini_NumberOfItems (source, sectionName);
      for(itemIndex = 1; itemIndex <= numItemsToMerge; itemIndex++)
      {
        Ini_NthItemName (source, sectionName, itemIndex, &itemName);

        errChk( Ini_GetPointerToRawString(source, sectionName, itemName, &valuePtr));
        errChk( Ini_PutRawString(destination, sectionName, itemName, valuePtr));
      }   /* for item */
    } /* if */
  }   /* for section */

Error:
  if(error < 0)
    return(error);
  else
    return(0);
}

/*******************************************************************************/

/*
    whichLine and singleLineItemName are only set if isMultiLine is set to TRUE.
    Each parameter is optional and can be NULL.
*/
static void Ini_AnalyzeItemName(const char *itemName, int *isMultiLine, int *whichLine, char singleLineItemName[])
{
  char    *lineTagPtr;
  char    *numPtr, *tempNumPtr;
  int     lineNum;

  if(isMultiLine)
    *isMultiLine = FALSE;

  lineTagPtr = strstr (itemName, kLineTag);
  if(!lineTagPtr)
    return;

  numPtr = tempNumPtr = lineTagPtr + strlen(kLineTag);

  if(!isdigit(*tempNumPtr))  /* line tag must be followed by a positive integer number */
    return;

  while(*tempNumPtr)
  {
    if(!isdigit(*tempNumPtr))  /* line tag must not be followed by anything but a number */
      return;
    tempNumPtr++;
  }

  lineNum = strtol (numPtr, NULL, 10);
  if(errno == ERANGE)
    return;

  if(isMultiLine)
    *isMultiLine = TRUE;

  if(whichLine)
    *whichLine = lineNum;

  if(singleLineItemName)
    StringCopyMax(singleLineItemName, (char *)itemName, lineTagPtr - itemName + 1);
}
