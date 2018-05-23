/* -*- Mode: Java; c-basic-offset: 4; tab-width: 20; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
package org.mozilla.gecko.home;

import android.content.res.Resources;
import android.database.Cursor;
import android.support.annotation.UiThread;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.db.BrowserContract;
import org.mozilla.gecko.util.ThreadUtils;

/*
 This is a simplified version on the CombinedHistoryAdapter. We do not need the synced devices and
 the recent closed tab.
 */
public class CliqzSimplifiedHistoryAdapter extends RecyclerView.Adapter<CombinedHistoryItem> implements CombinedHistoryRecyclerView.AdapterContextMenuBuilder {

    // Array for the time ranges in milliseconds covered by each section.
    static final HistorySectionsHelper.SectionDateRange[] sectionDateRangeArray = new HistorySectionsHelper.SectionDateRange[SectionHeader.values().length];

    // Semantic names for the time covered by each section
    public enum SectionHeader {
        TODAY,
        YESTERDAY,
        WEEK,
        THIS_MONTH,
        MONTH_AGO,
        TWO_MONTHS_AGO,
        THREE_MONTHS_AGO,
        FOUR_MONTHS_AGO,
        FIVE_MONTHS_AGO,
        OLDER_THAN_SIX_MONTHS
    }

    private HomeFragment.PanelStateChangeListener panelStateChangeListener;

    private Cursor historyCursor;

    private LinearLayoutManager linearLayoutManager; // Only used on the UI thread, so no need to be volatile.

    // We use a sparse array to store each section header's position in the panel [more cheaply than a HashMap].
    private final SparseArray<SectionHeader> sectionHeaders;

    public CliqzSimplifiedHistoryAdapter(Resources resources, int cachedRecentTabsCount) {
        super();
        sectionHeaders = new SparseArray<>();
        HistorySectionsHelper.updateRecentSectionOffset(resources, sectionDateRangeArray);
        this.setHasStableIds(true);
    }

    public void setPanelStateChangeListener(
            HomeFragment.PanelStateChangeListener panelStateChangeListener) {
        this.panelStateChangeListener = panelStateChangeListener;
    }

    @UiThread
    public void setLinearLayoutManager(LinearLayoutManager linearLayoutManager) {
        this.linearLayoutManager = linearLayoutManager;
    }

    public void setHistory(Cursor history) {
        historyCursor = history;
        populateSectionHeaders(historyCursor, sectionHeaders);
        notifyDataSetChanged();
    }

    @Override
    public CombinedHistoryItem onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        final LayoutInflater inflater = LayoutInflater.from(viewGroup.getContext());
        final View view;

        final CombinedHistoryItem.ItemType itemType = CombinedHistoryItem.ItemType.viewTypeToItemType(viewType);

