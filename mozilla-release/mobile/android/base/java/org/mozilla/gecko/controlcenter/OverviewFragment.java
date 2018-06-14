package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;
import com.github.mikephil.charting.data.PieEntry;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.anolysis.ControlCenterMetrics;
import org.mozilla.gecko.util.GeckoBundle;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class OverviewFragment extends ControlCenterFragment implements View.OnClickListener {

    private PieChart mPieChart;
    private TextView mDomainName;
    private TextView mTrackersBlocked;
    private RelativeLayout mTrustSiteButton;
    private RelativeLayout mRestrictSiteButton;
    private RelativeLayout mPauseGhosteryButton;
    private boolean mIsSiteTrusted;
    private boolean mIsSiteRestricted;
    private boolean mIsGhosteryPaused;
    private GeckoBundle controlCenterSettingsData;
    private final List<Integer> colors = new ArrayList<>();
    private final List<Integer> disabledColors = new ArrayList<>();

    public OverviewFragment() {
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.ghostery_overview_fragment, container, false);
        mPieChart = (PieChart) view.findViewById(R.id.donut);
        mDomainName = (TextView) view.findViewById(R.id.domain_name);
        mTrackersBlocked = (TextView) view.findViewById(R.id.trackers_blocked);
        mTrustSiteButton = (RelativeLayout) view.findViewById(R.id.ghostery_trust_site_button);
        mRestrictSiteButton = (RelativeLayout) view.findViewById(R.id.ghostery_restrict_site_button);
        mPauseGhosteryButton = (RelativeLayout) view.findViewById(R.id.ghostery_pause_button);
        mTrustSiteButton.setOnClickListener(this);
        mRestrictSiteButton.setOnClickListener(this);
        mPauseGhosteryButton.setOnClickListener(this);
        return view;
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.cc_title_overview);
    }

    @Override
    public void updateUI(GeckoBundle controlCenterSettingsData) {
        this.controlCenterSettingsData = controlCenterSettingsData;
        final int allowedTrackers = controlCenterSettingsData.getBundle("data").getBundle("summary").getBundle("trackerCounts").getInt("allowed");
        final int blockedTrackers = controlCenterSettingsData.getBundle("data").getBundle("summary").getBundle("trackerCounts").getInt("blocked");
        final int totalTrackers = allowedTrackers + blockedTrackers;
        final String domainName = controlCenterSettingsData.getBundle("data").getBundle("summary").getString("pageHost");
        final List<PieEntry> entries = new ArrayList<>();
        mIsGhosteryPaused = controlCenterSettingsData.getBundle("data").getBundle("summary").getBoolean("paused_blocking");
        final GeckoBundle[] categories = controlCenterSettingsData.getBundle("data").getBundle("summary").getBundleArray("categories");
        colors.clear();
        disabledColors.clear();
        for (GeckoBundle categoryBundle : categories != null ? categories : new GeckoBundle[0]) {
            final Categories category = Categories.safeValueOf(categoryBundle.getString("id"));
            final int trackersCount = categoryBundle.getInt("num_total");
            entries.add(new PieEntry(trackersCount));
            colors.add(ContextCompat.getColor(getContext(), category.categoryColor));
            disabledColors.add(ContextCompat.getColor(getContext(), category.categoryColorDisabled));
        }
        final PieDataSet set = new PieDataSet(entries, "cc");
        set.setColors(mIsGhosteryPaused ? disabledColors : colors);
        final PieData pieData = new PieData(set);
        pieData.setDrawValues(false);
        mPieChart.setHighlightPerTapEnabled(false);
        mPieChart.setData(pieData);
        mPieChart.setDrawEntryLabels(false);
        mPieChart.setCenterText(getResources().getQuantityString(R.plurals.cc_total_trackers_found, totalTrackers, totalTrackers));
        mPieChart.setCenterTextSize(22);
        mPieChart.setHoleRadius(70);
        mPieChart.setDescription(null);
        mPieChart.setDrawMarkers(false);
        mPieChart.getLegend().setEnabled(false);
        mPieChart.invalidate();
        mTrackersBlocked.setText(getResources().getQuantityString(R.plurals.cc_total_trackers_blocked, blockedTrackers, blockedTrackers));
        mDomainName.setText(domainName);
        final List<String> blackList = Arrays.asList(controlCenterSettingsData.getBundle("data")
                .getBundle("summary").getStringArray("site_blacklist"));
        final List<String> whiteList = Arrays.asList(controlCenterSettingsData.getBundle("data")
                .getBundle("summary").getStringArray("site_whitelist"));
        mIsSiteRestricted = blackList.contains(domainName);
        mIsSiteTrusted = whiteList.contains(domainName);
        if (!mIsGhosteryPaused) {
            styleButton(mTrustSiteButton, mIsSiteTrusted);
            styleButton(mRestrictSiteButton, mIsSiteRestricted);
        } else {
            styleButton(mPauseGhosteryButton, mIsGhosteryPaused);
        }
    }

    @Override
    public void refreshUI() {

    }

    //TODO: send messages to the extension on changing state of the button
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.ghostery_trust_site_button:
                handleTrustButtonClick();
                break;
            case R.id.ghostery_restrict_site_button:
                handleRestrictButtonClick();
                break;
            case R.id.ghostery_pause_button:
                handlePauseButtonClick();
                break;
        }
    }

    private void handlePauseButtonClick() {
        mIsGhosteryPaused = !mIsGhosteryPaused;
        styleButton(mPauseGhosteryButton, mIsGhosteryPaused);
        if (mIsGhosteryPaused) {
            styleButton(mTrustSiteButton, false);
            styleButton(mRestrictSiteButton, false);
            ControlCenterMetrics.pause();
        } else {
            ControlCenterMetrics.resume();
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putBoolean("paused_blocking", mIsGhosteryPaused);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        controlCenterSettingsData.getBundle("data").getBundle("summary").putBoolean("paused_blocking", mIsGhosteryPaused);
    }

    private void handleTrustButtonClick() {
        ControlCenterMetrics.trustSite();
        final String pageHost = controlCenterSettingsData.getBundle("data").getBundle("summary")
                .getString("pageHost");
        //get list of blacklisted and whitelisted sites and then add/remove current site and send back the new lists
        final String[] blackList = controlCenterSettingsData.getBundle("data")
                .getBundle("summary")
                .getStringArray("site_blacklist");
        final String[] whiteList = controlCenterSettingsData.getBundle("data")
                .getBundle("summary")
                .getStringArray("site_whitelist");
        final ArrayList<String> updatedBlackList;
        final ArrayList<String> updatedWhiteList;
        mIsSiteTrusted = !mIsSiteTrusted;
        styleButton(mTrustSiteButton, mIsSiteTrusted);
        if (mIsSiteTrusted) {
            styleButton(mRestrictSiteButton, false);
            styleButton(mPauseGhosteryButton, false);
            updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedBlackList.remove(pageHost);
            updatedWhiteList.add(pageHost);
            final GeckoBundle geckoBundle = new GeckoBundle();
            geckoBundle.putBoolean("paused_blocking", false);
            EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        } else {
            updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedWhiteList.remove(pageHost);
            ControlCenterMetrics.restrictSite();
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putStringArray("site_whitelist", updatedWhiteList);
        geckoBundle.putStringArray("site_blacklist", updatedBlackList);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        //update the data source so that other views can reflect changes
        controlCenterSettingsData.getBundle("data").getBundle("summary").putStringArray("site_blacklist", updatedBlackList);
        controlCenterSettingsData.getBundle("data").getBundle("summary").putStringArray("site_whitelist", updatedWhiteList);
    }

    private void handleRestrictButtonClick() {
        ControlCenterMetrics.restrictSite();
        final String pageHost = controlCenterSettingsData.getBundle("data").getBundle("summary")
                .getString("pageHost");
        //get list of blacklisted and whitelisted sites and then add/remove current site and send back the new lists
        final String[] blackList = controlCenterSettingsData.getBundle("data")
                .getBundle("summary")
                .getStringArray("site_blacklist");
        final String[] whiteList = controlCenterSettingsData.getBundle("data")
                .getBundle("summary")
                .getStringArray("site_whitelist");
        final ArrayList<String> updatedBlackList;
        final ArrayList<String> updatedWhiteList;
        mIsSiteRestricted = !mIsSiteRestricted;
        styleButton(mRestrictSiteButton, mIsSiteRestricted);
        if (mIsSiteRestricted) {
            styleButton(mTrustSiteButton, false);
            styleButton(mPauseGhosteryButton, false);
            updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedBlackList.add(pageHost);
            updatedWhiteList.remove(pageHost);
            final GeckoBundle geckoBundle = new GeckoBundle();
            geckoBundle.putBoolean("paused_blocking", false);
            EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        } else {
            updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedBlackList.remove(pageHost);
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putStringArray("site_whitelist", updatedWhiteList);
        geckoBundle.putStringArray("site_blacklist", updatedBlackList);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        //update the data source so that other views can reflect changes
        controlCenterSettingsData.getBundle("data").getBundle("summary").putStringArray("site_blacklist", updatedBlackList);
        controlCenterSettingsData.getBundle("data").getBundle("summary").putStringArray("site_whitelist", updatedWhiteList);
    }

    private void styleButton(RelativeLayout button, boolean state) {
        final int textAndIconColor = ContextCompat.getColor(getContext(),
                state ? android.R.color.white : android.R.color.black);
        final TextView buttonText = (TextView) button.findViewById(R.id.button_text);
        final ImageView buttonIcon = (ImageView) button.findViewById(R.id.button_icon);
        buttonIcon.setColorFilter(textAndIconColor, PorterDuff.Mode.SRC_ATOP);
        buttonText.setTextColor(textAndIconColor);
        //special cases for background color and text
        if (button == mPauseGhosteryButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.button_background_control_center_blue);
                buttonText.setText(getString(R.string.cc_resume_ghostery));
                buttonIcon.setImageResource(R.drawable.ic_resume);
                ((PieDataSet) mPieChart.getData().getDataSet()).setColors(disabledColors);
                mPieChart.invalidate();
            } else {
                button.setBackgroundResource(R.drawable.button_backgorund_control_center_white);
                buttonText.setText(R.string.cc_pause_ghostery);
                buttonIcon.setImageResource(R.drawable.ic_pause);
                ((PieDataSet) mPieChart.getData().getDataSet()).setColors(colors);
                mPieChart.invalidate();
            }
        } else if (button == mRestrictSiteButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.button_background_control_center_red);
            } else {
                button.setBackgroundResource(R.drawable.button_backgorund_control_center_white);
            }
        } else if (button == mTrustSiteButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.button_background_control_center_green);
            } else {
                button.setBackgroundResource(R.drawable.button_backgorund_control_center_white);
            }
        }
    }
}
