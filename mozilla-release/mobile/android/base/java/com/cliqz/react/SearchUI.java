package com.cliqz.react;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.view.ViewGroup;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.ReactRootView;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.common.LifecycleState;
import com.facebook.react.shell.MainReactPackage;

import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.AppBackgroundManager;
import org.mozilla.geckoview.BuildConfig;

/**
 * @author Khaled Tantawy
 */
public class SearchUI implements ReactInstanceManager.ReactInstanceEventListener {
    private ReactInstanceManager mReactInstanceManager;
    private ReactRootView mReactRootView;
    private ReactContext mReactContext;
    public boolean isReady = false;

    public SearchUI() {}

    public void initialize(Context context, Application application) {
        mReactRootView = new ReactRootView(context);
        mReactInstanceManager = ReactInstanceManager.builder()
                .setApplication(application)
                .setBundleAssetName("index.android.bundle")
                .setJSMainModulePath("index")
                .addPackage(new BridgePackage())
                .addPackage(new MainReactPackage())
                .setUseDeveloperSupport(BuildConfig.DEBUG)
                .setInitialLifecycleState(LifecycleState.RESUMED)
                .build();

        mReactRootView.startReactApplication(mReactInstanceManager, "BrowserCoreApp", null);
        final AppBackgroundManager backgroundManager = AppBackgroundManager.getInstance(context);
        if (PreferenceManager.getInstance().isBackgroundEnabled()) {
            backgroundManager.setViewBackground(mReactRootView, ContextCompat.getColor(context, R.color.general_blue_color));
        } else {
            backgroundManager.setViewBackgroundDefaultColor(mReactRootView);
        }
        isReady = true;
    }

    public void show() {
        if (mReactRootView != null) {
            mReactRootView.setVisibility(View.VISIBLE);
        }
    }

    public void showDevOptionsDialog() {
        mReactInstanceManager.showDevOptionsDialog();
    }

    public void hide() {
        mReactRootView.setVisibility(View.INVISIBLE);
    }

    public void addToView(ViewGroup parent) {
        if (mReactRootView != null) {
            parent.addView(mReactRootView);
        }
    }

    public void onResume(Activity activity) {
        mReactInstanceManager.onHostResume(activity);
    }

    public void onPause(Activity activity) {
        if (mReactInstanceManager != null) {
            mReactInstanceManager.onHostPause(activity);
        }
    }

    public void onDestroy(Activity activity) {
        if (mReactInstanceManager != null) {
            mReactInstanceManager.onHostDestroy(activity);
        }
        if (mReactRootView != null) {
            mReactRootView.unmountReactApplication();
        }
    }

    @Override
    public void onReactContextInitialized(ReactContext context) {
        mReactContext = context;
    }
    
    public void removeFromView(ViewGroup parent) {
        if (mReactRootView != null) {
            parent.removeView(mReactRootView);
        }
    }
}
