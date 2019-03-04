package com.cliqz.react.modules;

import android.content.SharedPreferences;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Dynamic;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;

import java.util.HashSet;
import java.util.Set;

public class PrefsModule extends ReactContextBaseJavaModule {
    private static final String PREF_PREFIX = "searchui.";
    private static final String SEARCHUI_PREF_NAMES = "searchui.prefNames";

    private final SharedPreferences mPreferences;

    public PrefsModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mPreferences = GeckoSharedPrefs.forProfile(reactContext);
    }

    @Override
    public String getName() {
        return "Prefs";
    }

    @Override
    public boolean canOverrideExistingModule() {
        return false;
    }

    @ReactMethod
    public void clearPref(String prefName) {
        // TODO: there is no way to clear pref from JAVA - need to send message to browser.js
        // PrefsHelper.setPref(prefName, null);
    }

    @ReactMethod
    public void setPref(String prefName, Dynamic value) {
        recordPrefName(prefName);
        Object val = null;
        switch (value.getType()) {
            case String:
                val = value.asString();
                break;
            case Boolean:
                val = value.asBoolean();
                break;
            case Number:
                val = value.asInt();
                break;
            default:
                return;
        }
        PrefsHelper.setPref(addPrefix(prefName), val);
    }

    @ReactMethod
    public void getAllPrefs(Promise promise) {
        WritableMap prefs = Arguments.createMap();
        final Set<String> prefNames = getPrefNames();

        PrefsHelper.getPrefs(prefNames.toArray(new String[] { }), new PrefsHelper.PrefHandler() {
            @Override
            public void prefValue(String pref, boolean value) {
                prefs.putBoolean(removePrefix(pref), value);
            }

            @Override
            public void prefValue(String pref, int value) {
                prefs.putInt(removePrefix(pref), value);
            }

            @Override
            public void prefValue(String pref, String value) {
                prefs.putString(removePrefix(pref), value);
            }

            @Override
            public void finish() {
                promise.resolve(prefs);
            }
        });
    }

    private void recordPrefName(String prefName) {
        final Set<String> prefNames = getPrefNames();

        if (prefNames.add(addPrefix(prefName))) {
            mPreferences.edit().putStringSet(SEARCHUI_PREF_NAMES, prefNames).apply();
        }
    }

    private String removePrefix(String prefName) {
        return prefName.substring(PREF_PREFIX.length());
    }

    private String addPrefix(String prefName) {
        return PREF_PREFIX + prefName;
    }

    private Set<String> getPrefNames() {
        final Set<String> prefNames = mPreferences.getStringSet(SEARCHUI_PREF_NAMES, new HashSet<>());
        prefNames.add(addPrefix("suggestionChoice"));
        prefNames.add(addPrefix("suggestionsEnabled"));
        return prefNames;
    }
}
