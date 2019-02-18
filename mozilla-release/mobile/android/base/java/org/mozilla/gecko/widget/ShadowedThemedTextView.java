package org.mozilla.gecko.widget;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.TypedArray;
import android.util.AttributeSet;

import org.mozilla.gecko.R;
import org.mozilla.gecko.widget.themed.ThemedTextView;

public class ShadowedThemedTextView extends ThemedTextView {

    private ColorStateList mShadowColors;
    private float mShadowDx;
    private float mShadowDy;
    private float mShadowRadius;

    public ShadowedThemedTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize(context, attrs, 0);
    }

    public ShadowedThemedTextView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initialize(context, attrs, defStyle);
    }

    private void initialize(final Context context, final AttributeSet attrs, final int defStyle) {
        final TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.ShadowedThemedTextView);

        final int attributeCount = a.getIndexCount();

        for (int i = 0; i < attributeCount; i++) {
            final int currentAttribute = a.getIndex(i);

            switch (currentAttribute) {
                case R.styleable.ShadowedThemedTextView_shadowColors:
                    mShadowColors = a.getColorStateList(currentAttribute);
                    break;
                case R.styleable.ShadowedThemedTextView_android_shadowDx:
                    mShadowDx = a.getFloat(currentAttribute, 0);
                    break;
                case R.styleable.ShadowedThemedTextView_android_shadowDy:
                    mShadowDy = a.getFloat(currentAttribute, 0);
                    break;
                case R.styleable.ShadowedThemedTextView_android_shadowRadius:
                    mShadowRadius = a.getFloat(currentAttribute, 0);
                    break;
                default:
                    break;
            }
        }

        a.recycle();
        updateShadowColor();
        invalidate();
    }


    private void updateShadowColor() {
        if (mShadowColors != null) {
            setShadowLayer(mShadowRadius, mShadowDx, mShadowDy, mShadowColors.getColorForState(getDrawableState(), 0));
        }
    }

    @Override
    public void setLightTheme(boolean isLightTheme) {
        super.setLightTheme(isLightTheme);
        updateShadowColor();
        refreshDrawableState();
        invalidate();
    }

}
