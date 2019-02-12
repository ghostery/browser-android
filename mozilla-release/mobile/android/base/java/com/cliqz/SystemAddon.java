package com.cliqz;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;

public class SystemAddon {
    private SystemAddon() {

    }

    public static void sendTelemetry(GeckoBundle signal, String schema) {
        final GeckoBundle signalBundle = new GeckoBundle();
        signalBundle.putBundle("data", signal);
        final GeckoBundle schemaBundle = new GeckoBundle();
        schemaBundle.putString("data", schema);
        sendMessage("core", "sendTelemetry", new GeckoBundle[] { signalBundle, schemaBundle });
    }

    public static void sendTelemetry(GeckoBundle signal) {
        final GeckoBundle signalBundle = new GeckoBundle();
        signalBundle.putBundle("data", signal);
        sendMessage("core", "sendTelemetry", new GeckoBundle[] { signalBundle });
    }

    public static void getABTests(EventCallback callback) {
        sendMessage("abtests-legacy", "getRunningTests", new GeckoBundle[] {}, callback);
    }

    public static void getStatus(EventCallback callback) {
        sendMessage("core", "status", new GeckoBundle[] {}, callback);
    }

    private static void sendMessage(String module, String action, GeckoBundle[] args, EventCallback callback) {
        final GeckoBundle data = new GeckoBundle(1);
        data.putString("module", module);
        data.putString("action", action);
        data.putBundleArray("args", args);
        EventDispatcher.getInstance().dispatch("SystemAddon:Request", data, callback);
    }

    private static void sendMessage(String module, String action, GeckoBundle[] args) {
        final GeckoBundle data = new GeckoBundle(1);
        data.putString("module", module);
        data.putString("action", action);
        data.putBundleArray("args", args);
        EventDispatcher.getInstance().dispatch("SystemAddon:Request", data);
    }
}
