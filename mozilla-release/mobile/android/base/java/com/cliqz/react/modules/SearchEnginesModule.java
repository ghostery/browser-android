package com.cliqz.react.modules;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;

/**
 * Copyright © Cliqz 2019
 */
public class SearchEnginesModule extends ReactContextBaseJavaModule {
    public SearchEnginesModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "SearchEngines";
    }

    @ReactMethod
    public void getSearchEngines(Promise promise) {
        WritableArray engines = Arguments.createArray();
        promise.resolve(engines);
    }
}
