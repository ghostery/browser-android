package org.mozilla.gecko.controlcenter;

import android.support.annotation.ColorRes;
import android.support.annotation.DrawableRes;

import org.mozilla.gecko.R;

/**
 * Copyright © Cliqz 2018
 */
public enum Categories {

    advertising(R.color.advertising, R.drawable.ic_category_advertising),
    audio_video_player(R.color.audio_video_player, R.drawable.ic_category_audio_video),
    comments(R.color.comments, R.drawable.ic_category_comments),
    customer_interaction(R.color.customer_interaction, R.drawable.ic_category_customer_interaction),
    essential(R.color.essential, R.drawable.ic_category_essential),
    pornvertising(R.color.pornvertising, R.drawable.ic_category_pornvertising),
    site_analytics(R.color.site_analytics, R.drawable.ic_category_site_analytics),
    social_media(R.color.social_media, R.drawable.ic_category_social_media),
    unknown(R.color.default_category, R.drawable.ic_category_pornvertising); //TODO get icon for unknown

    public static Categories safeValueOf(String value) {
        try {
            return Categories.valueOf(value);
        } catch (IllegalArgumentException e) {
            return unknown;
        }
    }

    public final @ColorRes int categoryColor;
    public final @DrawableRes int categoryIcon;

    Categories(@ColorRes int color, @DrawableRes int icon) {
        categoryColor = color;
        categoryIcon = icon;
    }
}
