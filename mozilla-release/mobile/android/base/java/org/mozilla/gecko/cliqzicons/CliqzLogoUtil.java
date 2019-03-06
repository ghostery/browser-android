package org.mozilla.gecko.cliqzicons;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;

/**
 * Copyright © Cliqz 2018
 */
public class CliqzLogoUtil {

    private static final float TEXT_SIZE_TOLERANCE = 1f;
    private static final float MARGIN_PROPORTION = 0.05f; // 5%

    @SuppressLint("UseSparseArrays")
    private static final HashMap<Integer, Integer> TEXT_SIZES_MAP = new HashMap<>();
    private static final float MAX_ACCEPTED_SIZE_DIFFERENCE = 2f;

    private enum LogoSizes {
        s48x48(48),
        s72x72(72),
        s96x96(96),
        s144x144(144),
        s192x192(192),
        s288x288(288),
        s384x384(384),
        s528x528(528),
        s672x672(672),
        s816x816(816),
        s1056x1056(1056),
        s1296x1296(1296),
        s1536x1536(1536),
        s1824x1824(1824);

        final int size;

        LogoSizes(int size) {
            this.size = size;
        }

        public static LogoSizes getSize(int maxSide) {
            final LogoSizes[] values = LogoSizes.values();
            int maxIndex = 0;
            float maxError = 1f / Math.abs(values[0].size - maxSide);
            for (int i = 1; i < values.length; i++) {
                final float error = 1f / Math.abs(values[i].size - maxSide);
                if (error > maxError) {
                    maxIndex = i;
                    maxError = error;
                }
            }
            return values[maxIndex];
        }
    }

    private static final String[] DEFAULT_COLORS = {"#c3043e", "#d7011d", "#e92207", "#f9204c",
            "#ff5349", "#e24b2c", "#f97136", "#ff951d", "#eab804", "#daca19", "#bacd39", "#91d131",
            "#74d463", "#79c28b", "#328c67", "#07613c", "#006567", "#50b1a2", "#4bd3b2", "#95b2cc",
            "#61cfd9", "#43bdd9", "#429abd", "#2fc4fc", "#5ea3f9", "#4592d8", "#506bb8", "#1747a6",
            "#073673", "#530773", "#8a52a2", "#9077e3", "#8c275f", "#d248ca", "#db5c8c", "#f8458f",
            "#f36e8d", "#fb91ae", "#c78e6d", "#999999", "#666666", "#333333"};

    //version of the brand database. we need to update version manually whenever the database is updated
    private static final String DATABASE_VERSION = "1515404421880";

    private static final String BASE_ICON_URL =
            "https://cdn.ghostery.net/brands-database/database/%s/pngs/%s/%s_%d.png";

    @SuppressLint("DefaultLocale")
    public static String getIconUrl(String url, int width, int height) {
        String[] urlParts = {};
        try {
            final URI uri = new URI(url);
            if (uri.getHost() != null) {
                urlParts = uri.getHost().split("\\.");
            }
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        //We need to get the rule from the brand database which matches the url
        //cliqz.example.de and ghostery.example.de can have different logos
        //similarly mobile.example.de and mobile.example.com can have different logos
        //
        // This is the example to give an idea what the variables below represent
        // In case of mobile.example.co.uk, mobile is predomain, example is domain and
        // .co.uk is the tld
        StringBuilder tld = new StringBuilder();
        final String preDomain;
        final String domain;
        if (urlParts.length > 2) {
            preDomain = urlParts[0];
            domain = urlParts[1];
            for (int i = 2; i < urlParts.length; i++) {
                tld.append(".");
                tld.append(urlParts[i]);
            }
        } else if (urlParts.length == 2) {
            preDomain = "";
            domain = urlParts[0];
            tld.append(urlParts[1]);
        } else {
            domain = "";
            preDomain = "";
        }
        final String[] availableRules = CliqzIconDatabase.getRulesForDomain(domain);
        String rule = "";
        if (availableRules == null) {
            rule = "$"; //default rule
        } else {
            for (String availableRule : availableRules) {
                if (availableRule.startsWith(preDomain)) {
                    rule = availableRule;
                    break;
                }
                if (availableRule.endsWith(tld.toString())) {
                    rule = availableRule;
                    break;
                }
            }
        }
        return String.format(BASE_ICON_URL, DATABASE_VERSION, domain, rule,
                LogoSizes.getSize(Math.max(width, height)).size);
    }

    public static Drawable getDefaultIcon(String url, int width, int height, int radius) {
        final String hostName = getHostName(url);
        int index = 0;
        for (int i = 0; i < hostName.length(); i++) {
            index += hostName.charAt(i);
        }
        index %= DEFAULT_COLORS.length;
        final int textSize = calculateTextSize(width, height);
        return new DefaultIconDrawable(hostName, Color.parseColor(DEFAULT_COLORS[index]), textSize, radius);
    }

    private static String getHostName(String url) {
        try {
            final URI uri = new URI(url);
            final String uriHost = uri.getHost();
            if (uriHost == null) {
                return "";
            }
            final String[] hostParts = uriHost.split("\\.");
            if (hostParts.length == 0) {
                // This should never happen, just in case
                return "?";
            } else if (hostParts.length <= 2) {
                // Urls like https://itsfoss.com that do not have a www., amp., m., etc as prefix
                return hostParts[0];
            } else {
                // Any other url, it will try get the longest component with at least 3 characters.
                // For a url like  www.ab.co.uk, it will return www as host: this is wrong but we
                // believe there aren't so many relevant websites with such url format.
                String host = hostParts[0];
                for (int i = 1; i < hostParts.length - 1; i++) {
                    final int len = hostParts[i].length();
                    if (len >= 3 && len > host.length()) {
                        host = hostParts[i];
                    }
                }
                return host;
            }
        } catch (URISyntaxException e) {
            e.printStackTrace();
            return "!";
        }
    }

    private static int calculateTextSize(int width, int height) {
        // i.e. 1920 x 1080, key = 192001080;
        final int key = 100000 * width + height;
        final Integer memorisedSize = TEXT_SIZES_MAP.get(key);
        if (memorisedSize != null) {
            return memorisedSize;
        }

        // Calculate the size
        final Paint paint = new Paint();
        final String sampleText = "mm";
        final float margin = width * MARGIN_PROPORTION;
        float deltaSize = Integer.MAX_VALUE;
        float prevSize = 0;
        float size = width;
        while (deltaSize > TEXT_SIZE_TOLERANCE) {
            paint.setTextSize(size);
            final float calculatedSize = paint.measureText(sampleText)+ 2f * margin;
            deltaSize = Math.abs(prevSize - size) / 2f;
            final float d = width - calculatedSize;
            prevSize = size;
            if (d < 0) {
                size -= deltaSize;
            } else if (d > MAX_ACCEPTED_SIZE_DIFFERENCE) {
                size += deltaSize;
            } else {
                break;
            }
        }

        synchronized (TEXT_SIZES_MAP) {
            TEXT_SIZES_MAP.put(key, (int) size);
        }

        return (int) size;
    }
}
