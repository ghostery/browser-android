// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html
/*
********************************************************************************
*   Copyright (C) 2005-2015, International Business Machines
*   Corporation and others.  All Rights Reserved.
********************************************************************************
*
* File WINTZ.CPP
*
********************************************************************************
*/

#include "unicode/utypes.h"

#if U_PLATFORM_USES_ONLY_WIN32_API

#include "wintz.h"
#include "cmemory.h"
#include "cstring.h"

#include "unicode/ures.h"
#include "unicode/ustring.h"
#include "uresimp.h"

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#   define VC_EXTRALEAN
#   define NOUSER
#   define NOSERVICE
#   define NOIME
#   define NOMCX
#include <windows.h>

U_NAMESPACE_BEGIN

// The max size of TimeZoneKeyName is 128, defined in DYNAMIC_TIME_ZONE_INFORMATION
#define MAX_TIMEZONE_ID_LENGTH 128

/**
* Main Windows time zone detection function.
* Returns the Windows time zone converted to an ICU time zone as a heap-allocated buffer, or nullptr upon failure.
* Note: We use the Win32 API GetDynamicTimeZoneInformation to get the current time zone info.
* This API returns a non-localized time zone name, which we can then map to an ICU time zone name.
*/
U_INTERNAL const char* U_EXPORT2
uprv_detectWindowsTimeZone()
{
    UErrorCode status = U_ZERO_ERROR;
    char* icuid = nullptr;
    char dynamicTZKeyName[MAX_TIMEZONE_ID_LENGTH];
    char tmpid[MAX_TIMEZONE_ID_LENGTH];
    int32_t len;
    int id = GEOID_NOT_AVAILABLE;
    int errorCode;
    wchar_t ISOcodeW[3] = {}; /* 2 letter ISO code in UTF-16 */
    char ISOcode[3] = {}; /* 2 letter ISO code in UTF-8 */

    DYNAMIC_TIME_ZONE_INFORMATION dynamicTZI;
    uprv_memset(&dynamicTZI, 0, sizeof(dynamicTZI));
    uprv_memset(dynamicTZKeyName, 0, sizeof(dynamicTZKeyName));
    uprv_memset(tmpid, 0, sizeof(tmpid));

<<<<<<< HEAD
    /* Obtain TIME_ZONE_INFORMATION from the API and get the non-localized time zone name. */
    if (TIME_ZONE_ID_INVALID == GetDynamicTimeZoneInformation(&dynamicTZI)) {
        return nullptr;
    }
||||||| merged common ancestors
    BOOL tryPreVistaFallback;
    OSVERSIONINFO osVerInfo;
=======
    /* Obtain TIME_ZONE_INFORMATION from the API and get the non-localized time zone name. */
    if (TIME_ZONE_ID_INVALID == GetDynamicTimeZoneInformation(&dynamicTZI)) {
        return nullptr;
    }

    id = GetUserGeoID(GEOCLASS_NATION);
    errorCode = GetGeoInfoW(id, GEO_ISO2, ISOcodeW, 3, 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
    id = GetUserGeoID(GEOCLASS_NATION);
    errorCode = GetGeoInfoW(id, GEO_ISO2, ISOcodeW, 3, 0);
||||||| merged common ancestors
    /* Obtain TIME_ZONE_INFORMATION from the API, and then convert it
       to TZI.  We could also interrogate the registry directly; we do
       this below if needed. */
    uprv_memset(&apiTZI, 0, sizeof(apiTZI));
    uprv_memset(&tziKey, 0, sizeof(tziKey));
    uprv_memset(&tziReg, 0, sizeof(tziReg));
    GetTimeZoneInformation(&apiTZI);
    tziKey.bias = apiTZI.Bias;
    uprv_memcpy((char *)&tziKey.standardDate, (char*)&apiTZI.StandardDate,
           sizeof(apiTZI.StandardDate));
    uprv_memcpy((char *)&tziKey.daylightDate, (char*)&apiTZI.DaylightDate,
           sizeof(apiTZI.DaylightDate));
=======
    // convert from wchar_t* (UTF-16 on Windows) to char* (UTF-8).
    u_strToUTF8(ISOcode, UPRV_LENGTHOF(ISOcode), nullptr,
        reinterpret_cast<const UChar*>(ISOcodeW), UPRV_LENGTHOF(ISOcodeW), &status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // convert from wchar_t* (UTF-16 on Windows) to char* (UTF-8).
    u_strToUTF8(ISOcode, UPRV_LENGTHOF(ISOcode), nullptr,
        reinterpret_cast<const UChar*>(ISOcodeW), UPRV_LENGTHOF(ISOcodeW), &status);
||||||| merged common ancestors
    /* Convert the wchar_t* standard name to char* */
    uprv_memset(apiStdName, 0, sizeof(apiStdName));
    wcstombs(apiStdName, apiTZI.StandardName, MAX_LENGTH_ID);
=======
    LocalUResourceBundlePointer bundle(ures_openDirect(nullptr, "windowsZones", &status));
    ures_getByKey(bundle.getAlias(), "mapTimezones", bundle.getAlias(), &status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    LocalUResourceBundlePointer bundle(ures_openDirect(nullptr, "windowsZones", &status));
    ures_getByKey(bundle.getAlias(), "mapTimezones", bundle.getAlias(), &status);
||||||| merged common ancestors
    tmpid[0] = 0;
=======
    // convert from wchar_t* (UTF-16 on Windows) to char* (UTF-8).
    u_strToUTF8(dynamicTZKeyName, UPRV_LENGTHOF(dynamicTZKeyName), nullptr,
        reinterpret_cast<const UChar*>(dynamicTZI.TimeZoneKeyName), -1, &status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // convert from wchar_t* (UTF-16 on Windows) to char* (UTF-8).
    u_strToUTF8(dynamicTZKeyName, UPRV_LENGTHOF(dynamicTZKeyName), nullptr,
        reinterpret_cast<const UChar*>(dynamicTZI.TimeZoneKeyName), -1, &status);
||||||| merged common ancestors
    id = GetUserGeoID(GEOCLASS_NATION);
    errorCode = GetGeoInfoW(id, GEO_ISO2, ISOcodeW, 3, 0);
    u_strToUTF8(ISOcodeA, 3, NULL, (const UChar *)ISOcodeW, 3, &status);
=======
    if (U_FAILURE(status)) {
        return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (U_FAILURE(status)) {
        return nullptr;
    }
||||||| merged common ancestors
    bundle = ures_openDirect(NULL, "windowsZones", &status);
    ures_getByKey(bundle, "mapTimezones", bundle, &status);
=======
    if (dynamicTZI.TimeZoneKeyName[0] != 0) {
        StackUResourceBundle winTZ;
        ures_getByKey(bundle.getAlias(), dynamicTZKeyName, winTZ.getAlias(), &status);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (dynamicTZI.TimeZoneKeyName[0] != 0) {
        UResourceBundle winTZ;
        ures_initStackObject(&winTZ);
        ures_getByKey(bundle.getAlias(), dynamicTZKeyName, &winTZ, &status);
        
        if (U_SUCCESS(status)) {
            const UChar* icuTZ = nullptr;
            if (errorCode != 0) {
                icuTZ = ures_getStringByKey(&winTZ, ISOcode, &len, &status);
||||||| merged common ancestors
    /*
        Windows Vista+ provides us with a "TimeZoneKeyName" that is not localized
        and can be used to directly map a name in our bundle. Try to use that first
        if we're on Vista or higher
    */
    uprv_memset(&osVerInfo, 0, sizeof(osVerInfo));
    osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
    tryPreVistaFallback = TRUE;
    result = getTZKeyName(regStdName, sizeof(regStdName));
    if(ERROR_SUCCESS == result) 
    {
        UResourceBundle* winTZ = ures_getByKey(bundle, regStdName, NULL, &status);
        if(U_SUCCESS(status)) 
        {
            const UChar* icuTZ = NULL;
            if (errorCode != 0) 
            {
                icuTZ = ures_getStringByKey(winTZ, ISOcodeA, &len, &status);
=======
        if (U_SUCCESS(status)) {
            const UChar* icuTZ = nullptr;
            if (errorCode != 0) {
                icuTZ = ures_getStringByKey(winTZ.getAlias(), ISOcode, &len, &status);
>>>>>>> upstream-releases
            }
            if (errorCode == 0 || icuTZ == nullptr) {
                /* fallback to default "001" and reset status */
                status = U_ZERO_ERROR;
<<<<<<< HEAD
                icuTZ = ures_getStringByKey(&winTZ, "001", &len, &status);
            }

            if (U_SUCCESS(status)) {
                int index = 0;

                while (!(*icuTZ == '\0' || *icuTZ == ' ')) {
                    // time zone IDs only contain ASCII invariant characters.
                    tmpid[index++] = (char)(*icuTZ++);
||||||| merged common ancestors
                icuTZ = ures_getStringByKey(winTZ, "001", &len, &status);
            }

            if(U_SUCCESS(status)) 
            {
                int index=0;
                while (! (*icuTZ == '\0' || *icuTZ ==' ')) 
                {
                    tmpid[index++]=(char)(*icuTZ++);  /* safe to assume 'char' is ASCII compatible on windows */
                }
                tmpid[index]='\0';
                tryPreVistaFallback = FALSE;
            }
        }
        ures_close(winTZ);
    }

    if(tryPreVistaFallback)
    {
        /* Note: We get the winid not from static tables but from resource bundle. */
        while (U_SUCCESS(status) && ures_hasNext(bundle))
        {
            UBool idFound = FALSE;
            const char* winid;
            UResourceBundle* winTZ = ures_getNextResource(bundle, NULL, &status);
            if (U_FAILURE(status)) 
            {
                break;
            }
            winid = ures_getKey(winTZ);
            result = getTZI(winid, &tziReg);

            if (result == ERROR_SUCCESS)
            {
                /* Windows alters the DaylightBias in some situations.
                   Using the bias and the rules suffices, so overwrite
                   these unreliable fields. */
                tziKey.standardBias = tziReg.standardBias;
                tziKey.daylightBias = tziReg.daylightBias;

                if (uprv_memcmp((char *)&tziKey, (char*)&tziReg, sizeof(tziKey)) == 0)
                {
                    const UChar* icuTZ = NULL;
                    if (errorCode != 0)
                    {
                        icuTZ = ures_getStringByKey(winTZ, ISOcodeA, &len, &status);
                    }
                    if (errorCode==0 || icuTZ==NULL) 
                    {
                        /* fallback to default "001" and reset status */
                        status = U_ZERO_ERROR;
                        icuTZ = ures_getStringByKey(winTZ, "001", &len, &status);
                    }

                    if (U_SUCCESS(status)) 
                    {
                        /* Get the standard name from the registry key to compare with
                           the one from Windows API call. */
                        uprv_memset(regStdName, 0, sizeof(regStdName));
                        result = getSTDName(winid, regStdName, sizeof(regStdName));
                        if (result == ERROR_SUCCESS) 
                        {
                            if (uprv_strcmp(apiStdName, regStdName) == 0) 
                            {
                                idFound = TRUE;
                            }
                        }

                        /* tmpid buffer holds the ICU timezone ID corresponding to the timezone ID from Windows.
                         * If none is found, tmpid buffer will contain a fallback ID (i.e. the time zone ID matching
                         * the current time zone information)
                         */
                        if (idFound || tmpid[0] == 0) 
                        {
                            /* if icuTZ has more than one city, take only the first (i.e. terminate icuTZ at first space) */
                            int index=0;
                            while (! (*icuTZ == '\0' || *icuTZ ==' ')) 
                            {
                                tmpid[index++]=(char)(*icuTZ++);  /* safe to assume 'char' is ASCII compatible on windows */
                            }
                            tmpid[index]='\0';
                        }
                    }
=======
                icuTZ = ures_getStringByKey(winTZ.getAlias(), "001", &len, &status);
            }

            if (U_SUCCESS(status)) {
                int index = 0;

                while (!(*icuTZ == '\0' || *icuTZ == ' ')) {
                    // time zone IDs only contain ASCII invariant characters.
                    tmpid[index++] = (char)(*icuTZ++);
>>>>>>> upstream-releases
                }
                tmpid[index] = '\0';
            }
        }
        ures_close(&winTZ);
    }

    // Copy the timezone ID to icuid to be returned.
    if (tmpid[0] != 0) {
        icuid = uprv_strdup(tmpid);
    }

    return icuid;
}

U_NAMESPACE_END
#endif /* U_PLATFORM_USES_ONLY_WIN32_API  */
