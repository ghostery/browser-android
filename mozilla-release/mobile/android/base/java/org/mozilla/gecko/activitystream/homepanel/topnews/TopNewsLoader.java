package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.location.Location;
import android.support.annotation.Nullable;
import android.support.v4.content.AsyncTaskLoader;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.stream.TopNewsRow;
import org.mozilla.gecko.search.SearchEngineManager;
import org.mozilla.gecko.util.FileUtils;
import org.mozilla.gecko.util.HttpHandler;
import org.mozilla.mozstumbler.service.Prefs;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * Copyright © Cliqz 2018
 */
public class TopNewsLoader extends AsyncTaskLoader<List<TopNews>> {

    private static final String CONTENT_TYPE_JSON = "application/json";
    private static final String NEWS_PAYLOAD = "{\"q\":\"\",\"results\":[{\"url\":\"rotated-top-news.cliqz.com\",\"snippet\":{}}]}";
    // TODO: Change this with branding
    private static final String NEWS_URL = "https://api.ghostery.net/api/v2/rich-header?path=/v2/map";
    private static final String TOP_NEWS_CACHE_FILE = "TopNewsCache.json";
    private final String TAG = TopNewsRow.class.getSimpleName();
    private String mData = null;
    private long mDataTimestamp = 0;
    // rewrite cached topNews file after 30 minutes
    private static final int CACHED_PERIOD = 1800000;

    public TopNewsLoader(Context context) {
        super(context);
    }

    @Override
    protected void onStartLoading() {
        // prevent reload the data if the loader still has the data
        if (mData == null || isCacheExpired()) {
            forceLoad();
        }
    }

    private boolean isCacheExpired() {
        return (System.currentTimeMillis() - mDataTimestamp > CACHED_PERIOD);
    }

    /**
     * Logic:
     *  - Get cached data if exist and it was written less than 30 minutes ago
     *  - Request TopNews endpoint to get the data
     *      * if succeed write it on cached and return it
     *      * if failed return the cached data if exist else null
     *
     * @return
     */
    @Override
    @Nullable
    public List<TopNews> loadInBackground() {
        String response = null;
        boolean succeedReadFromCache = false;
        final File cachedFile = getTopNewsCacheFile();
        mDataTimestamp = cachedFile.lastModified();
        if(!isCacheExpired()) {
            try {
                response  = FileUtils.readStringFromFile(cachedFile);
                succeedReadFromCache = true;
            } catch (IOException e) {
                Log.e(TAG,"can't read cached news");
            }
        }

        if (!succeedReadFromCache) {
            try {
                response = HttpHandler.sendRequest("PUT", getTopNewsUrl(Integer.MAX_VALUE),
                        CONTENT_TYPE_JSON, NEWS_PAYLOAD);
                if (response != null && response.equals(mData)) {
                    FileUtils.writeStringToFile(cachedFile, response);
                    mDataTimestamp = cachedFile.lastModified();
                } else if (response == null) {
                    response = FileUtils.readStringFromFile(cachedFile);
                }
            } catch (Exception e) {
                Log.e(TAG, "problem with loading top news");
                return null;
            }
        }

        // prevent reshowing news that are already be shown
        if (response.equals(mData)) {
            return null;
        }

        mData = response;
        try {
            return parseResult(new JSONObject(response));
        } catch (JSONException e) {
            Log.e(TAG, "problem with parsing top news");
            return null;
        }
    }

    private List<TopNews> parseResult(JSONObject result) {
        if (result == null) {
            return null;
        }
        final List<TopNews> topNews = new ArrayList<>(5);
        int breakingNewsCount = 0;
        int localNewsCount = 0;
        try {
            JSONObject data = result.getJSONArray("results").getJSONObject(0).getJSONObject("snippet").getJSONObject("extra");
            final JSONArray articles = data.getJSONArray("articles");
            final String newsVersion = data.getString("news_version");
            for (int i = 0; i < articles.length(); i++) {
                try {
                    final JSONObject article = articles.getJSONObject(i);

                    final String url = article.optString("url", "");
                    final String title = article.optString("title", "");
                    final String description = article.optString("description", "");
                    final String domain = article.optString("domain", "");
                    final String shortTitle = article.optString("short_title", "");
                    final String media = article.optString("media", "");
                    final boolean breaking = article.optBoolean("breaking", false);
                    final String breakingLabel = article.optString("breaking_label", "");
                    if (breaking) {
                        breakingNewsCount++;
                    }
                    final boolean isLocalNews = article.has("local_news");
                    if (isLocalNews) {
                        localNewsCount++;
                    }
                    final String localLabel = article.optString("local_label", "");
                    topNews.add(new TopNews(url, title, description, domain, shortTitle, media,
                            breaking, breakingLabel, isLocalNews, localLabel));
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return topNews;
    }

    @Nullable
    public URL getTopNewsUrl(int newsCount) {
        Prefs prefs = Prefs.getInstance(getContext());
        String locale = Locale.getDefault().toString().replace("_", "-");
        final String[] parts = locale.split("-");
        String lang = null;
        String country = GeckoSharedPrefs.forApp(getContext()).getString(SearchEngineManager
                .PREF_REGION_KEY, null);
        if (parts.length > 0) {
            lang = parts[0].toLowerCase();
        }
        final StringBuilder sb = new StringBuilder(NEWS_URL);
        sb.append("&locale=").append(locale);
        if (lang != null) {
            sb.append("&lang=").append(lang);
        }
        if (country != null) {
            sb.append("&country=").append(country);
        }
        sb.append("&count=").append(newsCount);
        sb.append("&platform=1");
        Location location = prefs.getGeofenceLocation();
        if (location != null) {
            sb.append("&loc=").append(location.getLatitude()).append(",")
                    .append(location.getLongitude());
        }
        try {
            return new URL(sb.toString());
        } catch (MalformedURLException e) {
            Log.e(TAG, "Error fetching request" + NEWS_URL);
            return null;
        }
    }

    private File getTopNewsCacheFile() {
        return new File(getContext().getCacheDir(), TOP_NEWS_CACHE_FILE);
    }
}
