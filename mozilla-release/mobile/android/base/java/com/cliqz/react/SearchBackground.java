package com.cliqz.react;

import com.facebook.react.ReactInstanceManager;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.common.LifecycleState;
import com.facebook.react.shell.MainReactPackage;
import org.mozilla.gecko.BuildConfig;

import android.app.Activity;
import android.app.Application;


public class SearchBackground {
    private static SearchBackground ourInstance;
    public final ReactInstanceManager mReactInstanceManager;
    private ReactContext mReactContext;

    public static SearchBackground getInstance() {
        return ourInstance;
    }

    public static void initialize(Application application) {
        ourInstance = new SearchBackground(application);
    }

    private SearchBackground(Application application) {
        mReactInstanceManager = ReactInstanceManager.builder()
                .setApplication(application)
                .setBundleAssetName("index.android.bundle")
                .setJSMainModulePath("index")
                .addPackage(new BridgePackage())
                .addPackage(new MainReactPackage())
                .setUseDeveloperSupport(BuildConfig.DEBUG)
                .setInitialLifecycleState(LifecycleState.RESUMED)
                .build();
        mReactInstanceManager.createReactContextInBackground();
    }

    public void showDevOptionsDialog() {
        mReactInstanceManager.showDevOptionsDialog();
    }
}
