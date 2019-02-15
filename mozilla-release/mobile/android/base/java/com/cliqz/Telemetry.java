package com.cliqz;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import org.mozilla.gecko.util.GeckoBundle;

/**
 * Telemetry helper
 *
 * Copyright © Cliqz 2019
 */
public class Telemetry {

    // Groups of all possible telemetry keys
    private final static class Keys {
        static final String TYPE = "type";
        static final String ACTION = "action";
        static final String CONTEXT = "context";
        static final String VIEW = "view";
        static final String TARGET = "target";
        static final String STYLE = "style";
    }

    // Groups of all possible telemetry values
    private static class Values {
        static final String TYPE_ONBOARDING = "onboarding";
        static final String ACTION_CLICK = "click";
        static final String ACTION_SHOW = "show";
        static final String CONTEXT_HOME_CUSTOMIZATION = "home_customization";
        static final String VIEW_HOME = "home" ;
    }

    /**
     * This method sends a show telemetry signal for the "Customize Home"
     * button (the one that sends the user to the settings")
     */
    public static void sendHomeSettingsShowTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.STYLE, ABManager.getInstance().getHomeSettingsStyle());
        sendTelemetry(Values.TYPE_ONBOARDING,
                Values.ACTION_SHOW,
                Values.CONTEXT_HOME_CUSTOMIZATION,
                Values.VIEW_HOME,
                extra);
    }

    /**
     * This method sends the telemetry signal about the user clicking on the "Customize Home"
     * button (the one that sends the user to the settings")
     */
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

    // Generalized send telemetry method with the common required fields
    @SuppressWarnings("SameParameterValue")
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
