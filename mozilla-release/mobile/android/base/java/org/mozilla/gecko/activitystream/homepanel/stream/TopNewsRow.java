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
 * @author Moaz Rashad
 */

public class TopNewsRow extends StreamViewHolder {
    public static final int LAYOUT_ID = R.layout.activity_stream_main_news;
    private final Drawable expandNewsIcon, collapseNewsIcon;
    TextView title;
    RecyclerView newsRView;
    TopNewsAdapter adapter;
    private boolean isNewsExpanded = false;

    public TopNewsRow(final View itemView, HomePager.OnUrlOpenListener onUrlOpenListener) {
        super(itemView);
        final Context context = itemView.getContext();
        expandNewsIcon = getDrawable(context, R.drawable
                .ic_action_expand);
        collapseNewsIcon = getDrawable(context, R.drawable.ic_action_collapse);

        title = (TextView) itemView.findViewById(R.id.news_title);
        title.setCompoundDrawablesWithIntrinsicBounds(null, null,
                expandNewsIcon, null);

        newsRView = (RecyclerView) itemView.findViewById(R.id.news_rview);
        adapter = new TopNewsAdapter(context,onUrlOpenListener);
        newsRView.setAdapter(adapter);
        RecyclerViewClickSupport.addTo(newsRView).setOnItemClickListener(adapter);

        title.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isNewsExpanded = !isNewsExpanded;
                final int newsCount = adapter.getTopNewsCount();
                if (isNewsExpanded) {
                    title.setCompoundDrawablesWithIntrinsicBounds(null, null,
                            collapseNewsIcon, null);
                    adapter.setNumShowedNews(newsCount);
                    adapter.notifyItemRangeInserted(adapter.INIT_NUM_SHOWED_NEWS,newsCount-adapter
                            .INIT_NUM_SHOWED_NEWS);
                } else {
                    title.setCompoundDrawablesWithIntrinsicBounds(null, null,
                            expandNewsIcon, null);
                    adapter.setNumShowedNews(adapter.INIT_NUM_SHOWED_NEWS);
                    adapter.notifyItemRangeRemoved(adapter.INIT_NUM_SHOWED_NEWS,newsCount-adapter.INIT_NUM_SHOWED_NEWS);
                }
            }
        });
    }

    public void bind(List<TopNews> topNews) {
        adapter.swap(topNews);
    }
}
