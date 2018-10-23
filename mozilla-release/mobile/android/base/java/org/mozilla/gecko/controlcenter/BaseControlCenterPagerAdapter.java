package org.mozilla.gecko.controlcenter;

import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 *
 * Abstraction layer for Control center adapter for different flavours
 */
public interface BaseControlCenterPagerAdapter {

    void init(ControlCenterCallbacks controlCenterCallbacks);

    void setTrackingData(GeckoBundle message);

    void updateCurrentView(int position);

    interface ControlCenterCallbacks {

        void hideControlCenter();

        void controlCenterSettingsChanged();
    }

}
