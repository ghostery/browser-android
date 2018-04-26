package org.mozilla.gecko.home;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.Loader;
import android.support.v4.graphics.drawable.DrawableCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;
import org.mozilla.gecko.GeckoSharedPrefs;
import org.mozilla.gecko.R;
import org.mozilla.gecko.myoffrz.MyOffrzLoader;
import org.mozilla.gecko.preferences.GeckoPreferences;
import org.mozilla.gecko.util.StringUtils;

import java.util.EnumSet;
import java.util.LinkedList;
import java.util.List;

import static android.support.v4.content.ContextCompat.getDrawable;

/**
 * Cliqz 2018
 * This file is derived from @{@link TopSitesPanel}.java
 */

public class MyOffrzPanel extends HomeFragment {

    private static final String LOGTAG = "GeckoMyOffrzPanel";
    private static final String ACTIONS_KEY = "actions";
    private static final String UI_INFO_KEY = "ui_info";
    private static final String TEMPLATE_DATA_KEY = "template_data";
    private static final String TEMPLATE_TITLE_KEY = "title";
    private static final String TEMPLATE_CODE_KEY = "code";
    private static final String ERROR_FALLBACK = "!!! ERROR !!!";
    private static final String TEMPLATE_DESCRIPTION_KEY = "desc";
    private static final String TEMPLATE_LOGO_KEY = "logo_url";
    private static final String ERROR_URL_FALLBAK = "http://cliqz.com";
    private static final String CALL_TO_ACTION_KEY = "call_to_action";
    private static final String CALL_TO_ACTION_TEXT_KEY = "text";
    private static final String CALL_TO_ACTION_URL_KEY = "url";
    private static final String MYOFFRZ_URL = "https://cliqz.com/myoffrz";
    // Cursor loader ID for MyOffrz
    private static final int LOADER_ID_MY_OFFRZ = 0;

    // Callbacks used for the search and favicon cursor loaders
    private MyOffrzPanel.MyOffrzLoaderCallbacks mMyOffrzLoaderCallbacks;

    private static final boolean logVerbose = Log.isLoggable(LOGTAG, Log.VERBOSE);

    View offersOuterContainer;

    ViewGroup onboardingVG , emptyOffersOuterContainer, offersContainer;

    ImageView onboardingIcon, emptyOffersIcon;

    TextView onboardingText;

    ProgressBar progressBar;

    Button learnMore;
    ImageButton closeOnBoarding;

