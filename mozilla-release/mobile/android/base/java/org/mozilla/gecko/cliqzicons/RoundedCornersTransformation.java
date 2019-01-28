package org.mozilla.gecko.cliqzicons;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;

import com.squareup.picasso.Transformation;

/**
 * Copyright © Cliqz 2018
 *
 * Util class to round the corners of a bitmap
 */
public class RoundedCornersTransformation implements Transformation {

    private final float radii;

    public RoundedCornersTransformation(float radii) {
        this.radii = radii;
    }

    public RoundedCornersTransformation() {
        this.radii = 0f;
    }

    private Bitmap createRoundedRectBitmap(Bitmap bitmap,
                                           float topLeftCorner, float topRightCorner,
                                           float bottomRightCorner, float bottomLeftCorner) {
        final Bitmap output = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(),
                Bitmap.Config.ARGB_8888);
        final Canvas canvas = new Canvas(output);

        final int color = Color.WHITE;
        final Paint paint = new Paint();
        final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
        final RectF rectF = new RectF(rect);
        final Path path = new Path();
        final float[] radii = new float[]{
                topLeftCorner, bottomLeftCorner,
                topRightCorner, topRightCorner,
                bottomRightCorner, bottomRightCorner,
                bottomLeftCorner, bottomLeftCorner
        };

        paint.setAntiAlias(true);
        canvas.drawARGB(0, 0, 0, 0);
        paint.setColor(color);
        path.addRoundRect(rectF, radii, Path.Direction.CW);
        canvas.drawPath(path, paint);
        paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_IN));
        canvas.drawBitmap(bitmap, rect, rect, paint);
        return output;
    }

    @Override
    public Bitmap transform(Bitmap source) {
        final int size = Math.min(source.getWidth(), source.getHeight());

        final int x = (source.getWidth() - size) / 2;
        final int y = (source.getHeight() - size) / 2;

        final Bitmap squaredBitmap = Bitmap.createBitmap(source, x, y, size, size);
        if (squaredBitmap != source) {
            source.recycle();
        }

        final Bitmap roundedBitmap;
        if (radii == 0f) {
            float r = size / 4f;
            roundedBitmap = createRoundedRectBitmap(squaredBitmap, r, r, r, r);
        } else {
            roundedBitmap = createRoundedRectBitmap(squaredBitmap, radii, radii, radii, radii);
        }

        squaredBitmap.recycle();

        return roundedBitmap;
    }

    @Override
    public String key() {
        return "rounded_corners";
    }
}
