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

    private final TopNewsAdapter mTopNewsAdapter;

    private final PreferenceManager mPreferenceManager;

    public TopNewsRow(final View itemView, HomePager.OnUrlOpenListener onUrlOpenListener) {
        super(itemView);
        final Context context = itemView.getContext();

        mPreferenceManager = PreferenceManager.getInstance(context);

        final RecyclerView newsRView = (RecyclerView) itemView.findViewById(R.id.news_recyclerview);
        mTopNewsAdapter = new TopNewsAdapter(context, onUrlOpenListener);
        newsRView.setAdapter(mTopNewsAdapter);
        RecyclerViewClickSupport.addTo(newsRView).setOnItemClickListener(mTopNewsAdapter);
    }

    public void bind(List<TopNews> topNews) {
        mTopNewsAdapter.swap(topNews);
    }

    public void updateTheme() {
        final boolean isLightTheme = mPreferenceManager.isLightThemeEnabled();
        ((ThemedRoundedCornerLayout) itemView).setLightTheme(isLightTheme);
        mTopNewsAdapter.setLightTheme(isLightTheme);
    }

}
