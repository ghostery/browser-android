package org.mozilla.gecko.toolbar;

import android.app.AlertDialog;
import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.support.v4.content.ContextCompat;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Button;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.lwt.LightweightTheme;
import org.mozilla.gecko.widget.themed.ThemedImageButton;

/**
 * Copyright &copy; Cliqz 2018
 */
class Ghosty extends ToolbarRoundButton implements View.OnClickListener {

    private boolean mShowingGhostery = false;

    public Ghosty(Context context, AttributeSet attrs) {
        super(context, attrs);
        initialize(context);
    }

    public Ghosty(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initialize(context);
    }

    private void initialize(Context context) {
        setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        if (mShowingGhostery) {
            EventDispatcher.getInstance().dispatch("Privacy:Hide", null);
        } else {
            EventDispatcher.getInstance().dispatch("Privacy:Show", null);
        }

        mShowingGhostery = !mShowingGhostery;
    }
}
