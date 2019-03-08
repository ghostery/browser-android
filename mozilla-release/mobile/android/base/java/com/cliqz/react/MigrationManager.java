package com.cliqz.react;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.util.Log;

import com.cliqz.ABManager;
import com.cliqz.react.modules.PrefsModule;

import org.mozilla.gecko.GeckoApp;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.preferences.GeckoPreferences;

/**
 * Copyright © Cliqz 2019
 */
public class MigrationManager {
    private static final String PREFS_VERSION_CODE = "cliqzVersionCode";
    private static final int GHOSTERY_VERSION_2_2_1 = 12255;
    private static final int GHOSTERY_VERSION_2_3 = 12555;

    private final static MigrationManager ourInstance = new MigrationManager();

    public static MigrationManager getInstance() {
        return ourInstance;
    }

    public void migrate(Context context) {
        final SharedPreferences prefs = GeckoSharedPrefs.forProfile(context);
        final int version = getVersionCode(context);
        final int prefsVersionCode = getPreviousVersionCode(prefs);

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

        prefs.edit().putInt(PREFS_VERSION_CODE, version).apply();
    }

    /**
     * Let Search handle search language
     */
    private void migrateBackendCountryLanguage() {
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, String countryCode) {
                if (countryCode == null || countryCode.equals("")) {
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

    private int getPreviousVersionCode(SharedPreferences prefs) {
        int versionCode = prefs.getInt(PREFS_VERSION_CODE, 0);

        // migrate the version code from gecko
        if (versionCode == 0) {
            versionCode = prefs.getInt(GeckoApp.PREFS_VERSION_CODE, 0);
            prefs.edit().putInt(PREFS_VERSION_CODE, versionCode).apply();
        }

        return versionCode;
    }

    private int getVersionCode(Context context) {
        int versionCode = 0;
        try {
            versionCode = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            Log.wtf(getClass().getSimpleName(), context.getPackageName() + " not found", e);
        }
        return versionCode;
    }
}
