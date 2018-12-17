package org.mozilla.gecko.activitystream.homepanel.stream;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.topnews.TopNewsAdapter;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;

import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class TopNewsRow extends StreamViewHolder {

    public static final int LAYOUT_ID = R.layout.activity_stream_main_topnews;

    private TopNewsAdapter adapter;

    public TopNewsRow(final View itemView, HomePager.OnUrlOpenListener onUrlOpenListener) {
        super(itemView);
        final Context context = itemView.getContext();

        final RecyclerView newsRView = (RecyclerView) itemView.findViewById(R.id.news_recyclerview);
        adapter = new TopNewsAdapter(context, onUrlOpenListener);
        newsRView.setAdapter(adapter);
        RecyclerViewClickSupport.addTo(newsRView).setOnItemClickListener(adapter);
    }

    public void bind(List<TopNews> topNews) {
        adapter.swap(topNews);
    }

}
