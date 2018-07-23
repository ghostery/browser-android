package org.mozilla.gecko.toolbar;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.support.v4.graphics.drawable.DrawableCompat;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.View;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.lwt.LightweightTheme;
import org.mozilla.gecko.util.ThreadUtils;

import java.util.Locale;

/**
 * Copyright © Cliqz 2018
 */
class Ghosty extends ToolbarRoundButton implements View.OnClickListener {

    public interface OnGhostyClickedListener {

        void toggleControlCenter();
    }

    private OnGhostyClickedListener mOnGhostyClickedListener;
    private Paint mPaint;
    private int mTrackerCount = 0;

    private Drawable mGhostyDrawable;
    private int mGhostyWidth;
    private int mGhostyHeight;
    private float mTextDistance;

    private Rect mOutTextBound = new Rect();

    public Ghosty(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize(context);
    }

    public Ghosty(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initialize(context);
    }

    private void initialize(Context context) {
        setOnClickListener(this);
        mPaint = new Paint();
        final DisplayMetrics metrics = getResources().getDisplayMetrics();
        final float textSize = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 10, metrics);
        mTextDistance = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 2, metrics);
        mPaint.setTextSize(textSize);
        mPaint.setTextAlign(Paint.Align.CENTER);
        mPaint.setColor(Color.WHITE);
        mGhostyDrawable = ContextCompat.getDrawable(context, R.drawable.url_bar_small_ghosty);
        DrawableCompat.setTint(mGhostyDrawable, Color.WHITE);
        mGhostyWidth = mGhostyDrawable.getIntrinsicWidth();
        mGhostyHeight = mGhostyDrawable.getIntrinsicHeight();
    }

    public void setOnGhostyClickedListener(OnGhostyClickedListener onGhostyClickedListener) {
        mOnGhostyClickedListener = onGhostyClickedListener;
    }

    private String getTrackerCountString() {
        if (mTrackerCount <= 0) {
            return "HELLO";
        } else {
            return String.format(Locale.US, "%d", mTrackerCount);
        }
    }

    @Override
    public void onClick(View view) {
        mOnGhostyClickedListener.toggleControlCenter();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        final int w = getWidth();
        final int h = getHeight();
        if (w == 0 || h == 0) {
            return;
        }
        final String count = getTrackerCountString();
        mPaint.getTextBounds(count, 0, count.length(), mOutTextBound);
        final float height = mGhostyHeight + mTextDistance + mOutTextBound.height();
        final float ghostyLeft = (w - mGhostyWidth) / 2;
        final float ghostyTop = (h - height) / 2;
        final float ghostyRight = ghostyLeft + mGhostyWidth;
        final float ghostyBottom = ghostyTop + mGhostyHeight;
        //canvas.drawARGB(255, 255, 255, 255);
        mGhostyDrawable.setBounds((int) ghostyLeft, (int) ghostyTop, (int) ghostyRight, (int) ghostyBottom);
        mGhostyDrawable.draw(canvas);
        setContentDescription(count);
        canvas.drawText(count, w / 2, ghostyTop + height, mPaint);
    }

    public final void setTrackerCount(int count) {
        ThreadUtils.assertOnUiThread();
        mTrackerCount = count;
        invalidate();
    }

    @Override
    public void setPrivateMode(boolean isPrivate) {
        super.setPrivateMode(isPrivate);
        if (mGhostyDrawable != null) {
            final int[] state = isPrivate ? new int[] { R.attr.state_private } : new int[] {};
            mGhostyDrawable.setState(state);
        }
    }
}
