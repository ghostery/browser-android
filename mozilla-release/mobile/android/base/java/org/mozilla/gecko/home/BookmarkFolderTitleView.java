

package org.mozilla.gecko.home;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;

import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.widget.themed.ThemedImageView;
import org.mozilla.gecko.widget.themed.ThemedLinearLayout;
import org.mozilla.gecko.widget.themed.ThemedTextView;

/**
 * Copyright © Cliqz 2018
 */
public class BookmarkFolderTitleView extends ThemedLinearLayout {

    private final ThemedTextView title;
    private final ThemedImageView icon;

    public BookmarkFolderTitleView(Context context) {
        this(context, null);
    }

    @SuppressWarnings("RedundantCast")
    public BookmarkFolderTitleView(Context context, AttributeSet attrs) {
        super(context, attrs);

        LayoutInflater.from(context).inflate(R.layout.bookmark_current_folder_row, this);

        title = (ThemedTextView) findViewById(R.id.title);
        icon = (ThemedImageView) findViewById(R.id.icon);
    }

    public void setTitle(String title) {
        this.title.setText(title);
    }

    /* Cliqz Start */
    public void updateTheme() {
        setLightTheme(PreferenceManager.getInstance().isLightThemeEnabled());
    }

    @Override
    public void setLightTheme(boolean isLightTheme) {
        super.setLightTheme(isLightTheme);
        title.setLightTheme(isLightTheme);
        icon.setLightTheme(isLightTheme);
    }
    /* Cliqz End */

}
