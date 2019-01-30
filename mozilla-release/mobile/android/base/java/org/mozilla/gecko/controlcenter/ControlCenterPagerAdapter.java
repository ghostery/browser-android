package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.BundleEventListener;
import org.mozilla.gecko.util.EventCallback;
import org.mozilla.gecko.util.GeckoBundle;

import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class ControlCenterPagerAdapter extends FragmentPagerAdapter
        implements BaseControlCenterPagerAdapter, BundleEventListener,
        GlobalTrackersFragment.GlobalTrackersCallback {

    private Context mContext;
    private final List<ControlCenterFragment> mFragmentList = new ArrayList<>();

    private GeckoBundle mData = new GeckoBundle();
    private byte[] mCCDataHash = null;

    private boolean mIsGlobalTrackingDataChanged = false;

    @Override
    public void trackingDataChanged() {
        mIsGlobalTrackingDataChanged = true;
    }

    public ControlCenterPagerAdapter(FragmentManager fm, Context context) {
        super(fm);
        this.mContext = context;

        EventDispatcher.getInstance().registerUiThreadListener(this,
                "Privacy:Info", null);
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
        globalTrackersFragment.setGlobalTrackersCallback(this);
        mFragmentList.add(overviewFragment);
        mFragmentList.add(siteTrackersFragment);
        mFragmentList.add(globalTrackersFragment);
    }

    public void setTrackingData(final GeckoBundle message) {
        for (ControlCenterFragment fragment : mFragmentList) {
            fragment.updateUI(message);
        }
    }

    public void updateCurrentView(int position) {
        mFragmentList.get(position).refreshUI();
    }

    @Override
    public void handleMessage(String event, GeckoBundle message, EventCallback callback) {
        if ("Privacy:Info".equals(event)) {
            this.mCCDataHash = ControlCenterUtils.getCCDataHash(message);
            this.mData = message;
            setTrackingData(message);
        }
    }

    public boolean isDataChanged() {
        if (mIsGlobalTrackingDataChanged) {
            mIsGlobalTrackingDataChanged = false;
            return true;
        }
        return mCCDataHash != null && !MessageDigest.isEqual(mCCDataHash, ControlCenterUtils.getCCDataHash(mData));
    }

}
