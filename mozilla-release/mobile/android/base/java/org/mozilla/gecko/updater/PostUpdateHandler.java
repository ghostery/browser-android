/* -*- Mode: Java; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.updater;

import android.content.res.AssetManager;
import android.content.SharedPreferences;
import android.support.annotation.NonNull;
import android.util.Log;

import org.mozilla.gecko.AppConstants;
import org.mozilla.gecko.BrowserApp;
import org.mozilla.gecko.BuildConfig;
import org.mozilla.gecko.Telemetry;
import org.mozilla.gecko.TelemetryContract;
import org.mozilla.gecko.delegates.BrowserAppDelegateWithReference;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.preferences.GeckoPreferences;
import org.mozilla.gecko.util.IOUtils;
import org.mozilla.gecko.util.ThreadUtils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Perform tasks in the background after the app has been installed/updated.
 */
public class PostUpdateHandler extends BrowserAppDelegateWithReference {
    private static final String LOGTAG = "GeckoPostUpdateHandler";
    private static final boolean DEBUG = false;

    /* Cliqz Start */
    private static final String PREFS_EXTENSIONS_UPDATE_PREFIX = "app.update.feature.";

    @Override
    public void onStart(final BrowserApp browserApp) {
        ThreadUtils.postToBackgroundThread(new Runnable() {
            @Override
            public void run() {
                final SharedPreferences prefs = GeckoSharedPrefs.forApp(browserApp);

                if (DEBUG) {
                    Log.d(LOGTAG, "Build ID changed since last start: '" +
                            AppConstants.MOZ_APP_BUILDID +
                            "', '" +
                            prefs.getString(GeckoPreferences.PREFS_APP_UPDATE_LAST_BUILD_ID, null)
                            + "'");
                }

                // Copy the bundled system add-ons from the APK to the data directory if needed.
                copyFeaturesFromAPK(browserApp);
            }
        });
    }

    /**
     * Copies the /assets/features folder out of the APK and into the app's data directory.
     */
    private void copyFeaturesFromAPK(BrowserApp browserApp) {
        if (DEBUG) {
            Log.d(LOGTAG, "Copying system add-ons from APK to dataDir");
        }

        final String dataDir = browserApp.getApplicationInfo().dataDir;
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(browserApp);
        final AssetManager assetManager = browserApp.getAssets();

        try {
            final String[] assetNames = assetManager.list("features");
            if (assetNames == null) {
                throw new IOException("No features folder in the assets");
            }

            for (int i = 0; i < assetNames.length; i++) {

                final String assetPath = "features/" + assetNames[i];
                final String assetPref = getAssetPrefName(assetNames[i]);
                final String assetBuildID = prefs.getString(assetPref, null);
                if (AppConstants.MOZ_APP_BUILDID.equals(assetBuildID)) {
                    // Nothing to do here the latest version was installed
                    continue;
                }

                if (DEBUG) {
                    Log.d(LOGTAG, "Copying '" + assetPath + "' from APK to dataDir");
                }

                InputStream assetStream;
                try {
                    assetStream = assetManager.open(assetPath);
                } catch (IOException e) {
                    Log.e(LOGTAG, "Can't open asset " + assetPath, e);
                    Telemetry.sendUIEvent(TelemetryContract.Event.FEATUREERROR, TelemetryContract.Method.SYSTEM, e.toString());
                    continue;
                }

                final File outFile = getDataFile(dataDir, assetPath);

                if (outFile == null) {
                    continue;
                }

                OutputStream outStream;
                try {
                    outStream = new FileOutputStream(outFile);
                } catch (FileNotFoundException e) {
                    Log.e(LOGTAG, "Can't open asset " + assetPath, e);
                    Telemetry.sendUIEvent(TelemetryContract.Event.FEATUREERROR, TelemetryContract.Method.SYSTEM, e.toString());
                    continue;
                }

                boolean success = false;
                try {
                    IOUtils.copy(assetStream, outStream);
                    success = true;
                } catch (IOException e) {
                    Log.e(LOGTAG, "Error copying '" + assetPath + "' from APK to dataDir");
                    Telemetry.sendUIEvent(TelemetryContract.Event.FEATUREERROR, TelemetryContract.Method.SYSTEM, e.toString());
                } finally {
                    outStream.close();
                    // Store the assetPref here to be transactional
                    if (success) {
                        prefs.edit().putString(assetPref, AppConstants.MOZ_APP_BUILDID).apply();
                    }
                }
            }
        } catch (IOException e) {
            Log.e(LOGTAG, "Error retrieving packaged system add-ons from APK", e);
            Telemetry.sendUIEvent(TelemetryContract.Event.FEATUREERROR, TelemetryContract.Method.SYSTEM, e.toString());
        }

        // Back compatibility: save the Build ID as Fennec does (if needed)
        if (!AppConstants.MOZ_APP_BUILDID.equals(prefs.getString(GeckoPreferences.PREFS_APP_UPDATE_LAST_BUILD_ID, null))) {
            prefs.edit().putString(GeckoPreferences.PREFS_APP_UPDATE_LAST_BUILD_ID, AppConstants.MOZ_APP_BUILDID).apply();
        }
    }

    @NonNull
    private String getAssetPrefName(@NonNull String assetName) {
        return PREFS_EXTENSIONS_UPDATE_PREFIX + assetName.replaceAll("[-@.]", "_");
    }
    /* Cliqz end */
    
    /**
     * Return a File instance in the data directory, ensuring
     * that the parent exists.
     *
     * @return null if the parents could not be created.
     */
    private File getDataFile(final String dataDir, final String name) {
        File outFile = new File(dataDir, name);
        File dir = outFile.getParentFile();

        if (!dir.exists()) {
            if (DEBUG) {
                Log.d(LOGTAG, "Creating " + dir.getAbsolutePath());
            }
            if (!dir.mkdirs()) {
                Log.e(LOGTAG, "Unable to create directories: " + dir.getAbsolutePath());
                Telemetry.sendUIEvent(TelemetryContract.Event.FEATUREERROR, TelemetryContract.Method.SYSTEM, "Can't create " + dir.getAbsolutePath());
                return null;
            }
        }

        return outFile;
    }
}
