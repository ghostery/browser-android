package com.cliqz.react.modules;

import android.content.Context;
import android.database.Cursor;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.GeckoThread;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2019
 */
public class AutoCompletionModule extends ReactContextBaseJavaModule {

    final private ReactContext mReactContext;

    public AutoCompletionModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
    }

    @Override
    public String getName() {
        return "AutoCompletion";
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void autoComplete(String data) {
        final GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("data", data);
        EventDispatcher.getInstance().dispatch("Search:Autocomplete", bundle);
    }
}
