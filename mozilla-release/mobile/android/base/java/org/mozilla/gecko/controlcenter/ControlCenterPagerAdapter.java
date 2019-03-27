package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import com.cliqz.SystemAddon;
import com.cliqz.react.SearchBackground;

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
        implements BaseControlCenterPagerAdapter,
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
        //mFragmentList.add(globalTrackersFragment);
    }

    public void updateInfo() {
        SystemAddon.getABTests(new EventCallback() {
            @Override
            public void sendSuccess(Object response) {
                System.out.print("test");
                final GeckoBundle message = (GeckoBundle) response;
            }

            @Override
            public void sendError(Object response) {

            }
        });
        SystemAddon.getPrivacyInfo(new EventCallback() {
            @Override
            public void sendSuccess(Object response) {
                final GeckoBundle message = (GeckoBundle) response;
                for (ControlCenterFragment fragment : mFragmentList) {
                    mCCDataHash = ControlCenterUtils.getCCDataHash(message);
                    mData = message;
                    fragment.updateUI(message);
                }
            }

            @Override
            public void sendError(Object response) {

            }
        });
    }

    public void updateCurrentView(int position) {
        mFragmentList.get(position).refreshUI();
    }

    public boolean isDataChanged() {
        if (mIsGlobalTrackingDataChanged) {
            mIsGlobalTrackingDataChanged = false;
            return true;
        }
        return mCCDataHash != null && !MessageDigest.isEqual(mCCDataHash, ControlCenterUtils.getCCDataHash(mData));
    }

}
