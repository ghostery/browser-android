package org.mozilla.gecko.util;

import android.content.Context;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;
import com.squareup.picasso.Transformation;

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

    private static final String BACKGROUND_IMAGE_URL_FORMAT = BuildConfig.DEBUG ?
            "https://cdn.cliqz.com/mobile/background/android/staging/%s_%d.jpg" :
            "https://cdn.cliqz.com/mobile/background/android/production/%s_%d.jpg";
    private static final int MIN_BACKGROUND_SIZE = 240;
    private static final int MAX_BACKGROUND_SIZE = 3840;
    private static final int SIZE_DELTA = 120;
    // private static final long LAYOUT_SAFE_DELAY = 1000L;

    private final DisplayMetrics displayMetrics = new DisplayMetrics();
    private final Resources resources;
    private final String backgroundImageUri;
    private final int backgroundSize;

    private AppBackgroundManager(Context context) {
        resources = context.getResources();
        WindowManager windowManager = (WindowManager) context.getApplicationContext()
                .getSystemService(Context.WINDOW_SERVICE);
        if (windowManager != null) {
            windowManager.getDefaultDisplay().getMetrics(displayMetrics);
        }
        // Find the right image size
        final int screenSize = Math.max(displayMetrics.widthPixels, displayMetrics.heightPixels);
        final int q = screenSize / SIZE_DELTA;
        final int r = screenSize % SIZE_DELTA;
        backgroundSize = (q * SIZE_DELTA) + (r > 0 ? SIZE_DELTA : 0);
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
        view.setTag(R.id.background_manager_mode_key, Mode.COLOR);
        removeLayoutChangedListener(view);
        // Default color is a gradient.
        view.setBackgroundResource(R.drawable.cliqz_default_background);
    }

    @UiThread
    private void removeLayoutChangedListener(View view) {
        final LayoutChangedListener listener =
                (LayoutChangedListener) view.getTag(R.id.background_manager_listener_key);
        if (listener != null) {
            view.removeOnLayoutChangeListener(listener);
            view.setTag(R.id.background_manager_listener_key, null);
        }
    }

    @UiThread
    public void setViewBackground(@NonNull final View view, @ColorInt int fallbackColor) {
        if (view.getTag(R.id.background_manager_mode_key) == Mode.IMAGE) {
            // Every thing is already in place, we have nothing to do here
            return;
        }
        view.setTag(R.id.background_manager_mode_key, Mode.IMAGE);
        view.setBackgroundColor(fallbackColor);
        addLayoutChangedListener(view);
        internalSetViewBackground(view);
    }

    private Rect genBitmapRect(View view) {
        final int screenWitdh;
        final int screenHeight;
        final int orientation = resources.getConfiguration().orientation;
        if (orientation == Configuration.ORIENTATION_PORTRAIT) {
            screenWitdh = Math.min(displayMetrics.widthPixels, displayMetrics.heightPixels);
            screenHeight = Math.max(displayMetrics.widthPixels, displayMetrics.heightPixels);
        } else {
            screenWitdh = Math.max(displayMetrics.widthPixels, displayMetrics.heightPixels);
            screenHeight = Math.min(displayMetrics.widthPixels, displayMetrics.heightPixels);
        }
        final int[] location = new int[2];
        view.getLocationOnScreen(location);
        final int offLeft = (backgroundSize - screenWitdh) / 2;
        final int offTop = (backgroundSize - screenHeight) / 2;
        final int l = offLeft + location[0];
        final int t = offTop + Math.max(0, location[1]);
        final int r = l + view.getWidth();
        final int b = t + view.getHeight();
        return new Rect(l, t, r, b);
    }

    @UiThread
    private void internalSetViewBackground(final View view) {
        // Somebody set a color, we do not set the background view
        if (!Mode.IMAGE.equals(view.getTag(R.id.background_manager_mode_key))) {
            return;
        }

        Picasso.with(view.getContext())
                .load(backgroundImageUri)
                .transform(new CropTransformation(view))
                .into((Target) view);
    }

    @UiThread
    private void addLayoutChangedListener(View view) {
        LayoutChangedListener listener =
                (LayoutChangedListener) view.getTag(R.id.background_manager_listener_key);
        if (listener == null) {
            final LayoutChangedListener newListener = new LayoutChangedListener(view);
            view.addOnLayoutChangeListener(newListener);
            view.setTag(R.id.background_manager_listener_key, newListener);
        }
    }

    private class LayoutChangedListener implements View.OnLayoutChangeListener {

        private final View view;

        LayoutChangedListener(View view) {
            this.view = view;
        }

        @Override
        public void onLayoutChange(View v, int left, int top, int right, int bottom,
                                   int oldLeft, int oldTop, int oldRight, int oldBottom) {
            final int width = right - left;
            final int height = bottom - top;
            final boolean layoutHasChanged = left != oldLeft || top != oldTop ||
                    right != oldRight || bottom != oldBottom;
            final boolean hasValidDimensions = width != 0 && height != 0;
            if (layoutHasChanged && hasValidDimensions) {
                if (BuildConfig.DEBUG) {
                    Log.d(TAG, String.format(Locale.US, "Layout changed - %d %d %d %d",
                            left, top, right, bottom));
                }
                internalSetViewBackground(view);
            }
        }
    }

    private class CropTransformation implements Transformation {

        final View view;

        CropTransformation(View view) {
            this.view = view;
        }

        @Override
        public Bitmap transform(Bitmap source) {
            final Rect bitmapRect = genBitmapRect(view);
            final Bitmap result = Bitmap.createBitmap(source, bitmapRect.left, bitmapRect.top, bitmapRect.right-bitmapRect.left,bitmapRect.bottom-bitmapRect.top);
            source.recycle();
            return result;
        }

        @Override
        public String key() {
            //need different keys for different orientation for caching purposes
            final int orientation = view.getResources().getConfiguration().orientation;
            final String rect = genBitmapRect(view).flattenToString();
            return String.format(Locale.US, "cliqzCrop-%d-%s", orientation, rect);
        }
    }
}

