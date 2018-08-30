package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.stream.TopNewsRow;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.preferences.GeckoPreferences;
import org.mozilla.gecko.util.StringUtils;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;

import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class TopNewsAdapter extends RecyclerView.Adapter<TopNewsCard> implements RecyclerViewClickSupport.OnItemClickListener {

    private final Context context;
    private final HomePager.OnUrlOpenListener onUrlOpenListener;
    private List<TopNews> topNews;
    private int numShowedNews = 0;

    public TopNewsAdapter(Context context, final HomePager.OnUrlOpenListener onUrlOpenListener) {
        this.context = context;
        this.onUrlOpenListener = onUrlOpenListener;
        topNews = new ArrayList<>();
    }

    public int getTopNewsCount() {
        return topNews.size();
    }

    public void setNumShowedNews(int numShowedNews) {
        this.numShowedNews = numShowedNews;
    }

    @Override
    public void onItemClicked(RecyclerView recyclerView, int position, View v) {
        final TopNews newsCard = topNews.get(position);

        onUrlOpenListener.onUrlOpen(StringUtils.decodeUserEnteredUrl(newsCard.getUrl()), EnumSet.noneOf
                (HomePager.OnUrlOpenListener.Flags.class));
    }

    public void clear() {
        topNews.clear();
        notifyDataSetChanged();
    }

    public void add(List<TopNews> data) {
        final SharedPreferences preferences = GeckoSharedPrefs.forProfile(context);
        boolean isNewsExpanded = preferences.getBoolean(GeckoPreferences.PREFS_CLIQZ_TAB_NEWS_EXPANDED,
                TopNewsRow.PREFS_DEFAULT_NEWS_VIEW_EXPANDED);
        if (isNewsExpanded) {
            numShowedNews = data.size();
        } else {
            numShowedNews = Math.min(TopNewsRow.COLLAPSED_NUM_NEWS_COUNT, data.size());
        }
        topNews.addAll(data);
        notifyDataSetChanged();
    }

    public void swap(List<TopNews> data) {
        clear();
        add(data);
    }

    @Override
    public void onBindViewHolder(TopNewsCard holder, int position) {
        holder.bind(topNews.get(position));
    }

    @Override
    public TopNewsCard onCreateViewHolder(ViewGroup parent, int viewType) {
        final LayoutInflater inflater = LayoutInflater.from(context);
        final View card = inflater.inflate(R.layout.activity_stream_news_card,
                parent, false);
        return new TopNewsCard(card);
    }

    @Override
    public int getItemCount() {
        return numShowedNews;
    }
}
