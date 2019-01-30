package com.cliqz.react;

import android.app.Fragment;
import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.uimanager.ViewManager;

import org.mozilla.gecko.GeckoThread;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.home.TopSitesPanel;

import java.util.ArrayList;
import java.util.List;

public class HistoryDialsPackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        List<NativeModule> modules = new ArrayList<>();

        modules.add(new HistoryDials(reactContext));

        return modules;
    }

    @Override
    @SuppressWarnings({"rawtypes"})
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
      List<ViewManager> viewManagers = new ArrayList<>();
      return viewManagers;
    }


    public class HistoryDials extends ReactContextBaseJavaModule {

        public HistoryDials(ReactApplicationContext reactContext) {
            super(reactContext);
        }

        @Override
        public String getName() {
            return "HistoryDials";
        }

        @ReactMethod
        public void getTopSites(final Promise promise) {
            final String[][] mockData = new String[][] {
              new String[] { "Wikipedia", "https://www.wikipedia.com", "today at 11:00", "999999", "WI"},
              new String[] { "Youtube", "https://www.youtube.com", "today", "800000", "YT" },
              new String[] { "Facebook", "https://www.facebook.com", "yesterday", "5048A5", "FB" }
            };

            // TODO get data from database

            final BrowserDB sdb = BrowserDB.from(GeckoThread.getActiveProfile());
            final Context context = getCurrentActivity().getApplicationContext();

            final Cursor cursor = sdb.getTopSites(context.getContentResolver(), 9, 30);
            final WritableArray wArray = Arguments.createArray();

            if (!cursor.moveToFirst()) {
                return;
            }

            do {
                final String title = cursor.getString(cursor.getColumnIndex("title"));
                final String url = cursor.getString(cursor.getColumnIndex("url"));
                final String lastVisited = cursor.getString(cursor.getColumnIndex("localDateLastVisited"));
                if (!url.isEmpty()) {
                    // add title to the arrayList of dictionaries
                    final WritableMap outData = Arguments.createMap();
                    outData.putString("title", title);
                    outData.putString("url", url);
                    outData.putString("lastVisited", lastVisited);
                    wArray.pushMap(outData);
                }


            } while (cursor.moveToNext());

            // For now return a static array
//            for (String[] row: mockData) {
//                final WritableMap outData = Arguments.createMap();
//                outData.putString("title", row[0]);
//                outData.putString("url", row[1]);
//                outData.putString("lastVisited", row[2]);
//                final WritableMap logoDetails = Arguments.createMap();
//                logoDetails.putString("backgroundColor", row[3]);
//                logoDetails.putString("text", row[4]);
//                outData.putMap("logoDetails", logoDetails);
//                wArray.pushMap(outData);
////                Log.d(BridgePackage.class.getSimpleName(), outData);
//            }
//            Log.d(BridgePackage.class.getSimpleName(), String.valueOf(wArray));

            promise.resolve(wArray);

        }
    }
}

