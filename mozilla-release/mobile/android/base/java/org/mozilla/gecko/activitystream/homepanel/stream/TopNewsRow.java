package org.mozilla.gecko.activitystream.homepanel.stream;

import android.view.View;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;

/**
 * @author Moaz Rashad
 */

public class TopNewsRow extends StreamViewHolder {

    public static final int LAYOUT_ID = R.layout.activity_stream_news_card;
    TextView title;
    public TopNewsRow(final View itemView) {
        super(itemView);
        title = (TextView) itemView.findViewById(R.id.title);
    }

    public void bind(TopNews topNews) {
        title.setText(topNews.getTitle());
    }
}

