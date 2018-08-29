/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.tabs;

import org.mozilla.gecko.R;
import org.mozilla.gecko.ThumbnailHelper;
import org.mozilla.gecko.widget.CropImageView;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.LayerDrawable;
import android.graphics.drawable.StateListDrawable;
import android.support.graphics.drawable.VectorDrawableCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.graphics.drawable.DrawableCompat;
import android.support.v7.app.AppCompatDelegate;
import android.util.AttributeSet;

/**
 *  A width constrained ImageView to show thumbnails of open tabs in the tabs panel.
 */
public class TabsPanelThumbnailView extends CropImageView {
    public static final String LOGTAG = "Gecko" + TabsPanelThumbnailView.class.getSimpleName();

    /* Cliqz start */
    private final Path clipPath;
    private final float[] roundedRectRadii;
    private final RectF rectF;

    public TabsPanelThumbnailView(final Context context) {
        this(context, null);
    }

    public TabsPanelThumbnailView(final Context context, final AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public TabsPanelThumbnailView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        clipPath = new Path();
        float radius = getResources().getDimension(R.dimen.tab_thumbnail_corner_radius);
        // The thumbnail has rounded corners from the bottom.
        roundedRectRadii =
                new float[] { 0, 0, 0, 0, radius, radius, radius, radius };
        rectF = new RectF();
    }

    @Override
    protected float getAspectRatio() {
        return ThumbnailHelper.TABS_PANEL_THUMBNAIL_ASPECT_RATIO;
    }

    @Override
    public void setImageDrawable(Drawable drawable) {
        boolean resize = true;

        if (drawable == null) {
            final StateListDrawable states = new StateListDrawable();
            states.addState(PRIVATE_STATE_SET, privateModeImageDrawable());
            states.addState(EMPTY_STATE_SET, normalModeImageDrawable());
            drawable = states;
            resize = false;
            setScaleType(ScaleType.CENTER_CROP);
        }

        super.setImageDrawable(drawable, resize);
    }

    private LayerDrawable normalModeImageDrawable() {
        final Context context = getContext();
        final Drawable backgroundLayer = ContextCompat.getDrawable(context, R.color.tab_item_thumbnail_default_bg);
        final Drawable logoLayer = ContextCompat.getDrawable(context, R.drawable.globe_light);
        final LayerDrawable layerDrawable = new LayerDrawable(new Drawable[] { backgroundLayer, logoLayer });
        final int padding = (int) getResources().getDimension(R.dimen.tab_thumbnail_normal_mode_image_padding);
        layerDrawable.setLayerInset(1, padding, padding, padding, padding);
        return layerDrawable;
    }

    private LayerDrawable privateModeImageDrawable() {
        final Context context = getContext();
        final Resources resources = getResources();
        final Drawable backgroundLayer = ContextCompat.getDrawable(context, android.R.color.black);
        final Drawable logoLayer = VectorDrawableCompat.create(resources, R.drawable.ic_ghosty_forget, null);
        DrawableCompat.setTint(logoLayer, ContextCompat.getColor(context, R.color.general_blue_color));
        final LayerDrawable layerDrawable = new LayerDrawable(new Drawable[] { backgroundLayer, logoLayer });
        final int padding = (int) resources.getDimension(R.dimen.tab_thumbnail_private_mode_image_padding);
        layerDrawable.setLayerInset(1, padding, padding, padding, padding);
        return layerDrawable;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        rectF.set(0, 0, this.getWidth(), this.getHeight());
        clipPath.addRoundRect(rectF, roundedRectRadii, Path.Direction.CW);
        canvas.clipPath(clipPath);
        super.onDraw(canvas);
    }
    /* Cliqz end */
}
