package org.mozilla.gecko.controlcenter;

import android.content.Context;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.support.graphics.drawable.VectorDrawableCompat;
import android.support.v7.widget.RecyclerView;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.style.AbsoluteSizeSpan;
import android.text.style.TypefaceSpan;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import org.mozilla.gecko.R;
import org.mozilla.gecko.util.GeckoBundle;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright © Cliqz 2018
 */

public class DashboardAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private Context mContext;
    private List<DashboardItemEntity> mDashboardItems;
    private enum ItemType {ONE_VIEW, TWO_VIEWS}
    private final int MIN_MONEY_BAR_VALUE = 5;
    public static final int AVERAGE_MONEY_BAR_VALUE = 5;

    public DashboardAdapter(Context context) {
        mContext = context;
        mDashboardItems = new ArrayList<>();
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        if (viewType == ItemType.ONE_VIEW.ordinal()) {
            final View view = LayoutInflater.from(mContext).inflate(R.layout.dashboard_item1,
                    parent, false);
            return new ViewHolder(view);
        } else {
            final View view = LayoutInflater.from(mContext).inflate(R.layout.dashboard_item2,
                    parent, false);
            return new ViewHolder2(view);
        }
    }

    public void setTwoItemsRowValues(ItemStructure view, DashboardItemEntity item) {
        final Drawable drawable = VectorDrawableCompat.create(mContext.getResources(), item
                .getIconResId(), null);
        view.itemIconView.setImageDrawable(drawable);
        String measurementText = item.getMeasurementValue();
        final int unitStartIdx = measurementText.length();
        if (!item.getMeasurementUnit().isEmpty()) {
            measurementText = measurementText.concat("\n").concat(item.getMeasurementUnit());
        }
        Spannable spannable = new SpannableString(measurementText);
        spannable.setSpan(new TypefaceSpan(mContext.getString(R.string.roboto_light)), unitStartIdx, measurementText.length()
                , Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        view.itemMeasurementView.setText(spannable);
        view.itemTitleView.setText(item.getTitle());
        view.itemContentView.setText(item.getContent());
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        if (holder.getItemViewType() == ItemType.TWO_VIEWS.ordinal()) {
            final ViewHolder2 viewHolder = (ViewHolder2) holder;
            setTwoItemsRowValues(viewHolder.leftItem, mDashboardItems.get(position));
            setTwoItemsRowValues(viewHolder.rightItem, mDashboardItems.get(position + 1));
        } else {
            final ViewHolder viewHolder = (ViewHolder) holder;
            // +1 because first 2 items is combined in 1 item
            final DashboardItemEntity curItem = mDashboardItems.get(position + 1);

            if (curItem.getIconResId() != -1) {
                final Drawable drawable = VectorDrawableCompat.create(mContext.getResources(), curItem
                        .getIconResId(), null);
                viewHolder.itemIconView.setVisibility(View.VISIBLE);
                viewHolder.itemIconView.setImageDrawable(drawable);
            } else {
                viewHolder.itemIconView.setVisibility(View.GONE);
            }

            if(curItem.getMeasurementValue().isEmpty()) { // no measurement value
                viewHolder.itemMeasurementView.setVisibility(View.GONE);
            } else {
                viewHolder.itemMeasurementView.setVisibility(View.VISIBLE);
                Spannable spannableValue, spannableUnit;
                String separator;
                if (curItem.getIconResId() != -1) { //measurement value exist with  icon
                    final int valueTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_value_size);
                    final int unitTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_unit_size);

                    spannableValue = new SpannableString(curItem.getMeasurementValue());
                    spannableValue.setSpan(new AbsoluteSizeSpan(valueTextSize), 0,
                            curItem.getMeasurementValue().length(),Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    spannableUnit = new SpannableString(curItem.getMeasurementUnit());
                    spannableUnit.setSpan(new AbsoluteSizeSpan(unitTextSize), 0,
                            curItem.getMeasurementUnit().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                    viewHolder.itemMeasurementView.setTypeface(Typeface.SANS_SERIF);
                    separator = "";
                } else { //measurement value exist with no icon
                    final int valueTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_value_size_large);

                    viewHolder.itemMeasurementView.setVisibility(View.VISIBLE);
                    spannableValue = new SpannableString(curItem.getMeasurementValue());
                    spannableValue.setSpan(new TypefaceSpan(mContext.getString(R.string.roboto_light))
                            , 0, curItem.getMeasurementValue().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    spannableUnit = new SpannableString(curItem.getMeasurementUnit());
                    spannableUnit.setSpan(new TypefaceSpan(mContext.getString(R.string.roboto_medium))
                            , 0, curItem.getMeasurementUnit().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    viewHolder.itemMeasurementView.setTextSize(TypedValue.COMPLEX_UNIT_PX, valueTextSize);
                    separator = "\n";
                }
                viewHolder.itemMeasurementView.setText(TextUtils.concat(spannableValue,separator,spannableUnit));
            }
            viewHolder.itemTitleView.setText(curItem.getTitle());
            viewHolder.itemContentView.setText(curItem.getContent());

            if(curItem.getOptionValue() == -1) {
                viewHolder.itemMoneyBarView.setVisibility(View.GONE);
                viewHolder.itemMoneyBarValueView.setVisibility(View.GONE);
            } else {
                viewHolder.itemMoneyBarView.setVisibility(View.VISIBLE);
                final int curMoneyValue = curItem.getOptionValue() + MIN_MONEY_BAR_VALUE;
                viewHolder.itemMoneyBarView.setProgress(curMoneyValue);
                viewHolder.itemMoneyBarValueView.setText(String.valueOf(curItem.getOptionValue() +
                        MIN_MONEY_BAR_VALUE).concat("€"));

                viewHolder.itemMoneyBarView.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                        viewHolder.itemMoneyBarValueView.setText(String.valueOf(progress +
                                MIN_MONEY_BAR_VALUE).concat(" €"));
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {

                    }
                });

                viewHolder.itemMoneyBarValueView.setVisibility(View.VISIBLE);
            }
        }
    }

    @Override
    public int getItemViewType(int position) {
        return position == 0 ? ItemType.TWO_VIEWS.ordinal() : ItemType.ONE_VIEW.ordinal();
    }

    @Override
    public int getItemCount() { // -1, because first 2 items combined in the first view
        return mDashboardItems.size() - 1;
    }

    void setData(GeckoBundle data) {
        // @TODO fetch the data and fill the list
    }

    void addItems(List<DashboardItemEntity> items) {
        mDashboardItems.clear();
        mDashboardItems.addAll(items);
        notifyDataSetChanged();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        public ImageView itemIconView;
        public TextView itemMeasurementView;
        public TextView itemTitleView;
        public TextView itemContentView;
        public SeekBar itemMoneyBarView;
        public TextView itemMoneyBarValueView;

        public ViewHolder(View itemView) {
            super(itemView);
            itemIconView = (ImageView) itemView.findViewById(R.id.item_icon);
            itemMeasurementView = (TextView) itemView.findViewById(R.id.item_measurement);
            itemTitleView = (TextView) itemView.findViewById(R.id.item_title);
            itemContentView = (TextView) itemView.findViewById(R.id.item_content);
            itemMoneyBarView = (SeekBar) itemView.findViewById(R.id.item_money_bar);
            itemMoneyBarValueView = (TextView) itemView.findViewById(R.id.item_money_bar_value);
        }
    }

    public class ItemStructure {
        public ImageView itemIconView;
        public TextView itemMeasurementView;
        public TextView itemTitleView;
        public TextView itemContentView;
    }

    public class ViewHolder2 extends RecyclerView.ViewHolder {
        final ItemStructure leftItem, rightItem;

        public ViewHolder2(View itemView) {
            super(itemView);
            leftItem = new ItemStructure();
            final View leftView = itemView.findViewById(R.id.left_item);
            findViews(leftView, leftItem);
            rightItem = new ItemStructure();
            final View rightView = itemView.findViewById(R.id.right_item);
            findViews(rightView, rightItem);
        }

        public void findViews(View view, ItemStructure item) {
            item.itemTitleView = (TextView) view.findViewById(R.id.item_title);
            item.itemContentView = (TextView) view.findViewById(R.id.item_content);
            item.itemMeasurementView = (TextView) view.findViewById(R.id.item_measurement);
            item.itemIconView = (ImageView) view.findViewById(R.id.item_icon);
        }
    }

    public void resetData() {
        for(int i = 0;i < mDashboardItems.size(); i++) {
            if(!mDashboardItems.get(i).getMeasurementValue().equals("")) {
                mDashboardItems.get(i).setMeasurementValue("0");
            }
        }
        notifyDataSetChanged();
    }
}
