package org.mozilla.gecko.preferences;

import android.content.Context;
import android.content.SharedPreferences;
import org.mozilla.gecko.GeckoSharedPrefs;
import static org.mozilla.gecko.myoffrz.MyOffrzUtils.isMyOffrzSupportedForLang;
/**
 * Copyright © Cliqz 2018
 */

public class PreferenceManager {

    private final SharedPreferences mAppSharedPreferences;

    public PreferenceManager(Context context) {
        mAppSharedPreferences = GeckoSharedPrefs.forApp(context);
    }

    public void setMyOffrzOnboardingEnabled(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.IS_MYOFFRZ_ONBOARDING_ENABLED, value).apply();
    }

    public boolean isMyOffrzOnboardingEnabled() {
        return  mAppSharedPreferences.getBoolean(GeckoPreferences.IS_MYOFFRZ_ONBOARDING_ENABLED,true);
    }

    public void setMyOffrzEnabled(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_SHOW_MYOFFRZ, value).apply();
    }

    public void setTelemetryEnabled(boolean value) {
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.CLIQZ_TELEMETRY_ENABLED, value).apply();
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

    public boolean isTelemetryEnabled() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.CLIQZ_TELEMETRY_ENABLED, false);
    }

    public boolean isMyOffrzEnable() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SHOW_MYOFFRZ, isMyOffrzSupportedForLang());
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
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREF_SEARCH_ENABLE_BROWSER_QUICKSEARCH,true);
    }

    public boolean isHumanWebEnabled(){
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_ENABLE_HUMAN_WEB,true);
    }

    public void setHumanWebEnabled(boolean value){
        final SharedPreferences.Editor editor = mAppSharedPreferences.edit();
        editor.putBoolean(GeckoPreferences.PREFS_ENABLE_HUMAN_WEB, value).apply();
    }
}
