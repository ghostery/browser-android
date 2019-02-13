/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 package org.mozilla.gecko.activitystream.homepanel;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.Loader;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;

import org.mozilla.gecko.AboutPages;
import org.mozilla.gecko.BrowserApp;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.R;
import org.mozilla.gecko.Tab;
import org.mozilla.gecko.Tabs;
import org.mozilla.gecko.activitystream.ActivityStreamTelemetry;
import org.mozilla.gecko.activitystream.homepanel.model.Highlight;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.model.TopStory;
import org.mozilla.gecko.activitystream.homepanel.topnews.TopNewsLoader;
import org.mozilla.gecko.activitystream.homepanel.topsites.TopSitesPage;
import org.mozilla.gecko.activitystream.homepanel.topsites.TopSitesPagerAdapter;
import org.mozilla.gecko.activitystream.homepanel.topstories.PocketStoriesLoader;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.fxa.FirefoxAccounts;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.preferences.GeckoPreferences;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;

import java.util.Calendar;
import java.util.Collections;
import java.util.List;

/* Cliqz Start */
public class ActivityStreamPanel extends FrameLayout implements Tabs.OnTabsChangedListener {
/* Cliqz End */
    private final StreamRecyclerAdapter adapter;

    private static final int LOADER_ID_HIGHLIGHTS = 0;
    private static final int LOADER_ID_TOPSITES = 1;
    private static final int LOADER_ID_POCKET = 2;
    /* Cliqz start */
    // add TopNews Loader ID
    private static final int LOADER_ID_TOP_NEWS = 3;
    // This date is Jan 01, 2019
    // Used to decide to show 'customize_newtab_view' or 'customize_newtab_snackbar'
    private static final long thresholdTime = 1546300800000L;
    /* Cliqz end */

    /**
     * Number of database entries to consider and rank for finding highlights.
     */
    private static final int HIGHLIGHTS_CANDIDATES = 500;

    /**
     * Number of highlights that should be returned (max).
     */
    private static final int HIGHLIGHTS_LIMIT = 10;

    public static final String PREF_POCKET_ENABLED = "pref_activitystream_pocket_enabled";
    public static final String PREF_VISITED_ENABLED = "pref_activitystream_visited_enabled";
    public static final String PREF_BOOKMARKS_ENABLED = "pref_activitystream_recentbookmarks_enabled";

    private final RecyclerView contentRecyclerView;

    private int desiredTileWidth;
    private int tileMargin;
    private final SharedPreferences sharedPreferences;
    private final PreferenceManager preferenceManager;

    /* Cliqz Start */
    private final View customizeNewTabView;
    private final View customizeNewTabViewSnackBar;
    /* Cliqz End */

