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
 *
 * Abstraction layer for Control center adapter for different flavours
 */
public abstract class BaseControlCenterPagerAdapter extends FragmentPagerAdapter  {

    protected Context mContext;
    protected final List<ControlCenterFragment> mFragmentList = new ArrayList<>();

    public BaseControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm);
        mContext = context;
    }

    public Fragment getItem(int position) {
        return mFragmentList.get(position);
    }

    public int getCount() {
        return mFragmentList.size();
    }

    public CharSequence getPageTitle(int position) {
        return mFragmentList.get(position).getTitle(mContext);
    }

    public void setTrackingData(final GeckoBundle message) {
        for(ControlCenterFragment fragment : mFragmentList) {
            fragment.updateUI(message);
        }
    }

    public void updateCurrentView(int position) {
        mFragmentList.get(position).refreshUI();
    }

    abstract void init(ControlCenterCallbacks controlCenterCallbacks);

    public interface ControlCenterCallbacks {

        void hideControlCenter();

        void controlCenterSettingsChanged();
    }
}
