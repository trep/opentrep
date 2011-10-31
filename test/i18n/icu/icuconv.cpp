
// STL
#include <cstdio>
#include <ctype.h>            /* for isspace, etc.    */
#include <cassert>
#include <cstring>
#include <cstdlib>  /* malloc */

#define DEBUG_TMI 0  /* define to 1 to enable Too Much Information */

#include "unicode/utypes.h"   /* Basic ICU data types */
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/ustring.h"  /* some more string fcns*/
#include "unicode/uchar.h"    /* char names           */
#include "unicode/uloc.h"
#include "unicode/unistr.h"

/* Some utility functions */

static const UChar kNone[] = { 0x0000 };

#define U_ASSERT(x)  { if(U_FAILURE(x)) {fflush(stdout);fflush(stderr); fprintf(stderr, #x " == %s\n", u_errorName(x)); assert(U_SUCCESS(x)); }}

/* Print a UChar if possible, in seven characters. */
void prettyPrintUChar(UChar c)
{
  if(  (c <= 0x007F) &&
       (isgraph(c))  ) {
    printf(" '%c'   ", (char)(0x00FF&c));
  } else if ( c > 0x007F ) {
    char buf[1000];
    UErrorCode status = U_ZERO_ERROR;
    int32_t o;
    
    o = u_charName(c, U_UNICODE_CHAR_NAME, buf, 1000, &status);
    if(U_SUCCESS(status) && (o>0) ) {
      buf[80] = 0;
      printf("%7s", buf);
    } else {
      o = u_charName(c, U_UNICODE_10_CHAR_NAME, buf, 1000, &status);
      if(U_SUCCESS(status) && (o>0)) {
        buf[5] = 0;
        printf("~%6s", buf);
      }
      else {
        printf(" ??????");
      }
    }
  } else {
    switch((char)(c & 0x007F)) {
    case ' ':
      printf(" ' '   ");
      break;
    case '\t':
      printf(" \\t    ");
      break;
    case '\n':
      printf(" \\n    ");
      break;
    default:
      printf("  _    ");
      break;
    }
  }
}


void printUChars(const char  *name = "?", 
                 const UChar *uch  = kNone,
                 int32_t     len   = -1 )
{
  int32_t i;

  if( (len == -1) && (uch) ) {
    len = u_strlen(uch);
  }

  printf("%5s: ", name);
  for( i = 0; i <len; i++) {
    printf("%-6d ", i);
  }
  printf("\n");

  printf("%5s: ", "uni");
  for( i = 0; i <len; i++) {
    printf("\\u%04X ", (int)uch[i]);
  }
  printf("\n");

  printf("%5s:", "ch");
  for( i = 0; i <len; i++) {
    prettyPrintUChar(uch[i]);
  }
  printf("\n");
}

void printBytes(const char  *name = "?", 
                 const char *uch  = "",
                 int32_t     len   = -1 )
{
  int32_t i;

  if( (len == -1) && (uch) ) {
    len = strlen(uch);
  }

  printf("%5s: ", name);
  for( i = 0; i <len; i++) {
    printf("%-4d ", i);
  }
  printf("\n");

  printf("%5s: ", "uni");
  for( i = 0; i <len; i++) {
    printf("\\x%02X ", 0x00FF & (int)uch[i]);
  }
  printf("\n");

  printf("%5s:", "ch");
  for( i = 0; i <len; i++) {
    if(isgraph(0x00FF & (int)uch[i])) {
      printf(" '%c' ", (char)uch[i]);
    } else {
      printf("     ");
    }
  }
  printf("\n");
}

void printUChar(UChar32 ch32)
{
    if(ch32 > 0xFFFF) {
      printf("ch: U+%06X\n", ch32);
    }
    else {
      UChar ch = (UChar)ch32;
      printUChars("C", &ch, 1);
    }
}

/*******************************************************************
  Very simple C sample to convert the word 'Moscow' in Russian in Unicode,
  followed by an exclamation mark (!) into the KOI8-R Russian code page.

  This example first creates a UChar String out of the Unicode chars.

  targetSize must be set to the amount of space available in the target
  buffer. After fromUChars is called, 
  len will contain the number of bytes in target[] which were
  used in the resulting codepage.  In this case, there is a 1:1 mapping
  between the input and output characters. The exclamation mark has the
  same value in both KOI8-R and Unicode.

  src: 0      1      2      3      4      5      6     
  uni: \u041C \u043E \u0441 \u043A \u0432 \u0430 \u0021 
   ch: CYRILL CYRILL CYRILL CYRILL CYRILL CYRILL   '!'  

 targ:  0    1    2    3    4    5    6  
  uni: \xED \xCF \xD3 \xCB \xD7 \xC1 \x21 
   ch:                                '!' 


Converting FROM unicode 
  to koi8-r.
  You must call ucnv_close to clean up the memory used by the
  converter.

  'len' returns the number of OUTPUT bytes resulting from the 
  conversion.
 */

