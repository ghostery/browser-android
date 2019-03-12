package com.cliqz;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2019
 */
public class SystemAddon {
    public static final String EXTENSION_ID = "firefox@ghostery.com";
    public static final String MODULE_CORE = "core";
    public static final String ACTION_SEND_TELEMETRY = "sendTelemetry";

    private static final String KEY_MODULE = "module";
    private static final String KEY_ARGS = "args";
    private static final String MESSAGE_NAME = "SystemAddon:Request";
    private static final String MODULE_ABTESTS_LEGACY = "abtests-legacy";
    private static final String ACTION_GET_RUNNING_TESTS = "getRunningTests";
    private static final String KEY_DATA = "data";
    private static final String ACTION_STATUS = "status";
    private static final String KEY_ACTION = "action";

    private SystemAddon() {}

    public static void sendTelemetry(GeckoBundle signal, String schema) {
        sendMessage(MODULE_CORE, ACTION_SEND_TELEMETRY, new GeckoBundle[] { wrap(signal), wrap(false), wrap(schema) });
    }

    public static void sendTelemetry(GeckoBundle signal) {
        sendMessage(MODULE_CORE, ACTION_SEND_TELEMETRY, new GeckoBundle[] { wrap(signal) });
    }

    public static void getABTests(EventCallback callback) {
        sendMessage(MODULE_ABTESTS_LEGACY, ACTION_GET_RUNNING_TESTS,  callback);
    }

    public static void getStatus(EventCallback callback) {
        sendMessage(MODULE_CORE, ACTION_STATUS, callback);
    }

    private static void sendMessage(String module, String action, GeckoBundle[] args, EventCallback callback) {
        final GeckoBundle data = new GeckoBundle();
        data.putString(KEY_MODULE, module);
        data.putString(KEY_ACTION, action);
        data.putBundleArray(KEY_ARGS, args);
        EventDispatcher.getInstance().dispatch(MESSAGE_NAME, data, callback);
    }

    private static void sendMessage(String module, String action, EventCallback callback) {
        final GeckoBundle data = new GeckoBundle();
        data.putString(KEY_MODULE, module);
        data.putString(KEY_ACTION, action);
        data.putBundleArray(KEY_ARGS, new GeckoBundle[] {});
        EventDispatcher.getInstance().dispatch(MESSAGE_NAME, data, callback);
    }

    private static void sendMessage(String module, String action, GeckoBundle[] args) {
        final GeckoBundle data = new GeckoBundle();
        data.putString(KEY_MODULE, module);
        data.putString(KEY_ACTION, action);
        data.putBundleArray(KEY_ARGS, args);
        EventDispatcher.getInstance().dispatch(MESSAGE_NAME, data);
    }

    private static GeckoBundle wrap(GeckoBundle bundle) {
        final GeckoBundle wrappingBundle = new GeckoBundle();
        wrappingBundle.putBundle(KEY_DATA, bundle);
        return wrappingBundle;
    }

    private static GeckoBundle wrap(String string) {
        final GeckoBundle wrappingBundle = new GeckoBundle();
        wrappingBundle.putString(KEY_DATA, string);
        return wrappingBundle;
    }

    private static GeckoBundle wrap(boolean bool) {
        final GeckoBundle wrappingBundle = new GeckoBundle();
        wrappingBundle.putBoolean(KEY_DATA, bool);
        return wrappingBundle;
    }
}
