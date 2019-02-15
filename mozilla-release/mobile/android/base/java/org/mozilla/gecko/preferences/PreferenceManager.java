package org.mozilla.gecko.preferences;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.PrefsHelper;

import static org.mozilla.gecko.myoffrz.MyOffrzUtils.isMyOffrzSupportedForLang;

/**
 * Copyright © Cliqz 2018
 */
public class PreferenceManager {

    private final SharedPreferences mAppSharedPreferences;
    @SuppressLint("StaticFieldLeak")
    private static PreferenceManager preferenceManager = null;
    private Context mContext;

    private PreferenceManager(Context context) {
        mAppSharedPreferences = GeckoSharedPrefs.forApp(context);
        mContext = context;
    }

    public static void init(Context context) {
        if (preferenceManager == null) {
            preferenceManager = new PreferenceManager(context.getApplicationContext());
        }
    }

    public static PreferenceManager getInstance() {
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

    boolean isGhosteryAutoUpdateEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_AUTO_UPDATE, true);
    }

    boolean areFirstPartyTrackersAllowed() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_ALLOW_FIRST_PARTY, true);
    }

    boolean areNewTrackersBlocked() {
        return  mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_GHOSTERY_BLOCK_NEW_TRACKERS, false);
    }

    public boolean isQuickSearchEnabled(){
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SEARCH_QUICKSEARCH_ENABLED,true);
    }

    boolean isHumanWebEnabled(){
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
        return  mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_BACKGROUND_ENABLED,false);
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

    public boolean isLightThemeEnabled() {
        return !isBlueThemeEnabled();
    }

    public boolean isBlueThemeEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_BLUE_THEME,true);
    }
}
