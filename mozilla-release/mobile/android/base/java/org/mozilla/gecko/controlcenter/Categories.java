package org.mozilla.gecko.controlcenter;

import android.support.annotation.ColorRes;
import android.support.annotation.DrawableRes;

import org.mozilla.gecko.R;

/**
 * Copyright © Cliqz 2018
 */
public enum Categories {

    advertising(R.color.cc_advertising, R.color.cc_advertising_disabled,
            R.color.cc_advertising_blocked, R.drawable.cc_ic_category_advertising),
    audio_video_player(R.color.cc_audio_video_player, R.color.cc_audio_video_player_disabled,
            R.color.cc_audio_video_player_blocked, R.drawable.cc_ic_category_audio_video),
    comments(R.color.cc_comments, R.color.cc_comments_disabled,
            R.color.cc_comments_blocked, R.drawable.cc_ic_category_comments),
    customer_interaction(R.color.cc_customer_interaction, R.color.cc_customer_interaction_disabled,
            R.color.cc_customer_interaction_blocked, R.drawable.cc_ic_category_customer_interaction),
    essential(R.color.cc_essential, R.color.cc_essential_disabled,
            R.color.cc_essential_blocked, R.drawable.cc_ic_category_essential),
    pornvertising(R.color.cc_pornvertising, R.color.cc_pornvertising_disabled,
            R.color.cc_pornvertising_blocked, R.drawable.cc_ic_category_pornvertising),
    site_analytics(R.color.cc_site_analytics, R.color.cc_site_analytics_disabled,
            R.color.cc_site_analytics_blocked, R.drawable.cc_ic_category_site_analytics),
    social_media(R.color.cc_social_media, R.color.cc_social_media_disabled,
            R.color.cc_social_media_blocked, R.drawable.cc_ic_category_social_media),
    unknown(R.color.cc_default_category, R.color.cc_default_category_disabled,
            R.color.cc_default_category_blocked, R.drawable.cc_ic_category_advertising); //TODO get icon for unknown

    public static Categories safeValueOf(String value) {
        try {
            return Categories.valueOf(value);
        } catch (IllegalArgumentException e) {
            return unknown;
        }
    }

    public final @ColorRes int categoryColor;
    public final @ColorRes int categoryColorDisabled;
    public final @ColorRes int categoryColorBlocked;
    public final @DrawableRes int categoryIcon;

    Categories(@ColorRes int color, @ColorRes int colorDisabled, @ColorRes int colorBlocked, @DrawableRes int icon) {
        categoryColor = color;
        categoryColorDisabled = colorDisabled;
        categoryColorBlocked = colorBlocked;
        categoryIcon = icon;
    }
}
