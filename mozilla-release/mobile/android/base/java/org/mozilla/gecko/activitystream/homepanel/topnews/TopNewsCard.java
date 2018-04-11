package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.graphics.Color;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.RecyclerView;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.TextUtils;
import android.text.style.ForegroundColorSpan;
import android.widget.RelativeLayout;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.icons.IconCallback;
import org.mozilla.gecko.icons.IconResponse;
import org.mozilla.gecko.icons.Icons;
import org.mozilla.gecko.util.URIUtils;
import org.mozilla.gecko.widget.FaviconView;

import java.net.URI;
import java.util.concurrent.Future;

/**
 * Cliqz 2018
 * This file is derived from @{@link org.mozilla.gecko.activitystream.homepanel.topsites.TopSitesCard}.java
 */

public class TopNewsCard extends RecyclerView.ViewHolder implements IconCallback {
    private final FaviconView faviconView;

    private final TextView titleView, urlView;
    private Future<IconResponse> ongoingIconLoad;

    private Context context;

    /* package-local */ TopNewsCard(final RelativeLayout card) {
        super(card);
        context = card.getContext();
        faviconView = (FaviconView) card.findViewById(R.id.favicon);
        titleView = (TextView) card.findViewById(R.id.title_view);
        urlView = (TextView) card.findViewById(R.id.url_view);
    }

    void bind(final TopNews topNews) {
        if (ongoingIconLoad != null) {
            ongoingIconLoad.cancel(true);
        }

        if (TextUtils.isEmpty(topNews.getUrl())) {
            // Sometimes we get top sites without or with an empty URL - even though we do not allow
            // this anywhere in our UI. However with 'sync' we are not in full control of the data.
            // Whenever the URL is empty or null we just clear a potentially previously set icon.
            faviconView.clearImage();
        } else {
            ongoingIconLoad = Icons.with(itemView.getContext())
                    .pageUrl(topNews.getUrl())
                    .build()
                    .execute(this);
        }

        titleView.setText(buildTitleSpannable(topNews));
        urlView.setText(URIUtils.getFormattedDomain(context, URI.create(topNews.getUrl()),
                true, 0));
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

    @Override
    public void onIconResponse(IconResponse response) {
        if (faviconView != null) {
            faviconView.updateImage(response);
        }
    }

}
