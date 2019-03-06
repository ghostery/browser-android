package com.cliqz.react;

import android.app.Activity;
import android.content.Context;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.view.ViewGroup;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.ReactRootView;
import com.facebook.react.bridge.ReactContext;

import org.mozilla.gecko.AppConstants;
import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.AppBackgroundManager;

/**
 * Copyright © Cliqz 2019
 */
public class SearchUI {
    private ReactRootView mReactRootView;
    private ReactInstanceManager mReactInstanceManager;

    public SearchUI() {}

    public void onCreate(Context context) {
        final SearchBackground mSearchBackground = SearchBackground.getInstance();
        mReactInstanceManager = mSearchBackground.mReactInstanceManager;

        mReactRootView = new ReactRootView(context);
        mReactRootView.startReactApplication(mReactInstanceManager, "BrowserCoreApp", null);


        // TODO: move this logic to AppBackgroundManager
        final AppBackgroundManager backgroundManager = AppBackgroundManager.getInstance(context);
        if (PreferenceManager.getInstance().isBackgroundEnabled()) {
            backgroundManager.setViewBackground(mReactRootView, ContextCompat.getColor(context, R.color.general_blue_color));
        } else {
            backgroundManager.setViewBackgroundDefaultColor(mReactRootView);
        }
    }

    public void show() {
        if (mReactRootView != null) {
            mReactRootView.setVisibility(View.VISIBLE);
        }
    }

    public void hide() {
        if (mReactRootView != null) {
            mReactRootView.setVisibility(View.INVISIBLE);
        }
    }

    public void addToView(ViewGroup parent) {
        if (mReactRootView != null) {
            parent.addView(mReactRootView);
        }
    }

    public void onResume(Activity activity) {
        mReactInstanceManager.onHostResume(activity);
        final ReactContext reactContext = mReactInstanceManager.getCurrentReactContext();
        if (reactContext != null) {
            reactContext.onHostResume(activity);
        }
    }

    public void onDestroy() {
        if (mReactRootView != null) {
            mReactRootView.unmountReactApplication();
        }
    }
}
