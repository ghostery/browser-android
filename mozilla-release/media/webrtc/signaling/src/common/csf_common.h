/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _CSF_COMMON_E58E5677_950A_424c_B6C2_CA180092E6A2_H
#define _CSF_COMMON_E58E5677_950A_424c_B6C2_CA180092E6A2_H

#include <assert.h>
#include <memory>
#include <vector>
#include <stdlib.h>

/*

This header file defines:

csf_countof
csf_sprintf
csf_vsprintf

*/

/*
  General security tip: Ensure that "format" is never a user-defined string.
  Format should ALWAYS be something that's built into your code, not user
  supplied. For example: never write:

  csf_sprintf(buffer, csf_countof(buffer), pUserSuppliedString);

  Instead write:

  csf_sprintf(buffer, csf_countof(buffer), "%s", pUserSuppliedString);

*/

#ifdef WIN32
<<<<<<< HEAD
#if !defined(_countof)
#if !defined(__cplusplus)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
||||||| merged common ancestors
    #if !defined(_countof)
        #if !defined(__cplusplus)
            #define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
        #else
            extern "C++"
            {
            template <typename _CountofType, size_t _SizeOfArray>
            char (*_csf_countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
            #define _countof(_Array) sizeof(*_csf_countof_helper(_Array))
            }
        #endif
    #endif
=======
#  if !defined(_countof)
#    if !defined(__cplusplus)
#      define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#    else
extern "C++" {
template <typename _CountofType, size_t _SizeOfArray>
char (*_csf_countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#      define _countof(_Array) sizeof(*_csf_countof_helper(_Array))
}
#    endif
#  endif
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
extern "C++" {
template <typename _CountofType, size_t _SizeOfArray>
char (*_csf_countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#define _countof(_Array) sizeof(*_csf_countof_helper(_Array))
}
||||||| merged common ancestors
    #define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
=======
#  define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
#endif
#else
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif
// csf_countof
||||||| merged common ancestors
//csf_countof
=======
// csf_countof
>>>>>>> upstream-releases

#define csf_countof(anArray) _countof(anArray)

// csf_sprintf

#ifdef _WIN32
<<<<<<< HEAD
// Unlike snprintf, sprintf_s guarantees that the buffer will be null-terminated
// (unless the buffer size is zero).
#define csf_sprintf(/* char* */ buffer,                                   \
                    /* size_t */ sizeOfBufferInCharsInclNullTerm,         \
                    /* const char * */ format, ...)                       \
  _snprintf_s(buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, \
              __VA_ARGS__)
||||||| merged common ancestors
  //Unlike snprintf, sprintf_s guarantees that the buffer will be null-terminated (unless the buffer size is zero).
  #define csf_sprintf(/* char* */ buffer, /* size_t */ sizeOfBufferInCharsInclNullTerm, /* const char * */ format, ...)\
    _snprintf_s (buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, __VA_ARGS__)
=======
// Unlike snprintf, sprintf_s guarantees that the buffer will be null-terminated
// (unless the buffer size is zero).
#  define csf_sprintf(/* char* */ buffer,                                   \
                      /* size_t */ sizeOfBufferInCharsInclNullTerm,         \
                      /* const char * */ format, ...)                       \
    _snprintf_s(buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, \
                __VA_ARGS__)
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define csf_sprintf(/* char */ buffer,                                    \
                    /* size_t */ sizeOfBufferInCharsInclNullTerm,         \
                    /* const char * */ format, ...)                       \
  snprintf(buffer, sizeOfBufferInCharsInclNullTerm, format, __VA_ARGS__); \
  buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
||||||| merged common ancestors
  #define csf_sprintf(/* char */ buffer, /* size_t */ sizeOfBufferInCharsInclNullTerm, /* const char * */ format, ...)\
    snprintf (buffer, sizeOfBufferInCharsInclNullTerm, format, __VA_ARGS__);\
    buffer[sizeOfBufferInCharsInclNullTerm-1] = '\0'
=======
#  define csf_sprintf(/* char */ buffer,                                    \
                      /* size_t */ sizeOfBufferInCharsInclNullTerm,         \
                      /* const char * */ format, ...)                       \
    snprintf(buffer, sizeOfBufferInCharsInclNullTerm, format, __VA_ARGS__); \
    buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
>>>>>>> upstream-releases
#endif

// csf_vsprintf

#ifdef _WIN32
<<<<<<< HEAD
#define csf_vsprintf(/* char* */ buffer,                                  \
                     /* size_t */ sizeOfBufferInCharsInclNullTerm,        \
                     /* const char * */ format, /* va_list */ vaList)     \
  vsnprintf_s(buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, \
              vaList);                                                    \
  buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
||||||| merged common ancestors
  #define csf_vsprintf(/* char* */ buffer, /* size_t */ sizeOfBufferInCharsInclNullTerm, /* const char * */ format, /* va_list */ vaList)\
    vsnprintf_s (buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, vaList);\
    buffer[sizeOfBufferInCharsInclNullTerm-1] = '\0'
=======
#  define csf_vsprintf(/* char* */ buffer,                                  \
                       /* size_t */ sizeOfBufferInCharsInclNullTerm,        \
                       /* const char * */ format, /* va_list */ vaList)     \
    vsnprintf_s(buffer, sizeOfBufferInCharsInclNullTerm, _TRUNCATE, format, \
                vaList);                                                    \
    buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define csf_vsprintf(/* char */ buffer,                               \
                     /* size_t */ sizeOfBufferInCharsInclNullTerm,    \
                     /* const char * */ format, /* va_list */ vaList) \
  vsprintf(buffer, format, vaList);                                   \
  buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
||||||| merged common ancestors
  #define csf_vsprintf(/* char */ buffer, /* size_t */ sizeOfBufferInCharsInclNullTerm, /* const char * */ format, /* va_list */ vaList)\
    vsprintf (buffer, format, vaList);\
    buffer[sizeOfBufferInCharsInclNullTerm-1] = '\0'
=======
#  define csf_vsprintf(/* char */ buffer,                               \
                       /* size_t */ sizeOfBufferInCharsInclNullTerm,    \
                       /* const char * */ format, /* va_list */ vaList) \
    vsprintf(buffer, format, vaList);                                   \
    buffer[sizeOfBufferInCharsInclNullTerm - 1] = '\0'
>>>>>>> upstream-releases
#endif

#endif
