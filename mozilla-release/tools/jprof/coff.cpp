/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "leaky.h"

#ifdef USE_COFF

<<<<<<< HEAD
#define LANGUAGE_C
#include <sym.h>
#include <cmplrs/stsupport.h>
#include <symconst.h>
#include <filehdr.h>
#include <ldfcn.h>
#include <string.h>
#include <stdlib.h>
||||||| merged common ancestors
#define	LANGUAGE_C
#include <sym.h>
#include <cmplrs/stsupport.h>
#include <symconst.h>
#include <filehdr.h>
#include <ldfcn.h>
#include <string.h>
#include <stdlib.h>
=======
#  define LANGUAGE_C
#  include <sym.h>
#  include <cmplrs/stsupport.h>
#  include <symconst.h>
#  include <filehdr.h>
#  include <ldfcn.h>
#  include <string.h>
#  include <stdlib.h>
>>>>>>> upstream-releases

#  ifdef IRIX4
extern "C" {
<<<<<<< HEAD
extern char *demangle(char const *in);
||||||| merged common ancestors
    extern char *demangle(char const* in);
=======
extern char* demangle(char const* in);
>>>>>>> upstream-releases
};
#  else
#    include <dem.h>
#  endif

<<<<<<< HEAD
static char *Demangle(char *rawName) {
#ifdef IRIX4
  return strdup(demangle(rawName));
#else
  char namebuf[4000];
  demangle(rawName, namebuf);
  return strdup(namebuf);
#endif
||||||| merged common ancestors
static char *Demangle(char *rawName)
{
#ifdef IRIX4
    return strdup(demangle(rawName));
#else
    char namebuf[4000];
    demangle(rawName, namebuf);
    return strdup(namebuf);
#endif
=======
static char* Demangle(char* rawName) {
#  ifdef IRIX4
  return strdup(demangle(rawName));
#  else
  char namebuf[4000];
  demangle(rawName, namebuf);
  return strdup(namebuf);
#  endif
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void leaky::readSymbols(const char *fileName) {
  LDFILE *ldptr;
||||||| merged common ancestors
void leaky::readSymbols(const char *fileName)
{
    LDFILE *ldptr;
=======
void leaky::readSymbols(const char* fileName) {
  LDFILE* ldptr;
>>>>>>> upstream-releases

  ldptr = ldopen(fileName, nullptr);
  if (!ldptr) {
    fprintf(stderr, "%s: unable to open \"%s\"\n", applicationName, fileName);
    exit(-1);
  }
  if (PSYMTAB(ldptr) == 0) {
    fprintf(stderr, "%s: \"%s\": has no symbol table\n", applicationName,
            fileName);
    exit(-1);
  }

  long isymMax = SYMHEADER(ldptr).isymMax;
  long iextMax = SYMHEADER(ldptr).iextMax;
  long iMax = isymMax + iextMax;

<<<<<<< HEAD
  long alloced = 10000;
  Symbol *syms = (Symbol *)malloc(sizeof(Symbol) * 10000);
  Symbol *sp = syms;
  Symbol *last = syms + alloced;
  SYMR symr;
||||||| merged common ancestors
    long alloced = 10000;
    Symbol* syms = (Symbol*) malloc(sizeof(Symbol) * 10000);
    Symbol* sp = syms;
    Symbol* last = syms + alloced;
    SYMR symr;
=======
  long alloced = 10000;
  Symbol* syms = (Symbol*)malloc(sizeof(Symbol) * 10000);
  Symbol* sp = syms;
  Symbol* last = syms + alloced;
  SYMR symr;
>>>>>>> upstream-releases

  for (long isym = 0; isym < iMax; isym++) {
    if (ldtbread(ldptr, isym, &symr) != SUCCESS) {
      fprintf(stderr, "%s: can't read symbol #%d\n", applicationName, isym);
      exit(-1);
    }
<<<<<<< HEAD
    if (isym < isymMax) {
      if ((symr.st == stStaticProc) ||
          ((symr.st == stProc) &&
           ((symr.sc == scText) || (symr.sc == scAbs))) ||
          ((symr.st == stBlock) && (symr.sc == scText))) {
        // Text symbol. Set name field to point to the symbol name
        sp->name = Demangle(ldgetname(ldptr, &symr));
        sp->address = symr.value;
        sp++;
        if (sp >= last) {
          long n = alloced + 10000;
          syms = (Symbol *)realloc(syms, (size_t)(sizeof(Symbol) * n));
          last = syms + n;
          sp = syms + alloced;
          alloced = n;
        }
      }
||||||| merged common ancestors

    int interesting = sp - syms;
    if (!quiet) {
	printf("Total of %d symbols\n", interesting);
=======
    if (isym < isymMax) {
      if ((symr.st == stStaticProc) ||
          ((symr.st == stProc) &&
           ((symr.sc == scText) || (symr.sc == scAbs))) ||
          ((symr.st == stBlock) && (symr.sc == scText))) {
        // Text symbol. Set name field to point to the symbol name
        sp->name = Demangle(ldgetname(ldptr, &symr));
        sp->address = symr.value;
        sp++;
        if (sp >= last) {
          long n = alloced + 10000;
          syms = (Symbol*)realloc(syms, (size_t)(sizeof(Symbol) * n));
          last = syms + n;
          sp = syms + alloced;
          alloced = n;
        }
      }
>>>>>>> upstream-releases
    }
  }

  int interesting = sp - syms;
  if (!quiet) {
    printf("Total of %d symbols\n", interesting);
  }
  usefulSymbols = interesting;
  externalSymbols = syms;
}

#endif /* USE_COFF */
