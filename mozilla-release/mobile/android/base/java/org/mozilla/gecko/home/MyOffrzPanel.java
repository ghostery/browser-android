package org.mozilla.gecko.home;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.ColorInt;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.graphics.drawable.VectorDrawableCompat;
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

import com.squareup.picasso.Picasso;

import org.json.JSONException;
import org.json.JSONObject;
import org.mozilla.gecko.R;
import org.mozilla.gecko.myoffrz.MyOffrzLoader;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.StringUtils;

import java.util.EnumSet;
import java.util.LinkedList;
import java.util.List;

import static android.support.v4.content.ContextCompat.getDrawable;

/**
 * Cliqz 2018
 * This file is derived from @{@link TopSitesPanel}.java
 */
public class MyOffrzPanel extends HomeFragment implements SharedPreferences
        .OnSharedPreferenceChangeListener{

    private static final String LOGTAG = "GeckoMyOffrzPanel";
    private static final String UI_INFO_KEY = "ui_info";
    private static final String TEMPLATE_DATA_KEY = "template_data";
    private static final String TEMPLATE_TITLE_KEY = "title";
    private static final String TEMPLATE_CODE_KEY = "code";
    private static final String ERROR_FALLBACK = "!!! ERROR !!!";
    private static final String TEMPLATE_DESCRIPTION_KEY = "desc";
    private static final String TEMPLATE_PICTURE_KEY = "picture_url";
    private static final String TEMPLATE_CONDITIONS = "conditions";
    private static final String ERROR_URL_FALLBAK = "http://cliqz.com";
    private static final String CALL_TO_ACTION_KEY = "call_to_action";
    private static final String CALL_TO_ACTION_TEXT_KEY = "text";
    private static final String CALL_TO_ACTION_URL_KEY = "url";
    private static final String MYOFFRZ_URL = "https://cliqz.com/myoffrz";

    // Callbacks used for the search and favicon cursor loaders
    private MyOffrzPanel.MyOffrzLoaderCallbacks mMyOffrzLoaderCallbacks;

    private static final boolean logVerbose = Log.isLoggable(LOGTAG, Log.VERBOSE);

    View offersOuterContainer, myOffrzDeactivateView;

    ViewGroup onboardingVG , emptyOffersOuterContainer, offersContainer, activationVG;

    ImageView onboardingIcon;

    TextView onboardingText, activationText, learnMoreActivation;

    ProgressBar progressBar;

    Button learnMoreOnBoarding, activate;
    ImageButton closeOnBoarding;

    PreferenceManager mPreferenceManager;

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
        onboardingVG = (ViewGroup) view.findViewById(R.id.offrz_onboaring_container);
        offersContainer = (ViewGroup) view.findViewById(R.id.offers_container);
        activationVG = (ViewGroup) view.findViewById(R.id.offrz_activation_container);
        offersOuterContainer = view.findViewById(R.id.offers_outer_container);
        myOffrzDeactivateView = view.findViewById(R.id.myoffrz_deactivate_view);
        onboardingIcon = (ImageView) view.findViewById(R.id.onboarding_feature_icon_iv);
        onboardingText = (TextView) view.findViewById(R.id.onboarding_feature_description_tv);
        activationText = (TextView) view.findViewById(R.id.activation_feature_description_tv);
        learnMoreActivation = (TextView) view.findViewById(R.id.learn_more_tv);
        learnMoreOnBoarding = (Button) view.findViewById(R.id.learn_more_btn);
        activate = (Button) view.findViewById(R.id.activate_btn);
        progressBar = (ProgressBar) view.findViewById(R.id.offers_loading_pb);
        closeOnBoarding = (ImageButton) view.findViewById(R.id.onboarding_close_btn);

        onboardingText.setText(R.string.myoffrz_onboarding_description);
        activationText.setText(R.string.myoffrz_activation_description);

        mPreferenceManager = PreferenceManager.getInstance(getContext().getApplicationContext());

        learnMoreOnBoarding.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                closeOnboarding();
                mUrlOpenListener.onUrlOpen(StringUtils.decodeUserEnteredUrl(MYOFFRZ_URL),
                        EnumSet.noneOf(HomePager.OnUrlOpenListener.Flags.class));
            }
        });

        learnMoreActivation.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
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

        activate.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                mPreferenceManager.setMyOffrzEnabled(true);
                mPreferenceManager.setMyOffrzOnboardingEnabled(false);
                activationVG.setVisibility(View.GONE);
                myOffrzDeactivateView.setVisibility(View.INVISIBLE);
                enableClickActionsOnOffrz(true);
            }
        });
        mPreferenceManager.registerOnSharedPreferenceChangeListener(this);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mMyOffrzLoaderCallbacks = new MyOffrzLoaderCallbacks();
    }

    @Override
    public void onResume() {
        super.onResume();
        updateLayouts();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mPreferenceManager.unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        updateLayouts();
    }

    private void updateLayouts(){
        if (!mPreferenceManager.isMyOffrzEnable()) {
            activationVG.setVisibility(View.VISIBLE);
            onboardingVG.setVisibility(View.GONE);
            myOffrzDeactivateView.setVisibility(View.VISIBLE);
            myOffrzDeactivateView.bringToFront();
            enableClickActionsOnOffrz(false);
        }else {
            activationVG.setVisibility(View.GONE);
            myOffrzDeactivateView.setVisibility(View.GONE);
            enableClickActionsOnOffrz(true);
            if (mPreferenceManager.isMyOffrzOnboardingEnabled()) {
                onboardingVG.setVisibility(View.VISIBLE);
            }
        }
    }

    private void enableClickActionsOnOffrz(boolean enable){
        for(int i = 0 ; i < offersContainer.getChildCount();i++){
            View curChild = offersContainer.getChildAt(i);
            if (curChild.getId() == R.id.offer_card){
                curChild.findViewById(R.id.terms_and_conditions_btn).setClickable(enable);
                curChild.findViewById(R.id.go_to_offer_btn).setClickable(enable);
                curChild.findViewById(R.id.offer_copy_code_btn).setClickable(enable);
            }
        }
    }

    @Override
    protected void load() {
        getLoaderManager().initLoader(R.id.offrz_loader_id, null, mMyOffrzLoaderCallbacks);
    }

    private void closeOnboarding() {
        onboardingVG.setVisibility(View.GONE);
        mPreferenceManager.setMyOffrzOnboardingEnabled(false);
    }

    private class MyOffrzLoaderCallbacks implements
            LoaderManager.LoaderCallbacks<JSONObject> {

        @Override
        public Loader<JSONObject> onCreateLoader(int id, Bundle args) {
            trace("Creating TopSitesLoader: " + id);
            return new MyOffrzLoader(getContext());
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

            if (!templateData.has(TEMPLATE_PICTURE_KEY)) {
                holder.image.setVisibility(View.GONE);
            } else {
                Picasso.with(getContext())
                        .load(Uri.parse(templateData.optString(TEMPLATE_PICTURE_KEY, ERROR_URL_FALLBAK)))
                        .into(holder.image);
            }

            if (!templateData.has(TEMPLATE_CONDITIONS)) {
                holder.termsAndConditions.setVisibility(View.GONE);
                holder.termsAndConditionsButton.setVisibility(View.GONE);
            } else {
                holder.termsAndConditions.setText(templateData.optString(TEMPLATE_CONDITIONS, ERROR_FALLBACK));
            }

            offersContainer.addView(offer);
            if(!mPreferenceManager.isMyOffrzEnable()) {
                myOffrzDeactivateView.setVisibility(View.VISIBLE);
                myOffrzDeactivateView.bringToFront();
                holder.enableClickActions(false);
            }else{
                myOffrzDeactivateView.setVisibility(View.INVISIBLE);
                holder.enableClickActions(true);
            }
        }

        private JSONObject getTemplateData(final JSONObject data) {
            try {
                return data.getJSONObject(UI_INFO_KEY).getJSONObject(TEMPLATE_DATA_KEY);
            } catch (JSONException e) {
                return null;
            }
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
            termsAndConditionsButton.setSelected(false);
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

            termsAndConditionsButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (termsAndConditions.getVisibility() == View.GONE) {
                        termsAndConditions.setVisibility(View.VISIBLE);
                        termsAndConditionsButton.setSelected(true);
                    } else {
                        termsAndConditions.setVisibility(View.GONE);
                        termsAndConditionsButton.setSelected(false);
                    }
                }
            });
            setTermsAndConditionButtonDrawables();
        }

        private void setTermsAndConditionButtonDrawables() {
            final Context context = getContext();
            final Drawable leftDrawable = getDrawable(context, R.drawable.ic_info_black).mutate();
            final Drawable rightDrawableSelected =
                    VectorDrawableCompat.create(getResources(), R.drawable.arrow_up, null)
                    .mutate();
            final Drawable rightDrawableUnselected =
                    VectorDrawableCompat.create(getResources(), R.drawable.arrow_down, null)
                    .mutate();
            final @ColorInt int color = ContextCompat.getColor(context, R.color.general_blue_color);
            DrawableCompat.setTint(rightDrawableSelected, color);
            DrawableCompat.setTint(rightDrawableUnselected, color);
            final StateListDrawable rightDrawable = new StateListDrawable();
            rightDrawable.addState(new int[] { android.R.attr.state_selected }, rightDrawableSelected);
            rightDrawable.addState(new int[] {}, rightDrawableUnselected);
            DrawableCompat.setTint(leftDrawable, color);

            termsAndConditionsButton.setCompoundDrawablesWithIntrinsicBounds(leftDrawable,null,rightDrawable,null);
        }

        void setOfferUrl(String url) {
            this.mUrl = url;
        }

        public void setCode(String code) {
            this.mCode = code;
        }

        public void enableClickActions(boolean enable){
            goToOffer.setClickable(enable);
            termsAndConditionsButton.setClickable(enable);
            copyCode.setClickable(enable);
        }
    }
}
