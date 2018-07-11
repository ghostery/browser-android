package org.mozilla.gecko.activitystream.homepanel.stream;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.activitystream.homepanel.topnews.TopNewsAdapter;
import org.mozilla.gecko.home.HomePager;
import org.mozilla.gecko.widget.RecyclerViewClickSupport;

import java.util.List;

import static android.support.v4.content.ContextCompat.getDrawable;

/**
 * Copyright © Cliqz 2018
 */

public class TopNewsRow extends StreamViewHolder {

    public static final int LAYOUT_ID = R.layout.activity_stream_main_topnews;
    private final Drawable expandNewsIcon;
    private final Drawable collapseNewsIcon;
    private TextView title;
    private TopNewsAdapter adapter;

    // By default, the 'Top News' list is expanded i.e. it shows all the top news.
    private boolean isNewsExpanded = true;

    // Number of items to show in the 'Top News' list when the list is collapsed.
    private static final int COLLAPSED_NUM_NEWS_COUNT = 3;

    public TopNewsRow(final View itemView, HomePager.OnUrlOpenListener onUrlOpenListener) {
        super(itemView);
        final Context context = itemView.getContext();
        expandNewsIcon = getDrawable(context, R.drawable
                .ic_action_expand);
        collapseNewsIcon = getDrawable(context, R.drawable.ic_action_collapse);

        title = (TextView) itemView.findViewById(R.id.news_title);
        title.setCompoundDrawablesWithIntrinsicBounds(null, null, collapseNewsIcon, null);

        final RecyclerView newsRView = (RecyclerView) itemView.findViewById(R.id.news_recyclerview);
        adapter = new TopNewsAdapter(context, onUrlOpenListener);
        newsRView.setAdapter(adapter);
        RecyclerViewClickSupport.addTo(newsRView).setOnItemClickListener(adapter);

        title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isNewsExpanded = !isNewsExpanded;
                final int newsCount = adapter.getTopNewsCount();
                final int changedCount = newsCount - COLLAPSED_NUM_NEWS_COUNT;
                if (isNewsExpanded) {
                    title.setCompoundDrawablesWithIntrinsicBounds(null, null,
                            collapseNewsIcon, null);
                    adapter.setNumShowedNews(newsCount);
                    if (changedCount > 0) {
                        adapter.notifyItemRangeInserted(COLLAPSED_NUM_NEWS_COUNT, changedCount);
                    }
                } else {
                    title.setCompoundDrawablesWithIntrinsicBounds(null, null,
                            expandNewsIcon, null);
                    adapter.setNumShowedNews(Math.min(newsCount, COLLAPSED_NUM_NEWS_COUNT));
                    if (changedCount > 0) {
                        adapter.notifyItemRangeRemoved(COLLAPSED_NUM_NEWS_COUNT, changedCount);
                    }
                }
            }
        });
    }

    public void bind(List<TopNews> topNews) {
        adapter.swap(topNews);
    }
}
