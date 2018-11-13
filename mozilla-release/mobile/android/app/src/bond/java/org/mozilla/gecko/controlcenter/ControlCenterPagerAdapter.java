package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.support.v4.app.FragmentManager;

public class ControlCenterPagerAdapter extends BaseControlCenterPagerAdapter{

    public ControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm, context);
    }

    @Override
    public void init(BaseControlCenterPagerAdapter.ControlCenterCallbacks controlCenterCallbacks) {
        final DashboardTodayFragment dashboardTodayFragment = new DashboardTodayFragment();
        final DashboardWeekFragment dashboardWeekFragment = new DashboardWeekFragment();
        mFragmentList.add(dashboardTodayFragment);
        mFragmentList.add(dashboardWeekFragment);
    }
}