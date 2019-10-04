# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

-sync-brand-short-name =
    { $case ->
       *[nom]
            { $capitalization ->
               *[upper] Synchronizacja
                [lower] synchronizacja
            }
        [gen]
            { $capitalization ->
               *[upper] Synchronizacji
                [lower] synchronizacji
            }
        [dat]
            { $capitalization ->
               *[upper] Synchronizacji
                [lower] synchronizacji
            }
        [acc]
            { $capitalization ->
               *[upper] Synchronizację
                [lower] synchronizację
            }
        [ins]
            { $capitalization ->
               *[upper] Synchronizacją
                [lower] synchronizacją
            }
        [loc]
            { $capitalization ->
               *[upper] Synchronizacji
                [lower] synchronizacji
            }
    }
# “Sync” can be localized, “Firefox” must be treated as a brand,
# and kept in English.
-sync-brand-name =
    { $case ->
       *[nom]
            { $capitalization ->
               *[upper] Synchronizacja Firefox
                [lower] synchronizacja Firefox
            }
        [gen]
            { $capitalization ->
               *[upper] Synchronizacji Firefox
                [lower] synchronizacji Firefox
            }
        [dat]
            { $capitalization ->
               *[upper] Synchronizacji Firefox
                [lower] synchronizacji Firefox
            }
        [acc]
            { $capitalization ->
               *[upper] Synchronizację Firefox
                [lower] synchronizację Firefox
            }
        [ins]
            { $capitalization ->
               *[upper] Synchronizacją Firefox
                [lower] synchronizacją Firefox
            }
        [loc]
            { $capitalization ->
               *[upper] Synchronizacji Firefox
                [lower] synchronizacji Firefox
            }
    }
# “Account” can be localized, “Firefox” must be treated as a brand,
# and kept in English.
-fxaccount-brand-name =
    { $case ->
       *[nom]
            { $capitalization ->
               *[upper] Konto Firefox
                [lower] konto Firefox
            }
        [gen]
            { $capitalization ->
               *[upper] Konta Firefox
                [lower] konta Firefox
            }
        [dat]
            { $capitalization ->
               *[upper] Kontu Firefox
                [lower] kontu Firefox
            }
        [acc]
            { $capitalization ->
               *[upper] Konto Firefox
                [lower] konto Firefox
            }
        [ins]
            { $capitalization ->
               *[upper] Kontem Firefox
                [lower] kontem Firefox
            }
        [loc]
            { $capitalization ->
               *[upper] Koncie Firefox
                [lower] koncie Firefox
            }
    }
