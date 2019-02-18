package com.cliqz.react;

import android.app.Application;
import android.content.Context;
import android.support.v4.content.ContextCompat;

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

    public SearchUI(Context context, Application application) {
        mReactRootView = new ReactRootView(context);
        mReactInstanceManager = ReactInstanceManager.builder()
                .setApplication(application)
                .setBundleAssetName("index.android.bundle")
                .setJSMainModulePath("index")
                .addPackage(new BridgePackage())
                .addPackage(new HistoryDialsPackage())
                .addPackage(new MainReactPackage())
                .setUseDeveloperSupport(BuildConfig.DEBUG)
                .setInitialLifecycleState(LifecycleState.RESUMED)
                .build();

        mReactRootView.startReactApplication(mReactInstanceManager, "BrowserCoreApp", null);
        final AppBackgroundManager backgroundManager = AppBackgroundManager.getInstance(context);
        if (PreferenceManager.getInstance(context).isBackgroundEnabled()) {
            backgroundManager.setViewBackground(mReactRootView, ContextCompat.getColor(context, R.color.general_blue_color));
        } else {
            backgroundManager.setViewBackgroundDefaultColor(mReactRootView);
        }
    }

    public ReactRootView getRootView() {
        return mReactRootView;
    }

    public ReactInstanceManager getInstanceManager() {
        return mReactInstanceManager;
    }

    @Override
    public void onReactContextInitialized(ReactContext context) {
        mReactContext = context;
    }
}
