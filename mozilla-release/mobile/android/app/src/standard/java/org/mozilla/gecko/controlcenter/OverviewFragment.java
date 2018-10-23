package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.support.annotation.ColorRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.BottomSheetBehavior;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.SwitchCompat;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.AbsoluteSizeSpan;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;
import com.github.mikephil.charting.data.PieEntry;
import com.github.mikephil.charting.utils.Utils;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.anolysis.ControlCenterMetrics;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.mozilla.gecko.util.GeckoBundleUtils.safeGetBoolean;
import static org.mozilla.gecko.util.GeckoBundleUtils.safeGetBundle;
import static org.mozilla.gecko.util.GeckoBundleUtils.safeGetInt;
import static org.mozilla.gecko.util.GeckoBundleUtils.safeGetString;
import static org.mozilla.gecko.util.GeckoBundleUtils.safeGetStringArray;

/**
 * Copyright © Cliqz 2018
 */
public class OverviewFragment extends ControlCenterFragment implements View.OnClickListener, CompoundButton.OnCheckedChangeListener {

    private BaseControlCenterPagerAdapter.ControlCenterCallbacks mControlCenterCallbacks;
    private PieChart mPieChart;
    private TextView mDomainName;
    private TextView mTrackersBlocked;
    private LinearLayout mTrustSiteButton;
    private LinearLayout mRestrictSiteButton;
    private LinearLayout mPauseGhosteryButton;
    private TextView mSmartBlockingCount;
    private TextView mCliqzAttrackCount;
    private TextView mAdBlockCount;
    private SwitchCompat mSmartBlockingSwitch;
    private SwitchCompat mAdBlockingSwitch;
    private SwitchCompat mAntiTrackingSwitch;
    private boolean mIsSiteTrusted;
    private boolean mIsSiteRestricted;
    private boolean mIsGhosteryPaused;
    private GeckoBundle controlCenterSettingsData;
    private final List<Integer> colors = new ArrayList<>();
    private final List<Integer> disabledColors = new ArrayList<>();
    private final List<Integer> blockedColors = new ArrayList<>();
    private TextView mNotchTitle;
    private TextView mInfoViewCount;
    private TextView mInfoViewTitle;
    private TextView mInfoViewSummary;
    private ImageView mInfoViewIcon;
    private View mInfoView;
    private View mOverview;
    private BottomSheetBehavior<View> mBottomSheetBehavior;
    private final BottomSheetBehavior.BottomSheetCallback mBottomSheetCallback =
            new BottomSheetBehavior.BottomSheetCallback() {
                @Override
                public void onStateChanged(@NonNull View bottomSheet, int newState) {
                    if (newState == BottomSheetBehavior.STATE_COLLAPSED) {
                        mOverview.setVisibility(View.VISIBLE);
                        mInfoView.setVisibility(View.INVISIBLE);
                        mNotchTitle.setText(R.string.cc_enhanced_options);
                    }
                }

                @Override
                public void onSlide(@NonNull View bottomSheet, float slideOffset) {

                }
            };

    public OverviewFragment() {
    }

    public void setControlCenterCallback(BaseControlCenterPagerAdapter.ControlCenterCallbacks callback) {
        mControlCenterCallbacks = callback;
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        //trick to redraw the view
        getActivity().getSupportFragmentManager()
                .beginTransaction()
                .detach(this)
                .attach(this)
                .commitAllowingStateLoss();
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.ghostery_overview_fragment, container, false);
        mPieChart = (PieChart) view.findViewById(R.id.cc_donut);
        mDomainName = (TextView) view.findViewById(R.id.cc_domain_name);
        mTrackersBlocked = (TextView) view.findViewById(R.id.cc_trackers_blocked);
        mTrustSiteButton = (LinearLayout) view.findViewById(R.id.cc_ghostery_trust_site_button);
        mRestrictSiteButton = (LinearLayout) view.findViewById(R.id.cc_ghostery_restrict_site_button);
        mPauseGhosteryButton = (LinearLayout) view.findViewById(R.id.cc_ghostery_pause_button);
        mAdBlockingSwitch = (SwitchCompat) view.findViewById(R.id.cc_enhanced_blocking_switch);
        mAntiTrackingSwitch = (SwitchCompat) view.findViewById(R.id.cc_enhanced_tracking_switch);
        mSmartBlockingSwitch = (SwitchCompat) view.findViewById(R.id.cc_smart_blocking_switch);
        mSmartBlockingCount = (TextView) view.findViewById(R.id.cc_smart_blocking_count);
        mCliqzAttrackCount = (TextView) view.findViewById(R.id.cc_enhanced_tracking_count);
        mAdBlockCount = (TextView) view.findViewById(R.id.cc_enhanced_blocking_count);
        mTrustSiteButton.setOnClickListener(this);
        mRestrictSiteButton.setOnClickListener(this);
        mPauseGhosteryButton.setOnClickListener(this);
        mInfoView = view.findViewById(R.id.cc_info_view);
        mOverview = view.findViewById(R.id.cc_overview_view);
        mInfoViewCount = (TextView) view.findViewById(R.id.cc_info_view_count);
        mInfoViewTitle = (TextView) view.findViewById(R.id.cc_info_view_title);
        mInfoViewSummary = (TextView) view.findViewById(R.id.cc_info_view_summary);
        mInfoViewIcon = (ImageView) view.findViewById(R.id.cc_info_view_icon);
        mNotchTitle = (TextView) view.findViewById(R.id.cc_notch_title);

