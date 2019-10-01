package com.cliqz;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Telemetry helper
 *
 * Copyright © Cliqz 2019
 */
public class Telemetry {

    private static long freshTabShownOn = Long.MAX_VALUE;

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
        static final String TYPE_HOME = "home";
        static final String TYPE_TOOLBAR = "toolbar";
        static final String TYPE_SEARCH = "search";
        static final String TYPE_NAVIGATION = "navigation";
        static final String ACTION_CLICK = "click";
        static final String ACTION_SHOW = "show";
        static final String ACTION_HIDE = "hide";
        static final String CONTEXT_HOME_CUSTOMIZATION = "home_customization";
        static final String VIEW_HOME = "home";
        static final String VIEW_NEWS = "news";
        static final String VIEW_WEB = "web";
        static final String TARGET_SETTINGS = "settings";
    }

    /**
     * This method sends a show telemetry signal for the "Customize Home"
     * button (the one that sends the user to the settings")
     */
    public static void sendHomeSettingsShowTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, Values.VIEW_HOME);
        extra.putString(Keys.STYLE, ABManager.getInstance().getHomeSettingsStyle());
        extra.putString(Keys.CONTEXT, Values.CONTEXT_HOME_CUSTOMIZATION);
        sendTelemetry(Values.TYPE_ONBOARDING, Values.ACTION_SHOW, extra);
    }

    /**
     * This method sends the telemetry signal about the user clicking on the "Customize Home"
     * button (the one that sends the user to the settings")
     */
    public static void sendHomeSettingsClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, Values.VIEW_HOME);
        extra.putString(Keys.TARGET, Values.TARGET_SETTINGS);
        extra.putString(Keys.STYLE, ABManager.getInstance().getHomeSettingsStyle());
        extra.putString(Keys.CONTEXT, Values.CONTEXT_HOME_CUSTOMIZATION);
        sendTelemetry(Values.TYPE_ONBOARDING, Values.ACTION_CLICK, extra);
    }

    public static void sendFreshTabShownTelemetry(PreferenceManager preferenceManager) {
        final GeckoBundle extra = new GeckoBundle();
        extra.putBoolean("is_topsites_on", preferenceManager.isTopSitesEnabled());
        extra.putBoolean("is_news_on", preferenceManager.isNewsEnabled());
        extra.putBoolean("is_background_on", preferenceManager.isBackgroundEnabled());
        extra.putString("theme", preferenceManager.isLightThemeEnabled() ? "lite" : "blue");
        sendTelemetry(Values.TYPE_HOME, Values.ACTION_SHOW, extra);
        freshTabShownOn = System.currentTimeMillis();
    }

    public static void sendFreshTabHiddenTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        long durationFreshTabShown = System.currentTimeMillis() - freshTabShownOn;
        durationFreshTabShown = durationFreshTabShown < 0L ? 0 : durationFreshTabShown;
        freshTabShownOn = Long.MAX_VALUE;
        extra.putLong("show_duration", durationFreshTabShown);
        sendTelemetry(Values.TYPE_HOME, Values.ACTION_HIDE, extra);
    }

    public static void sendTopSiteClickTelemetry(int index, int topSitesCount) {
        final GeckoBundle extra = new GeckoBundle();
        extra.putInt("index", index);
        extra.putInt("topsiteCount", topSitesCount);
        extra.putString(Keys.TARGET, "topsite");
        sendTelemetry(Values.TYPE_HOME, Values.ACTION_CLICK, extra);
    }

    public static void sendTopNewsClickTelemetry(int index, String newsType) {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString("target", newsType);
        extra.putInt("index", index);
        sendTelemetry(Values.TYPE_HOME, Values.ACTION_CLICK, extra);
    }

    public static void sendToggleNewsListTelemetry(boolean isNewsExpanded) {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, Values.VIEW_NEWS);
        extra.putString(Keys.TARGET, isNewsExpanded ? "show" : "hide");
        sendTelemetry(Values.TYPE_HOME, Values.ACTION_CLICK, extra);
    }

    public static void sendToolbarClickTelemetry(String target) {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, TelemetryUtils.getPageViewType());
        extra.putString(Keys.TARGET, target);
        sendTelemetry(Values.TYPE_TOOLBAR, Values.ACTION_CLICK, extra);
    }

    public static void sendSearchBarFocusChangeTelemetry(boolean hasFocus) {
        String action = hasFocus ? "focus" : "blur";
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, TelemetryUtils.getPageViewType());
        sendTelemetry(Values.TYPE_SEARCH, action, extra);
    }

    public static void sendSearchBarCloseClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.TARGET, "close");
        sendTelemetry(Values.TYPE_SEARCH, Values.ACTION_CLICK, extra);
    }

    public static void sendReadModeClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.VIEW, Values.VIEW_WEB);
        extra.putString(Keys.TARGET, "reader");
        sendTelemetry(Values.TYPE_SEARCH, Values.ACTION_CLICK, extra);
    }

    public static void sendBackClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.TARGET, "back");
        sendTelemetry(Values.TYPE_NAVIGATION, Values.ACTION_CLICK, extra);
    }

    public static void sendForwardClickTelemetry() {
        final GeckoBundle extra = new GeckoBundle();
        extra.putString(Keys.TARGET, "forward");
        sendTelemetry(Values.TYPE_NAVIGATION, Values.ACTION_CLICK, extra);
    }

    // Generalized send telemetry method with the common required fields
    @SuppressWarnings("SameParameterValue")
    private static void sendTelemetry(@NonNull String type,
                                      @NonNull String action,
                                      @Nullable GeckoBundle extra) {
        final GeckoBundle signal = extra != null ? new GeckoBundle(extra) : new GeckoBundle();
        signal.putString(Keys.TYPE, type);
        signal.putString(Keys.ACTION, action);

        SystemAddon.sendTelemetry(signal);
    }

}
