package com.cliqz.react;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.ReactRootView;
import com.facebook.react.bridge.ReactContext;

import org.mozilla.gecko.util.AppBackgroundManager;

/**
 * Copyright © Cliqz 2019
 */
public class SearchUI {
    private ReactRootView mReactRootView;
    private ReactInstanceManager mReactInstanceManager;

    private AppBackgroundManager mBackgroundManager;

    public SearchUI() {}

    public void onCreate(Context context) {
        if (mReactRootView != null) {
            return;
        }

        mBackgroundManager = AppBackgroundManager.getInstance(context);

        final SearchBackground mSearchBackground = SearchBackground.getInstance();
        mSearchBackground.createView(context);

        mReactInstanceManager = mSearchBackground.mReactInstanceManager;
        mReactRootView = mSearchBackground.mReactRootView;
    }

    public void show() {
        if (mReactRootView != null) {
            mReactRootView.setVisibility(View.VISIBLE);
            mBackgroundManager.setSearchViewBackground(mReactRootView);
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

    public void removeFromView(ViewGroup parent) {
        if (mReactRootView != null) {
            parent.removeView(mReactRootView);
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
            mReactRootView = null;
        }
    }
}
