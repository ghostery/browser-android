package com.cliqz.react;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.common.LifecycleState;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.facebook.react.shell.MainReactPackage;

import org.mozilla.gecko.BuildConfig;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.preferences.GeckoPreferences;
import org.mozilla.gecko.util.EventCallback;

import android.app.Application;
import android.util.SparseArray;

import java.util.concurrent.atomic.AtomicInteger;

public class SearchBackground implements ReactInstanceManager.ReactInstanceEventListener {
    private static SearchBackground ourInstance;
    public final ReactInstanceManager mReactInstanceManager;
    private ReactContext mReactContext;
    private SparseArray<EventCallback> callbacks = new SparseArray<>();
    AtomicInteger idGenerator = new AtomicInteger(1);

    public static SearchBackground getInstance() {
        return ourInstance;
    }

    public static void initialize(Application application) {
        ourInstance = new SearchBackground(application);
    }

    private SearchBackground(Application application) {
        mReactInstanceManager = ReactInstanceManager.builder()
                .setApplication(application)
                .setBundleAssetName("index.android.bundle")
                .setJSMainModulePath("index")
                .addPackage(new BridgePackage())
                .addPackage(new MainReactPackage())
                .setUseDeveloperSupport(BuildConfig.DEBUG)
                .setInitialLifecycleState(LifecycleState.RESUMED)
                .build();
        mReactInstanceManager.addReactInstanceEventListener(this);
        mReactInstanceManager.createReactContextInBackground();
        migrate();
    }

    private void migrate() {
        // Let Search handle search lanaguge
        PrefsHelper.getPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, new PrefsHelper.PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, String countryCode) {
                if (!(countryCode instanceof String) || countryCode.equals("")) {
                    return;
                }
                PrefsHelper.setPref(GeckoPreferences.PREFS_SEARCH_REGIONAL, "");
                setBackendCountry(countryCode);
            }
        });
    }

    public void showDevOptionsDialog() {
        mReactInstanceManager.showDevOptionsDialog();
    }

    private void callAction(String module, String action, Object... args) {
        final WritableMap eventBody = Arguments.createMap();
        eventBody.putString("module", module);
        eventBody.putString("action", action);
        eventBody.putArray("args", Arguments.fromJavaArgs(args));
        mReactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit("action", eventBody);
    }

    private void callAction(String module, String action, Object[] args, EventCallback callback) {
        final int id = idGenerator.getAndIncrement();
        callbacks.append(id, callback);
        final WritableMap eventBody = Arguments.createMap();
        eventBody.putInt("id", id);
        eventBody.putString("module", module);
        eventBody.putString("action", action);
        eventBody.putArray("args", Arguments.fromJavaArgs(args));
        mReactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class).emit("action", eventBody);
    }

    public static void replyToAction(int hash, ReadableMap response) {
        EventCallback callback = SearchBackground.getInstance().callbacks.get(hash);
        if (callback != null) {
            callback.sendSuccess(response);
        }
    }

    public static void startSearch(String query) {
        getInstance().callAction("search", "startSearch", new Object[] { query });
    }


    public static void getBackendCountries(EventCallback callback) {
        getInstance().callAction("search", "getBackendCountries", new Object[] { }, callback);
    }

    public static void setBackendCountry(String code) {
        getInstance().callAction("search", "setBackendCountry", new Object[] { code });
    }

    @Override
    public void onReactContextInitialized(ReactContext context) {
        mReactContext = context;
    }
}
