package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.graphics.Color;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.RecyclerView;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.ForegroundColorSpan;
import android.view.View;
import android.widget.ImageView;

import com.squareup.picasso.Picasso;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.cliqzicons.CliqzLogoUtil;
import org.mozilla.gecko.cliqzicons.RoundedCornersTransformation;
import org.mozilla.gecko.widget.themed.ThemedTextView;

/**
 * Cliqz 2018
 * This file is derived from @{@link org.mozilla.gecko.activitystream.homepanel.topsites.TopSitesCard}.java
 */

public class TopNewsCard extends RecyclerView.ViewHolder {

    private final ImageView mFaviconView;
    private final ThemedTextView mTitleView;
    private final ThemedTextView mUrlView;

    private Context context;

    /* package-local */ TopNewsCard(final View card) {
        super(card);
        context = card.getContext();
        mFaviconView = (ImageView) card.findViewById(R.id.favicon);
        mTitleView = (ThemedTextView) card.findViewById(R.id.title_view);
        mUrlView = (ThemedTextView) card.findViewById(R.id.url_view);
    }

    void bind(final TopNews topNews) {
        final int newsFavIconSize = context.getResources().getDimensionPixelSize(R.dimen.news_favicon_size);
        final int newsFavIconRadius = context.getResources().getDimensionPixelSize(R.dimen.news_item_favicon_radius);
        Picasso.with(context)
                .load(CliqzLogoUtil.getIconUrl(topNews.getUrl(), newsFavIconSize, newsFavIconSize))
                .transform(new RoundedCornersTransformation(newsFavIconRadius))
                .placeholder(
                        CliqzLogoUtil.getDefaultIcon(topNews.getUrl(), newsFavIconSize, newsFavIconSize))
                .into(mFaviconView);
        mTitleView.setText(buildTitleSpannable(topNews));
        mUrlView.setText(topNews.getDomain());
    }

    private CharSequence buildTitleSpannable(TopNews piece) {
        final SpannableStringBuilder builder = new SpannableStringBuilder();
        final String breakingLabel = piece.getBreakingLabel();
        final String localLabel = piece.getLocalLabel();
        if (piece.isBreaking() && breakingLabel != null && !breakingLabel.isEmpty()) {
            appendLabel(builder, breakingLabel.toUpperCase(), Color.RED);
        }
        if (piece.isLocalNews() && localLabel != null && !localLabel.isEmpty()) {
            final @ColorInt int color =
                    ContextCompat.getColor(context, R.color.news_local_span);
            appendLabel(builder, localLabel.toUpperCase(), color);
        }
        builder.append(piece.getTitle());
        return builder;
    }

    private void appendLabel(@NonNull SpannableStringBuilder builder, @NonNull String str,
                             @ColorInt int color) {
        final int oldLen = builder.length();
        builder.append(str).append(": ");
        builder.setSpan(new ForegroundColorSpan(color), oldLen, builder.length(),
                Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
    }

    void setLightTheme(boolean isLightTheme) {
        mTitleView.setLightTheme(isLightTheme);
        mUrlView.setLightTheme(isLightTheme);
    }

}