        final View mBottomSheetRootView = view.findViewById(R.id.cc_bottom_sheet_root_view);
        mBottomSheetBehavior = BottomSheetBehavior.from(mBottomSheetRootView);
        mBottomSheetBehavior.setBottomSheetCallback(mBottomSheetCallback);

        final View mAttrackInfoButton = view.findViewById(R.id.cc_enhanced_tracking_info_button);
        final View mAdBlockingInfoButton = view.findViewById(R.id.cc_enhanced_blocking_info_button);
        final View mSmartBlockingInfoButton = view.findViewById(R.id.cc_smart_blocking_info_button);
        final View mInfoViewCloseButton = view.findViewById(R.id.cc_close_info_view_button);
        mAttrackInfoButton.setOnClickListener(this);
        mAdBlockingInfoButton.setOnClickListener(this);
        mSmartBlockingInfoButton.setOnClickListener(this);
        mInfoViewCloseButton.setOnClickListener(this);

        return view;
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.cc_title_overview);
    }

    @Override
    public void updateUI(GeckoBundle controlCenterSettingsData) {
        if (getView() == null) {
            return; //return if view is not inflated yet
        }
        mNotchTitle.setText(R.string.cc_enhanced_options);
        mBottomSheetBehavior.setState(BottomSheetBehavior.STATE_COLLAPSED);
        this.controlCenterSettingsData = controlCenterSettingsData;
        final int allowedTrackers = safeGetInt(controlCenterSettingsData, "data/summary/trackerCounts/allowed");
        final int blockedTrackers = calculateBlockedTrackers();
        final int totalTrackers = calculateGhosteryAttrackCount() + calculateCliqzAttrackCount()
                + getAdBlockCount();
        final String domainName = safeGetString(controlCenterSettingsData, "data/summary/pageHost");
        final boolean isSmartBlockEnabled = safeGetBoolean(controlCenterSettingsData, "data/panel/panel/enable_smart_block");
        final boolean isAdBlockEnabled = safeGetBoolean(controlCenterSettingsData, "data/panel/panel/enable_ad_block");
        final boolean isAntiTrackingEnabled = safeGetBoolean(controlCenterSettingsData, "data/panel/panel/enable_anti_tracking");
        mSmartBlockingCount.setText(String.valueOf(getSmartBlockingCount()));
        mCliqzAttrackCount.setText(String.valueOf(calculateCliqzAttrackCount()));
        mAdBlockCount.setText(String.valueOf(getAdBlockCount()));

        mSmartBlockingSwitch.setOnCheckedChangeListener(null);
        mAntiTrackingSwitch.setOnCheckedChangeListener(null);
        mAdBlockingSwitch.setOnCheckedChangeListener(null);

        mSmartBlockingSwitch.setChecked(isSmartBlockEnabled);
        mAdBlockingSwitch.setChecked(isAdBlockEnabled);
        mAntiTrackingSwitch.setChecked(isAntiTrackingEnabled);

        mSmartBlockingSwitch.setOnCheckedChangeListener(this);
        mAntiTrackingSwitch.setOnCheckedChangeListener(this);
        mAdBlockingSwitch.setOnCheckedChangeListener(this);

        final List<PieEntry> entries = new ArrayList<>();
        mIsGhosteryPaused = safeGetBoolean(controlCenterSettingsData, "data/summary/paused_blocking");
        final GeckoBundle[] categories = safeGetBundle(controlCenterSettingsData, "data/summary").getBundleArray("categories");
        colors.clear();
        disabledColors.clear();
        blockedColors.clear();
        for (GeckoBundle categoryBundle : categories != null ? categories : new GeckoBundle[0]) {
            final Categories category = Categories.safeValueOf(categoryBundle.getString("id"));
            final int trackersCount = categoryBundle.getInt("num_total");
            entries.add(new PieEntry(trackersCount));
            colors.add(ContextCompat.getColor(getContext(), category.categoryColor));
            disabledColors.add(ContextCompat.getColor(getContext(), category.categoryColorDisabled));
            blockedColors.add(ContextCompat.getColor(getContext(), category.categoryColorBlocked));
        }
        if (entries.isEmpty()) {
            entries.add(new PieEntry(1));
            final @ColorRes int defaultColor = ContextCompat.getColor(getContext(), R.color.cc_default_category_disabled);
            colors.add(defaultColor);
            disabledColors.add(defaultColor);
            blockedColors.add(defaultColor);
        }
        final PieDataSet set = new PieDataSet(entries, "cc");
        if (mIsGhosteryPaused) {
            set.setColors(disabledColors);
        } else if (mIsSiteRestricted) {
            set.setColors(blockedColors);
        } else {
            set.setColors(colors);
        }
        final PieData pieData = new PieData(set);
        pieData.setDrawValues(false);
        mPieChart.setHighlightPerTapEnabled(false);
        mPieChart.setData(pieData);
        mPieChart.setDrawEntryLabels(false);
        final String pieChartText = getResources().getQuantityString(R.plurals.cc_total_trackers_found, totalTrackers, totalTrackers);
        final Spannable centerTextSpan = new SpannableString(pieChartText);
        centerTextSpan.setSpan(new AbsoluteSizeSpan((int)Utils.convertDpToPixel(40), false), 0,
                Integer.toString(totalTrackers).length(), Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
        mPieChart.setCenterText(centerTextSpan);
        mPieChart.setCenterTextSizePixels(getResources().getDimension(R.dimen.ghostery_cc_donut_text_size));
        mPieChart.setCenterTextRadiusPercent(85);
        mPieChart.setCenterTextColor(ContextCompat.getColor(getContext(), R.color.cc_text_color));
        mPieChart.setHoleRadius(80);
        mPieChart.setDescription(null);
        mPieChart.setDrawMarkers(false);
        mPieChart.getLegend().setEnabled(false);
        mPieChart.invalidate();
        final String trackersBlocked = getResources().getQuantityString(R.plurals.cc_total_trackers_blocked, blockedTrackers, blockedTrackers);
        final Spannable trackersBlockedSpan = new SpannableString(trackersBlocked);
        trackersBlockedSpan.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.cc_restricted)), 0,
                Integer.toString(blockedTrackers).length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        mTrackersBlocked.setText(trackersBlockedSpan);
        if (mDomainName != null) { //landscape mode has no domain name
            mDomainName.setText(domainName);
        }
        final List<String> blackList = Arrays.asList(safeGetStringArray(controlCenterSettingsData, "data/summary/site_blacklist"));
        final List<String> whiteList = Arrays.asList(safeGetStringArray(controlCenterSettingsData, "data/summary/site_whitelist"));
        mIsSiteRestricted = blackList.contains(domainName);
        mIsSiteTrusted = whiteList.contains(domainName);
        if (!mIsGhosteryPaused) {
            styleButton(mTrustSiteButton, mIsSiteTrusted);
            styleButton(mRestrictSiteButton, mIsSiteRestricted);
            styleButton(mPauseGhosteryButton, mIsGhosteryPaused);
        } else {
            styleButton(mPauseGhosteryButton, mIsGhosteryPaused);
        }
    }

    private int calculateGhosteryAttrackCount() {
        int totalTrackers = 0;
        final GeckoBundle[] categories = GeckoBundleUtils.safeGetBundleArray(controlCenterSettingsData, "data/summary/categories");
        for (GeckoBundle category : categories) {
            final GeckoBundle[] trackers = category.getBundleArray("trackers");
            if (trackers != null) {
                totalTrackers+=trackers.length;
            }
        }
        return totalTrackers;
    }

    private int calculateBlockedTrackers() {
        int totalBlocked = 0;
        final GeckoBundle[] categories = GeckoBundleUtils.safeGetBundleArray(controlCenterSettingsData, "data/summary/categories");
        if (mIsSiteRestricted) {
            for (GeckoBundle category : categories) {
                final GeckoBundle[] trackers = category.getBundleArray("trackers");
                if (trackers != null) {
                    totalBlocked += trackers.length;
                }
            }
            return totalBlocked;
        }
        if (mIsSiteTrusted || mIsGhosteryPaused) {
            return 0;
        }
        for (GeckoBundle category : categories) {
            final GeckoBundle[] trackers  = category.getBundleArray("trackers");
            if (trackers != null) {
                for (GeckoBundle tracker : trackers) {
                    final boolean isBlocked = tracker.getBoolean("blocked");
                    final boolean isTrusted = tracker.getBoolean("ss_allowed");
                    final boolean isRestricted = tracker.getBoolean("ss_blocked");
                    if (isBlocked && !isTrusted) {
                        totalBlocked++;
                    } else if (isRestricted) {
                        totalBlocked++;
                    }
                }
            }
        }
        return totalBlocked;
    }

    private int calculateCliqzAttrackCount() {
        int count = 0;
        final GeckoBundle attrackBundle = GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/cliqz/antitracking");
        final String[] categories =  attrackBundle.keys();
        for (String category : categories) {
            final GeckoBundle categoryBundle = GeckoBundleUtils.safeGetBundle(attrackBundle, category);
            final String[] trackers = categoryBundle.keys();
            for (String tracker : trackers) {
                if (categoryBundle.getString(tracker).equals("unsafe")) {
                    count++;
                }
            }
        }
        return count;
    }

    private int getAdBlockCount() {
        final GeckoBundle adBlockBundle = GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/cliqz/adblock");
        return adBlockBundle.getInt("totalCount");
    }

    private int getSmartBlockingCount() {
        return safeGetBundle(controlCenterSettingsData,"data/panel/panel/smartBlock/blocked").keys().length +
                safeGetBundle(controlCenterSettingsData, "data/panel/panel/smartBlock/unblocked").keys().length;
    }

    @Override
    public void refreshUI() {
        final int blockedTrackers = calculateBlockedTrackers();
        final String trackersBlocked = getResources().getQuantityString(R.plurals.cc_total_trackers_blocked, blockedTrackers, blockedTrackers);
        final Spannable trackersBlockedSpan = new SpannableString(trackersBlocked);
        trackersBlockedSpan.setSpan(new ForegroundColorSpan(getResources().getColor(R.color.cc_restricted)), 0,
                Integer.toString(blockedTrackers).length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        mTrackersBlocked.setText(trackersBlockedSpan);
        final PieData donutData = mPieChart.getData();
        if (donutData != null) {
            final PieDataSet donutDataset = ((PieDataSet) donutData.getDataSet());
            if (mIsGhosteryPaused) {
                donutDataset.setColors(disabledColors);
            } else if (mIsSiteRestricted) {
                donutDataset.setColors(blockedColors);
            } else {
                donutDataset.setColors(colors);
            }
            mPieChart.invalidate();
        }
    }

    @Override
    public void onClick(View v) {
        if (controlCenterSettingsData == null) {
            return;
        }
        switch (v.getId()) {
            case R.id.cc_ghostery_trust_site_button:
                handleTrustButtonClick();
                break;
            case R.id.cc_ghostery_restrict_site_button:
                handleRestrictButtonClick();
                break;
            case R.id.cc_ghostery_pause_button:
                handlePauseButtonClick();
                break;
            case R.id.cc_enhanced_tracking_info_button:
                handleTrackingInfoButtonClick();
                break;
            case R.id.cc_enhanced_blocking_info_button:
                handleAdBlockingInfoButtonClick();
                break;
            case R.id.cc_smart_blocking_info_button:
                handleSmartBlockingInfoButtonClick();
                break;
            case R.id.cc_close_info_view_button:
                handleInfoViewCloseButtonClick();
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        final GeckoBundle geckoBundle = new GeckoBundle();
        switch (buttonView.getId()) {
            case R.id.cc_enhanced_tracking_switch:
                geckoBundle.putBoolean("enable_anti_tracking", isChecked);
                break;
            case R.id.cc_enhanced_blocking_switch:
                geckoBundle.putBoolean("enable_ad_block", isChecked);
                break;
            case R.id.cc_smart_blocking_switch:
                geckoBundle.putBoolean("enable_smart_block", isChecked);
                break;
        }
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        mControlCenterCallbacks.controlCenterSettingsChanged();
    }

    private void handlePauseButtonClick() {
        mIsGhosteryPaused = !mIsGhosteryPaused;
        mIsSiteRestricted = false;
        mIsSiteTrusted = false;
        styleButton(mPauseGhosteryButton, mIsGhosteryPaused);
        if (mIsGhosteryPaused) {
            styleButton(mTrustSiteButton, false);
            styleButton(mRestrictSiteButton, false);
            //domain should no longer be restricted/trusted if ghostery is paused
            final String pageHost = GeckoBundleUtils.safeGetString(controlCenterSettingsData, "data/summary/pageHost");
            final String[] blackList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_blacklist");
            final String[] whiteList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_whitelist");
            final ArrayList<String> updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            final ArrayList<String> updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedBlackList.remove(pageHost);
            updatedWhiteList.remove(pageHost);
            //update the data source so that other views can reflect changes
            GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_blacklist", updatedBlackList);
            GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_whitelist", updatedWhiteList);
            final GeckoBundle geckoBundle = new GeckoBundle();
            geckoBundle.putStringArray("site_whitelist", updatedWhiteList);
            geckoBundle.putStringArray("site_blacklist", updatedBlackList);
            EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
            mControlCenterCallbacks.controlCenterSettingsChanged();
            ControlCenterMetrics.pause();
        } else {
            ControlCenterMetrics.resume();
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putBoolean("paused_blocking", mIsGhosteryPaused);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        mControlCenterCallbacks.controlCenterSettingsChanged();
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putBoolean("paused_blocking", mIsGhosteryPaused);
    }

    private void handleTrustButtonClick() {
        ControlCenterMetrics.trustSite();
        final String pageHost = GeckoBundleUtils.safeGetString(controlCenterSettingsData, "data/summary/pageHost");
        //get list of blacklisted and whitelisted sites and then add/remove current site and send back the new lists
        final String[] blackList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_blacklist");
        final String[] whiteList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_whitelist");
        final ArrayList<String> updatedBlackList;
        final ArrayList<String> updatedWhiteList;
        mIsSiteTrusted = !mIsSiteTrusted;
        mIsSiteRestricted = false;
        mIsGhosteryPaused = false;
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
            mControlCenterCallbacks.controlCenterSettingsChanged();
            Toast.makeText(getContext(), R.string.cc_toast_overview_trust, Toast.LENGTH_SHORT).show();
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
        mControlCenterCallbacks.controlCenterSettingsChanged();
        //update the data source so that other views can reflect changes
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_blacklist", updatedBlackList);
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_whitelist", updatedWhiteList);
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putBoolean("paused_blocking", false);
    }

    private void handleRestrictButtonClick() {
        ControlCenterMetrics.restrictSite();
        final String pageHost = GeckoBundleUtils.safeGetString(controlCenterSettingsData, "data/summary/pageHost");
        //get list of blacklisted and whitelisted sites and then add/remove current site and send back the new lists
        final String[] blackList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_blacklist");
        final String[] whiteList = GeckoBundleUtils.safeGetStringArray(controlCenterSettingsData, "data/summary/site_whitelist");
        final ArrayList<String> updatedBlackList;
        final ArrayList<String> updatedWhiteList;
        mIsSiteRestricted = !mIsSiteRestricted;
        mIsSiteTrusted = false;
        mIsGhosteryPaused = false;
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
            mControlCenterCallbacks.controlCenterSettingsChanged();
            Toast.makeText(getContext(), R.string.cc_toast_overview_restrict, Toast.LENGTH_SHORT).show();
        } else {
            updatedBlackList = new ArrayList<>(Arrays.asList(blackList != null ? blackList : new String[0]));
            updatedWhiteList = new ArrayList<>(Arrays.asList(whiteList != null ? whiteList : new String[0]));
            updatedBlackList.remove(pageHost);
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putStringArray("site_whitelist", updatedWhiteList);
        geckoBundle.putStringArray("site_blacklist", updatedBlackList);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        mControlCenterCallbacks.controlCenterSettingsChanged();
        //update the data source so that other views can reflect changes
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_blacklist", updatedBlackList);
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putStringArray("site_whitelist", updatedWhiteList);
        GeckoBundleUtils.safeGetBundle(controlCenterSettingsData, "data/summary").putBoolean("paused_blocking", false);
    }

    private void handleTrackingInfoButtonClick() {
        mOverview.setVisibility(View.INVISIBLE);
        mInfoView.setVisibility(View.VISIBLE);
        mInfoViewCount.setText(String.valueOf(calculateCliqzAttrackCount()));
        mInfoViewTitle.setText(getResources().getQuantityString(R.plurals.cc_trackers_blocked, calculateCliqzAttrackCount()));
        mInfoViewSummary.setText(R.string.cc_anti_tracking_about);
        mInfoViewIcon.setImageResource(R.drawable.cc_ic_anti_tracking);
        mNotchTitle.setText(R.string.cc_enhanced_anti_tracking);
    }

    private void handleAdBlockingInfoButtonClick() {
        mOverview.setVisibility(View.INVISIBLE);
        mInfoView.setVisibility(View.VISIBLE);
        mInfoViewCount.setText(String.valueOf(getAdBlockCount()));
        mInfoViewTitle.setText(getResources().getQuantityString(R.plurals.cc_ads_blocked, getAdBlockCount()));
        mInfoViewSummary.setText(R.string.cc_ad_blocking_about);
        mInfoViewIcon.setImageResource(R.drawable.cc_ic_ad_blocker);
        mNotchTitle.setText(R.string.cc_enhanced_ad_blocking);
    }

    private void handleSmartBlockingInfoButtonClick() {
        mOverview.setVisibility(View.INVISIBLE);
        mInfoView.setVisibility(View.VISIBLE);
        mInfoViewCount.setText(String.valueOf(getSmartBlockingCount()));
        mInfoViewTitle.setText(getResources().getQuantityString(R.plurals.cc_trackers_adjusted, getSmartBlockingCount()));
        mInfoViewSummary.setText(R.string.cc_smart_blocking_about);
        mInfoViewIcon.setImageResource(R.drawable.cc_ic_smart_blocking);
        mNotchTitle.setText(R.string.cc_smart_blocking);
    }

    private void handleInfoViewCloseButtonClick() {
        mOverview.setVisibility(View.VISIBLE);
        mInfoView.setVisibility(View.INVISIBLE);
        mNotchTitle.setText(R.string.cc_enhanced_options);
    }

    private void styleButton(ViewGroup button, boolean state) {
        final int textAndIconColor = ContextCompat.getColor(getContext(),
                state ? android.R.color.white : R.color.cc_text_color);
        final TextView buttonText = (TextView) button.findViewById(R.id.cc_button_text);
        final ImageView buttonIcon = (ImageView) button.findViewById(R.id.cc_button_icon);
        buttonIcon.setColorFilter(textAndIconColor, PorterDuff.Mode.SRC_ATOP);
        buttonText.setTextColor(textAndIconColor);
        //special cases for background color and text
        if (button == mPauseGhosteryButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.cc_button_background_blue);
                buttonText.setText(getString(R.string.cc_resume_ghostery));
                buttonIcon.setImageResource(R.drawable.cc_ic_resume);
            } else {
                button.setBackgroundResource(R.drawable.cc_button_background_white);
                buttonText.setText(R.string.cc_pause_ghostery);
                buttonIcon.setImageResource(R.drawable.cc_ic_pause);
            }
        } else if (button == mRestrictSiteButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.cc_button_background_red);
            } else {
                button.setBackgroundResource(R.drawable.cc_button_background_white);
            }
        } else if (button == mTrustSiteButton) {
            if (state) {
                button.setBackgroundResource(R.drawable.cc_button_background_green);
            } else {
                button.setBackgroundResource(R.drawable.cc_button_background_white);
            }
        }
        refreshUI();
    }
}
