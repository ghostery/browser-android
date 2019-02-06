package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.support.v7.widget.RecyclerView;
import android.view.View;

import org.mozilla.gecko.R;
import org.mozilla.gecko.widget.themed.ThemedTextView;

/**
 * Copyright © Cliqz 2018
 */
class TopNewsHeader extends RecyclerView.ViewHolder {

    private final ThemedTextView mToggleNewsText;
    private final ThemedTextView mNewsTitleText;

    TopNewsHeader(View itemView) {
        super(itemView);
        mToggleNewsText = (ThemedTextView) itemView.findViewById(R.id.news_display_toggle);
        mNewsTitleText = (ThemedTextView) itemView.findViewById(R.id.news_title);
    }

    void toggleHeaderText(boolean isNewsExpanded) {
        if (isNewsExpanded) {
            mToggleNewsText.setText(R.string.activity_stream_top_news_hide);
        } else {
            mToggleNewsText.setText(R.string.activity_stream_top_news_show);
        }
    }

    void setLightTheme(boolean isLightTheme) {
        mToggleNewsText.setLightTheme(isLightTheme);
        mNewsTitleText.setLightTheme(isLightTheme);
    }

}
