package org.mozilla.gecko.activitystream.homepanel.topnews;

import android.content.Context;
import android.support.annotation.UiThread;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import org.mozilla.gecko.R;
import org.mozilla.gecko.activitystream.homepanel.model.TopNews;
import org.mozilla.gecko.icons.IconCallback;
import org.mozilla.gecko.icons.IconResponse;
import org.mozilla.gecko.icons.Icons;
import org.mozilla.gecko.util.StringUtils;
import org.mozilla.gecko.util.URIUtils;
import org.mozilla.gecko.util.ViewUtil;
import org.mozilla.gecko.widget.FaviconView;

import java.lang.ref.WeakReference;
import java.net.URI;
import java.util.UUID;
import java.util.concurrent.Future;

/**
 * @author Moaz Rashad
 */

public class TopNewsCard extends RecyclerView.ViewHolder  implements IconCallback{
    private final FaviconView faviconView;

    private final TextView titleView,urlView;
    private Future<IconResponse> ongoingIconLoad;

    private TopNews topNews;

    /* package-local */ TopNewsCard(final RelativeLayout card) {
        super(card);

        faviconView = (FaviconView) card.findViewById(R.id.favicon);
        titleView = (TextView) card.findViewById(R.id.title_view);
        urlView = (TextView) card.findViewById(R.id.url_view);

        ViewUtil.enableTouchRipple(card);
    }

    void bind(final TopNews topNews) {
        this.topNews = topNews;

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
                    .skipNetwork()
                    .forActivityStream()
                    .build()
                    .execute(this);
        }

//        setTopNewsTitle(topNews);
//        setTopNewsUrl(topNews);
        titleView.setText(topNews.getTitle());
        urlView.setText(topNews.getUrl());

    }

//    private void setTopNewsUrl(final TopNews topNews){
//        urlView.setText(topNews.getUrl());
//    }
//    private void setTopNewsTitle(final TopNews topNews) {
//        URI topSiteURI = null; // not final so we can use in the Exception case.
//        boolean isInvalidURI = false;
//        try {
//            topSiteURI = new URI(topNews.getUrl());
//        } catch (final URISyntaxException e) {
//            isInvalidURI = true;
//        }
//
//        final boolean isSiteSuggestedFromDistribution = BrowserDB.from(itemView.getContext()).getSuggestedSites()
//                .containsSiteAndSiteIsFromDistribution(topNews.getUrl());
//
//        // Some already installed distributions are unlikely to be updated (OTA, system) and their suggested
//        // site titles were written for the old top sites, where we had more room to display titles: we want
//        // to provide them with more lines. However, it's complex to distinguish a distribution intended for
//        // the old top sites and the new one so for code simplicity, we allow all distributions more lines for titles.
//        titleView.setMaxLines(isSiteSuggestedFromDistribution ? 2 : 1);
//
//        // We use page titles with distributions because that's what the creators of those distributions expect to
//        // be shown. Also, we need a valid URI for our preferred case so we stop here if we don't have one.
//        final String pageTitle = topNews.getTitle();
//        if (isInvalidURI || isSiteSuggestedFromDistribution) {
//            final String updateText = !TextUtils.isEmpty(pageTitle) ? pageTitle : topNews.getUrl();
//            setTopNewsTitleHelper(titleView, updateText); // See comment below regarding setCenteredText.
//
//            // This is our preferred case: we display "subdomain.domain". People refer to sites by their domain ("it's
//            // on wikipedia!") and it's a clean look so we display the domain if we can.
//            //
//            // If the path is non-empty, we'd normally go to the case below (see that comment). However, if there's no
//            // title, we'd prefer to fallback on "subdomain.domain" rather than a url, which is really ugly.
//        } else if (URIUtils.isPathEmpty(topSiteURI) ||
//                (!URIUtils.isPathEmpty(topSiteURI) && TextUtils.isEmpty(pageTitle))) {
//            // Our AsyncTask calls setCenteredText(), which needs to have all drawable's in place to correctly
//            // layout the text, so we need to wait with requesting the title until we've set our pin icon.
//            final TopNewsCard.UpdateCardTitleAsyncTask titleAsyncTask = new TopNewsCard
//                    .UpdateCardTitleAsyncTask(itemView.getContext(),
//                    topSiteURI, titleView);
//            titleAsyncTask.execute();
//
//            // We have a site with a path that has a non-empty title. It'd be impossible to distinguish multiple sites
//            // with "subdomain.domain" so if there's a path, we have to use something else: "domain/path" would overflow
//            // before it's useful so we use the page title.
//        } else {
//            setTopNewsTitleHelper(titleView, pageTitle); // See comment above regarding setCenteredText.
//        }
//    }

    private static void setTopNewsTitleHelper(final TextView textView, final String title) {
        // We use consistent padding all around the title, and the top padding is never modified,
        // so we can pass that in as the default padding:
        ViewUtil.setCenteredText(textView, title, textView.getPaddingTop());
    }

    @Override
    public void onIconResponse(IconResponse response) {
        if(faviconView !=null) {
            faviconView.updateImage(response);
        }
    }

    /** Updates the text of the given view to the page domain. */
    private static class UpdateCardTitleAsyncTask extends URIUtils.GetFormattedDomainAsyncTask {
        private static final int VIEW_TAG_ID = R.id.title; // same as the view.

        private final WeakReference<TextView> titleViewWeakReference;
        private final UUID viewTagAtStart;

        UpdateCardTitleAsyncTask(final Context contextReference, final URI uri, final TextView titleView) {
            super(contextReference, uri, false, 1); // subdomain.domain.
            this.titleViewWeakReference = new WeakReference<>(titleView);

            // See isTagSameAsStartTag for details.
            viewTagAtStart = UUID.randomUUID();
            titleView.setTag(VIEW_TAG_ID, viewTagAtStart);
        }

        @Override
        protected void onPostExecute(final String hostText) {
            super.onPostExecute(hostText);
            final TextView titleView = titleViewWeakReference.get();
            if (titleView == null || !isTagSameAsStartTag(titleView)) {
                return;
            }

            final String updateText;
            if (TextUtils.isEmpty(hostText)) {
                updateText = "";
            } else {
                updateText = StringUtils.stripCommonSubdomains(hostText);
            }
            setTopNewsTitleHelper(titleView, updateText);
        }

        /**
         * Returns true if the tag on the given view matches the tag from the constructor. We do this to ensure
         * the View we're making this request for hasn't been re-used by the time this request completes.
         */
        @UiThread
        private boolean isTagSameAsStartTag(final View viewToCheck) {
            return viewTagAtStart.equals(viewToCheck.getTag(VIEW_TAG_ID));
        }
    }

}