UErrorCode convsample_02()
{
  printf("\n\n==============================================\n"
         "Sample 02: C: simple Unicode -> koi8-r conversion\n");


  // **************************** START SAMPLE *******************
  // "cat<cat>OK"
  UChar source[] = { 0x041C, 0x043E, 0x0441, 0x043A, 0x0432,
                     0x0430, 0x0021, 0x0000 };
  char target[100];
  UErrorCode status = U_ZERO_ERROR;
  UConverter *conv;
  int32_t     len;

  // set up the converter
  conv = ucnv_open("koi8-r", &status);
  assert(U_SUCCESS(status));

  // convert to koi8-r
  len = ucnv_fromUChars(conv, target, 100, source, -1, &status);
  assert(U_SUCCESS(status));

  // close the converter
  ucnv_close(conv);

  // ***************************** END SAMPLE ********************
  
  // Print it out
  printUChars("src", source);
  printf("\n");
  printBytes("targ", target, len);

  return U_ZERO_ERROR;
}


UErrorCode convsample_03()
{
  printf("\n\n==============================================\n"
         "Sample 03: C: print out all converters\n");

  int32_t count;
  int32_t i;

  // **************************** START SAMPLE *******************
  count = ucnv_countAvailable();
  printf("Available converters: %d\n", count);
  
  for(i=0;i<count;i++) 
  {
    printf("%s ", ucnv_getAvailableName(i));
  }

  // ***************************** END SAMPLE ********************
  
  printf("\n");

  return U_ZERO_ERROR;
}



#define BUFFERSIZE 17 /* make it interesting :) */

/*
  Converting from a codepage to Unicode in bulk..
  What is the best way to determine the buffer size?

     The 'buffersize' is in bytes of input.
    For a given converter, divinding this by the minimum char size
    give you the maximum number of Unicode characters that could be
    expected for a given number of input bytes.
     see: ucnv_getMinCharSize()

     For example, a single byte codepage like 'Latin-3' has a 
    minimum char size of 1. (It takes at least 1 byte to represent
    each Unicode char.) So the unicode buffer has the same number of
    UChars as the input buffer has bytes.

     In a strictly double byte codepage such as cp1362 (Windows
    Korean), the minimum char size is 2. So, only half as many Unicode
    chars as bytes are needed.

     This work to calculate the buffer size is an optimization. Any
    size of input and output buffer can be used, as long as the
    program handles the following cases: If the input buffer is empty,
    the source pointer will be equal to sourceLimit.  If the output
    buffer has overflowed, U_BUFFER_OVERFLOW_ERROR will be returned. 
 */

UErrorCode convsample_05()
{
  printf("\n\n==============================================\n"
         "Sample 05: C: count the number of letters in a UTF-8 document\n");

  FILE *f;
  int32_t count;
  char inBuf[BUFFERSIZE];
  const char *source;
  const char *sourceLimit;
  UChar *uBuf;
  UChar *target;
  UChar *targetLimit;
  UChar *p;
  int32_t uBufSize = 0;
  UConverter *conv;
  UErrorCode status = U_ZERO_ERROR;
  uint32_t letters=0, total=0;

  f = fopen("ref/ref_text_ru.txt", "r");
  if(!f)
  {
    fprintf(stderr, "Couldn't open file 'ref/ref_text_ru.txt' (UTF-8 data file).\n");
    return U_FILE_ACCESS_ERROR;
  }

  // **************************** START SAMPLE *******************
  conv = ucnv_open("utf-8", &status);
  assert(U_SUCCESS(status));

  uBufSize = (BUFFERSIZE/ucnv_getMinCharSize(conv));
  printf("input bytes %d / min chars %d = %d UChars\n",
         BUFFERSIZE, ucnv_getMinCharSize(conv), uBufSize);
  uBuf = (UChar*)malloc(uBufSize * sizeof(UChar));
  assert(uBuf!=NULL);

  // grab another buffer's worth
  while((!feof(f)) && 
        ((count=fread(inBuf, 1, BUFFERSIZE , f)) > 0) )
  {
    // Convert bytes to unicode
    source = inBuf;
    sourceLimit = inBuf + count;
    
    do
    {
        target = uBuf;
        targetLimit = uBuf + uBufSize;
        
        ucnv_toUnicode(conv, &target, targetLimit, 
                       &source, sourceLimit, NULL,
                       feof(f)?TRUE:FALSE,         /* pass 'flush' when eof */
                                   /* is true (when no more data will come) */
                       &status);
      
        if(status == U_BUFFER_OVERFLOW_ERROR)
        {
          // simply ran out of space - we'll reset the target ptr the next
          // time through the loop.
          status = U_ZERO_ERROR;
        }
        else
        {
          //  Check other errors here.
          assert(U_SUCCESS(status));
          // Break out of the loop (by force)
        }

        // Process the Unicode
        // Todo: handle UTF-16/surrogates

        for(p = uBuf; p<target; p++)
        {
          if(u_isalpha(*p))
            letters++;
          total++;
        }
    } while (source < sourceLimit); // while simply out of space
  }

  printf("%d letters out of %d total UChars.\n", letters, total);
  
  // ***************************** END SAMPLE ********************
  ucnv_close(conv);

  printf("\n");

  return U_ZERO_ERROR;
}
#undef BUFFERSIZE

