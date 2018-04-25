package org.mozilla.gecko.db;

/**
 * Copyright © Cliqz 2018
 */
public class GhosteryHistoryItem {

    protected final String url;
    protected final String title;

    GhosteryHistoryItem(String url, String title) {
        this.url = url;
        this.title = title;
    }

    @Override
    public int hashCode() {
        return (url+title).hashCode();
    }

    @Override
    public boolean equals(Object o) {
        return o instanceof GhosteryHistoryItem &&
                ((GhosteryHistoryItem) o).url.equals(this.url) &&
                ((GhosteryHistoryItem) o).title.equals(this.title);
    }
}
