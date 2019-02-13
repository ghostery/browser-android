package com.cliqz;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.support.annotation.NonNull;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;


/**
 * Copyright © Cliqz 2019
 * ABManager class that syncs Browser prefs with GeckoPrefs
 */
public class ABManager {
    private static final String ABTEST_VERSION = "abtest-version";
    public static final String NEWTAB_SETTINGS_TEXT_STYLE = "text";

    private static final class VERSIONS {
        static final int VERSION_1 = 1;

        static final int CURRENT = VERSION_1;
    }
    private static final class V1 {
        static final String PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE = "host.ui.newtab.settings-button-style";
    }

    private final SharedPreferences appSharedPreferences;
    private static final String[] PREFS = new String[] {
            V1.PREF_HOST_UI_NEWTAB_SETTINGS_BUTTON_STYLE
    };

    @SuppressLint("StaticFieldLeak")
    private static ABManager abManager = null;
    private final InnerPrefsHandler prefsHandler;

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
        prefsHandler = new InnerPrefsHandler();
        PrefsHelper.getPrefs(PREFS, prefsHandler);
    }

    // Migration mechanism in place for future development
    @SuppressWarnings("unused")
    private void migrate(int oldVersion) {
    }

    public static void init(Context context) {
        if (abManager == null) {
            abManager = new ABManager(context.getApplicationContext());
        }
    }

    public static ABManager getInstance() {
        return abManager;
    }

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