    public ActivityStreamPanel(final Context context, AttributeSet attrs) {
        super(context, attrs);

        /* Cliqz Start */
        Tabs.registerOnTabsChangedListener(this);
        setBackgroundColor(ContextCompat.getColor(context, android.R.color.transparent));
        /* Cliqz End */

        inflate(context, R.layout.as_content, this);

        adapter = new StreamRecyclerAdapter();
        sharedPreferences = GeckoSharedPrefs.forProfile(context);

        /* Cliqz Start */
        preferenceManager = PreferenceManager.getInstance();
        /* Cliqz End */

        contentRecyclerView = (RecyclerView) findViewById(R.id.activity_stream_main_recyclerview);
        contentRecyclerView.setAdapter(adapter);
        contentRecyclerView.setLayoutManager(/* Cliqz */ new CustomLinearLayoutManager(context));
        contentRecyclerView.setHasFixedSize(true);
        /* Cliqz Start */
        contentRecyclerView.setNestedScrollingEnabled(false);
        /* Cliqz End */
        // Override item animations to avoid horrible topsites refreshing
        contentRecyclerView.setItemAnimator(new StreamItemAnimator());
        /* Cliqz Start o/
        contentRecyclerView.addItemDecoration(new HighlightsDividerItemDecoration(context));
        /o Cliqz End */

        RecyclerViewClickSupport.addTo(contentRecyclerView)
                .setOnItemClickListener(adapter)
                .setOnItemLongClickListener(adapter);

        final Resources resources = getResources();
        desiredTileWidth = resources.getDimensionPixelSize(R.dimen.activity_stream_desired_tile_width);
        tileMargin = resources.getDimensionPixelSize(R.dimen.activity_stream_base_margin);

        ActivityStreamTelemetry.Extras.setGlobal(
                ActivityStreamTelemetry.Contract.FX_ACCOUNT_PRESENT,
                FirefoxAccounts.firefoxAccountsExist(context)
        );

        updateSharedPreferencesGlobalExtras(context, sharedPreferences);

        /* Cliqz Start */
        customizeNewTabView = findViewById(R.id.customize_newtab_view);
        customizeNewTabViewSnackBar = findViewById(R.id.customize_newtab_snackbar);

        final View customizeTabLink = customizeNewTabView.findViewById(R.id.customize_tab_link);
        // TODO setBackground to transparent based on the AB group
        // customizeTabLink.setBackgroundColor(Color.argb(0, 0, 0, 0));
        customizeTabLink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(context, GeckoPreferences.class);
                GeckoPreferences.setResourceToOpen(intent, "preferences_general");
                // We want to know when the Settings activity returns, because
                // we might need to redisplay based on a locale change.
                ((BrowserApp) context).startActivityForResult(intent, BrowserApp.ACTIVITY_REQUEST_PREFERENCES);
            }
        });

        final TextView dismiss = (TextView) customizeNewTabViewSnackBar.findViewById(R.id.dismiss);
        final TextView customize = (TextView) customizeNewTabViewSnackBar.findViewById(R.id.customize);

        dismiss.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                customizeNewTabViewSnackBar.setVisibility(View.GONE);
                preferenceManager.setShowCustomizeTabSnackBar(false);
            }
        });

        customize.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(context, GeckoPreferences.class);
                GeckoPreferences.setResourceToOpen(intent, "preferences_general");
                // We want to know when the Settings activity returns, because
                // we might need to redisplay based on a locale change.
                ((BrowserApp) context).startActivityForResult(intent, BrowserApp.ACTIVITY_REQUEST_PREFERENCES);
                customizeNewTabViewSnackBar.setVisibility(View.GONE);
                preferenceManager.setShowCustomizeTabSnackBar(false);
            }
        });

        final Tab tab = Tabs.getInstance().getSelectedTab();
        if (tab != null) {
            showCustomizeTabView(tab, null);
        }
        /* Cliqz End */
    }

    private void updateSharedPreferencesGlobalExtras(final Context context, final SharedPreferences sharedPreferences) {
        ActivityStreamTelemetry.Extras.setGlobal(
                ActivityStreamTelemetry.Contract.AS_USER_PREFERENCES,
                ActivityStreamTelemetry.getASUserPreferencesValue(context, sharedPreferences)
        );
    }

    void setOnUrlOpenListeners(HomePager.OnUrlOpenListener onUrlOpenListener, HomePager.OnUrlOpenInBackgroundListener onUrlOpenInBackgroundListener) {
        adapter.setOnUrlOpenListeners(onUrlOpenListener, onUrlOpenInBackgroundListener);
    }

    public void load(LoaderManager lm) {
        if (sharedPreferences.getBoolean(PREF_BOOKMARKS_ENABLED, true) || sharedPreferences.getBoolean(PREF_VISITED_ENABLED, true)) {
            lm.initLoader(LOADER_ID_HIGHLIGHTS, null, new HighlightsCallbacks());
        }

        // TODO: we should get the default values from resources for the other Top Sites sections above too.
        if (ActivityStreamConfiguration.isPocketRecommendingTopSites(getContext())) {
            lm.initLoader(LOADER_ID_POCKET, null, new PocketStoriesCallbacks());
        }
        /* Cliqz start */
        // add check to show the topsites if it's enabled
        if (isTopSitesEnabled()) {
            lm.initLoader(LOADER_ID_TOPSITES, null, new TopSitesCallback());
        } else {
            adapter.hideTopSitesSection();
        }
        // init TopNews Loader if show news enabled
        if (isNewsEnabled()) {
            lm.initLoader(LOADER_ID_TOP_NEWS, null, new TopNewsCallback());
        } else {
            adapter.hideTopNewsSection();
        }
        /* Cliqz end */
    }

    public void unload() {
        adapter.swapHighlights(Collections.<Highlight>emptyList());
        adapter.swapTopSitesCursor(null);
        /* Cliqz start */
        // set TopNews List empty
        adapter.swapTopNews(Collections.<TopNews>emptyList());
        Tabs.unregisterOnTabsChangedListener(this);
        /* Cliqz end */
    }

    public void reload(final LoaderManager lm, final Context context, final SharedPreferences sharedPreferences) {
        adapter.clearAndInit();

        updateSharedPreferencesGlobalExtras(context, sharedPreferences);

        // Destroy loaders so they don't restart loading when returning.
        lm.destroyLoader(LOADER_ID_HIGHLIGHTS);
        lm.destroyLoader(LOADER_ID_POCKET);
        /* Cliqz start */
        lm.destroyLoader(LOADER_ID_TOPSITES);
        // destroy TopNews Loader
        lm.destroyLoader(LOADER_ID_TOP_NEWS);
        /* Cliqz end */

        load(lm);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);

        // This code does two things:
        //  * Calculate the size of the tiles we want to display (using a desired width as anchor point)
        //  * Add padding to the left/right side if there's too much space that we do not need


        // Calculate how many tiles fit into the available horizontal space if we are using our
        // desired tile width.
        int fittingTiles = (w - tileMargin) / (desiredTileWidth + tileMargin);

        if (fittingTiles <= TopSitesPage.NUM_COLUMNS) {
            // We can't fit all tiles (or they fit exactly) if we are using the desired tiles width.
            // We will still show all tiles but they might be smaller than what we would like them to be.
            contentRecyclerView.setPadding(0, 0, 0, 0);
        } else if (fittingTiles > TopSitesPage.NUM_COLUMNS) {
            // We can fit more tiles than we want to display. Calculate how much space we need and
            // use the remaining space as padding on the left and right.
            int needed = TopSitesPage.NUM_COLUMNS * (desiredTileWidth + tileMargin) + tileMargin;
            int padding = (w - needed) / 2;

            // With the padding applied we have less space available for the tiles
            w = needed;

            contentRecyclerView.setPadding(padding, 0, padding, 0);
        }

        // Now calculate how large an individual tile is
        final int tilesSize = (w - (TopSitesPage.NUM_COLUMNS * tileMargin) - tileMargin) / TopSitesPage.NUM_COLUMNS;

        adapter.setTileSize(tilesSize);
    }

    /* Cliqz Start */
    @Override
    public void onTabChanged(Tab tab, Tabs.TabEvents msg, String data) {
        if (tab != null && AboutPages.isAboutHome(tab.getURL()) && (msg == Tabs.TabEvents.ADDED ||
                msg == Tabs.TabEvents.CLOSED || msg == Tabs.TabEvents.OPENED_FROM_TABS_TRAY)) {
            showCustomizeTabView(tab, msg);
        }
    }

    private void showCustomizeTabView(@NonNull Tab tab, Tabs.TabEvents msg) {
        if (tab.isPrivate() && msg == null) {
            customizeNewTabView.setVisibility(View.GONE);
            customizeNewTabViewSnackBar.setVisibility(View.GONE);
            return;
        } else if (tab.isPrivate() && msg != null && msg != Tabs.TabEvents.CLOSED) {
            // Special case where a user closes a private tab and the browser fallbacks to a normal tab.
            // Here no tab change event is received so here 'Tabs.TabEvents.CLOSED' event of the
            // closed private tab is used.
            customizeNewTabView.setVisibility(View.GONE);
            customizeNewTabViewSnackBar.setVisibility(View.GONE);
            return;
        }
        showCustomizeTabView();
    }

    private void showCustomizeTabView() {
        final Context context = getContext();
        final SharedPreferences appSharedPreferences = context.getSharedPreferences(
                GeckoSharedPrefs.APP_PREFS_NAME, 0);
        final int appLaunchCount = appSharedPreferences.getInt(
                GeckoPreferences.PREFS_APP_LAUNCH_COUNT, 0);

        // On start tab, new users are shown 'customize_newtab_view' always and
        // existing users are shown 'customize_newtab_snackbar' which the user can dismiss.
        final boolean showCustomizeTabView = preferenceManager.shouldShowCustomizeTabView();
        final boolean showCustomizeTabSnackBar = preferenceManager.shouldShowCustomizeTabSnackBar();
        long installedTime;
        try {
            installedTime = context.getPackageManager()
                    .getPackageInfo(context.getPackageName(), 0).firstInstallTime;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            Log.i("ActivityStreamPanel", "Error retrieving firstInstallTime");
            return;
        }

        if (installedTime > thresholdTime) {
            // New user.
            // Show 'customize_newtab_view' for all new users.
            if (appLaunchCount < 3 || showCustomizeTabView) {
                customizeNewTabView.setVisibility(View.VISIBLE);
                customizeNewTabViewSnackBar.setVisibility(View.GONE);
                if (!showCustomizeTabView) {
                    preferenceManager.setShowCustomizeTabView(true);
                }
                if (showCustomizeTabSnackBar) {
                    preferenceManager.setShowCustomizeTabSnackBar(false);
                }
            }
        } else {
            // Existing user.
            // For users for whom we were not counting the sessions. (These users have older Ghostery version).
            // For these users, we show 'customize_newtab_view'.
            if (appLaunchCount == 1) {
                customizeNewTabView.setVisibility(View.VISIBLE);
                customizeNewTabViewSnackBar.setVisibility(View.GONE);
                if (!showCustomizeTabView) {
                    preferenceManager.setShowCustomizeTabView(true);
                }
                if (showCustomizeTabSnackBar) {
                    preferenceManager.setShowCustomizeTabSnackBar(false);
                }
                return;
            }
            // For users for whom we were counting the sessions.
            // For these users, we show 'customize_newtab_snackbar'.
            if (appLaunchCount % 3 == 0 && showCustomizeTabSnackBar) {
                customizeNewTabViewSnackBar.setVisibility(View.VISIBLE);
            } else {
                customizeNewTabViewSnackBar.setVisibility(View.GONE);
            }
        }
    }
    /* Cliqz End */

    private class HighlightsCallbacks implements LoaderManager.LoaderCallbacks<List<Highlight>> {
        @Override
        public Loader<List<Highlight>> onCreateLoader(int id, Bundle args) {
            return new HighlightsLoader(getContext(), HIGHLIGHTS_CANDIDATES, HIGHLIGHTS_LIMIT);
        }

        @Override
        public void onLoadFinished(Loader<List<Highlight>> loader, List<Highlight> data) {
            adapter.swapHighlights(data);
        }

        @Override
        public void onLoaderReset(Loader<List<Highlight>> loader) {
            adapter.swapHighlights(Collections.<Highlight>emptyList());
        }
    }

    private class TopSitesCallback implements LoaderManager.LoaderCallbacks<Cursor> {
        @Override
        public Loader<Cursor> onCreateLoader(int id, Bundle args) {
            final int topSitesPerPage = TopSitesPage.NUM_COLUMNS * TopSitesPage.NUM_ROWS;

            final Context context = getContext();
            return BrowserDB.from(context).getActivityStreamTopSites(
                    context,
                    topSitesPerPage * TopSitesPagerAdapter.SUGGESTED_SITES_MAX_PAGES,
                    topSitesPerPage * TopSitesPagerAdapter.PAGES);
        }

        @Override
        public void onLoadFinished(Loader<Cursor> loader, Cursor data) {
            adapter.swapTopSitesCursor(data);
        }

        @Override
        public void onLoaderReset(Loader<Cursor> loader) {
            adapter.swapTopSitesCursor(null);
        }
    }

    private class PocketStoriesCallbacks implements LoaderManager.LoaderCallbacks<List<TopStory>> {

        @Override
        public Loader<List<TopStory>> onCreateLoader(int id, Bundle args) {
            return new PocketStoriesLoader(getContext());
        }

        @Override
        public void onLoadFinished(Loader<List<TopStory>> loader, List<TopStory> data) {
            adapter.swapTopStories(data);
        }

        @Override
        public void onLoaderReset(Loader<List<TopStory>> loader) {
            adapter.swapTopStories(Collections.<TopStory>emptyList());
        }


    }

    /* Cliqz start */
    // add TopNews Loader
    private class TopNewsCallback implements LoaderManager.LoaderCallbacks<List<TopNews>> {
        @Override
        public Loader<List<TopNews>> onCreateLoader(int id, Bundle args) {
            return new TopNewsLoader(getContext());
        }

        @Override
        public void onLoadFinished(Loader<List<TopNews>> loader, List<TopNews> data) {
            if(data != null) {
                adapter.swapTopNews(data);
            }
        }

        @Override
        public void onLoaderReset(Loader<List<TopNews>> loader) {
            adapter.swapTopNews(Collections.<TopNews>emptyList());
        }
    }

    // This part is derived from @{@link TabQueueHelper}.java
    public boolean isTopSitesEnabled(){
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(getContext());
        return  prefs.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_TOPSITES_ENABLED,true);
    }

    private boolean isNewsEnabled(){
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(getContext());
        return  prefs.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_NEWS_ENABLED,true);
    }

    // Disable predictive animations, see https://stackoverflow.com/a/33985508/1122966
    private static class CustomLinearLayoutManager extends LinearLayoutManager {
        CustomLinearLayoutManager(Context context) {
            super(context);
        }

        @Override
        public boolean supportsPredictiveItemAnimations() {
            return false;
        }
    }
    /* Cliqz end */
}
