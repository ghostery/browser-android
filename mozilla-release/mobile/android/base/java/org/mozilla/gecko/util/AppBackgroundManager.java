package org.mozilla.gecko.util;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import org.mozilla.gecko.BuildConfig;
import org.mozilla.gecko.R;

import java.util.Locale;

/**
 * Copyright © Cliqz 2018
 */
public class AppBackgroundManager {

    private enum Mode {
        COLOR,
        IMAGE
    }

    private static AppBackgroundManager appBackgroundManagerInstance = null;

    private static final String TAG = AppBackgroundManager.class.getSimpleName();

    // TODO: Make this branding dependant
    private static final String BACKGROUND_IMAGE_URL_FORMAT = BuildConfig.DEBUG ?
            "https://cdn.ghostery.net/mobile/background/android/staging/%s_%d.jpg" :
            "https://cdn.ghostery.net/mobile/background/android/production/%s_%d.jpg";
    private static final int MIN_BACKGROUND_SIZE = 240;
    private static final int MAX_BACKGROUND_SIZE = 3840;
    private static final int SIZE_DELTA = 120;
    // private static final long LAYOUT_SAFE_DELAY = 1000L;

    private final String backgroundImageUri;

    private AppBackgroundManager(Context context) {
        final WindowManager windowManager = (WindowManager) context.getApplicationContext()
                .getSystemService(Context.WINDOW_SERVICE);
        final DisplayMetrics displayMetrics = new DisplayMetrics();
        if (windowManager != null) {
            windowManager.getDefaultDisplay().getMetrics(displayMetrics);
        }
        // Find the right image size (divided by 2 to spare memory)
        final int screenSize = Math.max(displayMetrics.widthPixels, displayMetrics.heightPixels) / 2;
        final int q = screenSize / SIZE_DELTA;
        final int r = screenSize % SIZE_DELTA;
        final int backgroundSize = (q * SIZE_DELTA) + (r > 0 ? SIZE_DELTA : 0);
        if (backgroundSize < MIN_BACKGROUND_SIZE || backgroundSize > MAX_BACKGROUND_SIZE) {
            Log.e(TAG, "No valid image size found: " + backgroundSize);
            backgroundImageUri = null;
        } else {
            backgroundImageUri = String.format(Locale.US,
                    BACKGROUND_IMAGE_URL_FORMAT, "default", backgroundSize);
        }
    }

    public static AppBackgroundManager getInstance(Context context) {
        if (appBackgroundManagerInstance == null) {
            appBackgroundManagerInstance = new AppBackgroundManager(context);
        }
        return appBackgroundManagerInstance;
    }

    @UiThread
    public void setViewBackgroundDefaultColor(@NonNull View view) {
        if (view.getTag(R.id.background_manager_mode_key) == Mode.COLOR) {
            // Every thing is already in place, we have nothing to do here
            return;
        }
        view.setTag(R.id.background_manager_mode_key, Mode.COLOR);
        view.setTag(R.id.background_manager_target_key, null);
        // Default color is a gradient.
        view.setBackgroundResource(R.drawable.cliqz_default_background);
    }

    @UiThread
    public void setViewBackground(@NonNull final View view, @ColorInt int fallbackColor) {
        if (view.getTag(R.id.background_manager_mode_key) == Mode.IMAGE) {
            // Every thing is already in place, we have nothing to do here
            return;
        }
        view.setTag(R.id.background_manager_mode_key, Mode.IMAGE);
        // Picasso keeps only a week reference to the target: in order to keep the target alive,
        // we tag it in the view.
        final PicassoTarget target = new PicassoTarget(view);
        view.setTag(R.id.background_manager_target_key, target);
        view.setBackgroundColor(fallbackColor);

        Picasso.with(view.getContext())
                .load(backgroundImageUri)
                .into(target);
    }

    private class PicassoTarget implements Target {

        private final View view;

        PicassoTarget(View view) {
            this.view = view;
        }

        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            // Somebody set a color while we were loading, we do not set the background view
            if (!Mode.IMAGE.equals(view.getTag(R.id.background_manager_mode_key))) {
                return;
            }
            view.setTag(R.id.background_manager_target_key, null);
            final Drawable backgroundDrawable = new CustomBitmapDrawable(view.getResources(), bitmap);
            view.setBackground(backgroundDrawable);
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
            view.setTag(R.id.background_manager_target_key, null);
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    }

    private static class CustomBitmapDrawable extends StateListDrawable {

        final Rect srcRect;
        final Rect dstRect;
        final int size;
        final int urlBarHeight;
        private boolean mIsPrivate;
        final Paint paint;
        final Paint privatePaint;
        private Bitmap bitmap;

        CustomBitmapDrawable(Resources resources, Bitmap bitmap) {
            super();
            this.bitmap = bitmap;
            size = bitmap.getWidth() * 2;
            srcRect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
            dstRect = new Rect();
            urlBarHeight = (int) resources.getDimension(R.dimen.browser_toolbar_height);
            mIsPrivate = false;
            paint = new Paint();
            privatePaint = new Paint();
            privatePaint.setColor(android.R.color.black);
            privatePaint.setAlpha(64);
        }

        @Override
        protected boolean onStateChange(int[] stateSet) {
            final boolean oldState = mIsPrivate;
            mIsPrivate = false;
            for (int state: stateSet) {
                mIsPrivate |= state == R.attr.state_private;
            }
            return oldState != mIsPrivate;
        }

        @Override
        public void draw(Canvas canvas) {
            final int cw = canvas.getWidth();
            final int left = -1 * Math.abs(cw - size) / 2;
            final int top = -urlBarHeight;
            dstRect.set(left, top, left + size, top + size);
            canvas.drawBitmap(bitmap, srcRect, dstRect, paint);
            if(mIsPrivate){
                canvas.drawRect(dstRect, privatePaint);
            }
        }
    }
}

