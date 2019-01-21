package com.cliqz.mobile.test.driver;

import android.support.test.uiautomator.UiObject2;
import android.util.Log;

/**
 * Copyright © Cliqz 2019
 */
public class ElementData {

    private UiObject2 element;
    private boolean exists;
    private boolean checked;
    private boolean enabled;
    private boolean focused;
    private boolean selected;
    private String text;
    private String description;


    public ElementData(UiObject2 elem) {
        element = elem;
        try {
            exists = element.getResourceName()!=null;
            text = element.getText();
            description = element.getContentDescription();
            checked = element.isChecked();
            enabled = element.isEnabled();
            focused = element.isFocused();
            selected = element.isSelected();
        } catch (Exception e) {
            Log.w("AUTOBOTS", e.toString());
        }
    }

    public boolean hasChanged() {
        ElementData tempData = new ElementData(element);
        return this != tempData;
    }

}
