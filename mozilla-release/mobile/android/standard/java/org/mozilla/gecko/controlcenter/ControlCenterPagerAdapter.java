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
public class ControlCenterPagerAdapter extends FragmentPagerAdapter
        implements BaseControlCenterPagerAdapter {

    private Context mContext;
    private final List<ControlCenterFragment> mFragmentList = new ArrayList<>();

    public ControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm);
        this.mContext = context;
    }

    @Override
    public Fragment getItem(int position) {
        return mFragmentList.get(position);
    }

    @Override
    public int getCount() {
        return mFragmentList.size();
    }

    @Override
    public CharSequence getPageTitle(int position) {
        return mFragmentList.get(position).getTitle(mContext);
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
    
    @Override
    public void setTrackingData(final GeckoBundle message) {
        for(ControlCenterFragment fragment : mFragmentList) {
            fragment.updateUI(message);
        }
    }

    @Override
    public void updateCurrentView(int position) {
        mFragmentList.get(position).refreshUI();
    }
}