    private static void trace(final String message) {
        if (logVerbose) {
            Log.v(LOGTAG, message);
        }
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        final View view = inflater.inflate(R.layout.home_myoffrz_panel, container, false);

        emptyOffersOuterContainer = (ViewGroup) view.findViewById(R.id
                .empty_offers_outer_container);
        offersOuterContainer = view.findViewById(R.id.offers_outer_container);
        onboardingVG = (ViewGroup) view.findViewById(R.id.offrz_onboaring_container);
        offersContainer = (ViewGroup) view.findViewById(R.id.offers_container);
        onboardingIcon = (ImageView) view.findViewById(R.id.onboarding_feature_icon_iv);
        emptyOffersIcon = (ImageView) view.findViewById(R.id.empty_offers_icon_iv);
        onboardingText = (TextView) view.findViewById(R.id.onboarding_feature_description_tv);
        progressBar = (ProgressBar) view.findViewById(R.id.offers_loading_pb);
        learnMore = (Button) view.findViewById(R.id.learn_more_btn);
        closeOnBoarding = (ImageButton) view.findViewById(R.id.onboarding_close_btn);

        onboardingText.setText(R.string.myoffrz_onboarding_description);

        learnMore.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                closeOnboarding();
                mUrlOpenListener.onUrlOpen(StringUtils.decodeUserEnteredUrl(MYOFFRZ_URL),
                        EnumSet.noneOf(HomePager.OnUrlOpenListener.Flags.class));
            }
        });

        closeOnBoarding.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                closeOnboarding();
            }
        });
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mMyOffrzLoaderCallbacks = new MyOffrzLoaderCallbacks();
    }

    @Override
    public void onStart() {
        super.onStart();
        if (!isMyOffrzOnboardingEnabled()) {
            onboardingVG.setVisibility(View.GONE);
        } else {
            onboardingVG.setVisibility(View.VISIBLE);
        }
    }

    @Override
    protected void load() {
        getLoaderManager().initLoader(LOADER_ID_MY_OFFRZ, null, mMyOffrzLoaderCallbacks);

    }

    private void closeOnboarding() {
        onboardingVG.setVisibility(View.GONE);
        setMyOffrzOnboardingEnabled(false);
    }


    private class MyOffrzLoaderCallbacks implements
            LoaderManager.LoaderCallbacks<JSONObject> {

        @Override
        public Loader<JSONObject> onCreateLoader(int id, Bundle args) {
            trace("Creating TopSitesLoader: " + id);
            return new MyOffrzLoader(getActivity());
        }

        @Override
        public void onLoadFinished(Loader<JSONObject> loader, JSONObject data) {
            progressBar.setVisibility(View.GONE);
            if (data == null) {
                // display an error message here
                offersOuterContainer.setVisibility(View.INVISIBLE);
                emptyOffersOuterContainer.setVisibility(View.VISIBLE);
                return;
            }
            offersOuterContainer.setVisibility(View.VISIBLE);
            emptyOffersOuterContainer.setVisibility(View.GONE);

            // Remove all previous offers if needed
            final List<View> toBeRemoved = new LinkedList<>();
            for (int i = 0; i < offersContainer.getChildCount(); i++) {
                final View view = offersContainer.getChildAt(i);
                if (view.getId() == R.id.offer_card) {
                    toBeRemoved.add(view);
                }
            }
            for (View v: toBeRemoved) {
                offersContainer.removeView(v);
            }

            final LayoutInflater inflater = LayoutInflater.from(getContext());
            final View offer = inflater.inflate(R.layout.myoffrz_offer_template, offersContainer,
                    false);
            final ViewHolder holder = new ViewHolder(offer);

            // !!! No terms and conditions right now !!!
            holder.termsAndConditions.setVisibility(View.GONE);
            holder.termsAndConditionsButton.setVisibility(View.GONE);
            // !!! No terms and conditions right now !!!

            final JSONObject templateData = getTemplateData(data);
            if (templateData == null) {
                return;
            }
            if (!templateData.has(TEMPLATE_TITLE_KEY)) {
                return;
            }

            final JSONObject callToActionData = templateData.optJSONObject(CALL_TO_ACTION_KEY);
            if (callToActionData == null ||
                    !callToActionData.has(CALL_TO_ACTION_TEXT_KEY) ||
                    !callToActionData.has(CALL_TO_ACTION_URL_KEY)) {
                return;
            }

            holder.title.setText(templateData.optString(TEMPLATE_TITLE_KEY, ERROR_FALLBACK));
            holder.setOfferUrl(callToActionData.optString(CALL_TO_ACTION_URL_KEY, ERROR_URL_FALLBAK));
            holder.goToOffer
                    .setText(callToActionData.optString(CALL_TO_ACTION_TEXT_KEY, ERROR_FALLBACK));
            if (!templateData.has(TEMPLATE_CODE_KEY)) {
                return;
            }
            final String code = templateData.optString(TEMPLATE_CODE_KEY, ERROR_FALLBACK);
            holder.copyCode.setText(code);
            holder.setCode(code);

            if (!templateData.has(TEMPLATE_DESCRIPTION_KEY)) {
                holder.descrption.setVisibility(View.GONE);
            } else {
                holder.descrption.setText(templateData.optString(TEMPLATE_DESCRIPTION_KEY, ERROR_FALLBACK));
            }

            if (!templateData.has(TEMPLATE_LOGO_KEY)) {
                holder.image.setVisibility(View.GONE);
            } else {
                holder.image.setImageURI(Uri.parse(templateData.optString(TEMPLATE_LOGO_KEY, ERROR_URL_FALLBAK)));
            }

            offersContainer.addView(offer);
        }

        private JSONObject getTemplateData(final JSONObject data) {
            final Object[] path = new Object[] {
                    ACTIONS_KEY, 1, 1, 1, UI_INFO_KEY, TEMPLATE_DATA_KEY
            };
            Object currentNode = data;
            for (Object key: path) {
                if (currentNode instanceof JSONObject && key instanceof String) {
                    currentNode = ((JSONObject) currentNode).opt((String) key);
                } else if (currentNode instanceof JSONArray && key instanceof Integer) {
                    currentNode = ((JSONArray) currentNode).opt((Integer) key);
                } else {
                    return null;
                }
            }
            if (!(currentNode instanceof JSONObject)) {
                return null;
            }
            return (JSONObject) currentNode;
        }

        @Override
        public void onLoaderReset(Loader<JSONObject> loader) {

        }
    }

    class ViewHolder {

        TextView title, descrption, termsAndConditions;

        ImageView image;

        Button termsAndConditionsButton, copyCode, goToOffer;

        private String mUrl;
        private String mCode;

        ViewHolder(@NonNull View view) {
            title = (TextView) view.findViewById(R.id.offer_title_tv);
            descrption = (TextView) view.findViewById(R.id.offer_description_tv);
            image = (ImageView) view.findViewById(R.id.offer_image_iv);
            termsAndConditionsButton = (Button) view.findViewById(R.id.terms_and_conditions_btn);
            termsAndConditions = (TextView) view.findViewById(R.id.terms_and_conditions_tv);
            copyCode = (Button) view.findViewById(R.id.offer_copy_code_btn);
            goToOffer = (Button) view.findViewById(R.id.go_to_offer_btn);

            copyCode.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    final Context context = getContext();
                    if (context == null || mCode == null || mCode.isEmpty()) {
                        return;
                    }

                    final ClipboardManager clipboardManager =
                            (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
                    if (clipboardManager != null) {
                        final ClipData clipData = ClipData.newPlainText("text", mCode);
                        clipboardManager.setPrimaryClip(clipData);
                        Toast.makeText(context, R.string.myoffrz_message_code_copied, Toast.LENGTH_SHORT).show();
                    }
                }
            });

            goToOffer.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v) {
                    if (mUrl != null) {
                        mUrlOpenListener.onUrlOpen(StringUtils.decodeUserEnteredUrl(mUrl),
                                EnumSet.noneOf(HomePager.OnUrlOpenListener.Flags.class));
                    }
                }
            });

            setTermsAndConditionButtonDrawables();
        }

        private void setTermsAndConditionButtonDrawables() {
            final Context context = getContext();
            final Drawable leftDrawable = getDrawable(context, R.drawable.ic_info_black).mutate();
            final Drawable rightDrawable = getDrawable(context, R.drawable.ic_arrow_up).mutate();

            final @ColorInt int color = ContextCompat.getColor(context, R.color.general_blue_color);

            DrawableCompat.setTint(leftDrawable, color);
            DrawableCompat.setTint(rightDrawable, color);

            termsAndConditionsButton.setCompoundDrawablesWithIntrinsicBounds(leftDrawable,null,rightDrawable,null);
        }

        void setOfferUrl(String url) {
            this.mUrl = url;
        }

        public void setCode(String code) {
            this.mCode = code;
        }
    }

    /* Cliqz start */
    // add get and set function for MyOffrz OnBoarding pref.
    // This part is derived from @{@link TabQueuePanel}.java
    public void setMyOffrzOnboardingEnabled(boolean value) {
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(getActivity());
        final SharedPreferences.Editor editor = prefs.edit();
        editor.putBoolean(GeckoPreferences.IS_MYOFFRZ_ONBOARDING_ENABLED, value).apply();
    }
    
    // This part is derived from @{@link TabQueueHelper}.java
    public boolean isMyOffrzOnboardingEnabled() {
        final SharedPreferences prefs = GeckoSharedPrefs.forApp(getActivity());
        return  prefs.getBoolean(GeckoPreferences.IS_MYOFFRZ_ONBOARDING_ENABLED,true);
    }
}
