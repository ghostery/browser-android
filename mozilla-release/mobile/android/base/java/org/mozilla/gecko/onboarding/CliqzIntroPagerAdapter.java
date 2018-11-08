package org.mozilla.gecko.onboarding;

import android.content.Context;
import android.graphics.Color;
import android.support.annotation.DrawableRes;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.view.PagerAdapter;
import android.text.Html;
import android.text.SpannableStringBuilder;
import android.text.style.ForegroundColorSpan;
import android.text.style.RelativeSizeSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.TextView;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.R;
import org.mozilla.gecko.preferences.PreferenceManager;
import org.mozilla.gecko.util.CustomLinkMovementMethod;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Copyright © Cliqz 2018
 *
 * This is an implementation use a PagerAdapter which directly inflates layouts instead of using
 * fragments
 */
public class CliqzIntroPagerAdapter extends PagerAdapter {

    private interface CustomPageAction {
        void onInflated(@NonNull ViewGroup layout);
    }

    private static class FirstScreenAction implements CustomPageAction,
            CustomLinkMovementMethod.OnOpenLinkCallBack,
            CheckBox.OnCheckedChangeListener {

        @Override
        public void onInflated(@NonNull ViewGroup layout) {
            final TextView dataCollectionTv = (TextView) layout.findViewById(R.id.data_collection_detail_tv);
            final CustomLinkMovementMethod customLinkMovementMethod =
                    CustomLinkMovementMethod.getInstance(this);
            customLinkMovementMethod.init(CustomLinkMovementMethod.OPEN_IN_CUSTOM_TAB);
            dataCollectionTv.setMovementMethod(customLinkMovementMethod);
            dataCollectionTv.setText(Html.fromHtml(layout.getContext().getString(R.string.ghostery_onboarding_new_users_checkbox)));
            final CheckBox collectDataCb = (CheckBox) layout.findViewById(R.id.collect_data_cb);
            collectDataCb.setOnCheckedChangeListener(this);
        }

        @Override
        public void OnOpenLinkLoaded() {

        }

        @Override
        public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
            final Context context = compoundButton.getContext().getApplicationContext();
            final PreferenceManager preferenceManager = PreferenceManager.getInstance(context);
            preferenceManager.setTelemetryEnabled(b);
            preferenceManager.setHumanWebEnabled(b);
        }
    }

    private static class SecondScreenAction implements CustomPageAction, RadioButton.OnClickListener {

        @Override
        public void onInflated(@NonNull ViewGroup layout) {
            final Context context = layout.getContext();
            final RadioButton radio1 = (RadioButton) layout.findViewById(R.id.radio1);
            final RadioButton radio2 = (RadioButton) layout.findViewById(R.id.radio2);
            final RadioButton radio3 = (RadioButton) layout.findViewById(R.id.radio3);
            final SpannableStringBuilder builder = new SpannableStringBuilder();
            builder.append(context.getString(R.string.ghostery_onboarding_tracking_protection_radio2))
                    .append("\n");
            final int spanStart = builder.length();
            builder.append(context.getString(R.string.ghostery_onboarding_tracking_protection_radio2_subtext));
            builder.setSpan(new RelativeSizeSpan(0.7f), spanStart, builder.length(), 0);
            radio2.setText(builder);
            radio1.setOnClickListener(this);
            radio2.setOnClickListener(this);
            radio3.setOnClickListener(this);
        }

        @Override
        public void onClick(View view) {
            final GeckoBundle bundle = new GeckoBundle();
            switch (view.getId()) {
                case R.id.radio1:
                    bundle.putString("blockingPolicy", "BLOCKING_POLICY_NOTHING");
                    break;
                case R.id.radio2:
                    bundle.putString("blockingPolicy", "BLOCKING_POLICY_RECOMMENDED");
                    break;
                case R.id.radio3:
                    bundle.putString("blockingPolicy", "BLOCKING_POLICY_EVERYTHING");
                    break;
            }
            EventDispatcher.getInstance().dispatch("Privacy:SetBlockingPolicy", bundle);
        }
    }

    enum Screen {
        FIRST(R.layout.cliqz_intro_first_screen_content,
            R.drawable.cliqz_intro_ic_first_screen, new FirstScreenAction()),
        SECOND(R.layout.cliqz_intro_second_screen_content, R.drawable.cliqz_intro_ic_second_screen, new SecondScreenAction()),
        THIRD(R.layout.cliqz_intro_third_screen_content, R.drawable.cliqz_intro_ic_third_screen, null),
        FOURTH(R.layout.cliqz_intro_fourth_screen_content, R.drawable.cliqz_intro_ic_fourth_screen, null);

        final int layout;
        final int image;
        final CustomPageAction action;

        Screen(@LayoutRes int layout, @DrawableRes int image, @Nullable CustomPageAction action) {
            this.layout = layout;
            this.image = image;
            this.action = action;
        }
    }
    private Context mContext;

    public CliqzIntroPagerAdapter(Context context) {
        mContext = context;
    }

    @Override
    public Object instantiateItem(final ViewGroup container, int position) {
        final LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        final Screen screen = Screen.values()[position];
        final ViewGroup layout = (ViewGroup) layoutInflater.inflate(R.layout.cliqz_intro_structure, container, false);
        final LinearLayout contentContainer = (LinearLayout) layout.findViewById(R.id.content_container);
        final ImageView imageView = (ImageView) layout.findViewById(R.id.image);
        imageView.setImageResource(screen.image);
        layoutInflater.inflate(screen.layout, contentContainer, true);
        if (screen.action != null) {
            screen.action.onInflated(layout);
        }
        container.addView(layout);
        return layout;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object view) {
        container.removeView((View) view);
    }

    @Override
    public int getCount() {
        return Screen.values().length;
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == object;
    }
}
