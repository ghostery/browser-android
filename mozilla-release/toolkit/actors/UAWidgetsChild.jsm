/* vim: set ts=2 sw=2 sts=2 et tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
"use strict";

var EXPORTED_SYMBOLS = ["UAWidgetsChild"];

const { ActorChild } = ChromeUtils.import(
  "resource://gre/modules/ActorChild.jsm"
);
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");

const HANDLED_ELEMENTS = ["video", "audio", "embed", "object", "marquee"];

class UAWidgetsChild extends ActorChild {
  constructor(dispatcher) {
    super(dispatcher);

    this.widgets = new WeakMap();
    this.prefsCache = new Map();
  }

  handleEvent(aEvent) {
<<<<<<< HEAD
    switch (aEvent.type) {
      case "UAWidgetSetupOrChange":
        this.setupOrNotifyWidget(aEvent.target);
        break;
      case "UAWidgetTeardown":
        this.teardownWidget(aEvent.target);
        break;
    }
||||||| merged common ancestors
    switch (aEvent.type) {
      case "UAWidgetBindToTree":
      case "UAWidgetAttributeChanged":
        this.setupOrNotifyWidget(aEvent.target);
        break;
      case "UAWidgetUnbindFromTree":
        this.teardownWidget(aEvent.target);
        break;
    }
=======
    if (HANDLED_ELEMENTS.includes(aEvent.target.localName)) {
      switch (aEvent.type) {
        case "UAWidgetSetupOrChange":
          this.setupOrNotifyWidget(aEvent.target);
          break;
        case "UAWidgetTeardown":
          this.teardownWidget(aEvent.target);
          break;
      }
>>>>>>> upstream-releases

      // In case we are a nested frame, prevent the message manager of the
      // parent frame from receving the event.
      aEvent.stopPropagation();
    }
  }

  setupOrNotifyWidget(aElement) {
    let widget = this.widgets.get(aElement);
    if (!widget) {
      this.setupWidget(aElement);
      return;
    }
<<<<<<< HEAD
    if (typeof widget.wrappedJSObject.onchange == "function") {
      try {
        widget.wrappedJSObject.onchange();
      } catch (ex) {
        Cu.reportError(ex);
      }
||||||| merged common ancestors
    if (typeof widget.wrappedJSObject.onattributechange == "function") {
      widget.wrappedJSObject.onattributechange();
=======
    if (typeof widget.onchange == "function") {
      try {
        widget.onchange();
      } catch (ex) {
        Cu.reportError(ex);
      }
>>>>>>> upstream-releases
    }
  }

  setupWidget(aElement) {
    let uri;
    let widgetName;
    let prefKeys = [];
    switch (aElement.localName) {
      case "video":
      case "audio":
        uri = "chrome://global/content/elements/videocontrols.js";
<<<<<<< HEAD
        widgetName = "VideoControlsWidget";
||||||| merged common ancestors
        widgetName = "VideoControlsPageWidget";
=======
        widgetName = "VideoControlsWidget";
        prefKeys = [
          "media.videocontrols.picture-in-picture.video-toggle.enabled",
          "media.videocontrols.picture-in-picture.video-toggle.always-show",
        ];
>>>>>>> upstream-releases
        break;
<<<<<<< HEAD
      case "input":
        uri = "chrome://global/content/elements/datetimebox.js";
        widgetName = "DateTimeBoxWidget";
        break;
||||||| merged common ancestors
      case "input":
        // TODO (datetimebox)
        break;
      case "applet":
=======
>>>>>>> upstream-releases
      case "embed":
      case "object":
        uri = "chrome://global/content/elements/pluginProblem.js";
        widgetName = "PluginProblemWidget";
        break;
      case "marquee":
        uri = "chrome://global/content/elements/marquee.js";
        widgetName = "MarqueeWidget";
        break;
    }

    if (!uri || !widgetName) {
<<<<<<< HEAD
      Cu.reportError("Getting a UAWidgetSetupOrChange event on undefined element.");
||||||| merged common ancestors
=======
      Cu.reportError(
        "Getting a UAWidgetSetupOrChange event on undefined element."
      );
>>>>>>> upstream-releases
      return;
    }

    let shadowRoot = aElement.openOrClosedShadowRoot;
<<<<<<< HEAD
    if (!shadowRoot) {
      Cu.reportError("Getting a UAWidgetSetupOrChange event without the Shadow Root.");
      return;
    }

    let isSystemPrincipal = aElement.nodePrincipal.isSystemPrincipal;
    let sandbox = isSystemPrincipal ?
      Object.create(null) : Cu.getUAWidgetScope(aElement.nodePrincipal);
||||||| merged common ancestors
    let sandbox = aElement.nodePrincipal.isSystemPrincipal ?
      Object.create(null) : Cu.getUAWidgetScope(aElement.nodePrincipal);
=======
    if (!shadowRoot) {
      Cu.reportError(
        "Getting a UAWidgetSetupOrChange event without the Shadow Root."
      );
      return;
    }

    let isSystemPrincipal = aElement.nodePrincipal.isSystemPrincipal;
    let sandbox = isSystemPrincipal
      ? Object.create(null)
      : Cu.getUAWidgetScope(aElement.nodePrincipal);
>>>>>>> upstream-releases

    if (!sandbox[widgetName]) {
      Services.scriptloader.loadSubScript(uri, sandbox);
    }

    let prefs = Cu.cloneInto(
      this.getPrefsForUAWidget(widgetName, prefKeys),
      sandbox
    );

    let widget = new sandbox[widgetName](shadowRoot, prefs);
    if (!isSystemPrincipal) {
      widget = widget.wrappedJSObject;
    }
    this.widgets.set(aElement, widget);
<<<<<<< HEAD
    try {
      if (!isSystemPrincipal) {
        widget.wrappedJSObject.onsetup();
      } else {
        widget.onsetup();
      }
    } catch (ex) {
      Cu.reportError(ex);
    }
||||||| merged common ancestors
=======
    try {
      widget.onsetup();
    } catch (ex) {
      Cu.reportError(ex);
    }
>>>>>>> upstream-releases
  }

  teardownWidget(aElement) {
    let widget = this.widgets.get(aElement);
    if (!widget) {
      return;
    }
<<<<<<< HEAD
    if (typeof widget.wrappedJSObject.destructor == "function") {
      try {
        widget.wrappedJSObject.destructor();
      } catch (ex) {
        Cu.reportError(ex);
      }
||||||| merged common ancestors
    if (typeof widget.wrappedJSObject.destructor == "function") {
      widget.wrappedJSObject.destructor();
=======
    if (typeof widget.destructor == "function") {
      try {
        widget.destructor();
      } catch (ex) {
        Cu.reportError(ex);
      }
>>>>>>> upstream-releases
    }
    this.widgets.delete(aElement);
  }

  getPrefsForUAWidget(aWidgetName, aPrefKeys) {
    let result = this.prefsCache.get(aWidgetName);
    if (result) {
      return result;
    }

    result = {};
    for (let key of aPrefKeys) {
      switch (Services.prefs.getPrefType(key)) {
        case Ci.nsIPrefBranch.PREF_BOOL: {
          result[key] = Services.prefs.getBoolPref(key);
          break;
        }
        case Ci.nsIPrefBranch.PREF_INT: {
          result[key] = Services.prefs.getIntPref(key);
          break;
        }
        case Ci.nsIPrefBranch.PREF_STRING: {
          result[key] = Services.prefs.getStringPref(key);
          break;
        }
      }
    }

    this.prefsCache.set(aWidgetName, result);
    return result;
  }
}
