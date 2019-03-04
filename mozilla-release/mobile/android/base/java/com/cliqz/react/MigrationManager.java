package com.cliqz.react;

import com.cliqz.react.modules.PrefsModule;

import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.preferences.GeckoPreferences;

public class MigrationManager {
    private final static MigrationManager ourInstance = new MigrationManager();

    public static MigrationManager getInstance() {
        return ourInstance;
    }

    /**
     * Let Search handle search language
     */
    public void migrateBackendCountryLanguage() {
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, String countryCode) {
            if (!(countryCode instanceof String) || countryCode.equals("")) {
                return;
            }
            PrefsHelper.setPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, "");
            SearchBackground.getInstance().setBackendCountry(countryCode);
            }
        });
    }

    public void migrateQuerySuggestionsPref() {
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_QUERY_SUGGESTIONS, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, boolean enabled) {
                PrefsHelper.setPref(GeckoPreferences.PREFS_SEARCH_QUERY_SUGGESTIONS, "");
                PrefsHelper.setPref(PrefsModule.PREFS_SEARCH_QUERY_SUGGESTIONS, enabled);
            }
        });
    }
}
