package org.mozilla.gecko.onboarding;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.v4.view.PagerAdapter;
import android.text.Html;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.CustomLinkMovementMethod;

/**
 * Copyright © Cliqz 2018
 *
 * This is an implementation of PagerAdapter which directly inflates and views layouts instead of
 * using fragments
 */
public class CliqzIntroPagerAdapter extends PagerAdapter {

    private Context mContext;
    private final @LayoutRes
    int[] screens = new int[] {
            R.layout.cliqz_intro_first_screen,
            R.layout.cliqz_intro_second_screen,
            R.layout.cliqz_intro_third_screen,
            R.layout.cliqz_intro_fourth_screen
    };

    public CliqzIntroPagerAdapter(Context context) {
        mContext = context;
    }

    @Override
    public Object instantiateItem(final ViewGroup container, int position) {
        final LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        final ViewGroup layout = (ViewGroup) layoutInflater.inflate(screens[position], container, false);
        final Button startBrowsing = (Button) layout.findViewById(R.id.start_browsing);
        final TextView dataCollectionTv = (TextView) layout.findViewById(R.id.data_collection_detail_tv);
        if (dataCollectionTv != null) {
            final CustomLinkMovementMethod customLinkMovementMethod =
                    CustomLinkMovementMethod.getInstance(new CustomLinkMovementMethod.OnOpenLinkCallBack() {
                        @Override
                        public void OnOpenLinkLoaded() {

                        }
                    });
            customLinkMovementMethod.init(CustomLinkMovementMethod.OPEN_IN_CUSTOM_TAB);
            dataCollectionTv.setMovementMethod(customLinkMovementMethod);
            dataCollectionTv.setText(Html.fromHtml(mContext.getString(R.string.ghostery_onboarding_old_users_text)));
        }
        startBrowsing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                container.setVisibility(View.GONE);
            }
        });
        final CheckBox collectDataCb = (CheckBox) layout.findViewById(R.id.collect_data_cb);
        if (collectDataCb != null) {
            collectDataCb.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                    setPreferencesValues(isChecked);
                }
            });
            collectDataCb.setChecked(true);
            setPreferencesValues(true);
        }
        container.addView(layout);
        return layout;
    }

    private void setPreferencesValues(boolean value){
        final PreferenceManager preferenceManager = new PreferenceManager(mContext);
        preferenceManager.setTelemetryEnabled(value);
        preferenceManager.setHumanWebEnabled(value);
        preferenceManager.setMyOffrzEnabled(value);
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object view) {
        container.removeView((View) view);
    }

    @Override
    public int getCount() {
        return screens.length;
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == object;
    }
}
