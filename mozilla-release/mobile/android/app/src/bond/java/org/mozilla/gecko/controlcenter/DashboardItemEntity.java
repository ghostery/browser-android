package org.mozilla.gecko.controlcenter;

/**
 * Copyright © Cliqz 2018
 */

public class DashboardItemEntity {
    private String measurementValue;
    private String measurementUnit;
    private int iconResId;
    private String title;
    private String content;

    public DashboardItemEntity(String measurementValue, String measurementUnit, int iconResId, String title, String content) {
        this.measurementValue = measurementValue;
        this.measurementUnit = measurementUnit;
        this.iconResId = iconResId;
        this.title = title;
        this.content = content;
    }

    public String getMeasurementValue() {
        return measurementValue;
    }

    public void setMeasurementValue(String measurementValue) {
        this.measurementValue = measurementValue;
    }

    public String getMeasurementUnit() {
        return measurementUnit;
    }

    public void setMeasurementUnit(String measurementUnit) {
        this.measurementUnit = measurementUnit;
    }

    public int getIconResId() {
        return iconResId;
    }

    public void setIconResId(int iconResId) {
        this.iconResId = iconResId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }
}
