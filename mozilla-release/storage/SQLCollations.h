/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_storage_SQLCollations_h
#define mozilla_storage_SQLCollations_h

#include "mozStorageService.h"
#include "nscore.h"
#include "nsString.h"

#include "sqlite3.h"

namespace mozilla {
namespace storage {

/**
 * Registers the collating sequences declared here with the specified
 * database and Service.
 *
 * @param  aDB
 *         The database we'll be registering the collations with.
 * @param  aService
 *         The Service that owns the nsICollation used by our collations.
 * @return the SQLite status code indicating success or failure.
 */
int registerCollations(sqlite3* aDB, Service* aService);

////////////////////////////////////////////////////////////////////////////////
//// Predefined Functions

/**
 * Custom UTF-8 collating sequence that respects the application's locale.
 * Comparison is case- and accent-insensitive.  This is called by SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @param  aLen2
 *         The number of bytes in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollation8(void *aService, int aLen1, const void *aStr1, int aLen2,
                     const void *aStr2);
||||||| merged common ancestors
int localeCollation8(void *aService,
                                 int aLen1,
                                 const void *aStr1,
                                 int aLen2,
                                 const void *aStr2);
=======
int localeCollation8(void* aService, int aLen1, const void* aStr1, int aLen2,
                     const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-8 collating sequence that respects the application's locale.
 * Comparison is case-sensitive and accent-insensitive.  This is called by
 * SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @param  aLen2
 *         The number of bytes in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationCaseSensitive8(void *aService, int aLen1, const void *aStr1,
                                  int aLen2, const void *aStr2);
||||||| merged common ancestors
int localeCollationCaseSensitive8(void *aService,
                                              int aLen1,
                                              const void *aStr1,
                                              int aLen2,
                                              const void *aStr2);
=======
int localeCollationCaseSensitive8(void* aService, int aLen1, const void* aStr1,
                                  int aLen2, const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-8 collating sequence that respects the application's locale.
 * Comparison is case-insensitive and accent-sensitive.  This is called by
 * SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @param  aLen2
 *         The number of bytes in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationAccentSensitive8(void *aService, int aLen1,
                                    const void *aStr1, int aLen2,
                                    const void *aStr2);
||||||| merged common ancestors
int localeCollationAccentSensitive8(void *aService,
                                                int aLen1,
                                                const void *aStr1,
                                                int aLen2,
                                                const void *aStr2);
=======
int localeCollationAccentSensitive8(void* aService, int aLen1,
                                    const void* aStr1, int aLen2,
                                    const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-8 collating sequence that respects the application's locale.
 * Comparison is case- and accent-sensitive.  This is called by SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @param  aLen2
 *         The number of bytes in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationCaseAccentSensitive8(void *aService, int aLen1,
                                        const void *aStr1, int aLen2,
                                        const void *aStr2);
||||||| merged common ancestors
int localeCollationCaseAccentSensitive8(void *aService,
                                                    int aLen1,
                                                    const void *aStr1,
                                                    int aLen2,
                                                    const void *aStr2);
=======
int localeCollationCaseAccentSensitive8(void* aService, int aLen1,
                                        const void* aStr1, int aLen2,
                                        const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-16 collating sequence that respects the application's locale.
 * Comparison is case- and accent-insensitive.  This is called by SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes (not characters) in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @param  aLen2
 *         The number of bytes (not characters) in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollation16(void *aService, int aLen1, const void *aStr1, int aLen2,
                      const void *aStr2);
||||||| merged common ancestors
int localeCollation16(void *aService,
                                  int aLen1,
                                  const void *aStr1,
                                  int aLen2,
                                  const void *aStr2);
=======
int localeCollation16(void* aService, int aLen1, const void* aStr1, int aLen2,
                      const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-16 collating sequence that respects the application's locale.
 * Comparison is case-sensitive and accent-insensitive.  This is called by
 * SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes (not characters) in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @param  aLen2
 *         The number of bytes (not characters) in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationCaseSensitive16(void *aService, int aLen1, const void *aStr1,
                                   int aLen2, const void *aStr2);
||||||| merged common ancestors
int localeCollationCaseSensitive16(void *aService,
                                               int aLen1,
                                               const void *aStr1,
                                               int aLen2,
                                               const void *aStr2);
=======
int localeCollationCaseSensitive16(void* aService, int aLen1, const void* aStr1,
                                   int aLen2, const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-16 collating sequence that respects the application's locale.
 * Comparison is case-insensitive and accent-sensitive.  This is called by
 * SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes (not characters) in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @param  aLen2
 *         The number of bytes (not characters) in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationAccentSensitive16(void *aService, int aLen1,
                                     const void *aStr1, int aLen2,
                                     const void *aStr2);
||||||| merged common ancestors
int localeCollationAccentSensitive16(void *aService,
                                                 int aLen1,
                                                 const void *aStr1,
                                                 int aLen2,
                                                 const void *aStr2);
=======
int localeCollationAccentSensitive16(void* aService, int aLen1,
                                     const void* aStr1, int aLen2,
                                     const void* aStr2);
>>>>>>> upstream-releases

/**
 * Custom UTF-16 collating sequence that respects the application's locale.
 * Comparison is case- and accent-sensitive.  This is called by SQLite.
 *
 * @param  aService
 *         The Service that owns the nsICollation used by this collation.
 * @param  aLen1
 *         The number of bytes (not characters) in aStr1.
 * @param  aStr1
 *         The string to be compared against aStr2.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @param  aLen2
 *         The number of bytes (not characters) in aStr2.
 * @param  aStr2
 *         The string to be compared against aStr1.  It will be passed in by
 *         SQLite as a non-null-terminated char16_t* buffer.
 * @return aStr1 - aStr2.  That is, if aStr1 < aStr2, returns a negative number.
 *         If aStr1 > aStr2, returns a positive number.  If aStr1 == aStr2,
 *         returns 0.
 */
<<<<<<< HEAD
int localeCollationCaseAccentSensitive16(void *aService, int aLen1,
                                         const void *aStr1, int aLen2,
                                         const void *aStr2);
||||||| merged common ancestors
int localeCollationCaseAccentSensitive16(void *aService,
                                                     int aLen1,
                                                     const void *aStr1,
                                                     int aLen2,
                                                     const void *aStr2);
=======
int localeCollationCaseAccentSensitive16(void* aService, int aLen1,
                                         const void* aStr1, int aLen2,
                                         const void* aStr2);
>>>>>>> upstream-releases

}  // namespace storage
}  // namespace mozilla

#endif  // mozilla_storage_SQLCollations_h
