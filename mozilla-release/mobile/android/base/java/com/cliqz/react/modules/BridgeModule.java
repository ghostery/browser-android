package com.cliqz.react.modules;

import com.cliqz.ThemeManager;
import com.cliqz.react.SearchBackground;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

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
}
