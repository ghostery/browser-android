/*
Copyright © Cliqz 2018
 */
package org.mozilla.gecko.anolysis;

import android.support.annotation.NonNull;

import org.mozilla.gecko.EventDispatcher;
import org.mozilla.gecko.util.GeckoBundle;

/**
 * Analysis wrapper
 */
class Analysis {

    private final GeckoBundle message;

    Analysis(@NonNull GeckoBundle message) {
        this.message = message;
    }

    public void dispatch(@NonNull EventDispatcher dispatcher) {
        dispatcher.dispatch("Search:Analysis", this.message);
    }

    public void dispatch() {
        dispatch(EventDispatcher.getInstance());
    }
}
