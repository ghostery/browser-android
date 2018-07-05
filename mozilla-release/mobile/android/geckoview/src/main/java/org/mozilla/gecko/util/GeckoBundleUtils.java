/* Copyright © Cliqz 2018 */
package org.mozilla.gecko.util;

import android.support.annotation.NonNull;

/**
 * Cliqz utils to access and manipulate {@link GeckoBundle}s
 */
public final class GeckoBundleUtils {

    private GeckoBundleUtils() { /* No Instances */ }

    private  static <T> T getValueFromBundle(GeckoBundle bundle, String key, T defaultValue, Class<T> clazz) {
        if (key.isEmpty()) {
            return defaultValue;
        }

        final String[] segs = key.split("/");
        try {
            GeckoBundle currentBundle = bundle;
            for (int i = 0; i < segs.length - 1; i++) {
                currentBundle = currentBundle.getBundle(segs[i]);
            }
            final String fk = segs[segs.length-1];
            if (clazz == Integer.class) {
                return clazz.cast(currentBundle.getInt(fk));
            }
            if (clazz == Boolean.class) {
                return clazz.cast(currentBundle.getBoolean(fk));
            }
            if (clazz == String.class) {
                return clazz.cast(currentBundle.getString(fk));
            }
            if (clazz == GeckoBundle.class) {
                return clazz.cast(currentBundle.getBundle(fk));
            }
            if (clazz == String[].class) {
                return clazz.cast(currentBundle.getStringArray(fk));
            }
            if (clazz == GeckoBundle[].class) {
                return clazz.cast(currentBundle.getBundleArray(fk));
            }
            return defaultValue;
        } catch (Throwable tr) {
            return defaultValue;
        }
    }

    /**
     * Safely resolve an integer value inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/value")
     * @return the value or 0 if the key is not found
     */
    public static int safeGetInt(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, 0, Integer.class);
    }

    /**
     * Safely resolve a boolean value inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/value")
     * @return the value or false if the key is not found
     */
    public static boolean safeGetBoolean(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, false, Boolean.class);
    }

    /**
     * Safely resolve a String reference inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/string")
     * @return the value or an empty String if the key is not found
     */
    public static String safeGetString(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, "", String.class);
    }

    /**
     * Safely resolve a {@link GeckoBundle} reference inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/bundle")
     * @return the value or an empty {@link GeckoBundle} if the key is not found
     */
    public static GeckoBundle safeGetBundle(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, new GeckoBundle(), GeckoBundle.class);
    }

    /**
     * Safely resolve a String array reference inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/string_array")
     * @return the value or an empty String Array if the key is not found
     */
    public static String[] safeGetStringArray(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, new String[0], String[].class);
    }

    /**
     * Safely resolve a {@link GeckoBundle} array reference inside nested bundles
     *
     * @param bundle the top-level bundle
     * @param key the path to the value (i.e. "nested1/nested2/bundle")
     * @return the value or an empty {@link GeckoBundle} if the key is not found
     */
    public static GeckoBundle[] safeGetBundleArray(@NonNull GeckoBundle bundle, @NonNull String key) {
        return getValueFromBundle(bundle, key, new GeckoBundle[0], GeckoBundle[].class);
    }
}
