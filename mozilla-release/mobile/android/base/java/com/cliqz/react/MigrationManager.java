package com.cliqz.react;

import android.content.Context;
import android.content.SharedPreferences;

import com.cliqz.ABManager;
import com.cliqz.react.modules.PrefsModule;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.preferences.GeckoPreferences;

import static org.mozilla.gecko.GeckoApp.PREFS_VERSION_CODE;

/**
 * Copyright © Cliqz 2019
 */
public class MigrationManager {
    private static final int GHOSTERY_VERSION_2_2_1 = 12255;
    private static final int GHOSTERY_VERSION_2_3 = 12555;

    private final static MigrationManager ourInstance = new MigrationManager();

    public static MigrationManager getInstance() {
        return ourInstance;
    }

    /**
     * Let Search handle search language
     */
    private void migrateBackendCountryLanguage() {
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, String countryCode) {
            if (!(countryCode instanceof String) || countryCode.equals("")) {
                return;
            }
            PrefsHelper.setPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, "");
            SearchBackground.setBackendCountry(countryCode);
            }
        });
    }

    private void migrateQuerySuggestionsPref() {
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_QUERY_SUGGESTIONS, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, boolean enabled) {
                PrefsHelper.setPref(GeckoPreferences.PREFS_SEARCH_QUERY_SUGGESTIONS, "");
                PrefsHelper.setPref(PrefsModule.PREFS_SEARCH_QUERY_SUGGESTIONS, enabled);
            }
        });
    }

    public void migrate(Context context) {
        final SharedPreferences prefs = GeckoSharedPrefs.forProfile(context);;
        final int prefsVersionCode = prefs.getInt(PREFS_VERSION_CODE, 0);

        /* upgrading users */
        if (prefsVersionCode > 0) {
            if (prefsVersionCode <= GHOSTERY_VERSION_2_2_1) {
                if (prefs.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_BACKGROUND_ENABLED, true)) {
                    // New users should have the background image disabled by default, old users should have
                    // the background image enabled if they didn't explicitly disable it.
                    prefs.edit()
                            .putBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_BACKGROUND_ENABLED, true)
                            .apply();
                }
            }
            if (prefsVersionCode <= GHOSTERY_VERSION_2_3) {
                final MigrationManager migrationManager = MigrationManager.getInstance();
                migrationManager.migrateBackendCountryLanguage();
                migrationManager.migrateQuerySuggestionsPref();
            }
        } else /* new users */ {
            ABManager.getInstance().assignNewUsers();
        }
    }
}
