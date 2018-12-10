package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.util.AttributeSet;

import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.utils.Utils;

/**
 * Copyright © Cliqz 2018
 */
public class CliqzPieChart extends PieChart {

    private static final int MAX_HEIGHT_DP = 180;

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
        final int maxHeight = (int)Utils.convertDpToPixel(MAX_HEIGHT_DP);
        int measuredHeight = MeasureSpec.getSize(heightMeasureSpec);
        if (measuredHeight > maxHeight) {
            final int measureMode = MeasureSpec.getMode(heightMeasureSpec);
            //the layout should be a square so we also resize the width
            heightMeasureSpec = MeasureSpec.makeMeasureSpec(maxHeight, measureMode);
            widthMeasureSpec = heightMeasureSpec;
        }
        setMeasuredDimension(widthMeasureSpec, heightMeasureSpec);
    }
}
