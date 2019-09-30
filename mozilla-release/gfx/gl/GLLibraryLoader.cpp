/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GLLibraryLoader.h"

#include <regex>

#include "nsDebug.h"

#ifdef WIN32
#  include <windows.h>
#endif

namespace mozilla {
namespace gl {

<<<<<<< HEAD
bool GLLibraryLoader::OpenLibrary(const char* library) {
  PRLibSpec lspec;
  lspec.type = PR_LibSpec_Pathname;
  lspec.value.pathname = library;

  mLibrary = PR_LoadLibraryWithFlags(lspec, PR_LD_LAZY | PR_LD_LOCAL);
  if (!mLibrary) return false;

  return true;
||||||| merged common ancestors
bool
GLLibraryLoader::OpenLibrary(const char* library)
{
    PRLibSpec lspec;
    lspec.type = PR_LibSpec_Pathname;
    lspec.value.pathname = library;

    mLibrary = PR_LoadLibraryWithFlags(lspec, PR_LD_LAZY | PR_LD_LOCAL);
    if (!mLibrary)
        return false;

    return true;
=======
void ClearSymbols(const SymLoadStruct* const firstStruct) {
  for (auto itr = firstStruct; itr->symPointer; ++itr) {
    *itr->symPointer = nullptr;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLLibraryLoader::LoadSymbols(const SymLoadStruct* firstStruct,
                                  bool tryplatform, const char* prefix,
                                  bool warnOnFailure) {
  return LoadSymbols(mLibrary, firstStruct, tryplatform ? mLookupFunc : nullptr,
                     prefix, warnOnFailure);
}

PRFuncPtr GLLibraryLoader::LookupSymbol(const char* sym) {
  return LookupSymbol(mLibrary, sym, mLookupFunc);
}
||||||| merged common ancestors
bool
GLLibraryLoader::LoadSymbols(const SymLoadStruct* firstStruct,
                             bool tryplatform,
                             const char* prefix,
                             bool warnOnFailure)
{
    return LoadSymbols(mLibrary,
                       firstStruct,
                       tryplatform ? mLookupFunc : nullptr,
                       prefix,
                       warnOnFailure);
}

PRFuncPtr
GLLibraryLoader::LookupSymbol(const char* sym)
{
    return LookupSymbol(mLibrary, sym, mLookupFunc);
}
=======
bool SymbolLoader::LoadSymbols(const SymLoadStruct* const firstStruct,
                               const bool warnOnFailures) const {
  bool ok = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
PRFuncPtr GLLibraryLoader::LookupSymbol(PRLibrary* lib, const char* sym,
                                        PlatformLookupFunction lookupFunction) {
  PRFuncPtr res = 0;
||||||| merged common ancestors
PRFuncPtr
GLLibraryLoader::LookupSymbol(PRLibrary* lib,
                              const char* sym,
                              PlatformLookupFunction lookupFunction)
{
    PRFuncPtr res = 0;
=======
  for (auto itr = firstStruct; itr->symPointer; ++itr) {
    *itr->symPointer = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // try finding it in the library directly, if we have one
  if (lib) {
    res = PR_FindFunctionSymbol(lib, sym);
  }
||||||| merged common ancestors
    // try finding it in the library directly, if we have one
    if (lib) {
        res = PR_FindFunctionSymbol(lib, sym);
    }
=======
    for (const auto& s : itr->symNames) {
      if (!s) break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // then try looking it up via the lookup symbol
  if (!res && lookupFunction) {
    res = lookupFunction(sym);
  }
||||||| merged common ancestors
    // then try looking it up via the lookup symbol
    if (!res && lookupFunction) {
        res = lookupFunction(sym);
    }
=======
      const auto p = GetProcAddress(s);
      if (p) {
        *itr->symPointer = p;
        break;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // finally just try finding it in the process
  if (!res) {
    PRLibrary* leakedLibRef;
    res = PR_FindFunctionSymbolAndLibrary(sym, &leakedLibRef);
  }
||||||| merged common ancestors
    // finally just try finding it in the process
    if (!res) {
        PRLibrary* leakedLibRef;
        res = PR_FindFunctionSymbolAndLibrary(sym, &leakedLibRef);
    }
=======
    if (!*itr->symPointer) {
      if (warnOnFailures) {
        printf_stderr("Can't find symbol '%s'.\n", itr->symNames[0]);
      }
      ok = false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return res;
||||||| merged common ancestors
    return res;
=======
  return ok;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GLLibraryLoader::LoadSymbols(PRLibrary* lib,
                                  const SymLoadStruct* firstStruct,
                                  PlatformLookupFunction lookupFunction,
                                  const char* prefix, bool warnOnFailure) {
  char sbuf[MAX_SYMBOL_LENGTH * 2];
  int failCount = 0;

  const SymLoadStruct* ss = firstStruct;
  while (ss->symPointer) {
    *ss->symPointer = 0;

    for (int i = 0; i < MAX_SYMBOL_NAMES; i++) {
      if (ss->symNames[i] == nullptr) break;

      const char* s = ss->symNames[i];
      if (prefix && *prefix != 0) {
        strcpy(sbuf, prefix);
        strcat(sbuf, ss->symNames[i]);
        s = sbuf;
      }

      PRFuncPtr p = LookupSymbol(lib, s, lookupFunction);
      if (p) {
        *ss->symPointer = p;
        break;
      }
    }
||||||| merged common ancestors
bool
GLLibraryLoader::LoadSymbols(PRLibrary* lib,
                             const SymLoadStruct* firstStruct,
                             PlatformLookupFunction lookupFunction,
                             const char* prefix,
                             bool warnOnFailure)
{
    char sbuf[MAX_SYMBOL_LENGTH * 2];
    int failCount = 0;

    const SymLoadStruct* ss = firstStruct;
    while (ss->symPointer) {
        *ss->symPointer = 0;

        for (int i = 0; i < MAX_SYMBOL_NAMES; i++) {
            if (ss->symNames[i] == nullptr)
                break;

            const char* s = ss->symNames[i];
            if (prefix && *prefix != 0) {
                strcpy(sbuf, prefix);
                strcat(sbuf, ss->symNames[i]);
                s = sbuf;
            }

            PRFuncPtr p = LookupSymbol(lib, s, lookupFunction);
            if (p) {
                *ss->symPointer = p;
                break;
            }
        }

        if (*ss->symPointer == 0) {
            if (warnOnFailure) {
                printf_stderr("Can't find symbol '%s'.\n", ss->symNames[0]);
            }

            failCount++;
        }

        ss++;
    }
=======
// -
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (*ss->symPointer == 0) {
      if (warnOnFailure) {
        printf_stderr("Can't find symbol '%s'.\n", ss->symNames[0]);
      }
||||||| merged common ancestors
    return failCount == 0 ? true : false;
}
=======
PRFuncPtr SymbolLoader::GetProcAddress(const char* const name) const {
#ifdef DEBUG
  static const std::regex kRESymbol("[a-z].*");
  if (!std::regex_match(name, kRESymbol)) {
    gfxCriticalError() << "Bad symbol name : " << name;
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
      failCount++;
    }

    ss++;
  }

  return failCount == 0 ? true : false;
}

/*static*/ void GLLibraryLoader::ClearSymbols(
    const SymLoadStruct* const firstStruct) {
  for (auto cur = firstStruct; cur->symPointer; ++cur) {
    *cur->symPointer = nullptr;
  }
||||||| merged common ancestors
/*static*/ void
GLLibraryLoader::ClearSymbols(const SymLoadStruct* const firstStruct)
{
    for (auto cur = firstStruct; cur->symPointer; ++cur) {
        *cur->symPointer = nullptr;
    }
=======
  PRFuncPtr ret = nullptr;
  if (!ret && mLib) {
    ret = PR_FindFunctionSymbol(mLib, name);
  }
  if (!ret && mPfn) {
    ret = mPfn(name);
  }
  return ret;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
} /* namespace gl */
} /* namespace mozilla */
||||||| merged common ancestors
} /* namespace gl */
} /* namespace mozilla */

=======
}  // namespace gl
}  // namespace mozilla
>>>>>>> upstream-releases
