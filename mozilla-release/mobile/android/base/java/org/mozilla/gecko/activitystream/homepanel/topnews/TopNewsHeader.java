package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.TextView;

import org.mozilla.gecko.R;

/**
 * Copyright © Cliqz 2018
 */
class TopNewsHeader extends RecyclerView.ViewHolder {

    private final TextView toggleNewsText;

    TopNewsHeader(View itemView) {
        super(itemView);
        toggleNewsText = (TextView) itemView.findViewById(R.id.news_display_toggle);
    }

    void toggleHeaderText(boolean isNewsExpanded) {
        if (isNewsExpanded) {
            toggleNewsText.setText("Hide");
        } else {
            toggleNewsText.setText("Show");
        }
    }

}
