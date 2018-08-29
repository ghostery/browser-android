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
import android.widget.TextView;

import com.squareup.picasso.Picasso;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.cliqzicons.CliqzLogoUtil;

/**
 * Cliqz 2018
 * This file is derived from @{@link org.mozilla.gecko.activitystream.homepanel.topsites.TopSitesCard}.java
 */

public class TopNewsCard extends RecyclerView.ViewHolder {

    private final ImageView faviconView;
    private final TextView titleView;
    private final TextView urlView;


    private Context context;

    /* package-local */ TopNewsCard(final View card) {
        super(card);
        context = card.getContext();
        faviconView = (ImageView) card.findViewById(R.id.favicon);
        titleView = (TextView) card.findViewById(R.id.title_view);
        urlView = (TextView) card.findViewById(R.id.url_view);
    }

    void bind(final TopNews topNews) {
        final int newsFavIconSize = context.getResources().getDimensionPixelSize(R.dimen.news_favicon_size);
        Picasso.with(context)
                .load(CliqzLogoUtil.getIconUrl(topNews.getUrl(), newsFavIconSize, newsFavIconSize))
                .error(CliqzLogoUtil.getDefaultIcon(topNews.getUrl(), newsFavIconSize, newsFavIconSize))
                .into(faviconView);
        titleView.setText(buildTitleSpannable(topNews));
        urlView.setText(topNews.getDomain());
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

}
