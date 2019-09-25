package com.cliqz.mobile.test.pages;

import android.support.test.uiautomator.By;
import android.support.test.uiautomator.BySelector;
import android.support.test.uiautomator.Direction;
import android.support.test.uiautomator.UiObject2;

import com.cliqz.mobile.test.CliqzRunner;
import com.cliqz.mobile.test.driver.NativeDriver;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.List;

/**
 * Copyright © Cliqz 2019
 */
@RunWith(CliqzRunner.class)
public class OnboardingPage extends BasePage {

    @Override
    @Before
    public void setUp() throws Exception {
        // Define Prefs for the Tests
        onboardingPref(true);
        defaultBrowserPopupPref(false);
        super.setUp();
    }

    // *** Test Data and/or Requirements ***
    private static final int ONBOARDING_PAGES = 4;
    private static final int ONBOARDING_TRACKER_OPTIONS = 3;
    private static final String ONBOARDING_IMAGE_TEXT = "Introduction"; // Assert Equals
    private static final String ONBOARDING_START_BUTTON_TEXT = "Start Browsing"; // Assert Equals
    // Page 1
    private static final String[] ONBOARDING_PAGE_1_TEXT = {
            "Welcome", // Assert Equals
            "Ghostery", // Assert Contains
            "anonymous analytics" // Assert Contains
    };
    private static final boolean ONBOARDING_TELEMETRY_DEFAULT = false;
    // Page 2
    private static final String[] ONBOARDING_PAGE_2_TEXT = {
            "Ad & Tracker Blocking", // Assert Equals
            "Privacy Control Center" // Assert Contains
    };
    private static final boolean[] ONBOARDING_TRACKER_DEFAULT = new boolean[]{ false, true, false };
    private static final String[] ONBOARDING_TRACKER_TEXT = {
            "Block Nothing", // Assert Equals
            "Block Recommended\nAds and Adult Ads", // Assert Equals
            "Block Everything" // Assert Equals
    };
    // Page 3
    private static final String[] ONBOARDING_PAGE_3_TEXT = {
            "Ghost Search", // Assert Equals
            "anonymous with private search" // Assert Contains
    };
    // Page 4
    private static final String[] ONBOARDING_PAGE_4_TEXT = {
            "Start Tab", // Assert Equals
            "Customize the start page" // Assert Contains
    };

    // *** Locators ***
    private static final BySelector ONBOARDING_PAGER = By.res(
            NativeDriver.APP_PACKAGE+ ":id/cliqz_intro_pager");
    private static final BySelector INTRO_IMAGE = By.res(
            NativeDriver.APP_PACKAGE+":id/image");
    private static final BySelector ONBOARDING_LABEL = By.clazz(
            "android.widget.TextView");
    private static final BySelector START_BROWSING_BUTTON = By.res(
            NativeDriver.APP_PACKAGE+":id/start_browsing");
    private static final BySelector TAB_DOTS_MARKER_VIEW = By.res(
            NativeDriver.APP_PACKAGE+":id/cliqz_intro_tab_dots");
    private static final BySelector TAB_DOTS_MARKER = By.clazz(
            "android.support.v7.app.ActionBar$Tab");
    /* Page 1 */
    private static final BySelector TELEMETRY_CHECK_BOX = By.res(
            NativeDriver.APP_PACKAGE + ":id/collect_data_cb");
    /* Page 2 */
    private static final BySelector TRACKER_RADIO_BUTTON = By.clazz(
            "android.widget.RadioButton");

    // *** Page Methods ***
    private UiObject2 getOnBoardingPager() {
        return driver.getElement(ONBOARDING_PAGER);
    }

    private UiObject2 getIntroImage() {
        return driver.getElement(getOnBoardingPager(), INTRO_IMAGE);
    }

    private List<UiObject2> getPageText() {
        return driver.getElements(getOnBoardingPager(), ONBOARDING_LABEL);
    }

    private UiObject2 getStartBrowsingButton() {
        return driver.getElement(START_BROWSING_BUTTON);
    }

    private UiObject2 getTabDotsMarkerView() {
        return driver.getElement(TAB_DOTS_MARKER_VIEW);
    }

    private List<UiObject2> getTabDotsMarkers() {
        return driver.getElements(getTabDotsMarkerView(), TAB_DOTS_MARKER);
    }

    /* Page 1 */
    private UiObject2 getTelemetryCheckBox() {
        return driver.getElement(getOnBoardingPager(), TELEMETRY_CHECK_BOX);
    }

    /* Page 2 */
    private List<UiObject2> getTrackerRadioButtons() {
        return driver.getElements(getOnBoardingPager(), TRACKER_RADIO_BUTTON);
    }

