package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.util.AttributeSet;

import com.github.mikephil.charting.charts.PieChart;

/**
 * Copyright © Cliqz 2018
 */
public class CliqzPieChart extends PieChart {

    private int mMeasuredSize = Integer.MAX_VALUE;

    public CliqzPieChart(Context context) {
        super(context);
    }

    public CliqzPieChart(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public CliqzPieChart(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        final int width = MeasureSpec.getSize(widthMeasureSpec);
        final int height = MeasureSpec.getSize(heightMeasureSpec);
        final int min = Math.min(width, height);
        if (min < mMeasuredSize) {
            mMeasuredSize = min;
        }
        final int measureSpec = MeasureSpec.makeMeasureSpec(mMeasuredSize, MeasureSpec.EXACTLY);
        setMeasuredDimension(measureSpec, measureSpec);
    }
}