#define BUFFERSIZE 1024
typedef struct
{
  UChar32  codepoint;
  uint32_t frequency;
} CharFreqInfo;

UErrorCode convsample_06()
{
  printf("\n\n==============================================\n"
         "Sample 06: C: frequency distribution of letters in a UTF-8 document\n");

  FILE *f;
  int32_t count;
  char inBuf[BUFFERSIZE];
  const char *source;
  const char *sourceLimit;
  UChar *uBuf;
  int32_t uBufSize = 0;
  UConverter *conv;
  UErrorCode status = U_ZERO_ERROR;
  uint32_t letters=0, total=0;

  CharFreqInfo   *info;
  UChar32   charCount = 0x10000;  /* increase this if you want to handle non bmp.. todo: automatically bump it.. */
  UChar32   p;

  uint32_t ie = 0;
  uint32_t gh = 0;
  UChar32 l = 0;

  f = fopen("ref/ref_text_ru.txt", "r");
  if(!f)
  {
    fprintf(stderr, "Couldn't open file 'ref/ref_text_ru.txt' (UTF-8 data file).\n");
    return U_FILE_ACCESS_ERROR;
  }

  info = (CharFreqInfo*)malloc(sizeof(CharFreqInfo) * charCount);
  if(!info)
  {
    fprintf(stderr, " Couldn't allocate %d bytes for freq counter\n", sizeof(CharFreqInfo)*charCount);
  }

  /* reset frequencies */
  for(p=0;p<charCount;p++)
  {
    info[p].codepoint = p;
    info[p].frequency = 0;
  }

  // **************************** START SAMPLE *******************
  conv = ucnv_open("utf-8", &status);
  assert(U_SUCCESS(status));

  uBufSize = (BUFFERSIZE/ucnv_getMinCharSize(conv));
  printf("input bytes %d / min chars %d = %d UChars\n",
         BUFFERSIZE, ucnv_getMinCharSize(conv), uBufSize);
  uBuf = (UChar*)malloc(uBufSize * sizeof(UChar));
  assert(uBuf!=NULL);

  // grab another buffer's worth
  while((!feof(f)) && 
        ((count=fread(inBuf, 1, BUFFERSIZE , f)) > 0) )
  {
    // Convert bytes to unicode
    source = inBuf;
    sourceLimit = inBuf + count;
    
    while(source < sourceLimit)
    {
      p = ucnv_getNextUChar(conv, &source, sourceLimit, &status);
      if(U_FAILURE(status))
      {
        fprintf(stderr, "%s @ %d\n", u_errorName(status), total);
        status = U_ZERO_ERROR;
        continue;
      }
      U_ASSERT(status);
      total++;

      if(u_isalpha(p))
        letters++;

      if((u_tolower(l) == 'i') && (u_tolower(p) == 'e'))
        ie++;

      if((u_tolower(l) == 'g') && (u_tolower(p) == 0x0127))
        gh++;

      if(p>charCount)
      {
        fprintf(stderr, "U+%06X: oh.., we only handle BMP characters so far.. redesign!\n", p);
        return U_UNSUPPORTED_ERROR;
      }
      info[p].frequency++;
      l = p;
    }
  }

  fclose(f);
  ucnv_close(conv);

  printf("%d letters out of %d total UChars.\n", letters, total);
  printf("%d ie digraphs, %d gh digraphs.\n", ie, gh);

  // now, we could sort it..

  //  qsort(info, charCount, sizeof(info[0]), charfreq_compare);

  for(p=0;p<charCount;p++)
  {
    if(info[p].frequency)
    {
      printf("% 5d U+%06X ", info[p].frequency, p);
      if(p <= 0xFFFF)
      {
        prettyPrintUChar((UChar)p);
      }
      printf("\n");
    }
  }
  free(info);
  // ***************************** END SAMPLE ********************

  printf("\n");

  return U_ZERO_ERROR;
}
#undef BUFFERSIZE

#define BUFFERSIZE 219


/* main */

int main() {

  printf("Default Converter=%s\n", ucnv_getDefaultName() );
  
  convsample_02();  // C  , u->koi8r, conv
  convsample_03();  // C,   iterate

  convsample_05();  // C,  utf8->u, getNextUChar
  convsample_06(); // C freq counter thingy

  printf("End of converter samples.\n");
  
  fflush(stdout);
  fflush(stderr);
  
  return 0;
}
