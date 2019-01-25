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
        List<ViewManager> viewManagers = new ArrayList<>();
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
                    final GeckoBundle[] resultsArray = GeckoBundleUtils.safeGetBundleArray(message, "data");
                    final WritableArray wArray = Arguments.createArray();
                    for (GeckoBundle bundle : resultsArray) {
                        try {
                            wArray.pushMap(Utils.convertJsonToMap(bundle.toJSONObject()));
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }

                    mReactContext
                            .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                            .emit("search:renderResults", wArray);
                    break;
            }
        }

        @ReactMethod
        public void callBackgroundAction(final ReadableMap data, final Promise promise) {
            try {
                JSONObject json = Utils.convertMapToJson(data);
                EventDispatcher.getInstance().dispatch("Cards:CallBackgroundAction", GeckoBundle.fromJSONObject(json), new EventCallback() {
                    @Override
                    public void sendSuccess(Object response) {
                        if (response == null) {
                            promise.resolve(response);
                        } else {
                            GeckoBundle bundle = (GeckoBundle) response;
                            try {
                                JSONObject jsonRes = bundle.toJSONObject();
                                promise.resolve(Utils.convertJsonToMap(jsonRes));
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