    private boolean checkRadioButtonsStatus(List<UiObject2> radioButtons, boolean... statuses) {
        if (radioButtons.size() != statuses.length) {
            return false;
        }

        final UiObject2[] array = new UiObject2[radioButtons.size()];
        radioButtons.toArray(array);
        for (int i = 0; i < statuses.length; i++) {
            if (array[i].isChecked() != statuses[i]) {
                return false;
            }
        }
        return true;
    }

    // *** Common Assert Methods ***
    private void assertIntroImage() {
        final UiObject2 introImageElem = getIntroImage();
        Assert.assertTrue(
                "Check if an Image is present.",
                driver.exists(INTRO_IMAGE)
        );
        Assert.assertEquals(
                "Check if the Image Text is '" + ONBOARDING_IMAGE_TEXT + "'.",
                ONBOARDING_IMAGE_TEXT,
                introImageElem.getContentDescription()
        );
    }

    private void assertFooterElems(int pageNumber) {
        final UiObject2 startBrowsingButton = getStartBrowsingButton();
        final List<UiObject2> tabDotsMarkersElems = getTabDotsMarkers();
        Assert.assertTrue(
                "Check that '" + ONBOARDING_START_BUTTON_TEXT + "' Button is present.",
                driver.exists(START_BROWSING_BUTTON)
        );
        Assert.assertTrue(
                "Check that '" + ONBOARDING_START_BUTTON_TEXT + "' Button is Clickable",
                startBrowsingButton.isClickable()
        );
        Assert.assertEquals(
                "Check that '" + ONBOARDING_START_BUTTON_TEXT + "' Button is Clickable",
                ONBOARDING_START_BUTTON_TEXT,
                startBrowsingButton.getText()
        );
        Assert.assertTrue(
                "Check that the there is a Tabs Dots View.",
                driver.exists(TAB_DOTS_MARKER_VIEW)
        );
        Assert.assertEquals(
                "Check that there are correct amount of dots.",
                ONBOARDING_PAGES,
                tabDotsMarkersElems.size()
        );
        Assert.assertTrue(
                "Check that the Page No: "+Integer.toString(pageNumber)+" is selected.",
                tabDotsMarkersElems.get(pageNumber-1).isSelected()
        );
    }

    // *** Test Methods ***
    @Test
    public void testOnboardingPage1() {
        final List<UiObject2> page1TextElems = getPageText();
        final UiObject2 telemetryCheckBoxElem = getTelemetryCheckBox();
        // Test Page
        assertIntroImage();
        Assert.assertEquals(
                "Check if the Text Label 1 is '" + ONBOARDING_PAGE_1_TEXT[0] + "'.",
                ONBOARDING_PAGE_1_TEXT[0],
                page1TextElems.get(0).getText()
        );
        Assert.assertTrue(
                "Check if the Text Label 2 contains '" + ONBOARDING_PAGE_1_TEXT[1] + "'.",
                page1TextElems.get(1).getText().contains(ONBOARDING_PAGE_1_TEXT[1])
        );
        Assert.assertTrue(
                "Check if the Text Label 3 contains '" + ONBOARDING_PAGE_1_TEXT[2] + "'.",
                page1TextElems.get(2).getText().contains(ONBOARDING_PAGE_1_TEXT[2])
        );
        Assert.assertTrue(
                "Check if a CheckBox for Telemetry Exists.",
                telemetryCheckBoxElem.isEnabled()
        );
        Assert.assertEquals(
                "Check if the CheckBox is "
                        + (ONBOARDING_TELEMETRY_DEFAULT ? "checked" : "unchecked")
                        + " by Default.",
                ONBOARDING_TELEMETRY_DEFAULT,
                telemetryCheckBoxElem.isChecked()
        );
        // Enable Telemetry
        driver.clickAndWait(getTelemetryCheckBox());
        Assert.assertEquals(
                "Check if the CheckBox is now "
                        + (!ONBOARDING_TELEMETRY_DEFAULT ? "checked" : "unchecked") + ".",
                !ONBOARDING_TELEMETRY_DEFAULT,
                getTelemetryCheckBox().isChecked()
        );
        // TODO:: Check if the 'Send Data' in Settings is enabled.
        // Disable Telemetry
        driver.clickAndWait(getTelemetryCheckBox());
        Assert.assertEquals(
                "Check if the CheckBox is now "
                        + (ONBOARDING_TELEMETRY_DEFAULT ? "checked" : "unchecked"),
                ONBOARDING_TELEMETRY_DEFAULT,
                getTelemetryCheckBox().isChecked()
        );
        // TODO:: Check if the 'Send Data' in Settings is disabled.
        assertFooterElems(1);
        driver.log("Onboarding Page 1 is as Expected.");
    }

