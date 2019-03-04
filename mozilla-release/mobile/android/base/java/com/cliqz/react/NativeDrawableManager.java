package com.cliqz.react;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.support.v7.content.res.AppCompatResources;
import android.util.Log;
import android.widget.ImageView;

import com.facebook.react.uimanager.SimpleViewManager;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

import javax.annotation.Nullable;

/**
 * Copyright © Cliqz 2019
 */
public class NativeDrawableManager extends SimpleViewManager<ImageView> {

    private static final String REACT_CLASS = "NativeDrawable";
    private static final String TAG = NativeDrawableManager.class.getSimpleName();

    @Override
    public String getName() {
        return REACT_CLASS;
    }

    @Override
    protected ImageView createViewInstance(ThemedReactContext reactContext) {
        return new ImageView(reactContext);
    }

    @ReactProp(name = "color")
    public void setColorFilter(ImageView view, @Nullable String colorFilter) {
        view.setColorFilter(Color.parseColor(colorFilter));
    }

    @ReactProp(name = "source")
    public void setSrc(ImageView view, @Nullable String src) {
        final Context imageContext = view.getContext();
        final Resources resources = imageContext.getResources();
        final int id = resources.getIdentifier(src, "drawable", imageContext.getPackageName());
        if (id > 0) {
            final Drawable drawable = AppCompatResources.getDrawable(imageContext, id);
            view.setImageDrawable(drawable);
        } else {
            Log.e(TAG, "Vector drawable " + src  + " doesn't exist");
        }
        view.setScaleType(ImageView.ScaleType.FIT_XY);
    }
}