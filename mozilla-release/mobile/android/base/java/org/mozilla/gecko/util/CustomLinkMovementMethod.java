package org.mozilla.gecko.util;

import android.text.Layout;
import android.text.Spannable;
import android.text.method.LinkMovementMethod;
import android.text.method.MovementMethod;
import android.text.style.ClickableSpan;
import android.text.style.URLSpan;
import android.view.MotionEvent;
import android.widget.TextView;
import org.mozilla.gecko.Tabs;

/**
 * Copyright © Cliqz 2018
 */

public class CustomLinkMovementMethod extends LinkMovementMethod {

    private static CustomLinkMovementMethod sInstance;
    private OnOpenLinkCallBack onOpenLinkCallBack;

    public static MovementMethod getInstance(OnOpenLinkCallBack onOpenLinkCallBack) {
        if (sInstance == null) {
            sInstance = new CustomLinkMovementMethod();
            sInstance.onOpenLinkCallBack = onOpenLinkCallBack;
        }
        return sInstance;
    }

    @Override
    public boolean onTouchEvent(final TextView widget, final Spannable buffer, final MotionEvent event) {
        final int action = event.getAction();
        if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_UP) { // if you want only ACTION_DOWN,remove the ACTION_UP

            final int x = (int) event.getX() - widget.getTotalPaddingLeft() + widget.getScrollX();
            final int y = (int) event.getY() - widget.getTotalPaddingTop() + widget.getScrollY();
            final Layout layout = widget.getLayout();
            final int line = layout.getLineForVertical(y);
            final int off = layout.getOffsetForHorizontal(line, x);
            final ClickableSpan[] link = buffer.getSpans(off, off, ClickableSpan.class);
            if (link.length != 0) {
                Tabs.getInstance().loadUrlInTab(((URLSpan) link[0]).getURL());
                onOpenLinkCallBack.OnOpenLinkLoaded();
            }
            return true;
        }
        return super.onTouchEvent(widget, buffer, event);
    }

    public interface OnOpenLinkCallBack{
        void OnOpenLinkLoaded();
    }
}

