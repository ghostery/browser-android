package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 */

public class DashboardWeekFragment extends ControlCenterFragment {

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.bond_dashboard_week_fragment, container, false);
    }

    @Override
    public String getTitle(Context context) {
        return context.getString(R.string.bond_dashboard_week_title);
    }

    @Override
    public void updateUI(GeckoBundle data) {
    }

    @Override
    public void refreshUI() {
    }

    @Override
    public void refreshUIComponent(int id, boolean optionValue) {

    }
}
