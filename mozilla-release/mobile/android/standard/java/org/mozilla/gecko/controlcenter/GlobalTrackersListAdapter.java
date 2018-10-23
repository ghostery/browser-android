package org.mozilla.gecko.controlcenter;

import android.animation.ObjectAnimator;
import android.content.Context;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.AppCompatDrawableManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.Tabs;
import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

/**
 * Copyright © Cliqz 2018
 */
public class GlobalTrackersListAdapter extends BaseExpandableListAdapter {

    private GeckoBundle[] mListData;
    private GeckoBundle data;
    private Context mContext;
    private BaseControlCenterPagerAdapter.ControlCenterCallbacks mControlCenterCallbacks;

    GlobalTrackersListAdapter(Context context, BaseControlCenterPagerAdapter.ControlCenterCallbacks callbacks) {
        mControlCenterCallbacks = callbacks;
        mContext = context;
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
        return mListData[groupPosition].getBundleArray("trackers").length;
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
        return mListData[groupPosition].getBundleArray("trackers")[childPosition];
    }

    @Override
    public long getGroupId(int groupPosition) {
        return groupPosition;
    }

    @Override
    public long getChildId(int groupPosition, int childPosition) {
        return groupPosition * 100 + childPosition;
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
            LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.ghostery_list_item_category, null);
        }
        final View view = convertView.findViewById(R.id.tracker_options);
        view.setX(parent.getWidth());
        final GeckoBundle groupGeckoBundle = getGroup(groupPosition);
        final String categoryId = groupGeckoBundle.getString("id");
        //TODO add name to enum
        final String categoryName = groupGeckoBundle.getString("name");
        final Categories category = Categories.safeValueOf(categoryId);
        final int totalTrackers = groupGeckoBundle.getInt("num_total");
        final int blockedTrackers = groupGeckoBundle.getInt("num_blocked");
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
        blockedTrackersTextView.setText(mContext.getString(R.string.cc_num_blocked, blockedTrackers));
        if (blockedTrackers == 0) {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_unchecked);
        } else if (blockedTrackers == totalTrackers) {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_block);
        } else {
            stateCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_mixed);
        }
        stateCheckBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //block or unblock the whole category
                final GeckoBundle selectedAppIds = GeckoBundleUtils.safeGetBundle(data, "data/blocking/selected_app_ids");
                final GeckoBundle[] trackers = GeckoBundleUtils.safeGetBundleArray(groupGeckoBundle, "trackers");
                for (GeckoBundle tracker : trackers) {
                    final String trackerId = tracker.getString("id", "");
                    tracker.putBoolean("blocked", blockedTrackers < totalTrackers);
                    if (blockedTrackers < totalTrackers) {
                        selectedAppIds.putInt(trackerId, 1);
                    } else {
                        selectedAppIds.remove(trackerId);
                    }
                }
                if (blockedTrackers < totalTrackers) {
                    groupGeckoBundle.putInt("num_blocked", totalTrackers);
                } else {
                    groupGeckoBundle.putInt("num_blocked", 0);
                }
                //reflect the change in sitetrackerview also
                final GeckoBundle[] siteTrackerCategories = GeckoBundleUtils.safeGetBundleArray(data, "data/summary/categories");
                for (GeckoBundle siteTrackerCategory : siteTrackerCategories) {
                    final String siteTrackerCategoryId = siteTrackerCategory.getString("id");
                    if (categoryId.equals(siteTrackerCategoryId)) {
                        final int totalTrackers = siteTrackerCategory.getInt("num_total");
                        siteTrackerCategory.putInt("num_blocked", blockedTrackers < totalTrackers ? totalTrackers : 0);
                        final GeckoBundle[] siteTrackers = siteTrackerCategory.getBundleArray("trackers");
                        for (GeckoBundle tracker : siteTrackers) {
                            tracker.putBoolean("blocked", blockedTrackers < totalTrackers);
                        }
                    }
                }
                final GeckoBundle geckoBundle = new GeckoBundle();
                geckoBundle.putBundle("selected_app_ids", selectedAppIds);
                EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                mControlCenterCallbacks.controlCenterSettingsChanged();
                notifyDataSetChanged();
            }
        });
        return convertView;
    }

    @Override
    public View getChildView(final int groupPosition, int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
        if (convertView == null) {
            LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.ghostery_list_item_global_tracker, null);
        }
        final GeckoBundle childGeckoBundle = getChild(groupPosition, childPosition);
        final String trackerName = childGeckoBundle.getString("name");
        final String trackerId = childGeckoBundle.getString("id");
        final boolean isBlocked = childGeckoBundle.getBoolean("blocked");
        final TextView trackerNameTextView = (TextView) convertView.findViewById(R.id.tracker_name);
        final ImageView trackerCheckBox = (ImageView) convertView.findViewById(R.id.cb_block_tracker);
        final View blockButton = convertView.findViewById(R.id.block_button);
        final TextView blockButtonText = (TextView) convertView.findViewById(R.id.block_button_text);
        blockButton.setX(parent.getWidth());
        final View infoButton = convertView.findViewById(R.id.info_icon);
        infoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mControlCenterCallbacks.hideControlCenter();
                Tabs.getInstance().loadUrlInTab("https://whotracks.me/trackers/" + trackerName.toLowerCase().replace(" ", "_") + ".html");
            }
        });
        if (isBlocked) {
            trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_checked_block);
            blockButtonText.setText(R.string.cc_unblock);
        } else {
            trackerCheckBox.setImageResource(R.drawable.cc_ic_cb_unchecked);
            blockButtonText.setText(R.string.cc_block);
        }
        trackerNameTextView.setText(trackerName);
        final ObjectAnimator animation = ObjectAnimator.ofFloat(blockButton, "translationX",
                parent.getWidth() - mContext.getResources()
                        .getDimension(R.dimen.ghostery_list_item_action_button_width));
        animation.setDuration(400);
        trackerCheckBox.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                notifyDataSetChanged();
                animation.start();
            }
        });
        blockButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final GeckoBundle selectedAppIds = data.getBundle("data").getBundle("blocking")
                        .getBundle("selected_app_ids");
                final int numBlocked = getGroup(groupPosition).getInt("num_blocked");
                if (isBlocked) {
                    selectedAppIds.remove(trackerId);
                    getGroup(groupPosition).putInt("num_blocked", numBlocked-1);
                } else {
                    selectedAppIds.putInt(trackerId, 1);
                    getGroup(groupPosition).putInt("num_blocked", numBlocked+1);
                    Toast.makeText(mContext, R.string.cc_toast_site_block, Toast.LENGTH_SHORT).show();
                }
                final GeckoBundle geckoBundle = new GeckoBundle();
                geckoBundle.putBundle("selected_app_ids", selectedAppIds);
                EventDispatcher.getInstance().dispatch("Privacy:SetInfo", geckoBundle);
                mControlCenterCallbacks.controlCenterSettingsChanged();
                childGeckoBundle.putBoolean("blocked",!isBlocked);
                final GeckoBundle[] siteTrackerCategories = data.getBundle("data")
                        .getBundle("summary").getBundleArray("categories");
                for (GeckoBundle category : siteTrackerCategories) {
                    final GeckoBundle[] trackers = category.getBundleArray("trackers");
                    for (GeckoBundle tracker : trackers) {
                        if (tracker.getInt("id") == Integer.valueOf(trackerId)) {
                            tracker.putBoolean("blocked", !isBlocked);
                        }
                    }
                }
                notifyDataSetChanged();
            }
        });
        return convertView;
    }

    void setData(GeckoBundle data) {
        this.data = data;
        mListData = GeckoBundleUtils.safeGetBundleArray(data,"data/settings/categories");
        notifyDataSetChanged();
    }
}
