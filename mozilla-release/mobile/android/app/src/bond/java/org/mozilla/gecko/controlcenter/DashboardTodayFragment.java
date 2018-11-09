package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class DashboardTodayFragment extends ControlCenterFragment {

    private RecyclerView mDashBoardListView;
    private TextView mDashboardStateTextView;
    private View mDisableDashboardView;
    private DashboardAdapter mDashboardAdapter;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.bond_dashboard_today_fragment, container,
                false);
        mDashBoardListView = (RecyclerView) view.findViewById(R.id.dash_board_list_view);
        mDashboardStateTextView = (TextView) view.findViewById(R.id.dashboard_state_text);
        mDisableDashboardView = view.findViewById(R.id.dashboard_disable_view);
        mDashboardAdapter = new DashboardAdapter(getContext());
        mDashBoardListView.setAdapter(mDashboardAdapter);
        mDashBoardListView.setLayoutManager(new LinearLayoutManager(getContext()));
        changeDashboardState(true);//@todo real state, maybe from preference
        addDummyData();
        return view;
    }

    public void changeDashboardState(boolean isEnabled) {
        final int stateTextId;
        final int overlayVisibility;
        final int stateTextColorId;
        if(isEnabled) {
            stateTextId = R.string.bond_dashboard_contols_on;
            overlayVisibility = View.GONE;
            stateTextColorId = R.color.general_blue_color;
        } else {
            stateTextId = R.string.bond_dashboard_contols_off;
            overlayVisibility = View.VISIBLE;
            stateTextColorId = android.R.color.white;
        }
        mDisableDashboardView.setVisibility(overlayVisibility);
        final StringBuilder stateText = new StringBuilder();
        stateText.append(getString(R.string.bond_dashboard_ultimate_protection));
        stateText.append(" ");
        stateText.append(getString(stateTextId));

        mDashboardStateTextView.setTextColor(ContextCompat.getColor(getContext(),stateTextColorId));
        mDashboardStateTextView.setText(stateText);
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.bond_dashboard_today_title);
    }

    @Override
    public void updateUI(GeckoBundle data) {
        if (mDashboardAdapter == null) {
            return;
        }
        mDashboardAdapter.setData(data);
    }

    @Override
    public void refreshUI() {
        if (getView() == null) {
            return; //return if view is not inflated yet
        }
        changeDashboardState(
                (boolean)getActivity().findViewById(R.id.bond_dashboard_state_button).getTag());
        mDashboardAdapter.notifyDataSetChanged();
    }

    @Override
    public void refreshUIComponent(int id, boolean optionValue) {
        if(id == R.id.bond_dashboard_state_button) {
            changeDashboardState(optionValue);
        } else if (id == R.id.bond_dashboard_clear_button) {
            mDashboardAdapter.resetData();
        }
    }

    // @Todo should be removed
    public void addDummyData() {
        final List<DashboardItemEntity> dashboardItems = new ArrayList<>();
        dashboardItems.add(new DashboardItemEntity("255","MIN",R.drawable.ic_time_circle,"Time Saved",
                "That you can spend with your friends", -1));
        dashboardItems.add(new DashboardItemEntity("4732","",R.drawable.ic_ad_blocking_shiel,
                "Ads Blocked",
                "That you can enjoy surfing without ads", -1));
        dashboardItems.add(new DashboardItemEntity("251","MB",-1,"Data Saved","more that" +
                "enough to watch another video", -1));
        dashboardItems.add(new DashboardItemEntity("255","MIN",R.drawable.ic_battery,"Battery Saved",
                "so that you can enjoy your phone a little longer", -1));
        dashboardItems.add(new DashboardItemEntity("","",R.drawable.ic_anti_phishing_hook,"Phishing protection",
                "so that you can swim freely with our browser", -1));
        mDashboardAdapter.addItems(dashboardItems);
        dashboardItems.add(new DashboardItemEntity("4000","\n\tCOMPANIES",R.drawable
                .ic_eye,"Tracker Companies blocked",
                "...companies with most trackers: Google, Amaozn, Facebook,...", -1));
        /* @todo unhide the money item
        dashboardItems.add(new DashboardItemEntity("261","EUR",-1,"Money saved",
                "...how much is your time worth per h", AVERAGE_MONEY_BAR_VALUE));
        */
        mDashboardAdapter.addItems(dashboardItems);
    }
}
