package org.mozilla.gecko.util;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.text.Layout;
import android.text.Spannable;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.URLSpan;
import android.view.MotionEvent;
import android.widget.TextView;

import org.mozilla.gecko.Tabs;
import org.mozilla.gecko.customtabs.CustomTabsActivity;

/**
 * Copyright © Cliqz 2018
 *
 * Custom class to handle hyperlinks in TextViews. Using this class we can open links in our
 * own browser, either in a regular tab or a CustomTab (similar to chrome tab).
 * Set the behaviour by using the {@link #init(int)} method
 */
public class CustomLinkMovementMethod extends LinkMovementMethod {

    public static final int OPEN_IN_TAB = 1;
    public static final int OPEN_IN_CUSTOM_TAB = 2;

    private static CustomLinkMovementMethod sInstance;
    private OnOpenLinkCallBack onOpenLinkCallBack;
    private int mOpenType = OPEN_IN_CUSTOM_TAB;

    public static CustomLinkMovementMethod getInstance(OnOpenLinkCallBack onOpenLinkCallBack) {
        if (sInstance == null) {
            sInstance = new CustomLinkMovementMethod();
            sInstance.onOpenLinkCallBack = onOpenLinkCallBack;
        }
        return sInstance;
    }

    /**
     *
     * @param type specifies how the link should open. Use {@link #OPEN_IN_TAB} to open in a regular tab
     *             or {@link #OPEN_IN_CUSTOM_TAB} to open in a Mozilla CustomTab
     */
    public void init(int type) {
        mOpenType = type;
    }

    @Override
    public boolean onTouchEvent(final TextView widget, final Spannable buffer, final MotionEvent event) {
        final int action = event.getAction();
        if (action == MotionEvent.ACTION_UP) {
            final int x = (int) event.getX() - widget.getTotalPaddingLeft() + widget.getScrollX();
            final int y = (int) event.getY() - widget.getTotalPaddingTop() + widget.getScrollY();
            final Layout layout = widget.getLayout();
            final int line = layout.getLineForVertical(y);
            final int off = layout.getOffsetForHorizontal(line, x);
            final ClickableSpan[] link = buffer.getSpans(off, off, ClickableSpan.class);
            if (link.length != 0) {
                openUrl(link[0], widget.getContext());
            }
        }
        return true;
    }

    private void openUrl(ClickableSpan link, Context context) {
        switch (mOpenType) {
            case OPEN_IN_TAB:
                Tabs.getInstance().loadUrlInTab(((URLSpan) link).getURL());
                onOpenLinkCallBack.OnOpenLinkLoaded();
                break;
            case OPEN_IN_CUSTOM_TAB:
                final Intent intent = new Intent(context, CustomTabsActivity.class);
                intent.setData(Uri.parse(((URLSpan) link).getURL()));
                context.startActivity(intent);
                break;
        }
    }

    public interface OnOpenLinkCallBack {
        void OnOpenLinkLoaded();
    }
}

