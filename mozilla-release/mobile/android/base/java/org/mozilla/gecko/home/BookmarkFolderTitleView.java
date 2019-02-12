

package org.mozilla.gecko.home;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.widget.LinearLayout;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.widget.themed.ThemedImageView;
import org.mozilla.gecko.widget.themed.ThemedTextView;

/**
 * Copyright © Cliqz 2018
 */
public class BookmarkFolderTitleView extends LinearLayout {

    private final ThemedTextView mTitle;
    private final ThemedImageView mIcon;

    public BookmarkFolderTitleView(Context context) {
        this(context, null);
    }

    public BookmarkFolderTitleView(Context context, AttributeSet attrs) {
        super(context, attrs);

        LayoutInflater.from(context).inflate(R.layout.bookmark_current_folder_row, this);

        mTitle = (ThemedTextView) findViewById(R.id.title);
        mIcon = (ThemedImageView) findViewById(R.id.icon);
    }

    public void setTitle(String title) {
        mTitle.setText(title);
    }

    /* Cliqz Start */
    public void updateTheme() {
        setLightTheme(PreferenceManager.getInstance().isLightThemeEnabled());
    }

    public void setLightTheme(boolean isLightTheme) {
        mTitle.setLightTheme(isLightTheme);
        mIcon.setLightTheme(isLightTheme);
    }
    /* Cliqz End */

}
