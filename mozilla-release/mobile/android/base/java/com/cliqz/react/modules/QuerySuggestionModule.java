package com.cliqz.react.modules;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2019
 */
public class QuerySuggestionModule extends ReactContextBaseJavaModule {

    final private ReactContext mReactContext;

    public QuerySuggestionModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
    }

    @Override
    public String getName() {
        return "QuerySuggestion";
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void showQuerySuggestions(String query, ReadableArray suggestions) {
        String[] suggestionsArray = suggestions.toArrayList().toArray(new String[suggestions.size()]);
        final GeckoBundle bundle = new GeckoBundle(2);
        bundle.putString("query", query);
        bundle.putStringArray("suggestions", suggestionsArray);
        EventDispatcher.getInstance().dispatch("Search:QuerySuggestions", bundle);
    }
}
