package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.support.v4.app.Fragment;

import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 */
public abstract class ControlCenterFragment extends Fragment {

    public abstract String getTitle(Context context);

    public abstract void updateUI(GeckoBundle data);

    public abstract void refreshUI();
}
