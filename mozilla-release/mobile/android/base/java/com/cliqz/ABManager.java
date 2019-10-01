package com.cliqz;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.support.annotation.NonNull;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;

import static com.cliqz.react.modules.PrefsModule.PREFS_SEARCH_CARDS_LAYOUT;


/**
 * AB-tests manager on the Android side. It syncs Android prefs with GeckoPrefs (the source of truth
 * is the latter).
 *
 * Copyright © Cliqz 2019
 */
public class ABManager {
    private static final String ABTEST_VERSION = "abtest-version";
    public static final String NEWTAB_SETTINGS_TEXT_STYLE = "text";

    // Version keys to support migration
    private static final class VERSIONS {
        static final int VERSION_1 = 1;

        static final int CURRENT = VERSION_1;
    }

    // Grouping of all the AB-test enabled in version 1
    private static final class V1 {
        static final String PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE = "host.ui.newtab.settings-button-style";
    }

    private final SharedPreferences appSharedPreferences;

    // Add here the preferences that you want to be in sync
    private static final String[] PREFS = new String[] {
            V1.PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE
    };

    @SuppressLint("StaticFieldLeak")
    private static ABManager abManager = null;

    // This is a singleton that need a context, initialization is in BrowserApplication. The
    // context passed is not meant to be stored.
    private ABManager(Context context) {
        appSharedPreferences = GeckoSharedPrefs.forABTests(context);
        final int oldVersion;
        if (!appSharedPreferences.contains(ABTEST_VERSION)) {
            appSharedPreferences.edit().putInt(ABTEST_VERSION, VERSIONS.CURRENT).apply();
            oldVersion = VERSIONS.CURRENT;
        } else {
            oldVersion = appSharedPreferences.getInt(ABTEST_VERSION, VERSIONS.CURRENT);
        }
        if (oldVersion != VERSIONS.CURRENT) {
            migrate(oldVersion);
            appSharedPreferences.edit().putInt(ABTEST_VERSION, VERSIONS.CURRENT).apply();
        }

        PrefsHelper.getPrefs(PREFS, new InnerPrefsHandler());
    }

    /**
     * Sets up gecko prefs early in application startup so all browser components like
     * searchui or new tab page can rely on their existence.
     *
     * Note: remember to flush the prefs
     */
    public void assignNewUsers() {
        final double random = Math.random();
        if (random >= 0.5) {
            PrefsHelper.setPref(PREFS_SEARCH_CARDS_LAYOUT, "vertical", true);
        }
    }

    // Migration mechanism in place for future development
    @SuppressWarnings("unused")
    private void migrate(int oldVersion) {
    }

    /**
     * Initialize the manager (single instance)
     *
     * @param context The context used to initialize the singleton. It is not stored by the manager.
     */
    public static void init(Context context) {
        if (abManager == null) {
            abManager = new ABManager(context.getApplicationContext());
        }
    }

    /**
     * @return the ABManager instance
     */
    public static ABManager getInstance() {
        return abManager;
    }

    /**
     * @return the home style AB-Test
     */
    @NonNull
    public String getHomeSettingsStyle() {
        return appSharedPreferences.getString(V1.PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE, "text");
    }

    private class InnerPrefsHandler implements PrefsHelper.PrefHandler {

        private SharedPreferences.Editor mEditor = null;

        @Override
        public void prefValue(String pref, boolean value) {
            mEditor = nonNullEditor();
        }

        @Override
        public void prefValue(String pref, int value) {
            mEditor = nonNullEditor();
        }

        @Override
        public void prefValue(String pref, String value) {
            mEditor = nonNullEditor();
            if (V1.PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE.equals(pref)) {
                mEditor.putString(V1.PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE, value);
            }
        }

        private SharedPreferences.Editor nonNullEditor() {
            return mEditor != null ? mEditor : appSharedPreferences.edit();
        }

        @Override
        public void finish() {
            if (mEditor != null) {
                mEditor.apply();
            }
            mEditor = null;
        }
    }
}
