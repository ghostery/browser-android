package com.cliqz.mobile.test.driver;

import android.app.Instrumentation;
import android.content.Context;
import android.content.Intent;
import android.os.Environment;
import android.os.IBinder;
import android.support.test.InstrumentationRegistry;
import android.support.test.uiautomator.By;
import android.support.test.uiautomator.BySelector;
import android.support.test.uiautomator.Direction;
import android.support.test.uiautomator.UiDevice;
import android.support.test.uiautomator.UiObject2;
import android.support.test.uiautomator.Until;
import android.util.Log;

import org.mozilla.gecko.BuildConfig;

import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.fail;

/**
 * Copyright © Cliqz 2019
 */
public class NativeDriver {

    // *** Driver Settings ****
    private final static int IMPLICIT_WAIT = 5000;
    private final static int POLL_INTERVAL = 50;

    // *** Driver Actors ***
    private final UiDevice deviceDriver;
    public final Context instrumentedContext;
    public final Context targetContext;
    private final Intent intent;
    private final Instrumentation instrumentation;
    public static final String APP_PACKAGE = BuildConfig.APPLICATION_ID;

    // *** Constructor and Class Methods ***
    public NativeDriver() throws NullPointerException {
        instrumentation = InstrumentationRegistry.getInstrumentation();
        deviceDriver = UiDevice.getInstance(instrumentation);
        instrumentedContext = instrumentation.getContext();
        targetContext = instrumentation.getTargetContext();
        intent = instrumentedContext.getPackageManager().getLaunchIntentForPackage(APP_PACKAGE);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
    }

    public void launchActivity() {
        instrumentedContext.startActivity(intent);
        waitForDriverReady();
        deviceDriver.wait(Until.hasObject(By.depth(3)), IMPLICIT_WAIT);
    }

    public void waitForDriverReady() {
        deviceDriver.waitForIdle(3000);
    }

    public void disableAnimations() {
        try {
            Class<?> windowManagerStubClazz = Class.forName("android.view.IWindowManager$Stub");
            Method asInterface = windowManagerStubClazz.getDeclaredMethod("asInterface", IBinder.class);
            Class<?> serviceManagerClazz = Class.forName("android.os.ServiceManager");
            Method getService = serviceManagerClazz.getDeclaredMethod("getService", String.class);
            Class<?> windowManagerClazz = Class.forName("android.view.IWindowManager");
            Method setAnimationScales = windowManagerClazz.getDeclaredMethod("setAnimationScales", float[].class);
            Method getAnimationScales = windowManagerClazz.getDeclaredMethod("getAnimationScales");

            IBinder windowManagerBinder = (IBinder) getService.invoke(null, "window");
            Object windowManagerObj = asInterface.invoke(null, windowManagerBinder);
            float[] currentScales = (float[]) getAnimationScales.invoke(windowManagerObj);
            for (int i = 0; i < currentScales.length; i++) {
                currentScales[i] = 0.0f;
            }
            setAnimationScales.invoke(windowManagerObj, new Object[]{currentScales});
        } catch (Exception e) {
            logErr("SystemAnimations: Could not disable animation.");
        }
    }

    // *** Utility Methods ***
    public void sleep(long seconds) {
        try {
            Thread.sleep(seconds * 1000);
        } catch (InterruptedException ie) {
            logErr(ie.getMessage());
        }
    }

    public void log(String msg) {
        Log.d("AUTOBOTS", msg);
    }

    public void logErr(String msg) {
        Log.e("AUTOBOTS", msg);
    }

    public void logWarn(String msg) {
        Log.w("AUTOBOTS", msg);
    }

    private String deviceDirectory(String dirName) {
        String mDir = String.format(
                "%s/%s", Environment.getExternalStorageDirectory().getPath(), dirName);
        File scrDir = new File(mDir);
        if (!scrDir.exists()) {
            scrDir.mkdir();
        }
        return mDir;
    }

    public void takeScreenshot(String name) {
        log("Take Screenshot: " + name + ".png");
        String mDir = deviceDirectory("test-screenshots");
        deviceDriver.takeScreenshot(new File(String.format("%s/%s.%s", mDir, name, "png")));
    }

