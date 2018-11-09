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
            return new ItemViewHolder(view);
        } else {
            final View view = LayoutInflater.from(mContext).inflate(R.layout.dashboard_item2,
                    parent, false);
            return new TwoItemsViewHolder(view);
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
            final TwoItemsViewHolder viewHolder = (TwoItemsViewHolder) holder;
            setTwoItemsRowValues(viewHolder.leftItem, mDashboardItems.get(position));
            setTwoItemsRowValues(viewHolder.rightItem, mDashboardItems.get(position + 1));
        } else {
            final ItemViewHolder itemViewHolder = (ItemViewHolder) holder;
            // +1 because first 2 items is combined in 1 item
            final DashboardItemEntity curItem = mDashboardItems.get(position + 1);

            if (curItem.getIconResId() != -1) {
                final Drawable drawable = VectorDrawableCompat.create(mContext.getResources(), curItem
                        .getIconResId(), null);
                itemViewHolder.itemIconView.setVisibility(View.VISIBLE);
                itemViewHolder.itemIconView.setImageDrawable(drawable);
            } else {
                itemViewHolder.itemIconView.setVisibility(View.GONE);
            }

            if(curItem.getMeasurementValue().isEmpty()) { // no measurement value
                itemViewHolder.itemMeasurementView.setVisibility(View.GONE);
            } else {
                itemViewHolder.itemMeasurementView.setVisibility(View.VISIBLE);
                final Spannable spannableValue = new SpannableString(curItem.getMeasurementValue());
                final Spannable spannableUnit = new SpannableString(curItem.getMeasurementUnit());
                String separator;
                if (curItem.getIconResId() != -1) { //measurement value exist with  icon
                    final int valueTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_value_size);
                    final int unitTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_unit_size);

                    spannableValue.setSpan(new AbsoluteSizeSpan(valueTextSize), 0,
                            curItem.getMeasurementValue().length(),Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    spannableUnit.setSpan(new AbsoluteSizeSpan(unitTextSize), 0,
                            curItem.getMeasurementUnit().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                    itemViewHolder.itemMeasurementView.setTypeface(Typeface.SANS_SERIF);
                    separator = "";
                } else { //measurement value exist with no icon
                    final int valueTextSize = (int)mContext.getResources()
                            .getDimension(R.dimen.dashboard_measurement_value_size_large);

                    spannableValue.setSpan(new TypefaceSpan(mContext.getString(R.string.roboto_light))
                            , 0, curItem.getMeasurementValue().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    spannableUnit.setSpan(new TypefaceSpan(mContext.getString(R.string.roboto_medium))
                            , 0, curItem.getMeasurementUnit().length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);

                    itemViewHolder.itemMeasurementView.setTextSize(TypedValue.COMPLEX_UNIT_PX, valueTextSize);
                    separator = "\n";
                }
                itemViewHolder.itemMeasurementView.setText(TextUtils.concat(spannableValue,separator,spannableUnit));
            }
            itemViewHolder.itemTitleView.setText(curItem.getTitle());
            itemViewHolder.itemContentView.setText(curItem.getContent());

            if(curItem.getOptionValue() == -1) {
                itemViewHolder.itemMoneyBarView.setVisibility(View.GONE);
                itemViewHolder.itemMoneyBarValueView.setVisibility(View.GONE);
            } else {
                itemViewHolder.itemMoneyBarView.setVisibility(View.VISIBLE);
                final int curMoneyValue = curItem.getOptionValue() + MIN_MONEY_BAR_VALUE;
                itemViewHolder.itemMoneyBarView.setProgress(curMoneyValue);
                itemViewHolder.itemMoneyBarValueView.setText(String.valueOf(curItem.getOptionValue() +
                        MIN_MONEY_BAR_VALUE).concat("€"));

                itemViewHolder.itemMoneyBarView.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                        itemViewHolder.itemMoneyBarValueView.setText(String.valueOf(progress +
                                MIN_MONEY_BAR_VALUE).concat(" €"));
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {

                    }
                });

                itemViewHolder.itemMoneyBarValueView.setVisibility(View.VISIBLE);
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

    public void addItems(List<DashboardItemEntity> items) {
        mDashboardItems.clear();
        mDashboardItems.addAll(items);
        notifyDataSetChanged();
    }

    private class ItemViewHolder extends RecyclerView.ViewHolder {
        public ImageView itemIconView;
        public TextView itemMeasurementView;
        public TextView itemTitleView;
        public TextView itemContentView;
        public SeekBar itemMoneyBarView;
        public TextView itemMoneyBarValueView;

        public ItemViewHolder(View itemView) {
            super(itemView);
            itemIconView = (ImageView) itemView.findViewById(R.id.item_icon);
            itemMeasurementView = (TextView) itemView.findViewById(R.id.item_measurement);
            itemTitleView = (TextView) itemView.findViewById(R.id.item_title);
            itemContentView = (TextView) itemView.findViewById(R.id.item_content);
            itemMoneyBarView = (SeekBar) itemView.findViewById(R.id.item_money_bar);
            itemMoneyBarValueView = (TextView) itemView.findViewById(R.id.item_money_bar_value);
        }
    }

    private class ItemStructure {
        public ImageView itemIconView;
        public TextView itemMeasurementView;
        public TextView itemTitleView;
        public TextView itemContentView;
    }

    private class TwoItemsViewHolder extends RecyclerView.ViewHolder {
        final ItemStructure leftItem, rightItem;

        public TwoItemsViewHolder(View itemView) {
            super(itemView);
            final View leftView = itemView.findViewById(R.id.left_item);
            leftItem = findViews(leftView);
            final View rightView = itemView.findViewById(R.id.right_item);
            rightItem = findViews(rightView);
        }

        private ItemStructure findViews(View view) {
            final ItemStructure item = new ItemStructure();
            item.itemTitleView = (TextView) view.findViewById(R.id.item_title);
            item.itemContentView = (TextView) view.findViewById(R.id.item_content);
            item.itemMeasurementView = (TextView) view.findViewById(R.id.item_measurement);
            item.itemIconView = (ImageView) view.findViewById(R.id.item_icon);
            return item;
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
