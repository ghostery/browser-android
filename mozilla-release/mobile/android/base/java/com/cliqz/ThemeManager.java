package com.cliqz;

import com.cliqz.react.SearchBackground;
/**
 * Copyright © Cliqz 2019
 */
public class ThemeManager {
    public static final String THEME_DARK = "dark";
    public static final String THEME_LIGHT = "light";
    private String mCurrentTheme = THEME_LIGHT;

    private static final ThemeManager ourInstance = new ThemeManager();

    public static ThemeManager getInstance() {
        return ourInstance;
    }

    private ThemeManager() {
    }

    public void changeTheme(String theme) {
        if (mCurrentTheme.equals(theme)) {
            return;
        }
        mCurrentTheme = theme;
        SearchBackground.changeTheme(theme);
    }
}
