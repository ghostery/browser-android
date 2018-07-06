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
import org.mozilla.gecko.util.HttpHandler;
import org.mozilla.mozstumbler.service.Prefs;

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
    private static final String NEWS_URL = "https://api.cliqz.com/api/v2/rich-header?path=/v2/map";
    private final String TAG = TopNewsRow.class.getSimpleName();

    public TopNewsLoader(Context context) {
        super(context);
    }

    @Override
    protected void onStartLoading() {
        forceLoad();
    }

    @Override
    public List<TopNews> loadInBackground() {
        JSONObject result = HttpHandler.sendRequest("PUT", getTopNewsUrl(Integer.MAX_VALUE),
                CONTENT_TYPE_JSON, NEWS_PAYLOAD);
        return parseResult(result);
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
}
