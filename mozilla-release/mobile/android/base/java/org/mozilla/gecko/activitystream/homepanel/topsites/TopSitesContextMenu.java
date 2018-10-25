package org.mozilla.gecko.activitystream.homepanel.topsites;

import android.content.Context;
import android.support.v7.widget.PopupMenu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;

import org.mozilla.gecko.GeckoApplication;
import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.WebpageModel;
import org.mozilla.gecko.db.BrowserDB;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.util.ThreadUtils;

import java.util.EnumSet;

/**
 * Copyright © Cliqz 2018
 */
public class TopSitesContextMenu extends PopupMenu implements PopupMenu.OnMenuItemClickListener {

    private WebpageModel mTopSite;
    private HomePager.OnUrlOpenInBackgroundListener mOnUrlOpenInBackgroundListener;
    private Context mContext;

    private TopSitesContextMenu(View anchorView) {
        super(anchorView.getContext(), anchorView);
    }

    public static void show(View anchorView, WebpageModel topSite,
                       HomePager.OnUrlOpenInBackgroundListener onUrlOpenInBackgroundListener) {
        final TopSitesContextMenu menu = new TopSitesContextMenu(anchorView);
        menu.mTopSite = topSite;
        menu.mOnUrlOpenInBackgroundListener = onUrlOpenInBackgroundListener;
        menu.mContext = anchorView.getContext();
        final MenuInflater inflater = menu.getMenuInflater();
        inflater.inflate(R.menu.top_sites_context_menu, menu.getMenu());
        menu.setOnMenuItemClickListener(menu);
        final MenuItem pinItem = menu.getMenu().findItem(R.id.pin_site);
        if (Boolean.TRUE.equals(menu.mTopSite.isPinned())) {
            pinItem.setTitle(R.string.contextmenu_top_sites_unpin);
        }
        menu.show();
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.open_new_tab:
                mOnUrlOpenInBackgroundListener.onUrlOpenInBackgroundWithReferrer(mTopSite.getUrl(), null,
                        EnumSet.noneOf(HomePager.OnUrlOpenInBackgroundListener.Flags.class));
                return true;
            case R.id.open_ghost_tab:
                mOnUrlOpenInBackgroundListener.onUrlOpenInBackgroundWithReferrer(mTopSite.getUrl(), null,
                        EnumSet.of(HomePager.OnUrlOpenInBackgroundListener.Flags.PRIVATE));
                return true;
            case R.id.pin_site:
                ThreadUtils.postToBackgroundThread(new Runnable() {
                    @Override
                    public void run() {
                        final BrowserDB db = BrowserDB.from(mContext);

                        if (Boolean.TRUE.equals(mTopSite.isPinned())) {
                            db.unpinSiteForAS(mContext.getContentResolver(), mTopSite.getUrl());
                        } else {
                            db.pinSiteForAS(mContext.getContentResolver(), mTopSite.getUrl(), mTopSite.getTitle());
                        }
                        mTopSite.onStateCommitted();
                    }
                });
                return true;
            case R.id.add_page_shortcut:
                ThreadUtils.postToBackgroundThread(new Runnable() {
                    @Override
                    public void run() {
                        GeckoApplication.createBrowserShortcut(mTopSite.getTitle(), mTopSite.getUrl());
                    }
                });
                return true;
            case R.id.delete:
                ThreadUtils.postToBackgroundThread(new Runnable() {
                    @Override
                    public void run() {
                        final BrowserDB db = BrowserDB.from(mContext);
                        if (Boolean.TRUE.equals(mTopSite.isPinned())) {
                            db.unpinSiteForAS(mContext.getContentResolver(), mTopSite.getUrl());
                        }
                        db.removeHistoryEntry(mContext.getContentResolver(), mTopSite.getUrl());
                    }
                });
                return true;
            default:
                return false;
        }
    }
}
