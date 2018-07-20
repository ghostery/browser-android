/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.toolbar;

import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import org.mozilla.gecko.R;
import org.mozilla.gecko.Tab;
import org.mozilla.gecko.Tabs;
import org.mozilla.gecko.Telemetry;
import org.mozilla.gecko.TelemetryContract;
import org.mozilla.gecko.menu.MenuItemActionBar;
import org.mozilla.gecko.tabs.TabHistoryController;
import org.mozilla.gecko.util.DrawableUtil;
import org.mozilla.gecko.widget.themed.ThemedImageButton;

import java.util.Arrays;

/**
 * A base implementations of the browser toolbar for tablets.
 * This class manages any Views, variables, etc. that are exclusive to tablet.
 */
abstract class BrowserToolbarTabletBase extends BrowserToolbar {

    protected final LinearLayout actionItemBar;

    protected final BackButton backButton;
    protected final ForwardButton forwardButton;
    /* Cliqz start */
    // add cancel button
    protected final ThemedImageButton editCancel;
    /* Cliqz end */

    protected final View menuButtonMarginView;

    public BrowserToolbarTabletBase(final Context context, final AttributeSet attrs) {
        super(context, attrs);

        actionItemBar = (LinearLayout) findViewById(R.id.menu_items);

        backButton = (BackButton) findViewById(R.id.back);
        backButton.setEnabled(false);
        forwardButton = (ForwardButton) findViewById(R.id.forward);
        forwardButton.setEnabled(false);
        /* Cliqz start */
        // initialize cancel button and tint it to black
        editCancel = (ThemedImageButton)findViewById(R.id.edit_cancel);
        editCancel.setImageDrawable(DrawableUtil.tintDrawable(getContext(),R.drawable
                .ic_close_clear,android.R.color.black));
        /* Cliqz start */
        initButtonListeners();

        focusOrder.addAll(Arrays.asList(tabsButton, (View) backButton, (View) forwardButton, this));
        focusOrder.addAll(urlDisplayLayout.getFocusOrder());
        focusOrder.addAll(Arrays.asList(actionItemBar, menuButton));

        urlDisplayLayout.updateSiteIdentityAnchor(backButton);

        menuButtonMarginView = findViewById(R.id.menu_margin);
        if (menuButtonMarginView != null) {
            menuButtonMarginView.setVisibility(View.VISIBLE);
        }
    }

    private void initButtonListeners() {
        backButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                Tabs.getInstance().getSelectedTab().doBack();
            }
        });
        backButton.setOnLongClickListener(new Button.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                return tabHistoryController.showTabHistory(Tabs.getInstance().getSelectedTab(),
                        TabHistoryController.HistoryAction.BACK);
            }
        });

        forwardButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                Tabs.getInstance().getSelectedTab().doForward();
            }
        });
        forwardButton.setOnLongClickListener(new Button.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                return tabHistoryController.showTabHistory(Tabs.getInstance().getSelectedTab(),
                        TabHistoryController.HistoryAction.FORWARD);
            }
        });
        /* Cliqz start */
        // add clickListener to cancel button
        editCancel.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // If we exit editing mode during the animation,
                // we're put into an inconsistent state (bug 1017276).
                if (!isAnimating()) {
                    Telemetry.sendUIEvent(TelemetryContract.Event.CANCEL,
                            TelemetryContract.Method.ACTIONBAR,
                            getResources().getResourceEntryName(editCancel.getId()));
                    cancelEdit();
                }
            }
        });
        /* Cliqz end */
    }

    @Override
    protected boolean isTabsButtonOffscreen() {
        return false;
    }

    @Override
    public boolean addActionItem(final View actionItem) {
        if (actionItem instanceof MenuItemActionBar) {
            final MenuItemActionBar itemActionBar = (MenuItemActionBar) actionItem;
            itemActionBar.setImageTintList(ColorStateList.valueOf(Color.WHITE));
        }
        actionItemBar.addView(actionItem, LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
        return true;
    }

    @Override
    public void removeActionItem(final View actionItem) {
        actionItemBar.removeView(actionItem);
    }

    @Override
    protected void updateNavigationButtons(final Tab tab) {
        backButton.setEnabled(canDoBack(tab));
        forwardButton.setEnabled(canDoForward(tab));
    }

    @Override
    public void refresh() {
        super.refresh();
        forwardButton.setImageDrawable(getResources().getDrawable(R.drawable.ic_menu_forward));
        backButton.setImageDrawable(getResources().getDrawable(R.drawable.ic_menu_back));
    }

    @Override
    public void setNextFocusDownId(int nextId) {
        super.setNextFocusDownId(nextId);
        backButton.setNextFocusDownId(nextId);
        forwardButton.setNextFocusDownId(nextId);
    }

    @Override
    public void setPrivateMode(final boolean isPrivate) {
        super.setPrivateMode(isPrivate);

        backButton.setPrivateMode(isPrivate);
        forwardButton.setPrivateMode(isPrivate);
        /* Cliqz start */
        // change cancel button to private mode
        editCancel.setPrivateMode(isPrivate);
        /* Cliqz end */

        ((ThemedImageButton) menuButton).setPrivateMode(isPrivate);

        for (int i = 0; i < actionItemBar.getChildCount(); ++i) {
            final MenuItemActionBar child = (MenuItemActionBar) actionItemBar.getChildAt(i);
            child.setPrivateMode(isPrivate);
        }
    }

    @Override
    public View getDoorHangerAnchor() {
        return backButton;
    }

    protected boolean canDoBack(final Tab tab) {
        return (tab.canDoBack() && !isEditing());
    }

    protected boolean canDoForward(final Tab tab) {
        return (tab.canDoForward() && !isEditing());
    }
}
