package com.cliqz;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import org.mozilla.gecko.util.GeckoBundle;

public class Telemetry {
    private final static class Keys {
        static final String TYPE = "type";
        static final String ACTION = "action";
        static final String CONTEXT = "context";
        static final String VIEW = "view";
        static final String TARGET = "target";
        static final String STYLE = "style";
    }

    public static class Values {
        public static final String TYPE_ONBOARDING = "onboarding";
        public static final String ACTION_CLICK = "click";
        public static final String CONTEXT_HOME_CUSTOMIZATION = "home_customization";
        public static final String VIEW_HOME = "home" ;
    }

    public static void sendHomeSettingsClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.TARGET, Values.ACTION_CLICK);
        extra.putString(Keys.STYLE, ABManager.getInstance().getHomeSettingsStyle());
        sendTelemetry(Values.TYPE_ONBOARDING,
                Values.ACTION_CLICK,
                Values.CONTEXT_HOME_CUSTOMIZATION,
                Values.VIEW_HOME,
                extra);
    }

    /**
     *
     * @param type Type
     * @param action Action
     * @param context Context
     * @param view View
     * @param extra Extra
     */
    private static void sendTelemetry(@NonNull String type,
                                      @NonNull String action,
                                      @NonNull String context,
                                      @NonNull String view,
                                      @Nullable GeckoBundle extra) {
        final GeckoBundle signal = extra != null ? new GeckoBundle(extra) : new GeckoBundle();
        signal.putString(Keys.TYPE, type);
        signal.putString(Keys.ACTION, action);
        signal.putString(Keys.CONTEXT, context);
        signal.putString(Keys.VIEW, view);

        SystemAddon.sendTelemetry(signal);
    }
}
