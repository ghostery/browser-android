package org.mozilla.gecko.widget;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

import org.mozilla.gecko.R;
import org.mozilla.gecko.util.ViewUtil;

/**
 * A view displaying an animated three dots progress strip. It doesn't use the Android animation
 * framework being targeted to low-end devices (single or dual core) that can not cope very well
 * with addons initialization.
 *
 * NOTE: we saw an indeterminate {@link android.widget.ProgressBar} making our browser
 * initialization taking twice the time needed with this approach due to animation taking too much
 * CPU time on a dual core device.
 *
 * Copyright © Cliqz 2019
 */
public class ThreeDotsAnimatedView extends View implements Runnable {
    private static final int DEFAULT_INTER_DOTS_SPACE = ViewUtil.dpToPx(5);
    private static final int DEFAULT_DOT_RADIUS = ViewUtil.dpToPx(6);
    private static final boolean[][] STATES = new boolean[][] {
            new boolean[] { false, false, false },
            new boolean[] { true,  false, false },
            new boolean[] { true,  true,  false },
            new boolean[] { true,  true,  true  }
    };

    private int mDotRadius;
    private int mLowlightColor;
    private int mHighlightColor;
    private int mCurrentState = 0;
    private Paint mPaint;
    private boolean mIsAnimationRunning = false;
    private int mInterDotSpace;

    public ThreeDotsAnimatedView(Context context) {
        super(context);
        init(context, null, 0, 0);
    }

    public ThreeDotsAnimatedView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs, 0, 0);
    }

    public ThreeDotsAnimatedView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs, defStyleAttr, 0);
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public ThreeDotsAnimatedView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init(context, attrs, defStyleAttr, defStyleRes);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mLowlightColor = Color.GRAY;
        mHighlightColor = Color.WHITE;
        if (attrs == null) {
            return;
        }

        final TypedArray typedArray = context.getTheme().obtainStyledAttributes(
                attrs, R.styleable.ThreeDotsAnimatedView, defStyleAttr, defStyleRes);
        try {
            mLowlightColor = typedArray
                    .getColor(R.styleable.ThreeDotsAnimatedView_lowlightDotColor,
                              Color.GRAY);
            mHighlightColor = typedArray
                    .getColor(R.styleable.ThreeDotsAnimatedView_highlightDotColor,
                              Color.WHITE);
            mDotRadius = typedArray
                    .getDimensionPixelSize(R.styleable.ThreeDotsAnimatedView_dotsRadius,
                                           DEFAULT_DOT_RADIUS);
            mInterDotSpace = typedArray
                    .getDimensionPixelSize(R.styleable.ThreeDotsAnimatedView_interDotSpace,
                                           DEFAULT_INTER_DOTS_SPACE);
        } finally {
            typedArray.recycle();
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        // width = 3 circles * diameter (2 * radius) + 2 inter spaces
        final int width = 6 * mDotRadius + 2 * mInterDotSpace;
        final int height = 2 * mDotRadius;
        widthMeasureSpec = MeasureSpec.makeMeasureSpec(width, MeasureSpec.EXACTLY);
        heightMeasureSpec = MeasureSpec.makeMeasureSpec(height, MeasureSpec.EXACTLY);
        setMeasuredDimension(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        final int y = getHeight() / 2;
        final boolean[] state = STATES[mCurrentState];
        for (int i = 0; i < state.length; i++) {
            final boolean dotState = state[i];
            mPaint.setColor(dotState ? mHighlightColor : mLowlightColor);
            final int x = mDotRadius + (2 * mDotRadius + mInterDotSpace) * i;
            canvas.drawCircle(x, y, mDotRadius, mPaint);
        }
    }

    @Override
    protected void onVisibilityChanged(@NonNull View changedView, int visibility) {
        super.onVisibilityChanged(changedView, visibility);
        if (visibility == VISIBLE) {
            scheduleAnimation();
        } else {
            stopAnimation();
        }
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        scheduleAnimation();
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        stopAnimation();
    }

    private void scheduleAnimation() {
        if (mIsAnimationRunning) {
            return;
        }
        mIsAnimationRunning = true;
        postDelayed(this, 500L);
    }

    private void stopAnimation() {
        mIsAnimationRunning = false;
    }

    @Override
    public void run() {
        if (mIsAnimationRunning) {
            mCurrentState = (mCurrentState + 1) % STATES.length;
            postInvalidate();
            postDelayed(this, 500L);
        }
    }
}
