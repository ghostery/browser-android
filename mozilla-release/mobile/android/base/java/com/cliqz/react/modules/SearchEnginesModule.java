package com.cliqz.react.modules;

import android.util.Log;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeArray;
import com.facebook.react.bridge.WritableNativeMap;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2019
 */
public class SearchEnginesModule extends ReactContextBaseJavaModule implements BundleEventListener {
    private Promise mCurrentPromise;

    public SearchEnginesModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "SearchEnginesModule";
    }

    @Override
    public void initialize() {
        EventDispatcher.getInstance().registerUiThreadListener(this, "SearchEngines:Data");
    }

    @Override
    public boolean canOverrideExistingModule() {
        return false;
    }

    @Override
    public void onCatalystInstanceDestroy() {
        EventDispatcher.getInstance().unregisterUiThreadListener(this, "SearchEngines:Data");
    }

    @Override
    public void handleMessage(String event, GeckoBundle data, EventCallback callback) {
        switch (event) {
            case "SearchEngines:Data":
                if (mCurrentPromise == null) {
                    return;
                }

                final GeckoBundle[] engines = data.getBundleArray("searchEngines");
                final WritableArray enginesArray = new WritableNativeArray();
                // the first engine in the list is considered default
                boolean isDefaultSetOnce = true;

                for (final GeckoBundle engineBundle : engines) {
                    final WritableMap urls = new WritableNativeMap();
                    urls.putString("text/html", engineBundle.getString("template"));

                    final String searchTerm = engineBundle.getString("searchTerm");
                    final WritableMap engine = new WritableNativeMap();
                    engine.putBoolean("default", isDefaultSetOnce);
                    isDefaultSetOnce = false;
                    engine.putString("SearchTermComponent", searchTerm);
                    engine.putString("LocaleTermComponent", searchTerm);
                    engine.putString("name", engineBundle.getString("name"));
                    engine.putString("base_url", engineBundle.getString("base_url"));
                    engine.putMap("urls", urls);

                    enginesArray.pushMap(engine);
                }

                mCurrentPromise.resolve(enginesArray);
                mCurrentPromise = null;
                break;
            default:
                Log.w(getClass().getSimpleName(), "Unknown event " + event);
                break;
        }
    }

    @ReactMethod
    public void getSearchEngines(Promise promise) {
        mCurrentPromise = promise;
        EventDispatcher.getInstance().dispatch("SearchEngines:GetVisible", null);
    }
}
