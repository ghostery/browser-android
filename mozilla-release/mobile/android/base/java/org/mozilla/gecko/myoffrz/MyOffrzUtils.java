package org.mozilla.gecko.myoffrz;

import android.content.Context;
import android.content.SharedPreferences;

import org.mozilla.gecko.BrowserLocaleManager;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.LocaleManager;
import org.mozilla.gecko.preferences.GeckoPreferences;

import java.util.Locale;

/**
 * Copyright © Cliqz 2018
 */

public class MyOffrzUtils {
    public static boolean isMyOffrzSupportedForLang(){
        final LocaleManager localeManager = BrowserLocaleManager.getInstance();
        final Locale locale = localeManager.getDefaultSystemLocale();
        return Locale.GERMAN.getLanguage().equals(locale.getLanguage());
    }

    // This part is derived from @{@link TabQueueHelper}.java
    public static boolean isMyOffrzEnable(Context context){
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(context);
        return  prefs.getBoolean(GeckoPreferences.PREFS_SHOW_MYOFFRZ,isMyOffrzSupportedForLang());
    }
}
