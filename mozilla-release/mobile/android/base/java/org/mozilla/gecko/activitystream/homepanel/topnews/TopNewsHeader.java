package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.support.v7.widget.RecyclerView;
import android.view.View;

import org.mozilla.gecko.R;
import org.mozilla.gecko.widget.themed.ThemedTextView;

/**
 * Copyright © Cliqz 2018
 */
class TopNewsHeader extends RecyclerView.ViewHolder {

    private final ThemedTextView toggleNewsText;
    private final ThemedTextView newsTitleText;

    TopNewsHeader(View itemView) {
        super(itemView);
        toggleNewsText = (ThemedTextView) itemView.findViewById(R.id.news_display_toggle);
        newsTitleText = (ThemedTextView) itemView.findViewById(R.id.news_title);
    }

    void toggleHeaderText(boolean isNewsExpanded) {
        if (isNewsExpanded) {
            toggleNewsText.setText(R.string.activity_stream_top_news_hide);
        } else {
            toggleNewsText.setText(R.string.activity_stream_top_news_show);
        }
    }

    void setLightTheme(boolean isLightTheme) {
        toggleNewsText.setLightTheme(isLightTheme);
        newsTitleText.setLightTheme(isLightTheme);
    }

}
