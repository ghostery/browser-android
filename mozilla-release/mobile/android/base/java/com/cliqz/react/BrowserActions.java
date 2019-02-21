package com.cliqz.react;

import android.content.Context;
import android.database.Cursor;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
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
        final GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("uri", uri);
        EventDispatcher.getInstance().dispatch("Search:OpenLink", bundle);
    }

    @ReactMethod
    public void autocomplete(String data) {
        final GeckoBundle bundle = new GeckoBundle(1);
        bundle.putString("data", data);
        EventDispatcher.getInstance().dispatch("Search:Autocomplete", bundle);
    }

    @ReactMethod
    public void suggest(String query, ReadableArray suggestions) {
        String[] suggestionsArray = suggestions.toArrayList().toArray(new String[0]);
        final GeckoBundle bundle = new GeckoBundle(2);
        bundle.putString("query", query);
        bundle.putStringArray("suggestions", suggestionsArray );
        EventDispatcher.getInstance().dispatch("Search:QuerySuggestions", bundle);
    }

    @ReactMethod
    public void searchHistory(String query, Promise promise) {
        final int limit = 5;
        final BrowserDB sdb = BrowserDB.from(GeckoThread.getActiveProfile());
        final Context context = getCurrentActivity().getApplicationContext();
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

}
