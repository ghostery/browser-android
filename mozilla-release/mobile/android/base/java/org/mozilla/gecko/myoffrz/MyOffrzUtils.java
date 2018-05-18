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
}
