package org.mozilla.gecko.widget;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.res.Resources;
import android.os.Build;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.LinearLayout;

import org.mozilla.gecko.util.WindowUtils;

/**
 * Copyright © Cliqz 2018
 *
 * This view hides itself if its not anchored to the bottom of the screen.
 *
 * It remains visible if it's at the bottom of the screen or if it's pushed out of the screen to the bottom.
 * But hides itself when it's pushed up. For example - opening of a keyboard
 */
public class SelfHidingLinearLayout extends LinearLayout {
    final int[] mLocation = new int[2];
    int mNavBarHeight;

    public SelfHidingLinearLayout(Context context) {
        super(context);
        init();
    }

    public SelfHidingLinearLayout(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public SelfHidingLinearLayout(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public SelfHidingLinearLayout(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init() {
        final Resources resources = getContext().getResources();
        final int resourceId = resources.getIdentifier("navigation_bar_height", "dimen", "android");
        if (resourceId > 0) {
            mNavBarHeight = resources.getDimensionPixelSize(resourceId);
        } else {
            mNavBarHeight = 0;
        }
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        super.onLayout(changed, l, t, r, b);
        getLocationInWindow(mLocation);
        if (mLocation[1] < WindowUtils.getLargestDimension(getContext()) - getHeight() - mNavBarHeight) {
            setVisibility(INVISIBLE);
        } else {
            setVisibility(VISIBLE);
        }
    }
}
