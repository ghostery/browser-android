package org.mozilla.gecko.cliqzicons;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

import com.squareup.picasso.Transformation;

import java.net.URI;
import java.net.URISyntaxException;

/**
 * Copyright © Cliqz 2018
 */
public class CliqzLogoUtil {

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

    private static final String BASE_ICON_URL =
            "https://cdn.cliqz.com/brands-database/database/1521469421408/pngs/%s/$_%d.png";

    @SuppressLint("DefaultLocale")
    public static String getIconUrl(String url, int width, int height) {
        return String.format(BASE_ICON_URL, getHostName(url), LogoSizes.getSize(Math.max(width, height)).size);
    }

    public static Drawable getDefaultIcon(String url, int width, int height) {
        final String hostName = getHostName(url);
        int index = 0;
        for (int i = 0; i < hostName.length(); i++) {
            index += hostName.charAt(i);
        }
        index %= DEFAULT_COLORS.length;
        return new DefaultIconDrawable(hostName, Color.parseColor(DEFAULT_COLORS[index]),
                LogoSizes.getSize(Math.max(width, height)).size);
    }

    private static String getHostName(String url) {
        String host = "";
        try {
            final URI uri = new URI(url);
            //from observations getHost() returns hosts in following formats
            // - www.facebook.com, mobile.reuters.com, amp.time.com etc and we need just facebook, reuters or time
            if (uri.getHost() != null) {
                final String[] hostParts = uri.getHost().split("\\.");
                if (hostParts.length > 1) {
                    host = hostParts[1];
                } else {
                    host = "";
                }
            }
        } catch (URISyntaxException e) {
            e.printStackTrace();
        }
        return host;
    }
}
