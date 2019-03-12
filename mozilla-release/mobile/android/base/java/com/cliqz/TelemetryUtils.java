package com.cliqz;

import org.mozilla.gecko.AboutPages;
import org.mozilla.gecko.Tab;
import org.mozilla.gecko.Tabs;

/**
 * Copyright © Cliqz 2019
 */
public class TelemetryUtils {

    static String getPageViewType() {
        final Tab selectedTab = Tabs.getInstance().getSelectedTab();
        if (selectedTab != null) {
            return AboutPages.isAboutHome(selectedTab.getURL()) ? "home" : "web";
        } else {
            return "home";
        }
    }
}
