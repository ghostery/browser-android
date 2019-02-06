package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.StringUtils;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;

import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class TopNewsAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder>
        implements RecyclerViewClickSupport.OnItemClickListener {

    private static final int TYPE_HEADER = 0;
    private static final int TYPE_NEWS = 1;

    // Number of items to show in the 'Top News' list when the list is collapsed.
    private static final int COLLAPSED_NUM_NEWS_COUNT = 0;

    private final Context context;
    private final HomePager.OnUrlOpenListener onUrlOpenListener;
    private final PreferenceManager preferenceManager;

    private final List<TopNews> topNews;
    private int numShowedNews = 0;
    private boolean isNewsExpanded;

    private boolean mIsLightTheme;

    public TopNewsAdapter(Context context, final HomePager.OnUrlOpenListener onUrlOpenListener) {
        this.context = context;
        this.onUrlOpenListener = onUrlOpenListener;
        this.preferenceManager = PreferenceManager.getInstance(context);
        topNews = new ArrayList<>();
    }

    @Override
    public void onItemClicked(RecyclerView recyclerView, int position, View v) {
        if (position == 0) {
            final RecyclerView.ViewHolder viewHolder = recyclerView.findViewHolderForAdapterPosition(position);
            if (viewHolder != null) {
                isNewsExpanded = !isNewsExpanded;
                ((TopNewsHeader)viewHolder).toggleHeaderText(isNewsExpanded);
                toggleNews();
            }
        } else {
            final TopNews newsCard = topNews.get(position - 1);
            onUrlOpenListener.onUrlOpen(StringUtils.decodeUserEnteredUrl(newsCard.getUrl()), EnumSet.noneOf
                    (HomePager.OnUrlOpenListener.Flags.class));
        }
    }

    public void clear() {
        topNews.clear();
        notifyDataSetChanged();
    }

    public void add(List<TopNews> data) {
        numShowedNews = isNewsExpanded ? data.size() : Math.min(COLLAPSED_NUM_NEWS_COUNT, data.size());
        topNews.addAll(data);
        notifyDataSetChanged();
    }

    public void swap(List<TopNews> data) {
        clear();
        add(data);
    }

    private void toggleNews() {
        final int changedCount = topNews.size() - COLLAPSED_NUM_NEWS_COUNT;
        if (isNewsExpanded) {
            this.numShowedNews = topNews.size();
            if (changedCount > 0) {
                notifyItemRangeInserted(COLLAPSED_NUM_NEWS_COUNT + 1, changedCount);
            }
        } else {
            this.numShowedNews = Math.min(topNews.size(), COLLAPSED_NUM_NEWS_COUNT);
            if (changedCount > 0) {
                notifyItemRangeRemoved(COLLAPSED_NUM_NEWS_COUNT + 1, changedCount);
            }
        }
        preferenceManager.setNewsViewExpanded(isNewsExpanded);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        if (holder.getItemViewType() == TYPE_NEWS) {
            final TopNewsCard topNewsCard = (TopNewsCard) holder;
            topNewsCard.bind(topNews.get(position - 1));
            topNewsCard.setLightTheme(mIsLightTheme);
        } else if (holder.getItemViewType() == TYPE_HEADER) {
            final TopNewsHeader topNewsHeader = (TopNewsHeader) holder;
            topNewsHeader.toggleHeaderText(isNewsExpanded);
            topNewsHeader.setLightTheme(mIsLightTheme);
        }
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        final LayoutInflater inflater = LayoutInflater.from(context);
        final View view;
        if (viewType == TYPE_HEADER) {
            isNewsExpanded = preferenceManager.isNewsViewExpanded();
            view = inflater.inflate(R.layout.activity_stream_news_header, parent, false);
            return new TopNewsHeader(view);
        } else if (viewType == TYPE_NEWS) {
            view = inflater.inflate(R.layout.activity_stream_news_card, parent, false);
            return new TopNewsCard(view);
        }
        throw new RuntimeException("Incorrect view type " + viewType);
    }

    @Override
    public int getItemCount() {
        // Accounting for the header view.
        return numShowedNews + 1;
    }

    @Override
    public int getItemViewType(int position) {
        return position == 0 ? TYPE_HEADER : TYPE_NEWS;
    }

    public void setLightTheme(boolean isLightTheme) {
        this.mIsLightTheme = isLightTheme;
    }

}
