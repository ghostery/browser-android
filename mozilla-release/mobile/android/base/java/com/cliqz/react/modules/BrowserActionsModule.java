package com.cliqz.react.modules;

import android.content.Context;
import android.database.Cursor;
import android.view.inputmethod.InputMethodManager;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.BrowserApp;
import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.GeckoThread;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2019
 */
public class BrowserActionsModule extends ReactContextBaseJavaModule {

    final private ReactContext mReactContext;

    public BrowserActionsModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
    }

    @Override
    public String getName() {
        return "BrowserActions";
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void openLink(String uri) {
        final GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("uri", uri);
        EventDispatcher.getInstance().dispatch("Search:OpenLink", bundle);
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void autocomplete(String data) {
        final GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("data", data);
        EventDispatcher.getInstance().dispatch("Search:Autocomplete", bundle);
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void suggest(String query, ReadableArray suggestions) {
        String[] suggestionsArray = suggestions.toArrayList().toArray(new String[suggestions.size()]);
        final GeckoBundle bundle = new GeckoBundle(2);
        bundle.putString("query", query);
        bundle.putStringArray("suggestions", suggestionsArray);
        EventDispatcher.getInstance().dispatch("Search:QuerySuggestions", bundle);
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void searchHistory(String query, Promise promise) {
        final int limit = 5;
        final BrowserDB sdb = BrowserDB.from(GeckoThread.getActiveProfile());
        final Context context = getReactApplicationContext();
        final Cursor cursor = sdb.getRankedHistoryForQuery(context.getContentResolver(), query, limit);

        final WritableArray wArray = Arguments.createArray();

        if (!cursor.moveToFirst()) {
            promise.resolve(wArray);
            cursor.close();
            return;
        }

        do {
            final String title = cursor.getString(cursor.getColumnIndex("title"));
            final String url = cursor.getString(cursor.getColumnIndex("url"));
            final WritableMap outData = Arguments.createMap();
            outData.putString("title", title);
            outData.putString("url", url);
            wArray.pushMap(outData);
        } while (cursor.moveToNext());

        promise.resolve(wArray);
        cursor.close();
    }

    @ReactMethod
    @SuppressWarnings("unused")
    public void hideKeyboard() {
        try {
            final InputMethodManager imm = (InputMethodManager) mReactContext.getSystemService(Context.INPUT_METHOD_SERVICE);
            imm.hideSoftInputFromWindow(mReactContext.getCurrentActivity().getCurrentFocus().getWindowToken(), 0);
            ((BrowserApp)mReactContext.getCurrentActivity()).hideCliqzQuerySuggestions();
        } catch (Exception ex) {
            // DO NOTHING
        }
    }

}
