package org.mozilla.gecko.onboarding;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.v4.view.PagerAdapter;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;

import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;

/**
 * Copyright © Cliqz 2018
 *
 * This is an implementation of PagerAdapter which directly inflates and views layouts instead of
 * using fragments
 */
public class CliqzIntroPagerAdapter extends PagerAdapter {

    private Context mContext;
    private final @LayoutRes
    int[] screens = new int[]{R.layout.cliqz_intro_first_screen,
            R.layout.cliqz_intro_second_screen,
            R.layout.cliqz_intro_third_screen,
            R.layout.cliqz_intro_fourth_screen};

    public CliqzIntroPagerAdapter(Context context) {
        mContext = context;
    }

    @Override
    public Object instantiateItem(final ViewGroup container, int position) {
        final PreferenceManager preferenceManager = new PreferenceManager(mContext);
        final LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        final ViewGroup layout = (ViewGroup) layoutInflater.inflate(screens[position], container, false);
        final Button startBrowsing = (Button) layout.findViewById(R.id.start_browsing);
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
                    preferenceManager.setTelemetryEnabled(isChecked);
                    preferenceManager.setHumanWebEnabled(isChecked);
                }
            });
            collectDataCb.setChecked(true);
            preferenceManager.setTelemetryEnabled(true);
        }
        container.addView(layout);
        return layout;
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
