package org.mozilla.gecko.controlcenter;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.graphics.Paint;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.AppCompatDrawableManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.mozilla.gecko.BrowserApp;
import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.Tabs;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class SiteTrackersListAdapter extends BaseExpandableListAdapter {

    private static final int STATE_UNCHECKED = 0;
    private static final int STATE_CHECKED = 1;
    private static final int STATE_MIXED = 2;
    private static final int STATE_TRUSTED = 3;
    private static final int STATE_RESTRICTED = 4;

    private GeckoBundle[] mListData;
    private GeckoBundle data;
    private Context mContext;
    private BaseControlCenterPagerAdapter.ControlCenterCallbacks mControlCenterCallbacks;

    SiteTrackersListAdapter(Context context, BaseControlCenterPagerAdapter.ControlCenterCallbacks callbacks) {
        mContext = context;
        mControlCenterCallbacks = callbacks;
    }

    @Override
    public int getGroupCount() {
        if (mListData == null) {
            return 0;
        }
        return mListData.length;
    }

    @Override
    public int getChildrenCount(int groupPosition) {
        if (mListData == null) {
            return 0;
        }
        return mListData[groupPosition].getBundleArray("trackers").length + 1;
    }

    @Override
    public GeckoBundle getGroup(int groupPosition) {
        if (mListData == null) {
            return null;
        }
        return mListData[groupPosition];
    }

    @Override
    public GeckoBundle getChild(int groupPosition, int childPosition) {
        if (mListData == null) {
            return null;
        }
        return mListData[groupPosition].getBundleArray("trackers")[childPosition - 1];
    }

    @Override
    public long getGroupId(int groupPosition) {
        return groupPosition;
    }

    @Override
    public long getChildId(int groupPosition, int childPosition) {
        return groupPosition * 2000 + childPosition;
    }

    @Override
    public boolean hasStableIds() {
        return false;
    }

    @Override
    public boolean isChildSelectable(int groupPosition, int childPosition) {
        return true;
    }

    @Override
    public View getGroupView(int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {
        if (convertView == null) {
            final LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.ghostery_list_item_category, null);
        }
        final View view = convertView.findViewById(R.id.tracker_options);
        view.setX(parent.getWidth());
        final GeckoBundle groupGeckoBundle = getGroup(groupPosition);
        final String categoryId = groupGeckoBundle.getString("id");
        //TODO add name to enum
        final String categoryName = groupGeckoBundle.getString("name");
        final Categories category = Categories.safeValueOf(categoryId);
        final int totalTrackers = calculateTotalTrackerCount(groupGeckoBundle);
        final int blockedTrackers = calculateBlockedTrackerCount(groupGeckoBundle);
        final TextView categoryNameTextView = (TextView) convertView.findViewById(R.id.category_name);
        final TextView totalTrackersTextView = (TextView) convertView.findViewById(R.id.total_trackers);
        final TextView blockedTrackersTextView = (TextView) convertView.findViewById(R.id.blocked_trackers);
        final ImageView categoryIcon = (ImageView) convertView.findViewById(R.id.category_icon);
        final ImageView stateCheckBox = (ImageView) convertView.findViewById(R.id.cb_block_all);
        final ImageView stateArrow = (ImageView) convertView.findViewById(R.id.cc_state_arrow);
        stateArrow.setImageResource(isExpanded ? R.drawable.cc_ic_collapse_arrow : R.drawable.cc_ic_expand_arrow);
        categoryIcon.setImageDrawable(AppCompatDrawableManager.get().getDrawable(mContext, category.categoryIcon));
        categoryNameTextView.setText(categoryName);
        totalTrackersTextView.setText(mContext.getResources().getQuantityString(R.plurals.cc_total_trackers, totalTrackers, totalTrackers));
        blockedTrackersTextView.setText(mContext.getResources().getString(R.string.cc_num_blocked, blockedTrackers));
        int numBlocked = 0;
        int numTrusted = 0;
        final GeckoBundle[] trackers = groupGeckoBundle.getBundleArray("trackers");
        if (trackers != null) {
            for (GeckoBundle tracker : trackers) {
                if (tracker.getBoolean("ss_allowed")) {
                    numTrusted++;
                }
                if (tracker.getBoolean("blocked")) {
                    numBlocked++;
                }
            }
        }
        final String pagehost = GeckoBundleUtils.safeGetString(data, "data/summary/pageHost");
        final List<String> blacklist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_blacklist"));
        final List<String> whitelist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_whitelist"));
        final boolean isWhiteListed = whitelist.contains(pagehost);
        final boolean isBlackListed = blacklist.contains(pagehost);
        final int state;
        if (isBlackListed) {
            state = STATE_RESTRICTED;
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_restricted);
        } else if (isWhiteListed) {
            state = STATE_TRUSTED;
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_trust);
        } else if (numBlocked == 0) {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_unchecked);
            state = STATE_UNCHECKED;
        } else if (numBlocked == totalTrackers && numTrusted == 0) {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_block);
            state = STATE_CHECKED;
        } else if (numBlocked > 0 && (numTrusted > 0 || numBlocked < totalTrackers)) {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_mixed);
            state = STATE_MIXED;
        } else {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_unchecked);
            state = STATE_UNCHECKED;
        }
        stateCheckBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final int newState;
                switch (state) {
                    case STATE_UNCHECKED:
                        newState = STATE_CHECKED;
                        break;
                    case STATE_CHECKED:
                        newState = STATE_UNCHECKED;
                        break;
                    case STATE_MIXED:
                        newState = STATE_CHECKED;
                        break;
                    default:
                        newState = STATE_UNCHECKED;
                }
                if (trackers != null) {
                    final GeckoBundle selectedAppIds = GeckoBundleUtils.safeGetBundle(data,"data/blocking/selected_app_ids");
                    for (GeckoBundle tracker : trackers) {
                        final String trackerId = Integer.toString(tracker.getInt("id"));
                        if (newState == STATE_CHECKED) {
                            tracker.putBoolean("blocked", true);
                            selectedAppIds.putInt(trackerId, 1);
                        } else {
                            tracker.putBoolean("blocked", false);
                            selectedAppIds.remove(trackerId);
                        }
                    }
                    final GeckoBundle geckoBundle = new GeckoBundle();
                    geckoBundle.putBundle("selected_app_ids", selectedAppIds);
                    EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                    mControlCenterCallbacks.controlCenterSettingsChanged();
                    notifyDataSetChanged();
                }
            }
        });
        return convertView;
    }

    @Override
    public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView, final ViewGroup parent) {
        //we check if the view is not null and is of the corresponding type so that it can be recycled
        if (convertView == null || (convertView.getId() == R.id.header_layout && childPosition != 0)
                || (convertView.getId() == R.id.main_layout && childPosition == 0)) {
            LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(childPosition == 0 ? R.layout.ghostery_tracker_list_header : R.layout.ghostery_list_item_tracker, null);
        }
        if (childPosition > 0) {
            final View view = convertView.findViewById(R.id.tracker_options);
            view.setX(parent.getWidth());
            final GeckoBundle childGeckoBundle = getChild(groupPosition, childPosition);
            final String pagehost = GeckoBundleUtils.safeGetString(data, "data/summary/pageHost");
            final String trackerName = childGeckoBundle.getString("name");
            final int trackerId = childGeckoBundle.getInt("id");
            final boolean isBlocked = childGeckoBundle.getBoolean("blocked");
            final boolean isSiteSpecificBlocked = childGeckoBundle.getBoolean("ss_blocked");
            final boolean isSiteSpecificAllowed = childGeckoBundle.getBoolean("ss_allowed");
            final TextView trackerNameTextView = (TextView) convertView.findViewById(R.id.tracker_name);
            final ImageView trackerCheckBox = (ImageView) convertView.findViewById(R.id.cb_block_tracker);
            final View infoButton = convertView.findViewById(R.id.info_icon);
            infoButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    mControlCenterCallbacks.hideControlCenter();
                    Tabs.getInstance().loadUrlInTab("https://whotracks.me/trackers/" + trackerName.toLowerCase().replace(" ", "_") + ".html");
                }
            });
            final View trustButton = view.findViewById(R.id.trust_button);
            final View restrictButton = view.findViewById(R.id.restrict_button);
            final View blockButton = view.findViewById(R.id.block_button);
            final TextView trustButtonTv = (TextView) view.findViewById(R.id.trust_button_text);
            final TextView restrictButtonTv = (TextView) view.findViewById(R.id.restrict_button_text);
            final TextView blockButtonTv = (TextView) view.findViewById(R.id.block_button_text);
            trustButtonTv.setText(isSiteSpecificAllowed ?  R.string.cc_untrust : R.string.cc_trust);
            restrictButtonTv.setText(isSiteSpecificBlocked ? R.string.cc_unrestrict : R.string.cc_restrict);
            blockButtonTv.setText(isBlocked ? R.string.cc_unblock : R.string.cc_block);
            final boolean isPaused = data.getBundle("data").getBundle("summary").getBoolean("paused_blocking");
            if (!isPaused) {
                trustButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        hideOptionsMenu(v);
                        final GeckoBundle siteSpecificUnblockBundle = data.getBundle("data").getBundle("blocking")
                                .getBundle("site_specific_unblocks");
                        final GeckoBundle siteSpecificBlockBundle = data.getBundle("data").getBundle("blocking")
                                .getBundle("site_specific_blocks");
                        final int[] siteSpecificUnblockList = siteSpecificUnblockBundle.getIntArray(pagehost);
                        final int[] siteSpecificBlockList = siteSpecificBlockBundle.getIntArray(pagehost);
                        final ArrayList<Integer> updatedUnblockList = new ArrayList<>();
                        final ArrayList<Integer> updatedBlockList = new ArrayList<>();
                        if (isSiteSpecificAllowed) {
                            childGeckoBundle.putBoolean("ss_allowed", false);
                            for (int unblockedTrackerId : siteSpecificUnblockList) {
                                if (unblockedTrackerId != trackerId) {
                                    updatedUnblockList.add(unblockedTrackerId);
                                }
                            }
                        } else {
                            childGeckoBundle.putBoolean("ss_allowed", true);
                            childGeckoBundle.putBoolean("ss_blocked", false);
                            final int unblockListLength = siteSpecificUnblockList == null ? 0 : siteSpecificUnblockList.length;
                            for (int i = 0; i < unblockListLength; i++) {
                                updatedUnblockList.add(siteSpecificUnblockList[i]);
                            }
                            updatedUnblockList.add(trackerId);
                            final int blockListLength = siteSpecificBlockList == null ? 0 : siteSpecificBlockList.length;
                            for (int i = 0; i < blockListLength; i++) {
                                updatedBlockList.add(siteSpecificBlockList[i]);
                            }
                            updatedBlockList.remove(Integer.valueOf(trackerId)); //remove the site from the restricted list if its there
                            Toast.makeText(mContext, R.string.cc_toast_site_trust, Toast.LENGTH_SHORT).show();
                        }

                        siteSpecificUnblockBundle.putIntArray(pagehost, updatedUnblockList);
                        siteSpecificBlockBundle.putIntArray(pagehost, updatedBlockList);

                        final GeckoBundle geckoBundle = new GeckoBundle();
                        geckoBundle.putBundle("site_specific_unblocks", siteSpecificUnblockBundle);
                        geckoBundle.putBundle("site_specific_blocks", siteSpecificBlockBundle);
                        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                        mControlCenterCallbacks.controlCenterSettingsChanged();
                    }
                });
                restrictButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        hideOptionsMenu(v);
                        final GeckoBundle siteSpecificUnblockBundle = data.getBundle("data").getBundle("blocking")
                                .getBundle("site_specific_unblocks");
                        final GeckoBundle siteSpecificBlockBundle = data.getBundle("data").getBundle("blocking")
                                .getBundle("site_specific_blocks");
                        final int[] siteSpecificUnblockList = siteSpecificUnblockBundle.getIntArray(pagehost);
                        final int[] siteSpecificBlockList = siteSpecificBlockBundle.getIntArray(pagehost);
                        final ArrayList<Integer> updatedUnblockList = new ArrayList<>();
                        final ArrayList<Integer> updatedBlockList = new ArrayList<>();
                        if (isSiteSpecificBlocked) {
                            childGeckoBundle.putBoolean("ss_blocked", false);
                            for (int blockedTrackerId : siteSpecificBlockList) {
                                if (blockedTrackerId != trackerId) {
                                    updatedBlockList.add(blockedTrackerId);
                                }
                            }
                        } else {
                            childGeckoBundle.putBoolean("ss_allowed", false);
                            childGeckoBundle.putBoolean("ss_blocked", true);
                            final int blockListLength = siteSpecificBlockList == null ? 0 : siteSpecificBlockList.length;
                            for (int i = 0; i < blockListLength; i++) {
                                updatedBlockList.add(siteSpecificBlockList[i]);
                            }
                            updatedBlockList.add(trackerId);
                            final int unBlockListLength = siteSpecificUnblockList == null ? 0 : siteSpecificUnblockList.length;
                            for (int i = 0; i < unBlockListLength; i++) {
                                updatedUnblockList.add(siteSpecificUnblockList[i]);
                            }
                            updatedUnblockList.remove(Integer.valueOf(trackerId)); //remove the site from the trusted list if its there
                            Toast.makeText(mContext, R.string.cc_toast_site_restrict, Toast.LENGTH_SHORT).show();
                        }

                        siteSpecificUnblockBundle.putIntArray(pagehost, updatedUnblockList);
                        siteSpecificBlockBundle.putIntArray(pagehost, updatedBlockList);

                        final GeckoBundle geckoBundle = new GeckoBundle();
                        geckoBundle.putBundle("site_specific_unblocks", siteSpecificUnblockBundle);
                        geckoBundle.putBundle("site_specific_blocks", siteSpecificBlockBundle);
                        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                        mControlCenterCallbacks.controlCenterSettingsChanged();
                    }
                });
                blockButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        hideOptionsMenu(v);
                        final GeckoBundle selectedAppIds = data.getBundle("data").getBundle("blocking")
                                .getBundle("selected_app_ids");
                        if (isBlocked) {
                            selectedAppIds.remove(Integer.toString(trackerId));
                            childGeckoBundle.putBoolean("blocked", false);
                        } else {
                            selectedAppIds.putInt(Integer.toString(trackerId), 1);
                            childGeckoBundle.putBoolean("blocked", true);
                            Toast.makeText(mContext, R.string.cc_toast_site_block, Toast.LENGTH_SHORT).show();
                        }
                        final GeckoBundle geckoBundle = new GeckoBundle();
                        geckoBundle.putBundle("selected_app_ids", selectedAppIds);
                        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                        mControlCenterCallbacks.controlCenterSettingsChanged();
                    }
                });
            }
            trackerNameTextView.setText(trackerName);

            final List<String> blacklist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_blacklist"));
            final List<String> whitelist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_whitelist"));
            final boolean isWhiteListed = whitelist.contains(pagehost);
            final boolean isBlackListed = blacklist.contains(pagehost);
            if (isBlackListed
                    || (isSiteSpecificBlocked && !isWhiteListed)
                    || (isBlocked && !(isSiteSpecificAllowed || isWhiteListed))) {
                trackerNameTextView.setPaintFlags(trackerNameTextView.getPaintFlags() | Paint.STRIKE_THRU_TEXT_FLAG);
            } else {
                trackerNameTextView.setPaintFlags(trackerNameTextView.getPaintFlags() & (~Paint.STRIKE_THRU_TEXT_FLAG));
            }
            if (isWhiteListed) {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_trust);
            } else if (isBlackListed) {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_restricted);
            } else if (isSiteSpecificAllowed) {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_trust);
            } else if (isSiteSpecificBlocked) {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_restricted);
            } else if (isBlocked) {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_block);
            } else {
                trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_unchecked);
            }
            final float fullWidth = 3 * mContext.getResources().getDimension(R.dimen.ghostery_list_item_action_button_width);
            final float partialWidth = 2 * mContext.getResources().getDimension(R.dimen.ghostery_list_item_action_button_width);
            final ObjectAnimator animation = ObjectAnimator.ofFloat(view, "translationX",
                    parent.getWidth() - (isSiteSpecificAllowed || isSiteSpecificBlocked || isWhiteListed || isBlackListed
                            ? partialWidth : fullWidth));
            animation.setDuration(400);
            if (isPaused || isBlackListed || isWhiteListed) {
                trackerCheckBox.setOnClickListener(null);
            } else {
                trackerCheckBox.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        notifyDataSetChanged();
                        animation.start();
                    }
                });
            }
        }
        return convertView;
    }

    void setData(GeckoBundle data) {
        this.data = data;
        mListData = GeckoBundleUtils.safeGetBundleArray(data,"data/summary/categories");
        notifyDataSetChanged();
    }

    private void hideOptionsMenu(View v) {
        final View optionsMenuView = (View) v.getParent();
        final View listItemView = (View) optionsMenuView.getParent();
        final ObjectAnimator animation = ObjectAnimator.ofFloat(optionsMenuView, "translationX", listItemView.getWidth());
        animation.setDuration(400);
        animation.start();
        animation.addListener(new Animator.AnimatorListener() {
            @Override
            public void onAnimationStart(Animator animation) {

            }

            @Override
            public void onAnimationEnd(Animator animation) {
                notifyDataSetChanged();
            }

            @Override
            public void onAnimationCancel(Animator animation) {

            }

            @Override
            public void onAnimationRepeat(Animator animation) {

            }
        });
    }

    public void blockAllTrackers() {
        final GeckoBundle selectedAppIds = GeckoBundleUtils.safeGetBundle(data,"data/blocking/selected_app_ids");
        final GeckoBundle[] categories = GeckoBundleUtils.safeGetBundleArray(data, "data/summary/categories");
        for (GeckoBundle category : categories) {
            final int totalTrackers = category.getInt("num_total", 0);
            category.putInt("num_blocked", totalTrackers);
            final GeckoBundle[] trackers = GeckoBundleUtils.safeGetBundleArray(category, "trackers");
            for (GeckoBundle tracker : trackers) {
                final String trackerId = Integer.toString(tracker.getInt("id"));
                tracker.putBoolean("blocked", true);
                selectedAppIds.putInt(trackerId, 1);
            }
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putBundle("selected_app_ids", selectedAppIds);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        mControlCenterCallbacks.controlCenterSettingsChanged();
        notifyDataSetChanged();
    }

    public void unBlockAllTrackers() {
        final GeckoBundle selectedAppIds = GeckoBundleUtils.safeGetBundle(data,"data/blocking/selected_app_ids");
        final GeckoBundle[] categories = GeckoBundleUtils.safeGetBundleArray(data, "data/summary/categories");
        for (GeckoBundle category : categories) {
            category.putInt("num_blocked", 0);
            final GeckoBundle[] trackers = GeckoBundleUtils.safeGetBundleArray(category, "trackers");
            for (GeckoBundle tracker : trackers) {
                final String trackerId = Integer.toString(tracker.getInt("id"));
                tracker.putBoolean("blocked", false);
                selectedAppIds.remove(trackerId);
            }
        }
        final GeckoBundle geckoBundle = new GeckoBundle();
        geckoBundle.putBundle("selected_app_ids", selectedAppIds);
        EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
        mControlCenterCallbacks.controlCenterSettingsChanged();
        notifyDataSetChanged();
    }

    private int calculateTotalTrackerCount(GeckoBundle categoryBundle) {
        final GeckoBundle[] trackers = categoryBundle.getBundleArray("trackers");
        return trackers == null ? 0 : trackers.length;
    }

    private int calculateBlockedTrackerCount(GeckoBundle categoryBundle) {
        final boolean isGhosteryPaused = GeckoBundleUtils.safeGetBoolean(data, "data/summary/paused_blocking");
        if (isGhosteryPaused) {
            return 0;
        }
        final String pagehost = GeckoBundleUtils.safeGetString(data, "data/summary/pageHost");
        final List<String> whitelist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_whitelist"));
        if (whitelist.contains(pagehost)) {
            return 0;
        }
        final List<String> blacklist = Arrays.asList(GeckoBundleUtils.safeGetStringArray(data, "data/summary/site_blacklist"));
        final GeckoBundle[] trackers = categoryBundle.getBundleArray("trackers");
        if (blacklist.contains(pagehost)) {
            return trackers != null ? trackers.length : 0;
        }
        int blockedTrackerCount = 0;
        if (trackers != null) {
            for (GeckoBundle tracker : trackers) {
                final boolean isBlocked = tracker.getBoolean("blocked");
                final boolean isTrusted = tracker.getBoolean("ss_allowed");
                final boolean isRestricted = tracker.getBoolean("ss_blocked");
                if (isBlocked && !isTrusted) {
                    blockedTrackerCount++;
                } else if (isRestricted) {
                    blockedTrackerCount++;
                }
            }
        }
        return blockedTrackerCount;
    }
}
