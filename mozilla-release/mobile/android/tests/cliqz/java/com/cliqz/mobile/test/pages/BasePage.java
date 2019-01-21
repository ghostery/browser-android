package com.cliqz.mobile.test.pages;

import android.content.SharedPreferences;

import com.cliqz.mobile.test.driver.NativeDriver;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.rules.TestName;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.firstrun.FirstrunAnimationContainer;
import org.mozilla.gecko.preferences.GeckoPreferences;

import java.io.File;
import java.io.IOException;

/**
 * Copyright © Cliqz 2019
 */
public class BasePage {

    public static NativeDriver driver;

    @Rule
    public TestName testName = new TestName();

    @BeforeClass
    public static void setUpClass() {
        driver = new NativeDriver();
    }

    @Before
    public void setUp() throws Exception {
        driver.disableAnimations();
        driver.launchActivity();
        driver.sleep(1); // Cool down after the Activity is launched
    }

    @After
    public void tearDown() {
        driver.takeScreenshot(testName.getMethodName()
                + "_TestEndScreenshot");
    }

    public void onboardingPref(boolean enabled) {
        final SharedPreferences prefs = GeckoSharedPrefs.forProfile(driver.targetContext);
        if (enabled) {
            prefs.edit()
                    .remove(FirstrunAnimationContainer.PREF_FIRSTRUN_ENABLED)
                    .remove("firstrun_uuid")
                    .apply();
        } else {
            prefs.edit()
                    .putBoolean(FirstrunAnimationContainer.PREF_FIRSTRUN_ENABLED, false)
                    .apply();
        }
    }

    public void defaultBrowserPopupPref(boolean enabled) {
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(driver.targetContext);
        int count = enabled ? 2 : 4;
        prefs.edit()
                .putInt(GeckoPreferences.PREFS_APP_LAUNCH_COUNT, count)
                .apply();
    }

    public void clearData() {
        try {
            File[] appDataFiles = driver.targetContext.getFilesDir().listFiles();
            if (appDataFiles != null) {
                for (File dataFile : appDataFiles) {
                    Runtime.getRuntime().exec("rm -rf " + dataFile.toString());
                }
            }
        } catch(IOException ioe) {
            driver.log(ioe.toString());
        }
    }
}
