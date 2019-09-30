/* -*- Mode: Java; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: nil; -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.mozilla.gecko.preferences;

import android.content.Context;
import android.preference.Preference;
import android.util.AttributeSet;

import org.mozilla.gecko.Tabs;

class LinkPreference extends Preference {
    /* Cliqz start */
    private String mUrl;
    private static final String URL_ATTRIBUTE_NAME = "url";

    public LinkPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        parseAttrs(context, attrs);
    }
    public LinkPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        parseAttrs(context, attrs);
    }

    private void parseAttrs(Context context, AttributeSet attrs) {
        final int urlAttrValueId = attrs.getAttributeResourceValue(null,URL_ATTRIBUTE_NAME,-1);
        mUrl = (urlAttrValueId != -1) ? context.getString(urlAttrValueId) : attrs
                .getAttributeValue(null,URL_ATTRIBUTE_NAME);
    }
    /* Cliqz end */

    public void setUrl(String url) {
        mUrl = url;
    }

    @Override
    protected void onClick() {
        Tabs.getInstance().loadUrlInTab(mUrl);
        callChangeListener(mUrl);
    }
}
