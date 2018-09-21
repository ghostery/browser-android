package org.mozilla.gecko.myoffrz;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v4.content.AsyncTaskLoader;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.mozilla.gecko.AppConstants;
import org.mozilla.gecko.util.FileUtils;
import org.mozilla.gecko.util.HttpHandler;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Copyright © Cliqz 2018
 */
public class MyOffrzLoader extends AsyncTaskLoader<JSONObject> {

    private static final long SIX_HOURS_IN_S = 21600L;
    private static final String METHOD_GET = "GET";
    private static final String MYOFFRZ_CACHE_FILE = "MyOffrzCache.json";
    private static final String TAG = MyOffrzLoader.class.getSimpleName();
    private static final String VALIDITY_KEY = "validity";
    // TODO: Replace this with branding constants
    private static final String ENDPOINT_PREFIX = "https://offers-api.ghostery.net/api/v1/";
    // private static final String ENDPOINT_PREFIX = "http://offers-api-stage.clyqz.com:8181/api/v1/";
    private static final String SUBTRIGGERS_URL_FORMAT = ENDPOINT_PREFIX +
            "loadsubtriggers?parent_id=root&t_eng_ver=22&channel=%s";
    private static final String OFFRZ_URL_FORMAT = ENDPOINT_PREFIX +
            "offers?intent_name=%s&t_eng_ver=22&channel=%s";
    private static final String CONDITION_KEY = "condition";
    private static final String ACTIONS_KEY = "actions";
    private static final String UI_INFO_KEY = "ui_info";
    private static final String TEMPLATE_DATA_KEY = "template_data";
    private static Pattern CONFIG_LOCATION_PATTERN =
        Pattern.compile(".*\"\\$if_pref\"\\s*,\\s*\\[\\s*\"config_location\"\\s*,\\s*\"([^\"]+)\".*");
    private static final Pattern ACTIONS_PATTERN =
        Pattern.compile(".*\"name\"\\s*:\\s*\"([^\"]+).*");
    private static final String DOWNLOAD_DATE = "browserDownloadDate";

    private JSONObject mOffrz = null;

    public MyOffrzLoader(@NonNull Context context) {
        super(context);
    }

    // Indirection for testing
    private File getCacheFile() {
        return new File(getContext().getCacheDir(), MYOFFRZ_CACHE_FILE);
    }

    private JSONObject getCachedOffrz(File cachedOffrzFile){
        try {
            final JSONObject firstOffrz = FileUtils.readJSONObjectFromFile(cachedOffrzFile);
            if (!expired(firstOffrz)) {
                return firstOffrz;
            }
            // force deletion the file if expired
            throw new JSONException("Force deletion");
        } catch (FileNotFoundException e) {
            // Nothing to do here, we can not have the file
        } catch (JSONException e) {
            // Delete the file if it is not valid
            if (!cachedOffrzFile.delete()) {
                Log.w(TAG, "Can't delete cached offrz file");
            }
        } catch (IOException e) {
            Log.e(TAG, "Can't open offrz cache");
        }
        return null;
    }

    @Override
    public JSONObject loadInBackground() {
        // There was an error loading the cache or the offer expired
        final File cachedOffrzFile = getCacheFile();
        final JSONObject cachedOffrz = getCachedOffrz(cachedOffrzFile);
        if(cachedOffrz != null){
            return cachedOffrz;
        }

        JSONObject firstOffrz;

        try {
            // Decode the right action for locale
            final String actionName = loadSubTriggers();
            final String response = HttpHandler.sendRequest(METHOD_GET,
                    new URL(String.format(OFFRZ_URL_FORMAT, actionName, AppConstants.MYOFFRZ_CHANNEL)),
                    null, null);
            if (response == null) {
                return null;
            }
            final JSONArray offrz = new JSONArray(response);
            firstOffrz = offrz.getJSONObject(0);
            // Add download date here, otherwise the offer will be expired even if it is freshly
            // downloaded, check expired(...) logic.
            final long now = System.currentTimeMillis() / 1000L; // in seconds
            firstOffrz.put(DOWNLOAD_DATE, now);
            if (expired(firstOffrz)) {
                // Even the downloaded offer is expired
                return null;
            }
            // Add download date
        } catch (MalformedURLException e) {
            Log.e(TAG, "Malformed hardcoded url");
            return null;
        } catch (JSONException e) {
            Log.e(TAG, "Can't parse json response");
            return null;
        } catch (Exception e) {
            // This blog prevents generic crashes
            Log.e(TAG, "Generic failure", e);
            return null;
        }
        try {
            FileUtils.writeJSONObjectToFile(cachedOffrzFile, firstOffrz);
        } catch (IOException e) {
            Log.e(TAG, "Can't cache data to " + MYOFFRZ_CACHE_FILE);
        }
        return firstOffrz;
    }

    private String loadSubTriggers() throws MalformedURLException, JSONException {
        final URL url = new URL(String.format(SUBTRIGGERS_URL_FORMAT, AppConstants.MYOFFRZ_CHANNEL));
        final String response = HttpHandler.sendRequest(METHOD_GET, url, null, null);
        if (response == null) {
            return null;
        }

        final String country = Locale.getDefault().getCountry().toLowerCase();
        final JSONArray jsonArray = new JSONArray(response);
        for (int i = 0; i < jsonArray.length(); i++) {
            final JSONObject jsonObject = jsonArray.getJSONObject(i);
            final String condition = jsonObject.getJSONArray(CONDITION_KEY).toString();
            final Matcher locationMatcher = CONFIG_LOCATION_PATTERN.matcher(condition);
            if (!locationMatcher.matches()) {
                continue;
            }
            final String location = locationMatcher.group(1);
            if (!country.equals(location)) {
                continue;
            }
            final String actions = jsonObject.getJSONArray(ACTIONS_KEY).toString();
            final Matcher actionsMatcher = ACTIONS_PATTERN.matcher(actions);
            if (actionsMatcher.matches()) {
                return actionsMatcher.group(1);
            }
        }
        return null;
    }

    @Override
    public void deliverResult(JSONObject data) {
        mOffrz = data;
        super.deliverResult(mOffrz);
    }

    @Override
    protected void onStartLoading() {
        if (mOffrz != null && !expired(mOffrz)) {
            super.deliverResult(mOffrz);
        } else {
            forceLoad();
        }
    }

    private static boolean expired(JSONObject offer) {
        final long now = System.currentTimeMillis() / 1000L; // seconds
        try {
            final long validity = offer.getJSONObject(UI_INFO_KEY).getJSONObject(TEMPLATE_DATA_KEY)
                    .getLong(VALIDITY_KEY);
            // Using `now - SIX_HOURS_IS_S - 1` as default in the following line ensures we
            // download a new file if there were no DOWNLOAD_DATE in the cached one
            final long downloadDate = offer.optLong(DOWNLOAD_DATE, now - SIX_HOURS_IN_S -1);
            return now > validity || now > (downloadDate + SIX_HOURS_IN_S);
        } catch (JSONException e) {
            return true;
        }
    }
}
