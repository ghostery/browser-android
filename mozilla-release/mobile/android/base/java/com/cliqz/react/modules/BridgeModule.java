package com.cliqz.react.modules;

import android.util.Log;

import com.cliqz.AppearanceManager;
import com.cliqz.react.SearchBackground;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.PrefsHelper.PrefHandlerBase;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import static com.cliqz.react.modules.PrefsModule.PREFS_SEARCH_CARDS_LAYOUT;

/**
 * Copyright © Cliqz 2019
 */
public class BridgeModule extends ReactContextBaseJavaModule implements BundleEventListener {

    private final ReactApplicationContext mReactContext;

    public BridgeModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
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
            default:
                Log.w(getClass().getSimpleName(), "Unknown event " + event);
                break;
        }
    }

    @ReactMethod
    public void replyToAction(int hash, ReadableMap response) {
        SearchBackground.replyToAction(hash, response);
    }

    /**
     * Config has to be in format:
     *
     * {
     *     "appearance": "light|dark",
     *     "layout": "vertical|horizontal"
     * }
     *
     */
    @ReactMethod
    public void getConfig(final Promise promise) {
        final WritableMap config = Arguments.createMap();
        config.putString("appearance", AppearanceManager.getInstance().getCurrent());

        PrefsHelper.getPrefs(new String[] { PREFS_SEARCH_CARDS_LAYOUT }, new PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, boolean value) {
            }

            @Override
            public void prefValue(String prefName, int value) {
            }

            @Override
            public void prefValue(String prefName, String value) {
                if (prefName.equals(PREFS_SEARCH_CARDS_LAYOUT)) {
                    config.putString("layout", value);
                }
            }

            @Override
            public void finish() {
                promise.resolve(config);
            }
        });
    }
}
