package com.cliqz.react;

import com.cliqz.react.modules.BridgeModule;
import com.cliqz.react.modules.BrowserActionsModule;
import com.cliqz.react.modules.LocaleConstantsModule;
import com.cliqz.react.modules.PrefsModule;
import com.cliqz.react.modules.SearchEnginesModule;
import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;

import java.util.ArrayList;
import java.util.List;

/**
 * Copyright © Cliqz 2019
 */
public class BridgePackage implements ReactPackage {
    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        final List<NativeModule> modules = new ArrayList<>();

        modules.add(new BridgeModule(reactContext));
        modules.add(new PrefsModule(reactContext));
        modules.add(new BrowserActionsModule(reactContext));
        modules.add(new LocaleConstantsModule(reactContext));
        modules.add(new SearchEnginesModule(reactContext));

        return modules;
    }

    @Override
    @SuppressWarnings({"rawtypes"})
    public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
        final List<ViewManager> viewManagers = new ArrayList<>();
        viewManagers.add(new NativeDrawableManager());

        return viewManagers;
    }
}
