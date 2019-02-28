package com.cliqz.react;

import android.util.Log;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.facebook.react.uimanager.ViewManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.GeckoThread;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

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
        private final ReactApplicationContext mReactContext;

        public Bridge(ReactApplicationContext reactContext) {
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
    }
}
