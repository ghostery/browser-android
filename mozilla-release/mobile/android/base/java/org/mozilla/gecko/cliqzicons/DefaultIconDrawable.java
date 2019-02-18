package org.mozilla.gecko.cliqzicons;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.support.annotation.NonNull;

/**
 * Copyright © Cliqz 2018
 */
public class DefaultIconDrawable extends Drawable {

    private final String name;
    private final int color;
    private final int radius;
    private final Paint paint;

    /**
     * A drawable made by the name in white with given size over the color background
     *
     * @param name The text to render centered in the drawable
     * @param color The background color
     * @param textSize icon text size in pixels
     */
    DefaultIconDrawable(@NonNull String name, int color, int textSize, int radius) {
        this.name = name.length() > 1 ? name.substring(0,2) : name;
        this.color = color;
        this.radius = radius;
        this.paint = new Paint(Paint.ANTI_ALIAS_FLAG|Paint.SUBPIXEL_TEXT_FLAG);
        paint.setTextSize(textSize);
        paint.setFakeBoldText(true);
        paint.setTextAlign(Paint.Align.CENTER);
        paint.setColor(Color.WHITE);
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        final Rect bounds = getBounds();
        final float w = bounds.width();
        final float h = bounds.height();
        paint.setColor(color);
        canvas.drawRoundRect(0F, 0F, w, h, radius, radius, paint);
        final float xPos = (w / 2F);
        final float yPos = (h / 2F) - ((paint.descent() + paint.ascent()) / 2F);
        paint.setColor(Color.WHITE);
        canvas.drawText(name, xPos, yPos, paint);
    }

    @Override
    public void setAlpha(int alpha) {
        // Nothing to do here
    }

    @Override
    public void setColorFilter(ColorFilter colorFilter) {
        // Nothing to do here
    }

    @Override
    public int getOpacity() {
        return PixelFormat.OPAQUE;
    }
}

