package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.support.v7.app.AlertDialog;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 */
public class RestoreDefaultsDialog implements DialogInterface.OnClickListener{

    private BaseControlCenterPagerAdapter.ControlCenterCallbacks controlCenterCallbacks;
    private RestoreDialogCallbacks restoreDialogCallbacks;

    private RestoreDefaultsDialog() {
    }

    public interface RestoreDialogCallbacks {

        void onRestore();
    }

    public static void show(Context context,
                            BaseControlCenterPagerAdapter.ControlCenterCallbacks controlCenterCallbacks,
                            RestoreDialogCallbacks restoreDialogCallbacks) {
        final RestoreDefaultsDialog restoreDefaultsDialog = new RestoreDefaultsDialog();
        restoreDefaultsDialog.controlCenterCallbacks = controlCenterCallbacks;
        restoreDefaultsDialog.restoreDialogCallbacks = restoreDialogCallbacks;
        new AlertDialog.Builder(context, R.style.Cliqz_AlertDialogTheme)
                .setTitle(R.string.cc_reset_changes_dialog_title)
                .setMessage(R.string.cc_reset_changes_dialog_message)
                .setPositiveButton(R.string.cc_reset, restoreDefaultsDialog)
                .setCancelable(true)
                .setNegativeButton(R.string.default_browser_dialog_cancel, restoreDefaultsDialog)
                .show();
    }

    @Override
    public void onClick(DialogInterface dialog, int which) {
        switch (which) {
            case AlertDialog.BUTTON_POSITIVE:
                final Handler handler = new Handler();
                final EventDispatcher evd = EventDispatcher.getInstance();
                final GeckoBundle geckoBundle = new GeckoBundle();
                geckoBundle.putStringArray("site_whitelist", new String[0]);
                geckoBundle.putStringArray("site_blacklist", new String[0]);
                geckoBundle.putBundle("site_specific_unblocks", new GeckoBundle());
                geckoBundle.putBundle("site_specific_blocks", new GeckoBundle());
                geckoBundle.putBoolean("paused_blocking", false);
                geckoBundle.putBoolean("enable_anti_tracking", true);
                geckoBundle.putBoolean("enable_ad_block", true);
                geckoBundle.putBoolean("enable_smart_block", true);
                evd.dispatch("Privacy:SetInfo", geckoBundle);
                controlCenterCallbacks.controlCenterSettingsChanged();

                final GeckoBundle updatePolicyBundle = new GeckoBundle();
                updatePolicyBundle.putString("blockingPolicy", "UPDATE_BLOCK_RECOMMENDED");
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        evd.dispatch("Privacy:SetBlockingPolicy", updatePolicyBundle);
                    }
                }, 500);

                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        //we fetch the new data from the extension, after resetting everything
                        evd.dispatch("Privacy:GetInfo",null);
                    }
                }, 1000);
                restoreDialogCallbacks.onRestore();
                break;
        }
    }
}