        switch (itemType) {
            case SECTION_HEADER:
                view = inflater.inflate(R.layout.home_header_row, viewGroup, false);
                return new CombinedHistoryItem.HeaderItem(view);

            case HISTORY:
                view = inflater.inflate(R.layout.home_item_row, viewGroup, false);
                return new CombinedHistoryItem.HistoryItem(view);
            default:
                throw new IllegalArgumentException("Unexpected Home Panel item type");
        }
    }

    @Override
    public void onBindViewHolder(CombinedHistoryItem viewHolder, int position) {
        final CombinedHistoryItem.ItemType itemType = getItemTypeForPosition(position);
        final int localPosition = transformAdapterPositionForDataStructure(itemType, position);

        switch (itemType) {
            case SECTION_HEADER:
                ((CombinedHistoryItem.HeaderItem) viewHolder).textView.setText(getSectionHeaderTitle(sectionHeaders.get(localPosition)));
                break;

            case HISTORY:
                if (historyCursor == null || !historyCursor.moveToPosition(localPosition)) {
                    throw new IllegalStateException("Couldn't move cursor to position " + localPosition);
                }
                ((CombinedHistoryItem.HistoryItem) viewHolder).bind(historyCursor);
                break;
        }
    }

    /**
     * Transform an adapter position to the position for the data structure backing the item type.
     *
     * The type is not strictly necessary and could be fetched from <code>getItemTypeForPosition</code>,
     * but is used for explicitness.
     *
     * @param type ItemType of the item
     * @param position position in the adapter
     * @return position of the item in the data structure
     */
    @UiThread
    private int transformAdapterPositionForDataStructure(CombinedHistoryItem.ItemType type, int position) {
        if (type == CombinedHistoryItem.ItemType.HISTORY) {
            return position - getHeadersBefore(position);
        } else {
            return position;
        }
    }

    @UiThread
    private CombinedHistoryItem.ItemType getItemTypeForPosition(int position) {
        final int sectionPosition = transformAdapterPositionForDataStructure(CombinedHistoryItem.ItemType.SECTION_HEADER, position);
        if (sectionHeaders.get(sectionPosition) != null) {
            return CombinedHistoryItem.ItemType.SECTION_HEADER;
        }
        return CombinedHistoryItem.ItemType.HISTORY;
    }

    @UiThread
    @Override
    public int getItemViewType(int position) {
        return CombinedHistoryItem.ItemType.itemTypeToViewType(getItemTypeForPosition(position));
    }

    @UiThread
    @Override
    public int getItemCount() {
        final int historySize = historyCursor == null ? 0 : historyCursor.getCount();
        return historySize + sectionHeaders.size();
    }

    /**
     * Returns stable ID for each position. Data behind historyCursor is a sorted Combined view.
     *
     * @param position view item position for which to generate a stable ID
     * @return stable ID for given position
     */
    @UiThread
    @Override
    public long getItemId(int position) {
        // Two randomly selected large primes used to generate non-clashing IDs.
        final long PRIME_BOOKMARKS = 32416189867L;
        final long PRIME_SECTION_HEADERS = 32416187737L;

        // RecyclerView.NO_ID is -1, so let's start from -2 for our hard-coded IDs.
        final int RECENT_TABS_ID = -2;
        final int SYNCED_DEVICES_ID = -3;

        switch (getItemTypeForPosition(position)) {
            case SECTION_HEADER:
                // We might have multiple section headers, so we try get unique IDs for them.
                return position * PRIME_SECTION_HEADERS;
            case HISTORY:
                final int historyPosition = transformAdapterPositionForDataStructure(
                        CombinedHistoryItem.ItemType.HISTORY, position);
                if (!historyCursor.moveToPosition(historyPosition)) {
                    return RecyclerView.NO_ID;
                }

                final int historyIdCol = historyCursor.getColumnIndexOrThrow(BrowserContract.Combined.HISTORY_ID);
                final long historyId = historyCursor.getLong(historyIdCol);

                if (historyId != -1) {
                    return historyId;
                }

                final int bookmarkIdCol = historyCursor.getColumnIndexOrThrow(BrowserContract.Combined.BOOKMARK_ID);
                final long bookmarkId = historyCursor.getLong(bookmarkIdCol);

                // Avoid clashing with historyId.
                return bookmarkId * PRIME_BOOKMARKS;
            default:
                throw new IllegalStateException("Unexpected Home Panel item type");
        }
    }

    /**
     * Add only the SectionHeaders that have history items within their range to a SparseArray, where the
     * array index is the position of the header in the history-only (no clients) ordering.
     * @param c data Cursor
     * @param sparseArray SparseArray to populate
     */
    @UiThread
    private void populateSectionHeaders(Cursor c, SparseArray<SectionHeader> sparseArray) {
        ThreadUtils.assertOnUiThread();

        sparseArray.clear();

        if (c == null || !c.moveToFirst()) {
            return;
        }

        SectionHeader section = null;

        do {
            final int historyPosition = c.getPosition();
            final long visitTime = c.getLong(c.getColumnIndexOrThrow(BrowserContract.History.DATE_LAST_VISITED));
            final SectionHeader itemSection = getSectionFromTime(visitTime);

            if (section != itemSection) {
                section = itemSection;
                sparseArray.append(historyPosition + sparseArray.size(), section);
            }

            if (section == SectionHeader.OLDER_THAN_SIX_MONTHS) {
                break;
            }
        } while (c.moveToNext());
    }

    private static String getSectionHeaderTitle(SectionHeader section) {
        return sectionDateRangeArray[section.ordinal()].displayName;
    }

    private static SectionHeader getSectionFromTime(long time) {
        for (int i = 0; i < SectionHeader.OLDER_THAN_SIX_MONTHS.ordinal(); i++) {
            if (time > sectionDateRangeArray[i].start) {
                return SectionHeader.values()[i];
            }
        }

        return SectionHeader.OLDER_THAN_SIX_MONTHS;
    }

    /**
     * Returns the number of section headers before the given history item at the adapter position.
     * @param position position in the adapter
     */
    private int getHeadersBefore(int position) {
        // Skip the first header case because there will always be a header.
        for (int i = 1; i < sectionHeaders.size(); i++) {
            // If the position of the header is greater than the history position,
            // return the number of headers tested.
            if (sectionHeaders.keyAt(i) > position) {
                return i;
            }
        }
        return sectionHeaders.size();
    }

    @Override
    public HomeContextMenuInfo makeContextMenuInfoFromPosition(View view, int position) {
        final CombinedHistoryItem.ItemType itemType = getItemTypeForPosition(position);
        if (itemType == CombinedHistoryItem.ItemType.HISTORY) {
            final HomeContextMenuInfo info = new HomeContextMenuInfo(view, position, -1);

            historyCursor.moveToPosition(transformAdapterPositionForDataStructure(CombinedHistoryItem.ItemType.HISTORY, position));
            return populateHistoryInfoFromCursor(info, historyCursor);
        }
        return null;
    }

    protected static HomeContextMenuInfo populateHistoryInfoFromCursor(HomeContextMenuInfo info, Cursor cursor) {
        info.url = cursor.getString(cursor.getColumnIndexOrThrow(BrowserContract.Combined.URL));
        info.title = cursor.getString(cursor.getColumnIndexOrThrow(BrowserContract.Combined.TITLE));
        info.historyId = cursor.getInt(cursor.getColumnIndexOrThrow(BrowserContract.Combined.HISTORY_ID));
        info.itemType = HomeContextMenuInfo.RemoveItemType.HISTORY;
        final int bookmarkIdCol = cursor.getColumnIndexOrThrow(BrowserContract.Combined.BOOKMARK_ID);
        if (cursor.isNull(bookmarkIdCol)) {
            // If this is a combined cursor, we may get a history item without a
            // bookmark, in which case the bookmarks ID column value will be null.
            info.bookmarkId =  -1;
        } else {
            info.bookmarkId = cursor.getInt(bookmarkIdCol);
        }
        return info;
    }

}
