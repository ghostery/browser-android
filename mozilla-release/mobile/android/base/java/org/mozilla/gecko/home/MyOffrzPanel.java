package org.mozilla.gecko.home;

import android.os.Bundle;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.Loader;
import android.util.Log;

import org.json.JSONObject;
import org.mozilla.gecko.myoffrz.MyOffrzLoader;

/**
 * Cliqz 2018
 * This file is derived from @{@link TopSitesPanel}.java
 */


public class MyOffrzPanel extends HomeFragment {

    private static final String LOGTAG = "GeckoMyOffrzPanel";

    // Cursor loader ID for MyOffrz
    private static final int LOADER_ID_MY_OFFRZ = 0;

    // Callbacks used for the search and favicon cursor loaders
    private MyOffrzPanel.MyOffrzLoaderCallbacks mMyOffrzLoaderCallbacks;

    private static final boolean logVerbose = Log.isLoggable(LOGTAG, Log.VERBOSE);

    private static void trace(final String message) {
        if (logVerbose) {
            Log.v(LOGTAG, message);
        }
    }

    @Override
    protected void load() {
        getLoaderManager().initLoader(LOADER_ID_MY_OFFRZ, null, mMyOffrzLoaderCallbacks);

    }



    private class MyOffrzLoaderCallbacks implements
            LoaderManager.LoaderCallbacks<JSONObject> {

        @Override
        public Loader<JSONObject> onCreateLoader(int id, Bundle args) {
            trace("Creating TopSitesLoader: " + id);
            return new MyOffrzLoader(getActivity());
        }

        @Override
        public void onLoadFinished(Loader<JSONObject> loader, JSONObject data) {

        }

        @Override
        public void onLoaderReset(Loader<JSONObject> loader) {

        }
    }
}
