

package org.mozilla.gecko.home;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.mozilla.gecko.R;

/**
 * Copyright © Cliqz 2018
 */
public class BookmarkFolderTitleView extends LinearLayout {

    private final TextView mTitle;

    public BookmarkFolderTitleView(Context context) {
        this(context, null);
    }

    public BookmarkFolderTitleView(Context context, AttributeSet attrs) {
        super(context, attrs);

        LayoutInflater.from(context).inflate(R.layout.bookmark_current_folder_row, this);

        mTitle = (TextView) findViewById(R.id.title);
    }

    public void setTitle(String title) {
        mTitle.setText(title);
    }

}
