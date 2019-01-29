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
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * @author Khaled Tantawy
 */
public class BridgePackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        List<NativeModule> modules = new ArrayList<>();

        modules.add(new Bridge(reactContext));

        return modules;
    }

    @Override
    @SuppressWarnings({"rawtypes"})
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        return Collections.emptyList();
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
            EventDispatcher.getInstance().registerGeckoThreadListener(this, "Search:renderResults");
        }

        @Override
        public boolean canOverrideExistingModule() {
            return false;
        }

        @Override
        public void onCatalystInstanceDestroy() {
            EventDispatcher.getInstance().unregisterGeckoThreadListener(this, "Search:renderResults");
        }

        @Override
        public void handleMessage(String event, GeckoBundle message, EventCallback callback) {
            switch (event) {
                case "Search:renderResults":
                    long t0 = System.currentTimeMillis();
                    final String resultsArray = GeckoBundleUtils.safeGetString(message, "data");
                    final String ts1 = GeckoBundleUtils.safeGetString(message, "ts1");
                    final String ts2 = GeckoBundleUtils.safeGetString(message, "ts2");
                    final String ts3 = GeckoBundleUtils.safeGetString(message, "ts3");
                    Log.d("XXXX ts1 ts2", ts1 + " " + ts2);
//                    final WritableArray wArray = Arguments.createArray();
//                    for (GeckoBundle bundle : resultsArray) {
//                        try {
//                            wArray.pushMap(Utils.convertJsonToMap(bundle.toJSONObject()));
//                        } catch (JSONException e) {
//                            e.printStackTrace();
//                        }
//                    }
                    WritableMap map = Arguments.createMap();
                    map.putString("results", resultsArray);
                    map.putString("ts1", ts1);
                    map.putString("ts2", ts2);
                    map.putString("ts3", ts3);
                    map.putString("ts4", String.valueOf(System.currentTimeMillis()));
                    long t1 = System.currentTimeMillis();
                    Log.d("XXXX results geckobundle to writablearray", t0 + " " + t1 + " " + (t1 - t0));
                    mReactContext
                            .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                            .emit("search:renderResults", map);
                    break;
            }
        }

        @ReactMethod
        public void callBackgroundAction(final ReadableMap data, final Promise promise) {
            try {
                long t0 = System.currentTimeMillis();
                JSONObject json = Utils.convertMapToJson(data);
                GeckoBundle geckoBundle = GeckoBundle.fromJSONObject(json);
                long t1 = System.currentTimeMillis();
                Log.d("XXXX readablemap to geckobundle", t0 + " " + t1 + " " + (t1 - t0));
                EventDispatcher.getInstance().dispatch("Cards:CallBackgroundAction", geckoBundle, new EventCallback() {
                    @Override
                    public void sendSuccess(Object response) {
                        long t2 = System.currentTimeMillis();
                        Log.d("XXXX java to browserjs and back", "" + (t2 - t1));
                        if (response == null) {
                            promise.resolve(response);
                        } else {
                            GeckoBundle bundle = (GeckoBundle) response;
                            try {
                                JSONObject jsonRes = bundle.toJSONObject();
                                WritableMap map = Utils.convertJsonToMap(jsonRes);
                                long t3 = System.currentTimeMillis();
                                Log.d("XXXX geckobundle to writable map", t2 + " " + t3 + " " + (t3 - t2));
                                promise.resolve(map);
                            } catch (JSONException e) {
                                promise.reject(e);
                            }
                        }
                    }

                    @Override
                    public void sendError(Object response) {
                        try {
                            String message = (String) response;
                            promise.reject(new Throwable(message));
                        } catch (ClassCastException e) {
                            promise.reject(e);
                        }
                    }
                });
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }
}
