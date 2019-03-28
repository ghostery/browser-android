package com.cliqz.react.modules;

import android.content.SharedPreferences;

import com.cliqz.react.SearchBackground;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Dynamic;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;

import java.util.HashSet;
import java.util.Set;

/**
 * Copyright © Cliqz 2019
 */
public class PrefsModule extends ReactContextBaseJavaModule {
    private static final String PREF_PREFIX = "searchui.";
    private static final String SEARCHUI_PREF_NAMES = "searchui.prefNames";
    public static String PREFS_SEARCH_QUERY_SUGGESTIONS = PREF_PREFIX + "suggestionsEnabled";
    public static String PREFS_SEARCH_ADULT_RESULTS = PREF_PREFIX + "adultContentFilter";
    public static String PREFS_SEARCH_CARDS_LAYOUT = PREF_PREFIX + "cards.layout";
    private final Set<String> mPrefIgnoreList = new HashSet<>();

    private final SharedPreferences mPreferences;
    private final PrefsHelper.PrefHandler mPrefHandler = new PrefsHelper.PrefHandler() {
        @Override
        public void prefValue(String pref, boolean value) {
            notifyPrefChange(pref, value);
        }

        @Override
        public void prefValue(String pref, int value) {
            notifyPrefChange(pref, value);
        }

        @Override
        public void prefValue(String pref, String value) {
            notifyPrefChange(pref, value);
        }

        @Override
        public void finish() {

        }
    };
    private final ReactApplicationContext mReactContext;

    public PrefsModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
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
        mPrefIgnoreList.add(prefName);
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
        final WritableMap prefs = Arguments.createMap();
        PrefsHelper.getPrefs(getPrefNamesArray(), new PrefsHelper.PrefHandler() {
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
                startPrefsListener();
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
        if (prefName.startsWith(PREF_PREFIX)) {
            return prefName.substring(PREF_PREFIX.length());
        }
        return prefName;
    }

    private String addPrefix(String prefName) {
        return PREF_PREFIX + prefName;
    }

    private Set<String> getPrefNames() {
        final Set<String> prefNames = mPreferences.getStringSet(SEARCHUI_PREF_NAMES, new HashSet<>());
        prefNames.add(addPrefix("suggestionChoice"));
        prefNames.add(PREFS_SEARCH_QUERY_SUGGESTIONS);
        prefNames.add(PREFS_SEARCH_ADULT_RESULTS);
        return prefNames;
    }

    private String[] getPrefNamesArray() {
        final Set<String> namesSet = getPrefNames();
        final String[] names = new String[namesSet.size()];
        return namesSet.toArray(names);
    }

    private void startPrefsListener() {
        PrefsHelper.removeObserver(mPrefHandler);
        PrefsHelper.addObserver(getPrefNamesArray(), mPrefHandler);
    }

    private void notifyPrefChange(String prefName, Object value) {
        final String key = removePrefix(prefName);
        if (mPrefIgnoreList.contains(key)) {
            mPrefIgnoreList.remove(key);
            return;
        }
        final WritableArray keyValue = Arguments.fromJavaArgs(new Object[]{ key, value });

        SearchBackground.getInstance().reportPrefChange(keyValue);
    }
}
