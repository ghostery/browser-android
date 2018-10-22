package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import org.mozilla.gecko.util.GeckoBundle;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class ControlCenterPagerAdapter extends BaseControlCenterPagerAdapter {

    public ControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm,context);
    }

    @Override
    public void init(ControlCenterCallbacks controlCenterCallbacks) {
        final SiteTrackersFragment siteTrackersFragment = new SiteTrackersFragment();
        final GlobalTrackersFragment globalTrackersFragment = new GlobalTrackersFragment();
        final OverviewFragment overviewFragment = new OverviewFragment();
        siteTrackersFragment.setControlCenterCallback(controlCenterCallbacks);
        globalTrackersFragment.setControlCenterCallback(controlCenterCallbacks);
        overviewFragment.setControlCenterCallback(controlCenterCallbacks);
        mFragmentList.add(overviewFragment);
        mFragmentList.add(siteTrackersFragment);
        mFragmentList.add(globalTrackersFragment);
    }
}
