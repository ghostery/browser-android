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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Copyright © Cliqz 2019
 */
@RunWith(CliqzRunner.class)
public class FreshTabPage extends BasePage {

    // *** Test Data and/or Requirements ***
    private static final String URL_BAR_TEXT = "Ghost Search"; // Assert Equals
    private static final String EMPTY_TOP_SITE_TEXT = "Your most visited websites will appear here"; // Assert Equals
    private static final String GHOSTY_TEXT = "HELLO"; // Assert Equals
    private static final String MENU_BUTTON_TEXT = "Menu"; // Assert Equals

    // *** Locators ***
    private static final BySelector TOP_TOOL_BAR = By.res(
            NativeDriver.APP_PACKAGE + ":id/browser_toolbar");
    private static final BySelector URL_BAR = By.res(
            NativeDriver.APP_PACKAGE + ":id/url_bar_title");
    private static final BySelector URL_BAR_EDIT = By.res(
            NativeDriver.APP_PACKAGE + ":id/url_edit_text");
    private static final BySelector URL_BAR_EDIT_CANCEL = By.res(
            NativeDriver.APP_PACKAGE + ":id/edit_cancel");
    private static final BySelector GHOSTY_ICON = By.res(
            NativeDriver.APP_PACKAGE + ":id/ghosty");
    private static final BySelector TABS_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":id/tabs_counter");
    private static final BySelector TABS_COUNTER = By.res(
            NativeDriver.APP_PACKAGE + ":id/counter_text");
    private static final BySelector MENU_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":id/menu");
    private static final BySelector PAGE_VIEW = By.res(
            NativeDriver.APP_PACKAGE + ":id/home_pager");
    private static final BySelector PAGE_VIEW_BUTTON_BAR = By.clazz(
            "android.widget.HorizontalScrollView"); // Find with PAGE_VIEW as base element
    private static final BySelector PAGE_VIEW_BUTTONS = By.clazz(
            "android.widget.ImageView");
    private static final int PAGE_VIEW_BUTTON_COUNT = 4;
    private static final BySelector TOP_SITES_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":drawable/ic_home_cliqz");
    private static final BySelector HISTORY_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":drawable/ic_history_white");
    private static final BySelector OFFERZ_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":drawable/ic_offrz_white");
    private static final BySelector FAVS_BUTTON = By.res(
            NativeDriver.APP_PACKAGE + ":drawable/ic_star_white");
    private static final BySelector TOP_SITES_NEWS_VIEW = By.res(
            NativeDriver.APP_PACKAGE + ":id/activity_stream_main_recyclerview");
    private static final BySelector TOP_SITES_VIEW = By.clazz(
            "android.widget.FrameLayout");
    private static final BySelector EMPTY_TOP_SITES_ELEMENT = By.res(
            NativeDriver.APP_PACKAGE + ":id/empty_topsites_text");
    private static final BySelector NEWS_VIEW = By.res(
            NativeDriver.APP_PACKAGE + ":id/news_recyclerview");
    private static final BySelector NEWS_SECTION_TOGGLE = By.res(
            NativeDriver.APP_PACKAGE + ":id/news_display_toggle");
    private static final BySelector NEWS_ITEM_ICON = By.res(
            NativeDriver.APP_PACKAGE + ":id/favicon");
    private static final BySelector NEWS_ITEM_TITLE = By.res(
            NativeDriver.APP_PACKAGE + ":id/title_view");
    private static final BySelector NEWS_ITEM_URL = By.res(
            NativeDriver.APP_PACKAGE + ":id/url_view");

    // *** Override Class Methods ***
    @Override
    @Before
    public void setUp() throws Exception {
        super.clearData();
        super.onboardingPref(false);
        super.defaultBrowserPopupPref(false);
        super.setUp();
        if(getNewsToggleButton().getText().equals("Hide")) {
            getNewsToggleButton().click();
        }
    }

    // *** Page Methods ***
    private UiObject2 getTopToolBar() {
        return driver.getElement(TOP_TOOL_BAR);
    }

    private UiObject2 getUrlBar() {
        return driver.getElement(getTopToolBar(), URL_BAR);
    }

    private UiObject2 getUrlBarEdit() {
        return driver.getElement(getTopToolBar(), URL_BAR_EDIT);
    }

    private UiObject2 getUrlBarEditCancel() {
        return driver.getElement(getTopToolBar(), URL_BAR_EDIT_CANCEL, false);
    }

    private UiObject2 getGhostyIcon() {
        return driver.getElement(getTopToolBar(), GHOSTY_ICON);
    }

    private UiObject2 getTabsButton() {
        return driver.getElement(getTopToolBar(), TABS_BUTTON);
    }

    private int getTabsCount() {
        return Integer.valueOf(
                driver.getElement(getTabsButton(), TABS_COUNTER).getText()
        );
    }

    private UiObject2 getMenuButton() {
        return driver.getElement(getTopToolBar(), MENU_BUTTON);
    }

    private UiObject2 getPageView() {
        return driver.getElement(PAGE_VIEW);
    }

    private UiObject2 getPageViewButtonBar() {
        return driver.getElement(getPageView(), PAGE_VIEW_BUTTON_BAR);
    }

    private List<UiObject2> getPageViewButtonList() {
        return driver.getElements(getPageViewButtonBar(), PAGE_VIEW_BUTTONS);
    }

    private UiObject2 getPageViewButton(BySelector selector) {
        return driver.getElement(getPageViewButtonBar(), selector);
    }

    private UiObject2 getTopSitesAndNewsView() {
        return driver.getElement(TOP_SITES_NEWS_VIEW);
    }

    private UiObject2 getTopSitesSection() {
        return driver.getElement(getTopSitesAndNewsView(), TOP_SITES_VIEW);
    }

    private UiObject2 getTopSitesEmptyText() {
        return driver.getElement(getTopSitesSection(), EMPTY_TOP_SITES_ELEMENT);
    }

    private UiObject2 getNewsSection() {
        return driver.getElement(getTopSitesAndNewsView(), NEWS_VIEW);
    }

    private UiObject2 getNewsToggleButton() {
        return driver.getElement(getNewsSection(), NEWS_SECTION_TOGGLE);
    }

    private List<UiObject2> getNewsListElements() {
        final List<UiObject2> newsList = getNewsSection().getChildren();
        newsList.remove(0);
        return newsList;
    }

    private UiObject2 getNewsIcon(UiObject2 newsItem) {
        return driver.getElement(newsItem, NEWS_ITEM_ICON, 500);
    }

    private UiObject2 getNewsTitle(UiObject2 newsItem) {
        return driver.getElement(newsItem, NEWS_ITEM_TITLE, 500);
    }

    private UiObject2 getNewsUrl(UiObject2 newsItem) {
        return driver.getElement(newsItem, NEWS_ITEM_URL, 500);
    }

    private List<Map<String, String>> getNewsListData() {
        List<Map<String, String>> newsData = new ArrayList<>();
        boolean found = true;
        int count = 0;
        while(found && (count<=5)) {
            found = false;
            List<UiObject2> newsElemList = getNewsListElements();
            if(newsElemList.size() == 0){
                throw new Error("News Section is Empty !");
            }
            for (int i = 0; i < newsElemList.size(); i++) {
                Map<String, String> data = new HashMap<>();
                try {
                    driver.log(getNewsTitle(newsElemList.get(i)).getText());
                    data.put("title", getNewsTitle(newsElemList.get(i)).getText());
                    data.put("url", getNewsUrl(newsElemList.get(i)).getText());
                    data.put("icon", (getNewsIcon(newsElemList.get(i)) != null) ? "true" : "false");
                    if (!newsData.contains(data)) {
                        newsData.add(data);
                        found = true;
                    }
                } catch(Error e) {
                    driver.logWarn(e.toString());
                    found = true;
                }
            }
            getNewsSection().swipe(Direction.UP, 0.5f);
            count++;
        }
        getNewsSection().swipe(Direction.DOWN, 1.0f);
        return newsData;
    }

    // *** Test Methods ***
    @Test
    public void testUrlBar() {
        UiObject2 urlBarElem = getUrlBar();
        Assert.assertTrue(
                "Check that the Url Bar exists.",
                driver.exists(URL_BAR)
        );
        Assert.assertEquals(
                "Check if the Url Bar Text is correct.",
                URL_BAR_TEXT,
                urlBarElem.getText()
        );
        driver.clickAndWait(urlBarElem);
        final UiObject2 urlBarEditElem = getUrlBarEdit();
        final UiObject2 urlBarEditCancelElem = getUrlBarEditCancel();
        Assert.assertTrue(
                "Check that the Url Bar Edit exists.",
                driver.exists(URL_BAR_EDIT)
        );
        Assert.assertTrue(
                "Check that the Url Bar Cancel exists.",
                driver.exists(URL_BAR_EDIT_CANCEL)
        );
        Assert.assertFalse(
                "Check that the Url Bar, Ghosty, Tabs and Menu Buttons do not exist.",
                driver.exists(URL_BAR, GHOSTY_ICON, TABS_BUTTON, MENU_BUTTON)
        );
        driver.typeAndWait(urlBarEditElem, "Hello");
        Assert.assertEquals(
                "Check that the URL Bar Edit Text changes to 'Hello'.",
                "Hello.com",
                urlBarEditElem.getText().replace("/", "")
        );
        driver.clickAndWait(urlBarEditCancelElem);
        urlBarElem = getUrlBar();
        Assert.assertNotSame(
                "Check that the URL Bar Edit Text is NOT 'Hello'.",
                "Hello.com",
                urlBarElem.getText().replace("/", "")
        );
        Assert.assertEquals(
                "Check if the Url Bar Text is correct.",
                URL_BAR_TEXT,
                urlBarElem.getText()
        );
        Assert.assertFalse(
                "Check that the Url Bar Edit no longer exists.",
                driver.exists(URL_BAR_EDIT)
        );
        Assert.assertTrue(
                "Check that the Url Bar, Ghosty, Tabs and Menu Buttons exist.",
                driver.exists(URL_BAR, GHOSTY_ICON, TABS_BUTTON, MENU_BUTTON)
        );
        driver.log("Url Bar is working as intended.");
    }

    @Test
    public void testGhosty() {
        final UiObject2 ghostyElem = getGhostyIcon();
        Assert.assertTrue(
                "Check that the Ghosty Icon exists.",
                driver.exists(GHOSTY_ICON)
        );
        Assert.assertEquals(
                "Check if the Ghosty Text is correct.",
                GHOSTY_TEXT,
                ghostyElem.getContentDescription()
        );
    }

    @Test
    public void testTabsButton() {
        Assert.assertTrue(
                "Check that the Tabs Button exists.",
                driver.exists(TABS_BUTTON)
        );
        Assert.assertEquals(
                "Check if the Tabs Count is 1.",
                1,
                getTabsCount()
        );
    }

    @Test
    public void testMenuButton() {
        final UiObject2 menuButtonElem = getMenuButton();
        Assert.assertTrue(
                "Check that the Menu Button exists.",
                driver.exists(MENU_BUTTON)
        );
        Assert.assertEquals(
                "Check if the Menu Button Text is correct.",
                MENU_BUTTON_TEXT,
                menuButtonElem.getContentDescription()
        );
    }

    @Test
    public void testFreshTabViewButtons() {
        final List<UiObject2> buttonList = getPageViewButtonList();
        Assert.assertEquals(
                "Check if Number of Buttons on the TabBar is Correct.",
                buttonList.size(),
                PAGE_VIEW_BUTTON_COUNT
        );
        Assert.assertNotNull(
                "Check that TopSites Button Exist.",
                getPageViewButton(TOP_SITES_BUTTON)
        );
        Assert.assertNotNull(
                "Check that History Button Exist.",
                getPageViewButton(HISTORY_BUTTON)
        );
        Assert.assertNotNull(
                "Check that MyOfferz Button Exist.",
                getPageViewButton(OFFERZ_BUTTON)
        );
        Assert.assertNotNull(
                "Check that Favorites Button Exist.",
                getPageViewButton(FAVS_BUTTON)
        );
    }

    @Test
    public void testTopSitesSectionEmpty() {
        final UiObject2 emptyTextElement = getTopSitesEmptyText();
        Assert.assertNotNull(
                "Check that a Text is shown if Top Sites are empty.",
                emptyTextElement
        );
        Assert.assertEquals(
                "Check that the Text shown for Empty Top Sites is correct.",
                EMPTY_TOP_SITE_TEXT,
                emptyTextElement.getText()
        );
    }

    @Test
    public void testNewsSection() {
        Assert.assertEquals(
                "Check if News Section is Collapsed.",
                "Show",
                getNewsToggleButton().getText()
        );
        driver.clickAndWait(getNewsToggleButton());
        Assert.assertEquals(
                "Check if News Section is Expanded and text is changed to Hide.",
                "Hide",
                getNewsToggleButton().getText()
        );
        driver.clickAndWait(getNewsToggleButton());
        Assert.assertEquals(
                "Check if News Section is Collapsed Again.",
                "Show",
                getNewsToggleButton().getText()
        );
        driver.clickAndWait(getNewsToggleButton(), 1);
        List<Map<String, String>> newsData = getNewsListData();
        Assert.assertNotNull(
                "Check that the News List is not Null",
                newsData
        );
        for(int i=0; i<newsData.size(); i++){
            driver.log(newsData.get(i).toString());
            Assert.assertEquals(
                    "Check if News No: " + i + " has an icon.",
                    "true",
                    newsData.get(i).get("icon")
            );
            Assert.assertNotNull(
                    "Check if News No: " + i + " has a title.",
                    newsData.get(i).get("title")
            );
            Assert.assertNotNull(
                    "Check if News No: " + i + " has a url.",
                    newsData.get(i).get("url")
            );
        }
        getNewsToggleButton().click();
    }
}