    public void waitForChange(ElementData elem, long timeout) {
        final long startTime = System.currentTimeMillis();
        while (!elem.hasChanged()) {
            try {
                Thread.sleep(25);
            } catch (InterruptedException ie) {
                logWarn(ie.getMessage());
            }
            if (System.currentTimeMillis() - timeout > startTime) {
                throw new Error("Element is still same after timeout.");
            }
        }
    }

    public void waitForChange(ElementData elem) {
        waitForChange(elem, IMPLICIT_WAIT);
    }

    public void clickAndWait(UiObject2 clickElement, int waitTimeInSecs) {
        clickElement.click();
        sleep(waitTimeInSecs);
    }

    public void clickAndWait(UiObject2 clickElement, UiObject2 changeElement) {
        ElementData tempData = new ElementData(changeElement);
        clickElement.click();
        waitForChange(tempData);
    }

    public void clickAndWait(UiObject2 element) {
        clickAndWait(element, element);
    }

    public void typeAndWait(UiObject2 element, String text, int waitTimeInSecs) {
        element.setText(text);
        sleep(waitTimeInSecs);
    }

    public void typeAndWait(UiObject2 element, String text) {
        ElementData tempData = new ElementData(element);
        element.setText(text);
        sleep(1);
        waitForChange(tempData);
    }

    // *** Element Methods ***
    public List<UiObject2> getElements(BySelector selector,
                                       long timeout,
                                       boolean failIfNotFound) {
        waitForDriverReady();
        List<UiObject2> elemList = new ArrayList<UiObject2>();
        final long startTime = System.currentTimeMillis();
        while (elemList.isEmpty()) {
            elemList = deviceDriver.findObjects(selector);
            try {
                Thread.sleep(POLL_INTERVAL);
            } catch (InterruptedException ie) {
                logErr(ie.getMessage());
            }
            if (System.currentTimeMillis() - timeout > startTime) {
                if (failIfNotFound) {
                    throw new Error("Element not Found: " + selector.toString());
                }
                else {
                    logWarn("Element not found: " + selector.toString());
                    break;
                }
            }
        }
        return elemList;
    }

    public List<UiObject2> getElements(BySelector selector,
                                        boolean failIfNotFound) {
        return getElements(selector,
                IMPLICIT_WAIT,
                failIfNotFound);
    }

    public List<UiObject2> getElements(BySelector selector,
                                        long timeout) {
        return getElements(selector,
                timeout,
                true);
    }

    public List<UiObject2> getElements(BySelector selector) {
        return getElements(selector,
                IMPLICIT_WAIT,
                true);
    }

    public UiObject2 getElement(BySelector selector,
                                        long timeout,
                                        boolean failIfNotFound) {
        return getElements(selector, timeout, failIfNotFound).get(0);
    }

    public UiObject2 getElement(BySelector selector,
                                 boolean failIfNotFound) {
        return getElements(selector, failIfNotFound).get(0);
    }

    public UiObject2 getElement(BySelector selector,
                                 long timeout) {
        return getElements(selector, timeout).get(0);
    }

    public UiObject2 getElement(BySelector selector) {
        return getElements(selector).get(0);
    }

    public List<UiObject2> getElements(UiObject2 baseElement,
                                       BySelector selector,
                                       long timeout,
                                       boolean failIfNotFound) {
        waitForDriverReady();
        List<UiObject2> elemList = new ArrayList<UiObject2>();
        long curTime = System.currentTimeMillis();
        while (elemList.isEmpty()) {
            elemList = baseElement.findObjects(selector);
            try {
                Thread.sleep(POLL_INTERVAL);
            } catch (InterruptedException ie) {
                logErr(ie.getMessage());
            }
            if (System.currentTimeMillis() - timeout > curTime) {
                if (failIfNotFound) {
                    throw new Error("Element not Found: " + selector.toString());
                }
                else {
                    logWarn("Element not found: " + selector.toString());
                }
            }
        }
        return elemList;
    }

    public List<UiObject2> getElements(UiObject2 baseElement,
                                       BySelector selector,
                                       long timeout) {
        return getElements(baseElement,
                selector,
                timeout,
                true);
    }

