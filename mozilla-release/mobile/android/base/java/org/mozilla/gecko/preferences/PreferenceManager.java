package org.mozilla.gecko.preferences;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;
import org.mozilla.gecko.cliqztelemetry.Telemetry;

import static org.mozilla.gecko.myoffrz.MyOffrzUtils.isMyOffrzSupportedForLang;

/**
 * Copyright © Cliqz 2018
 */
public class PreferenceManager {

    private static final String SESSION_ID = "sessionid";
    private static final String TELEMETRY_SEQUENCE = "telemetrysequence";
    private static final String TIME_OF_LAST_SIGNAL = "timelastenvsignal";
    private final SharedPreferences mAppSharedPreferences;
    private static PreferenceManager preferenceManager = null;
    private Context mContext;

    private PreferenceManager(Context context) {
        mAppSharedPreferences = GeckoSharedPrefs.forApp(context);
        mContext = context;
    }

    public static PreferenceManager getInstance(Context context) {
        if (preferenceManager == null) {
            preferenceManager = new PreferenceManager(context);
        }
        return preferenceManager;
    }

    public void setMyOffrzOnboardingEnabled(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_MYOFFRZ_ONBOARDING_ENABLED, value).apply();
    }

    public boolean isMyOffrzOnboardingEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_MYOFFRZ_ONBOARDING_ENABLED,
                true);
    }

    public void setGhosteryAutoUpdate(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_GHOSTERY_AUTO_UPDATE, value).apply();
    }

    public void setAllowFirstPartyTrackers(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_GHOSTERY_ALLOW_FIRST_PARTY, value).apply();
    }

    public void setBlockNewTrackers(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_GHOSTERY_BLOCK_NEW_TRACKERS, value).apply();
    }

    public boolean isGhosteryAutoUpdateEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_AUTO_UPDATE, true);
    }

    public boolean areFirstPartyTrackersAllowed() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_ALLOW_FIRST_PARTY, true);
    }

    public boolean areNewTrackersBlocked() {
        return  mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_BLOCK_NEW_TRACKERS, false);
    }

    public boolean isQuickSearchEnabled(){
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SEARCH_QUICKSEARCH_ENABLED,true);
    }

    public boolean isHumanWebEnabled(){
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_ENABLE_HUMAN_WEB,true);
    }

    public void setHumanWebEnabled(boolean value){
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_ENABLE_HUMAN_WEB, value).apply();
    }

    public void registerOnSharedPreferenceChangeListener(OnSharedPreferenceChangeListener listener) {
        mAppSharedPreferences.registerOnSharedPreferenceChangeListener(listener);
    }

    public void unregisterOnSharedPreferenceChangeListener(OnSharedPreferenceChangeListener listener) {
        mAppSharedPreferences.unregisterOnSharedPreferenceChangeListener(listener);
    }

    public boolean isQuerySuggestionsEnabled(){
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SEARCH_QUERY_SUGGESTIONS,
                true);
    }

    public boolean isAutocompleteEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_AUTO_COMPLETE, true);
    }

    public boolean isBackgroundEnabled() {
        return  mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_BACKGROUND_ENABLED,true);
    }

    public boolean shouldShowCustomizeTabView() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SHOW_CUSTOMIZE_TAB_VIEW, false);
    }

    public void setShowCustomizeTabView(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_SHOW_CUSTOMIZE_TAB_VIEW, value).apply();
    }

    public boolean shouldShowCustomizeTabSnackBar() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SHOW_CUSTOMIZE_TAB_SNACKBAR, true);
    }

    public void setShowCustomizeTabSnackBar(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_SHOW_CUSTOMIZE_TAB_SNACKBAR, value).apply();
    }

    public boolean isNewsViewExpanded() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_NEWS_EXPANDED,
                GeckoPreferences.PREFS_DEFAULT_NEWS_VIEW_EXPANDED);
    }

    public void setNewsViewExpanded(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_NEWS_EXPANDED, value).apply();
    }

    public String getSessionId() {
        return mAppSharedPreferences.getString(SESSION_ID, "");
    }

    public void setSessionId(String sessionId) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putString(SESSION_ID, sessionId).apply();
    }

    public int getAutoIncrementSequenceNumber() {
        final int sequenceNumber = mAppSharedPreferences.getInt(TELEMETRY_SEQUENCE, 0);
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putInt(TELEMETRY_SEQUENCE, (sequenceNumber + 1) % Integer.MAX_VALUE).apply();
        return sequenceNumber;
    }
    
    public void setTimeOfLastEnvSignal(long time) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putLong(TIME_OF_LAST_SIGNAL, time).apply();
    }

    public long getTimeOfLastEnvSignal() {
        return mAppSharedPreferences.getLong(TIME_OF_LAST_SIGNAL, 0);
    }

    public boolean isLightThemeEnabled() {
        return !isBlueThemeEnabled();
    }

    public boolean isBlueThemeEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_BLUE_THEME,true);
    }
}
