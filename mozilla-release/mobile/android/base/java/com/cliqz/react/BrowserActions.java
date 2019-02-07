package com.cliqz.react;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * @author Khaled Tantawy
 */
public class BrowserActions extends ReactContextBaseJavaModule {

    public BrowserActions(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "BrowserActions";
    }

    @ReactMethod
    public void openLink(String uri) {
        GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("uri", uri);
        EventDispatcher.getInstance().dispatch("Search:OpenLink", bundle);
    }

    @ReactMethod
    public void autocomplete(String data) {
        GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("data", data);
        EventDispatcher.getInstance().dispatch("Search:Autocomplete", bundle);
    }

    @ReactMethod
    public void suggest(String query, ReadableArray suggestions) {
        String[] suggestionsArray = suggestions.toArrayList().toArray(new String[0]);
        GeckoBundle bundle = new GeckoBundle(2);
        bundle.putString("query", query);
        bundle.putStringArray("suggestions", suggestionsArray );
        EventDispatcher.getInstance().dispatch("Search:QuerySuggestions", bundle);
    }

}
