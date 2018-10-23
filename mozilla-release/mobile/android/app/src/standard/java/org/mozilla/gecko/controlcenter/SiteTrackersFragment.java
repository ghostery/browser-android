package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.PopupMenu;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.Arrays;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class SiteTrackersFragment extends ControlCenterFragment implements View.OnClickListener, PopupMenu.OnMenuItemClickListener{

    private SiteTrackersListAdapter mTrackerListAdapter;
    private BaseControlCenterPagerAdapter.ControlCenterCallbacks mControlCenterCallbacks;
    private GeckoBundle mControlCenterData;
    private View mOverlay;

    public SiteTrackersFragment() {
    }

    public void setControlCenterCallback(BaseControlCenterPagerAdapter.ControlCenterCallbacks callback) {
        mControlCenterCallbacks = callback;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.ghostery_site_trackers_fragment, container, false);
        final ExpandableListView mTrackersList = (ExpandableListView) view.findViewById(R.id.trackers_list);
        mOverlay = view.findViewById(R.id.disabled_overlay);
        mTrackersList.setOnGroupExpandListener(new ExpandableListView.OnGroupExpandListener() {
            int previousExpandedGroup = -1;
            @Override
            public void onGroupExpand(int groupPosition) {
                if ((previousExpandedGroup != -1) && (previousExpandedGroup != groupPosition)) {
                    mTrackersList.collapseGroup(previousExpandedGroup);
                }
                previousExpandedGroup = groupPosition;
            }
        });
        View mOverflowMenu = view.findViewById(R.id.overflow_menu);
        mOverflowMenu.setOnClickListener(this);
        mTrackerListAdapter = new SiteTrackersListAdapter(getContext(), mControlCenterCallbacks);
        mTrackersList.setAdapter(mTrackerListAdapter);
        return view;
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.cc_title_site_trackers);
    }

    @Override
    public void updateUI(GeckoBundle data) {
        mControlCenterData = data;
        if (mTrackerListAdapter == null) {
            return;
        }
        mTrackerListAdapter.setData(data);
        shouldShowOverlay();
    }

    @Override
    public void refreshUI() {
        if (getView() == null) {
            return; //return if view is not inflated yet
        }
        mTrackerListAdapter.notifyDataSetChanged();
        shouldShowOverlay();
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.block_all:
                mTrackerListAdapter.blockAllTrackers();
                return true;
            case R.id.unblock_all:
                mTrackerListAdapter.unBlockAllTrackers();
                return true;
            default:
                return false;
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.overflow_menu:
                showOverflowMenu(v);
                break;
        }

    }

    public void showOverflowMenu(View v) {
        final PopupMenu popup = new PopupMenu(getContext(), v);
        final MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.ghostery_site_trackers_overflow_menu, popup.getMenu());
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    private void shouldShowOverlay() {
        final String pageHost = GeckoBundleUtils.safeGetString(mControlCenterData, "data/summary/pageHost");
        final List<String> blacklist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(mControlCenterData,"data/summary/site_blacklist"));
        final List<String> whitelist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(mControlCenterData,"data/summary/site_whitelist"));
        final boolean isPaused = GeckoBundleUtils.safeGetBoolean(mControlCenterData, "data/summary/paused_blocking");
        final boolean isWhiteListed = whitelist.contains(pageHost);
        final boolean isBlackListed = blacklist.contains(pageHost);
        if (isPaused || isWhiteListed || isBlackListed) {
            mOverlay.setVisibility(View.VISIBLE);
        } else {
            mOverlay.setVisibility(View.GONE);
        }
    }
}
