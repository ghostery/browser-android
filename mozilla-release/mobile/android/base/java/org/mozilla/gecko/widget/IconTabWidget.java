/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.widget;

import org.mozilla.gecko.R;
import org.mozilla.gecko.widget.themed.ThemedTextView;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TabWidget;
import android.widget.TextView;

public class IconTabWidget extends TabWidget {

    /* Cliqz Start */
    private static final int[] STATE_PRIVATE_MODE = { R.attr.state_private };

    private boolean mIsPrivate;

    private static final int[] LIGHT_THEME = { R.attr.light_theme };
    private static final int[] LIGHT_THEME_PRIVATE_MODE = { R.attr.light_theme, R.attr.state_private };
    private boolean mIsLightTheme;
    /* Cliqz End */

    OnTabChangedListener mListener;
    private final int mButtonLayoutId;

    /* Cliqz Start */
    @Override
    protected int[] onCreateDrawableState(int extraSpace) {
        final int[] addedState;

        if (mIsLightTheme && mIsPrivate) {
            addedState = LIGHT_THEME_PRIVATE_MODE;
        } else if (mIsLightTheme) {
            addedState = LIGHT_THEME;
        } else if (mIsPrivate) {
            addedState = STATE_PRIVATE_MODE;
        } else {
            addedState = new int[]{};
        }

        final int[] drawableState = super.onCreateDrawableState(extraSpace + addedState.length);
        mergeDrawableStates(drawableState, addedState);

        return drawableState;
    }

    public void setPrivateMode(boolean isPrivate) {
        if (this.mIsPrivate != isPrivate) {
            this.mIsPrivate = isPrivate;
            refreshDrawableState();
            invalidate();
        }
    }

    public void setLightTheme(boolean isLightTheme) {
        if (this.mIsLightTheme != isLightTheme) {
            this.mIsLightTheme = isLightTheme;
            for (int i = 0; i < this.getChildCount(); i++) {
                ((ThemedTextView)this.getChildAt(i)).setLightTheme(isLightTheme);
            }
            refreshDrawableState();
            invalidate();
        }
    }
    /* Cliqz End */

    public static interface OnTabChangedListener {
        public void onTabChanged(int tabIndex);
    }

    public IconTabWidget(Context context, AttributeSet attrs) {
        super(context, attrs);

        TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.IconTabWidget);
        mButtonLayoutId = a.getResourceId(R.styleable.IconTabWidget_android_layout, 0);
        a.recycle();

        if (mButtonLayoutId == 0) {
            throw new RuntimeException("You must supply layout attribute");
        }
    }

    public View addTab(final int imageResId, final int stringResId, final int buttonID) {
        View button = LayoutInflater.from(getContext()).inflate(mButtonLayoutId, this, false);

        /* Cliqz start o/
        // change button view to text and add Id to it
        ((ImageButton) button).setImageResource(imageResId);
        button.setContentDescription(getContext().getString(stringResId));*/
        ((TextView) button).setText(getContext().getString(stringResId));
        button.setId(buttonID);
        /* Cliqz end */
        addView(button);
        button.setOnClickListener(new TabClickListener(getTabCount() - 1));
        button.setOnFocusChangeListener(this);
        return button;
    }

    public void setTabSelectionListener(OnTabChangedListener listener) {
        mListener = listener;
    }

    @Override
    public void onFocusChange(View view, boolean hasFocus) {
    }

    private class TabClickListener implements OnClickListener {
        private final int mIndex;

        public TabClickListener(int index) {
            mIndex = index;
        }

        @Override
        public void onClick(View view) {
            if (mListener != null)
                mListener.onTabChanged(mIndex);
        }
    }

    /**
     * Fetch the Drawable icon corresponding to the given panel.
     *
     * @param panel to fetch icon for.
     * @return Drawable instance, or null if no icon is being displayed, or the icon does not exist.
     */
    public Drawable getIconDrawable(int index) {
        // We can have multiple views in the tabs panel for each child. This finds the
        // first view corresponding to the given tab. This varies by Android
        // version. The first view should always be our ImageButton, but let's
        // be safe.
        final View view = getChildTabViewAt(index);
        if (view instanceof ImageButton) {
            return ((ImageButton) view).getDrawable();
        }
        return null;
    }

    public void setIconDrawable(int index, int resource) {
        // We can have multiple views in the tabs panel for each child. This finds the
        // first view corresponding to the given tab. This varies by Android
        // version. The first view should always be our ImageButton, but let's
        // be safe.
        final View view = getChildTabViewAt(index);
        if (view instanceof ImageButton) {
            ((ImageButton) view).setImageResource(resource);
        }
    }
}
