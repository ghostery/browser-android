package org.mozilla.gecko.activitystream.homepanel.stream;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.topnews.TopNewsAdapter;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;
import org.mozilla.gecko.widget.ThemedRoundedCornerLayout;

import java.util.List;

/**
 * Copyright © Cliqz 2018
 */
public class TopNewsRow extends StreamViewHolder {

    public static final int LAYOUT_ID = R.layout.activity_stream_main_topnews;

    private final TopNewsAdapter adapter;

    private final PreferenceManager preferenceManager;

    public TopNewsRow(final View itemView, HomePager.OnUrlOpenListener onUrlOpenListener) {
        super(itemView);
        final Context context = itemView.getContext();

        preferenceManager = PreferenceManager.getInstance(context);

        final RecyclerView newsRView = (RecyclerView) itemView.findViewById(R.id.news_recyclerview);
        adapter = new TopNewsAdapter(context, onUrlOpenListener);
        newsRView.setAdapter(adapter);
        RecyclerViewClickSupport.addTo(newsRView).setOnItemClickListener(adapter);
    }

    public void bind(List<TopNews> topNews) {
        adapter.swap(topNews);
    }

    public void setLightTheme() {
        final boolean isLightTheme = preferenceManager.isLightThemeEnabled();
        ((ThemedRoundedCornerLayout) itemView).setLightTheme(isLightTheme);
        adapter.setLightTheme(isLightTheme);
    }

}
