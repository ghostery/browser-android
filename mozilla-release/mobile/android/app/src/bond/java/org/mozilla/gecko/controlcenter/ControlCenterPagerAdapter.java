package org.mozilla.gecko.controlcenter;


import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import org.mozilla.gecko.util.GeckoBundle;

public class ControlCenterPagerAdapter extends FragmentPagerAdapter
        implements BaseControlCenterPagerAdapter {

    public ControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        return null;
    }

    @Override
    public int getCount() {
        return 0;
    }

    @Override
    public void init(BaseControlCenterPagerAdapter.ControlCenterCallbacks controlCenterCallbacks) {

    }

    @Override
    public void setTrackingData(GeckoBundle message) {

    }

    @Override
    public void updateCurrentView(int position) {

    }
}