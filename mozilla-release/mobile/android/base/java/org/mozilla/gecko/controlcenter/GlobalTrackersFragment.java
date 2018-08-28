package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;
import android.support.v7.widget.PopupMenu;
import android.widget.ProgressBar;

import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 */
public class GlobalTrackersFragment extends ControlCenterFragment implements View.OnClickListener, PopupMenu.OnMenuItemClickListener {

    private GlobalTrackersListAdapter mTrackerListAdapter;
    private ControlCenterViewPager.ControlCenterCallbacks mControlCenterCallbacks;
    private ProgressBar progressBar;

    public GlobalTrackersFragment() {
    }

    public void setControlCenterCallback(ControlCenterViewPager.ControlCenterCallbacks callback) {
        mControlCenterCallbacks = callback;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.ghostery_global_trackers_fragment, container, false);
        final ExpandableListView mTrackersList = (ExpandableListView) view.findViewById(R.id.trackers_list);
        progressBar = (ProgressBar) view.findViewById(R.id.progress_bar);
        final View mOverflowMenu = view.findViewById(R.id.overflow_menu);
        mOverflowMenu.setOnClickListener(this);
        mTrackerListAdapter = new GlobalTrackersListAdapter(getContext(), mControlCenterCallbacks);
        mTrackersList.setAdapter(mTrackerListAdapter);
        return view;
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.cc_title_global_trackers);
    }

    @Override
    public void updateUI(GeckoBundle data) {
        mTrackerListAdapter.setData(data);
        if (progressBar.getVisibility() == View.VISIBLE) {
            progressBar.setVisibility(View.GONE);
        }
    }

    @Override
    public void refreshUI() {
        if (getView() == null) {
            return; //return if view is not inflated yet
        }
        mTrackerListAdapter.notifyDataSetChanged();
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
            case R.id.reset_settings:
                RestoreDefaultsDialog.show(getContext(), new RestoreDefaultsDialog.RestoreDialogCallbacks() {
                    @Override
                    public void onRestore() {
                        progressBar.setVisibility(View.VISIBLE);
                    }
                });
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
        inflater.inflate(R.menu.ghostery_global_trackers_overflow_menu, popup.getMenu());
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }
}
