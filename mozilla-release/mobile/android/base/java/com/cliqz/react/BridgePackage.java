package com.cliqz.react;

import android.content.SharedPreferences;

import com.cliqz.ThemeManager;
import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Dynamic;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.ViewManager;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Copyright © Cliqz 2019
 */
public class BridgePackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        final List<NativeModule> modules = new ArrayList<>();

        modules.add(new Bridge(reactContext));
        modules.add(new BrowserActions(reactContext));

        return modules;
    }

    @Override
    @SuppressWarnings({"rawtypes"})
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        final List<ViewManager> viewManagers = new ArrayList<>();
        viewManagers.add(new NativeDrawableManager());

        return viewManagers;
    }

    public class Bridge extends ReactContextBaseJavaModule implements BundleEventListener {
        private static final String PREF_PREFIX = "searchui.";
        private static final String SEARCHUI_PREF_NAMES = "searchui.prefNames";

        private final SharedPreferences xPreferences;

        private final ReactApplicationContext mReactContext;

        public Bridge(ReactApplicationContext reactContext) {
            super(reactContext);
            mReactContext = reactContext;
            xPreferences = GeckoSharedPrefs.forProfile(mReactContext);
        }

        @Override
        public String getName() {
            return "Bridge";
        }

        @Override
        public void initialize() {
            EventDispatcher.getInstance().registerUiThreadListener(this, "Search:Search");
        }

        @Override
        public boolean canOverrideExistingModule() {
            return false;
        }

        @Override
        public void onCatalystInstanceDestroy() {
            EventDispatcher.getInstance().unregisterUiThreadListener(this, "Search:Search");
        }

        @Override
        public void handleMessage(String event, GeckoBundle message, EventCallback callback) {
            switch (event) {
                case "Search:Search":
                    final String query = GeckoBundleUtils.safeGetString(message, "q");
                    SearchBackground.startSearch(query);
                    break;
            }
        }

        @ReactMethod
        public void replyToAction(int hash, ReadableMap response) {
            SearchBackground.replyToAction(hash, response);
        }

        //TODO create private methods for getTheme and getCardStyle

        @ReactMethod
        public void getConfig(final Promise promise) {
            final WritableMap outData = Arguments.createMap();
            outData.putString("theme", ThemeManager.THEME_LIGHT);
            outData.putString("cardStyle", "vertical");
            promise.resolve(outData);
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
                xPreferences.edit().putStringSet(SEARCHUI_PREF_NAMES, prefNames).apply();
            }
        }

        private String removePrefix(String prefName) {
            return prefName.substring(PREF_PREFIX.length());
        }

        private String addPrefix(String prefName) {
            return PREF_PREFIX + prefName;
        }

        private Set<String> getPrefNames() {
            final Set<String> prefNames = xPreferences.getStringSet(SEARCHUI_PREF_NAMES, new HashSet<>());
            prefNames.add(addPrefix("suggestionChoice"));
            prefNames.add(addPrefix("suggestionsEnabled"));
            return prefNames;
        }
    }
}
