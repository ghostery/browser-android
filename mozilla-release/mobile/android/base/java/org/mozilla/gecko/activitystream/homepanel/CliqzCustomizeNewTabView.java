package org.mozilla.gecko.activitystream.homepanel;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.support.annotation.Nullable;
import android.util.AttributeSet;

import org.mozilla.gecko.widget.SelfHidingLinearLayout;

/**
 * Copyright © Cliqz 2019
 */
public class CliqzCustomizeNewTabView extends SelfHidingLinearLayout {

    protected OnVisibilityChange mVisibilityChangeListener = null;

    public interface OnVisibilityChange {
        void onChange(int newVisibility);
    }

    public CliqzCustomizeNewTabView(Context context) {
        super(context);
    }

    public CliqzCustomizeNewTabView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public CliqzCustomizeNewTabView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public CliqzCustomizeNewTabView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    public void setVisibility(int visibility) {
        super.setVisibility(visibility);
        if (mVisibilityChangeListener != null) {
            mVisibilityChangeListener.onChange(visibility);
        }
    }

    public void setOnVisibilityChangeListener(OnVisibilityChange visibilityChangeListener) {
        this.mVisibilityChangeListener = visibilityChangeListener;
    }
}