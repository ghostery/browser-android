package com.cliqz.react.modules;

import android.util.Log;

import com.cliqz.AppearanceManager;
import com.cliqz.SystemAddon;
import com.cliqz.react.SearchBackground;
import com.cliqz.react.Utils;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.PrefsHelper.PrefHandlerBase;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import static com.cliqz.react.modules.PrefsModule.PREFS_SEARCH_CARDS_LAYOUT;

/**
 * Copyright © Cliqz 2019
 */
public class BridgeModule extends ReactContextBaseJavaModule implements BundleEventListener {

    private static final String EVENT_SEARCH_SEARCH = "Search:Search";
    private static final String EVENT_SEARCH_OPEN_LINK = "Search:OpenLink";
    private static final String EVENT_CONTENT_LOCATION_CHANGE = "Content:LocationChange";
    private static final String EVENT_CONTENT_STATE_CHANGE = "Content:StateChange";
    private final ReactApplicationContext mReactContext;
    private String[] EVENTS = new String[] {
            EVENT_SEARCH_SEARCH,
            EVENT_SEARCH_OPEN_LINK,
            EVENT_CONTENT_LOCATION_CHANGE,
            EVENT_CONTENT_STATE_CHANGE,
    };

    public BridgeModule(ReactApplicationContext reactContext) {
        super(reactContext);
        mReactContext = reactContext;
    }

    @Override
    public String getName() {
        return "Bridge";
    }

    @Override
    public void initialize() {
        EventDispatcher.getInstance().registerUiThreadListener(this, EVENTS);
    }

    @Override
    public boolean canOverrideExistingModule() {
        return false;
    }

    @Override
    public void onCatalystInstanceDestroy() {
        EventDispatcher.getInstance().unregisterUiThreadListener(this, EVENTS);
    }

    @Override
    public void handleMessage(String event, GeckoBundle message, EventCallback callback) {
        switch (event) {
            case EVENT_SEARCH_SEARCH:
                final String query = GeckoBundleUtils.safeGetString(message, "q");
                SearchBackground.startSearch(query);
                break;
            case EVENT_SEARCH_OPEN_LINK:
                SearchBackground.stopSearch();
                break;
            case EVENT_CONTENT_LOCATION_CHANGE:
                SearchBackground.notifyLocationChange(message.getString("uri"), message.getInt("tabID"));
                break;
            case EVENT_CONTENT_STATE_CHANGE:
                // TODO: disabled for now - not sure if needed
                // SearchBackground.notifyStateChange(message.getString("uri"), message.getInt("tabID"));
                break;
            default:
                Log.w(getClass().getSimpleName(), "Unknown event " + event);
                break;
        }
    }

    @ReactMethod
    public void replyToAction(int hash, ReadableMap response) {
        SearchBackground.replyToAction(hash, response);
    }

    /**
     * Config has to be in format:
     *
     * {
     *     "appearance": "light|dark",
     *     "layout": "vertical|horizontal"
     * }
     *
     */
    @ReactMethod
    public void getConfig(final Promise promise) {
        final WritableMap config = Arguments.createMap();
        config.putString("appearance", AppearanceManager.getInstance().getCurrent());

        PrefsHelper.getPrefs(new String[] { PREFS_SEARCH_CARDS_LAYOUT }, new PrefHandlerBase() {
            @Override
            public void prefValue(String prefName, boolean value) {
            }

            @Override
            public void prefValue(String prefName, int value) {
            }

            @Override
            public void prefValue(String prefName, String value) {
                if (prefName.equals(PREFS_SEARCH_CARDS_LAYOUT)) {
                    config.putString("layout", value);
                }
            }

            @Override
            public void finish() {
                promise.resolve(config);
            }
        });
    }

    @ReactMethod
    public void sendExternalMessage(String extensionId, ReadableMap message) {
        if (!SystemAddon.EXTENSION_ID.equals(extensionId)) {
            Log.w(getClass().getSimpleName(), "external message to unknown extension id " + extensionId);
            return;
        }

        final String module = message.getString("moduleName");
        final String action = message.getString("action");
        final ReadableArray args = message.getArray("args");

        if (SystemAddon.MODULE_CORE.equals(module) && SystemAddon.ACTION_SEND_TELEMETRY.equals(action)) {
            final GeckoBundle signal = Utils.convertReadableMapToGeckoBundle(args.getMap(0));
            final String schema = args.getString(2);

            if (schema != null) {
                SystemAddon.sendTelemetry(signal, schema);
            } else {
                SystemAddon.sendTelemetry(signal);
            }
        }
    }
}
