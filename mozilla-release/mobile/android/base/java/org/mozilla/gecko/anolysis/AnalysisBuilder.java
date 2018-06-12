/*
 * Copyright © Cliqz 2018
 */
package org.mozilla.gecko.anolysis;

import android.support.annotation.NonNull;

import org.mozilla.gecko.util.GeckoBundle;

import java.util.Deque;
import java.util.LinkedList;

/**
 * Anolysis analysis builder
 */
public class AnalysisBuilder {

    private final String schema;
    private final Deque<GeckoBundle> dataStack;
    private final GeckoBundle data;
    private boolean mImmediate = false;

    public AnalysisBuilder(@NonNull String schema) {
        if (schema.isEmpty()) {
            throw new IllegalArgumentException("Empty schema");
        }
        this.schema = schema;
        this.data = new GeckoBundle();
        this.dataStack = new LinkedList<>();
        this.dataStack.push(this.data);
    }

    public AnalysisBuilder setImmediate(boolean value) {
        this.mImmediate = value;
        return this;
    }

    public Analysis build() {
        final GeckoBundle message = new GeckoBundle();
        message.putString("schema", schema);
        message.putBundle("data", this.data);
        message.putBoolean("immediate", mImmediate);
        return new Analysis(message);
    }

    public AnalysisBuilder openDictionary(@NonNull String name) {
        checkValidName(name);
        final GeckoBundle current = dataStack.peek();
        final GeckoBundle next = new GeckoBundle();
        current.putBundle(name, next);
        dataStack.push(next);
        return this;
    }

    public AnalysisBuilder closeDictionary() {
        dataStack.pop();
        // The root message should always be on the stack
        if (dataStack.isEmpty()) {
            throw new RuntimeException("Illegal closeDictionary");
        }
        return this;
    }

    public AnalysisBuilder putString(@NonNull String name, String value) {
        checkValidName(name);
        final GeckoBundle current = dataStack.peek();
        current.putString(name, value == null ? "" : value);
        return this;
    }

    public AnalysisBuilder putNumber(@NonNull String name, double value) {
        checkValidName(name);
        final GeckoBundle current = dataStack.peek();
        current.putDouble(name, value);
        return this;
    }

    public AnalysisBuilder putBoolean(@NonNull String name, boolean value) {
        checkValidName(name);
        final GeckoBundle current = dataStack.peek();
        current.putBoolean(name, value);
        return this;
    }

    public void dispatch() {
        build().dispatch();
    }

    private void checkValidName(String name) {
        if (name == null || name.isEmpty()) {
            throw new IllegalArgumentException("Invalid name: " + name == null ? "null" : "empty");
        }
    }
}
