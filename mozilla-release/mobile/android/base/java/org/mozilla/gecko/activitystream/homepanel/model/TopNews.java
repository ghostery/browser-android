package org.mozilla.gecko.activitystream.homepanel.model;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import org.mozilla.gecko.activitystream.Utils;
import org.mozilla.gecko.activitystream.homepanel.StreamRecyclerAdapter;

/**
 *  Copyright © Cliqz 2018
 */

public class TopNews implements WebpageRowModel{
    private final String url;
    private final String title;
    private final String description;
    private final String domain;
    private final String shortTitle;
    private final String media;
    private final String breakingLabel;
    private final boolean breaking;
    private final boolean isLocalNews;
    private final String localLabel;

    public TopNews(String url, String title, String description, String domain, String shortTitle,
            String media, boolean breaking, String breakingLabel, boolean isLocalNews, String localLabel) {
        this.url = url;
        this.title = title;
        this.description = description;
        this.domain = domain;
        this.shortTitle = shortTitle;
        this.media = media;
        this.breaking = breaking;
        this.breakingLabel = breakingLabel;
        this.isLocalNews = isLocalNews;
        this.localLabel = localLabel;
    }

    public String getUrl() {
        return url;
    }

    @NonNull
    @Override
    public String getImageUrl() {
        return getUrl();
    }

    @Nullable
    @Override
    public Boolean isBookmarked() {
        return null;
    }

    @Nullable
    @Override
    public Boolean isPinned() {
        return null;
    }

    @Override
    public void updateBookmarked(boolean bookmarked) {

    }

    @Override
    public void updatePinned(boolean pinned) {

    }

    @Override
    public void onStateCommitted() {

    }

    public String getTitle() {
        return title;
    }

    public String getDescription() {
        return description;
    }

    public String getDomain() {
        return domain;
    }

    public String getShortTitle() {
        return shortTitle;
    }

    public String getMedia() {
        return media;
    }

    public String getBreakingLabel() {
        return breakingLabel;
    }

    public boolean isBreaking() {
        return breaking;
    }

    public boolean isLocalNews() {
        return isLocalNews;
    }

    public String getLocalLabel() {
        return localLabel;
    }

    @Override
    public StreamRecyclerAdapter.RowItemType getRowItemType() {
        return StreamRecyclerAdapter.RowItemType.TOP_NEWS;
    }

    @Override
    public Utils.HighlightSource getSource() {
        return null;
    }

    @Override
    public long getUniqueId() {
        return 0;
    }
}