    public List<UiObject2> getElements(UiObject2 baseElement,
                                       BySelector selector) {
        return getElements(baseElement,
                selector,
                IMPLICIT_WAIT,
                true);
    }


    public List<UiObject2> getElements(UiObject2 baseElement,
                                       BySelector selector,
                                       boolean failIfNotFound) {
        return getElements(baseElement,
                selector,
                IMPLICIT_WAIT,
                failIfNotFound);
    }

    public UiObject2 getElement(UiObject2 baseElement,
                                BySelector selector,
                                long timeout,
                                boolean failIfNotFound) {
        return getElements(baseElement, selector, timeout, failIfNotFound).get(0);
    }

    public UiObject2 getElement(UiObject2 baseElement,
                                BySelector selector,
                                long timeout) {
        return getElements(baseElement, selector, timeout).get(0);
    }

    public UiObject2 getElement(UiObject2 baseElement,
                                BySelector selector,
                                boolean failIfNotFound) {
        return getElements(baseElement, selector, failIfNotFound).get(0);
    }

    public UiObject2 getElement(UiObject2 baseElement,
                                BySelector selector) {
        return getElements(baseElement, selector).get(0);
    }

    // *** Action Methods ***
    public void pageSwipe(Direction dir, int count) {
        int scrWidth = deviceDriver.getDisplayWidth();
        int scrHeight = deviceDriver.getDisplayHeight();
        for (int i=0; i<count; i++) {
            waitForDriverReady();
            switch (dir) {
                case LEFT:
                    deviceDriver.swipe(scrWidth - 10, scrHeight / 2,
                            1, scrHeight / 2, 2);
                    log("Screen Swiped in the direction: " + dir.toString() +
                            ". From X: " + Integer.toString(scrWidth - 10) +
                            " To X: " + Integer.toString(1));
                    break;
                case RIGHT:
                    deviceDriver.swipe(1, scrHeight / 2,
                            scrWidth - 10, scrHeight / 2, 2);
                    log("Screen Swiped in the direction: " + dir.toString() +
                            ". From X: " + Integer.toString(1) +
                            " To X: " + Integer.toString(scrWidth - 10));
                    break;
                case DOWN:
                    deviceDriver.swipe(scrWidth / 2, 200,
                            scrWidth / 2, scrHeight - 200, 2);
                    log("Screen Swiped in the direction: " + dir.toString() +
                            ". From Y: " + Integer.toString(100) +
                            " To Y: " + Integer.toString(scrHeight - 200));
                    break;
                case UP:
                    deviceDriver.swipe(scrWidth / 2, scrHeight - 200,
                            scrWidth / 2, 200, 2);
                    log("Screen Swiped in the direction: " + dir.toString() +
                            ". From Y: " + Integer.toString(scrHeight - 200) +
                            " To Y: " + Integer.toString(100));
                    break;
            }
            sleep(1);
        }
    }

    public void pageSwipe(Direction dir) {
        pageSwipe(dir, 1);
    }

    public void pageSwipe(int startX, int startY, int endX, int endY, int count) {
        for (int i=0; i<count; i++) {
            waitForDriverReady();
            deviceDriver.swipe(startX, startY, endX, endY, 2);
            log("Screen Swiped ** FROM X: " + Integer.toString(startX) +
                    " Y: " + Integer.toString(startY) +
                    ". **  ** TO X: " + Integer.toString(endX) +
                    " Y: " + Integer.toString(endY) + " **.");
            sleep(1);
        }
    }

    public void pageSwipe(int startX, int startY, int endX, int endY) {
        pageSwipe(startX, startY, endX, endY, 1);
    }

    public void changeOrientation() throws android.os.RemoteException {
        if (deviceDriver.isNaturalOrientation()){
            deviceDriver.setOrientationLeft();
        } else {
            deviceDriver.setOrientationNatural();
        }
    }

    public boolean exists(BySelector... selectors) {
        for (BySelector selector : selectors) {
            if (!deviceDriver.hasObject(selector)) {
                return false;
            }
        }
        return true;
    }

}
