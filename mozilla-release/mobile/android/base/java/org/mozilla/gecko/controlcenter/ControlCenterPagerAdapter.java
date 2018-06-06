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
public class ControlCenterPagerAdapter extends FragmentPagerAdapter {

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

    public void addFragment(ControlCenterFragment fragment) {
        mFragmentList.add(fragment);
    }

    public void setTrackingData(final GeckoBundle message) {
        for(ControlCenterFragment fragment : mFragmentList) {
            fragment.updateUI(message);
        }
    }

    public void updateCurrentView(int position) {
        mFragmentList.get(position).refreshUI();
    }
}