    @Test
    public void testOnboardingPage2() {
        // Swipe to Page 2
        int scrollCount = 1;
        while (!getTabDotsMarkers().get(1).isSelected() && scrollCount < 5) {
            driver.pageSwipe(Direction.LEFT);
            scrollCount++;
        }
        final List<UiObject2> page2TextElems = getPageText();
        final List<UiObject2> trackerRadioElems = getTrackerRadioButtons();
        // Test Page
        assertIntroImage();
        Assert.assertEquals(
                "Check if the Text Label 1 is '" + ONBOARDING_PAGE_2_TEXT[0] + "'.",
                ONBOARDING_PAGE_2_TEXT[0],
                page2TextElems.get(0).getText()
        );
        Assert.assertTrue(
                "Check if the Text Label 2 contains '" + ONBOARDING_PAGE_2_TEXT[1] + "'.",
                page2TextElems.get(1).getText().contains(ONBOARDING_PAGE_2_TEXT[1])
        );
        Assert.assertEquals(
                "Check that there are correct number of options for tracker blocking.",
                ONBOARDING_TRACKER_OPTIONS,
                trackerRadioElems.size()
        );
        Assert.assertEquals(
                "Check that the 1st button is for '" + ONBOARDING_TRACKER_TEXT[0] + "'.",
                ONBOARDING_TRACKER_TEXT[0],
                trackerRadioElems.get(0).getText()
        );
        Assert.assertEquals(
                "Check that the 2nd button is for '" + ONBOARDING_TRACKER_TEXT[1] + "'.",
                ONBOARDING_TRACKER_TEXT[1],
                trackerRadioElems.get(1).getText()
        );
        Assert.assertEquals(
                "Check that the 3rd button is for '" + ONBOARDING_TRACKER_TEXT[2] + "'.",
                ONBOARDING_TRACKER_TEXT[2],
                trackerRadioElems.get(2).getText()
        );
        Assert.assertTrue(
                "Check that the default option is selected.",
                checkRadioButtonsStatus(trackerRadioElems, ONBOARDING_TRACKER_DEFAULT));
        for (int i=0; i<trackerRadioElems.size(); i++){
            boolean[] statuses = {false, false, false};
            driver.clickAndWait(getTrackerRadioButtons().get(i));
            statuses[i] = true;
            Assert.assertTrue(
                    "Check that the '" + ONBOARDING_TRACKER_TEXT[i] + "' is selected.",
                    checkRadioButtonsStatus(getTrackerRadioButtons(), statuses)
            );
        }
        // TODO:: Check if selecting the option here sets the option in Control Center.
        assertFooterElems(2);
        driver.log("Onboarding Page 2 is as Expected.");
    }

    @Test
    public void testOnboardingPage3() {
        // Swipe to Page 2
        int scrollCount = 1;
        while (!getTabDotsMarkers().get(2).isSelected() && scrollCount < 5) {
            driver.pageSwipe(Direction.LEFT);
            scrollCount++;
        }
        final List<UiObject2> page3TextElems = getPageText();
        // Test Page
        assertIntroImage();
        Assert.assertEquals(
                "Check if the Text Label 1 is '" + ONBOARDING_PAGE_3_TEXT[0] + "'.",
                ONBOARDING_PAGE_3_TEXT[0],
                page3TextElems.get(0).getText()
        );
        Assert.assertTrue(
                "Check if the Text Label 2 contains '" + ONBOARDING_PAGE_3_TEXT[1] + "'.",
                page3TextElems.get(1).getText().contains(ONBOARDING_PAGE_3_TEXT[1])
        );
        assertFooterElems(3);
        driver.log("Onboarding Page 3 is as Expected.");
    }

    @Test
    public void testOnboardingPage4() {
        // Swipe to Page 3
        int scrollCount = 1;
        while (!getTabDotsMarkers().get(3).isSelected() && scrollCount < 5) {
            driver.pageSwipe(Direction.LEFT);
            scrollCount++;
        }
        final List<UiObject2> page4TextElems = getPageText();
        // Test Page
        assertIntroImage();
        Assert.assertEquals(
                "Check if the Text Label 1 is '" + ONBOARDING_PAGE_4_TEXT[0] + "'.",
                ONBOARDING_PAGE_4_TEXT[0],
                page4TextElems.get(0).getText()
        );
        Assert.assertTrue(
                "Check if the Text Label 2 contains '" + ONBOARDING_PAGE_4_TEXT[1] + "'.",
                page4TextElems.get(1).getText().contains(ONBOARDING_PAGE_4_TEXT[1])
        );
        assertFooterElems(4);
        driver.log("Onboarding Page 4 is as Expected.");
    }

}
