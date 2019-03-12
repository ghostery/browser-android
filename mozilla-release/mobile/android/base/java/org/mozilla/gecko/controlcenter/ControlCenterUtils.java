package org.mozilla.gecko.controlcenter;

import org.mozilla.gecko.util.GeckoBundle;
import org.mozilla.gecko.util.GeckoBundleUtils;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;

public class ControlCenterUtils {

    private ControlCenterUtils() {
    }

    static byte[] getCCDataHash(GeckoBundle geckoBundle) {
        MessageDigest m;
        try {
            m = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            return new byte[0];
        }

        m.update(getByteArray(GeckoBundleUtils.safeGetStringArray(geckoBundle,
                "data/summary/site_blacklist")));
        m.update(getByteArray(GeckoBundleUtils.safeGetStringArray(geckoBundle,
                "data/summary/site_whitelist")));
        m.update(new byte[]{(byte) (GeckoBundleUtils.safeGetBoolean(geckoBundle,
                "data/summary/paused_blocking") ? 1 : 0)});
        m.update(new byte[]{(byte) (GeckoBundleUtils.safeGetBoolean(geckoBundle,
                "data/panel/panel/enable_smart_block") ? 1 : 0)});
        m.update(new byte[]{(byte) (GeckoBundleUtils.safeGetBoolean(geckoBundle,
                "data/panel/panel/enable_ad_block") ? 1 : 0)});
        m.update(new byte[]{(byte) (GeckoBundleUtils.safeGetBoolean(geckoBundle,
                "data/panel/panel/enable_anti_tracking") ? 1 : 0)});
        m.update(new byte[]{(byte) (GeckoBundleUtils.safeGetBoolean(geckoBundle,
                "data/cliqz/adblock/disabledForDomain") ? 1 : 0)});

        final GeckoBundle[] summaryCategories = GeckoBundleUtils.safeGetBundleArray(geckoBundle,
                "data/summary/categories");
        if (summaryCategories != null) {
            for (GeckoBundle category : summaryCategories) {
                final GeckoBundle[] trackers = category.getBundleArray("trackers");
                if (trackers != null) {
                    for (GeckoBundle tracker : trackers) {
                        m.update(new byte[]{(byte) (tracker.getBoolean("blocked") ? 1 : 0)});
                        m.update(new byte[]{(byte) (tracker.getBoolean("ss_allowed") ? 1 : 0)});
                        m.update(new byte[]{(byte) (tracker.getBoolean("ss_blocked") ? 1 : 0)});

                        final GeckoBundle[] sources = tracker.getBundleArray("sources");
                        if (sources != null) {
                            for (GeckoBundle source : sources) {
                                m.update(new byte[]{(byte) (source.getBoolean("blocked") ? 1 : 0)});
                            }
                        }
                    }
                }
            }
        }

        final GeckoBundle[] settingsCategories = GeckoBundleUtils.safeGetBundleArray(geckoBundle,
                "data/settings/categories");
        if (settingsCategories != null) {
            for (GeckoBundle category : settingsCategories) {
                final GeckoBundle[] trackers = category.getBundleArray("trackers");
                if (trackers != null) {
                    for (GeckoBundle tracker : trackers) {
                        m.update(new byte[]{(byte) (tracker.getBoolean("blocked") ? 1 : 0)});
                    }
                }
            }
        }

        return m.digest();
    }

    private static byte[] getByteArray(String[] stringArray) {
        final ByteArrayOutputStream stream = new ByteArrayOutputStream();
        final byte[] byteArray;
        try {
            for (final String string : stringArray) {
                stream.write(string.getBytes());
            }
            byteArray = stream.toByteArray();
            stream.flush();
            stream.close();
        } catch (IOException e) {
            return new byte[0];
        }
        return byteArray;
    }

}
