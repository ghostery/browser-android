package org.mozilla.gecko.antiphishing;

import android.support.annotation.VisibleForTesting;
import android.util.Log;

import java.io.InputStreamReader;
import java.io.Reader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.MessageDigest;
import java.util.Locale;

/**
 * Copyright © Cliqz 2018
 */
public class AntiPhishing {

    private static final String TAG = AntiPhishing.class.getSimpleName();
    private static final String DEFAULT_ENDPOINT = "https://antiphishing.cliqz.com/api/bwlist";

    public interface AntiPhishingCallback {
        void onUrlProcessed(String url, boolean isPhishing);
    }

    private final Cache cache;
    private final String endpoint;

    public AntiPhishing() {
        this(new Cache());
    }

    @VisibleForTesting
    AntiPhishing(Cache cache) {
        this(cache, DEFAULT_ENDPOINT);
    }

    @VisibleForTesting
    AntiPhishing(Cache cache, String endpoint) {
        this.cache = cache;
        this.endpoint = endpoint;
    }

    /**
     * It processes the given url to check against our anti-phishing filter.
     * @param url the url the user wants to check
     * @param callback an {@link AntiPhishingCallback} instance that will be called as soon as we
     *                 know if the if the url is for phishing or not. <b>The callback can be called
     *                 on a different thread than the original one.</b>
     */
    public void processUrl(String url, AntiPhishingCallback callback) {
        if (callback == null) { return; }
        final String host = AntiPhishingUtils.getDomain(url);
        if (host.isEmpty()) {
            Log.w(TAG, "processUrl called with empty or null string");
            callback.onUrlProcessed("", false);
        }
        try {
            final String md5Hash = calculateMD5(host);
            final Cache.Result result = cache.check(md5Hash);
            switch (result) {
                case BLACKLIST:
                    callback.onUrlProcessed(url, true);
                    break;
                case FAULT:
                    fetchHash(url, md5Hash, callback);
                    break;
                default:
                    callback.onUrlProcessed(url, false);
            }
        } catch (MD5Exception e) {
            Log.e(TAG, e.getMessage(), e.getCause());
            callback.onUrlProcessed(url, false);
        }
    }

    private void fetchHash(final String url, final String md5Hash, final AntiPhishingCallback callback) {
        final Runnable fetcher = new Runnable() {
            @Override
            public void run() {
                final String md5prefix = AntiPhishingUtils.splitMD5(md5Hash)[0];
                final String formattedUrl = String.format(Locale.US, "%s?md5=%s", endpoint, md5prefix);
                HttpURLConnection connection = null;
                try {
                    final URL curl = new URL(formattedUrl);
                    connection = (HttpURLConnection) curl.openConnection();
                    final Reader reader = new InputStreamReader(connection.getInputStream());
                    final CacheEntry response = CacheEntry.from(reader);
                    cache.addToCache(md5prefix, response);
                    processUrl(url, callback);
                    reader.close();
                } catch (Exception e) {
                    // Keep as plain java as possible
                    e.printStackTrace();
                    callback.onUrlProcessed(url, false);
                } finally {
                    if (connection != null) {
                        connection.disconnect();
                    }
                }
            }
        };
        new Thread(fetcher).start();
    }

    /**
     * Given a message (a string) will compute the hex representation of its MD5 hash.
     *
     * @param message a non-null string
     * @return the hexadecimal string representation of the message md5 hash
     * @throws MD5Exception if it can not calculate the md5 string
     */
    private static String calculateMD5(String message) throws MD5Exception {
        try {
            final MessageDigest md = MessageDigest.getInstance("MD5");
            md.reset();
            final byte[] digest = md.digest(message.getBytes());
            final byte[] hexDigest = new byte[digest.length * 2];
            for (int i = 0; i < digest.length; i++) {
                final byte high = (byte) ((digest[i] & 0xf0) >> 4);
                final byte low = (byte) (digest[i] & 0x0f);
                final int hi = i * 2;
                hexDigest[hi] = (byte) (high < 0x0a ? 0x30 + high : 0x57 + high);
                hexDigest[hi + 1] = (byte) (low < 0x0a ? 0x30 + low : 0x57 + low);
            }
            return new String(hexDigest);
        } catch (Throwable e) {
            // Rethrow the exception wrapper in the MD5Exception
            throw new MD5Exception(e);
        }
    }

    public static class MD5Exception extends Exception {
        MD5Exception(Throwable cause) {
            super("Can't calculate MD5 string", cause);
        }
    }
}
