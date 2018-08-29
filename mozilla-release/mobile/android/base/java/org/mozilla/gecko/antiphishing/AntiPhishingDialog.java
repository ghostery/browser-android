package org.mozilla.gecko.antiphishing;

import android.content.DialogInterface;
import android.content.res.Resources;
import android.support.v7.app.AlertDialog;
import android.text.method.LinkMovementMethod;
import android.view.KeyEvent;
import android.widget.TextView;

import org.mozilla.gecko.BrowserApp;
import org.mozilla.gecko.R;

/**
 * Copyright © Cliqz 2018
 */
public class AntiPhishingDialog extends AlertDialog {

    private AntiPhishingDialogActionListener listener;

    public interface AntiPhishingDialogActionListener {
        void backPressed();
    }

    public AntiPhishingDialog(BrowserApp app) {
        super(app);
        this.listener = app;
        final Resources resources = app.getResources();
        setTitle(resources.getString(R.string.antiphishing_dialog_title));
        setButton(DialogInterface.BUTTON_POSITIVE,
                resources.getString(R.string.antiphishing_walk_away), onClickListener);
        setButton(DialogInterface.BUTTON_NEGATIVE,
                resources.getString(R.string.antiphishing_ignore_danger), onClickListener);
        setCancelable(false);
        setUrl("unknown");
        setOnKeyListener(onKeyListener);
    }

    private final OnKeyListener onKeyListener = new DialogInterface.OnKeyListener() {
        @Override
        public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
            if (keyCode == KeyEvent.KEYCODE_BACK) {
                listener.backPressed();
                dialog.dismiss();
                return true;
            }
            return false;
        }
    };

    private final OnClickListener onClickListener = new OnClickListener() {
        @Override
        public void onClick(DialogInterface dialog, int which) {
            switch (which) {
                case DialogInterface.BUTTON_POSITIVE:
                    listener.backPressed();
                    break;
                case DialogInterface.BUTTON_NEGATIVE:
                    break;
                default:
                    break;
            }
            dialog.dismiss();
        }
    };

    public void setUrl(String url) {
        setMessage(getContext().getString(R.string.antiphishing_message, url));
    }

    @Override
    public void show() {
        super.show();
        TextView.class.cast(findViewById(android.R.id.message))
                .setMovementMethod(LinkMovementMethod.getInstance());
    }

}
