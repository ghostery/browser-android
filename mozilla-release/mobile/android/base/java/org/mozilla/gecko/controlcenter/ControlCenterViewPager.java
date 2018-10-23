package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.util.AttributeSet;

import com.booking.rtlviewpager.RtlViewPager;

/**
 * Copyright © Cliqz 2018
 */
public class ControlCenterViewPager extends RtlViewPager {

    public ControlCenterViewPager(Context context) {
        this(context, null);
    }

    public ControlCenterViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
        addOnPageChangeListener(new OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
            }

            @Override
            public void onPageSelected(int position) {
                ((ControlCenterPagerAdapter)getAdapter()).updateCurrentView(position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {

            }
        });
    }
}
