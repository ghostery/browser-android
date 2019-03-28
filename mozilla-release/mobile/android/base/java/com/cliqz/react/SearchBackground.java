package com.cliqz.react;

import android.app.Application;
import android.util.SparseArray;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeArray;
import com.facebook.react.bridge.WritableNativeMap;
import com.facebook.react.common.LifecycleState;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.facebook.react.shell.MainReactPackage;

import org.mozilla.gecko.BuildConfig;
import org.mozilla.gecko.util.EventCallback;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Copyright © Cliqz 2019
 */
public class SearchBackground implements ReactInstanceManager.ReactInstanceEventListener {
    private static SearchBackground ourInstance;
    public final ReactInstanceManager mReactInstanceManager;
    private ReactContext mReactContext;
    private final SparseArray<EventCallback> callbacks = new SparseArray<>();
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
                .setInitialLifecycleState(LifecycleState.BEFORE_RESUME)
                .build();
        mReactInstanceManager.addReactInstanceEventListener(this);
        mReactInstanceManager.createReactContextInBackground();
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
        final WritableMap params = new WritableNativeMap();
        // TODO add search params
        getInstance().callAction("search", "startSearch", query, params, getSearchSenderArgument());
    }

    public static void stopSearch() {
        final WritableMap params = new WritableNativeMap();
        params.putString("entryPoint", "browserBar");
        getInstance().callAction("search", "stopSearch", params, getSearchSenderArgument());
    }

    public static void reportSelection(String query, boolean isPrivateMode, boolean isSearchQuery, boolean isAutocompleted) {
        final WritableMap result = new WritableNativeMap();
        result.putInt("index", -1);
        result.putString("url", query);

        final WritableMap param = new WritableNativeMap();
        final String provider = isAutocompleted ? "cliqz" : "instant";
        final String type = isSearchQuery ? "supplementary-search" : "navigate-to";
        param.putString("provider", provider);
        param.putString("type", type);
        param.putBoolean("isFromAutoCompletedUrl", isAutocompleted);
        param.putString("action", "enter");
        param.putString("elementName", "");
        param.putBoolean("isNewTab", false);
        param.putBoolean("isPrivateMode", isPrivateMode);
        param.putBoolean("isPrivateResult", false);
        param.putString("query", query);
        param.putString("url", query);
        param.putMap("rawResult", result);

        getInstance().callAction("search", "reportSelection", param, getSearchSenderArgument());

        stopSearch();
    }

    public static void getBackendCountries(EventCallback callback) {
        getInstance().callAction("search", "getBackendCountries", new Object[] { }, callback);
    }

    public static void setBackendCountry(String code) {
        getInstance().callAction("search", "setBackendCountry", code);
    }

    public static void changeAppearance(String theme) {
        getInstance().callAction("ui", "changeAppearance", theme);
    }

    public static void notifySearchEngineChange() {
        getInstance().mReactContext
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit("SearchEngines:SetDefault", null);
    }

    public static void notifyLocationChange(String url, int tabId) {
        final WritableMap windowTreeInformation =  new WritableNativeMap();
        windowTreeInformation.putInt("tabId", tabId);

        final WritableMap message = new WritableNativeMap();
        message.putString("url", url);
        message.putMap("windowTreeInformation", windowTreeInformation);
        
        getInstance().callAction("core", "notifyLocationChange", message);
    }

    public static void notifyStateChange(String url, int tabId) {
        final WritableMap windowTreeInformation =  new WritableNativeMap();
        windowTreeInformation.putInt("tabId", tabId);

        final WritableMap message = new WritableNativeMap();
        message.putString("urlSpec", url);
        message.putString("originalUrl", url);
        message.putMap("windowTreeInformation", windowTreeInformation);

        getInstance().callAction("core", "notifyStateChange", message);
    }

    @Override
    public void onReactContextInitialized(ReactContext context) {
        mReactContext = context;
    }

    private static ReadableMap getSearchSenderArgument() {
        final WritableMap sender = new WritableNativeMap();
        sender.putString("contextId", "mobile-cards");
        return sender;
    }
}
