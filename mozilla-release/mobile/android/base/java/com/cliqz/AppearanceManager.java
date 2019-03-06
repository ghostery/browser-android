package com.cliqz;

import com.cliqz.react.SearchBackground;
/**
 * Copyright © Cliqz 2019
 */
public class AppearanceManager {
    public static final String APPEARANCE_DARK = "dark";
    public static final String APPEARANCE_LIGHT = "light";
    private String mCurrentApearance = APPEARANCE_LIGHT;

    private static final AppearanceManager ourInstance = new AppearanceManager();

    public static AppearanceManager getInstance() {
        return ourInstance;
    }

    private AppearanceManager() {
    }

    public String getCurrent() {
        return mCurrentApearance;
    }

    public void change(String appearance) {
        if (mCurrentApearance.equals(appearance)) {
            return;
        }
        mCurrentApearance = appearance;
        SearchBackground.changeAppearance(appearance);
    }
}
