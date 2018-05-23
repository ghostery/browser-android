package org.mozilla.gecko.preferences;

import android.content.Context;
import android.content.SharedPreferences;
import org.mozilla.gecko.GeckoSharedPrefs;
import static org.mozilla.gecko.myoffrz.MyOffrzUtils.isMyOffrzSupportedForLang;
/**
 * Copyright © Cliqz 2018
 */

public class PreferenceManager {

    final SharedPreferences mAppSharedPreferences;

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

    public boolean isMyOffrzEnable() {
        return mAppSharedPreferences.getBoolean(GeckoPreferences.PREFS_SHOW_MYOFFRZ, isMyOffrzSupportedForLang());
    }

    public boolean isBlockAdsEnabled() {
        return  mAppSharedPreferences.getBoolean(GeckoPreferences
                .PREFS_BLOCK_ADS,true);
    }
}
