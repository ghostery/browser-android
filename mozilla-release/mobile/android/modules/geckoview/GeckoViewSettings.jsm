/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

var EXPORTED_SYMBOLS = ["GeckoViewSettings"];

const { GeckoViewModule } = ChromeUtils.import(
  "resource://gre/modules/GeckoViewModule.jsm"
);
const { XPCOMUtils } = ChromeUtils.import(
  "resource://gre/modules/XPCOMUtils.jsm"
);

XPCOMUtils.defineLazyGetter(this, "MOBILE_USER_AGENT", function() {
  return Cc["@mozilla.org/network/protocol;1?name=http"].getService(
    Ci.nsIHttpProtocolHandler
  ).userAgent;
});

XPCOMUtils.defineLazyGetter(this, "DESKTOP_USER_AGENT", function() {
  return MOBILE_USER_AGENT.replace(
    /Android \d.+?; [a-zA-Z]+/,
    "X11; Linux x86_64"
  ).replace(/Gecko\/[0-9\.]+/, "Gecko/20100101");
});

<<<<<<< HEAD
XPCOMUtils.defineLazyGetter(
  this, "MOBILE_USER_AGENT",
  function() {
    return Cc["@mozilla.org/network/protocol;1?name=http"]
           .getService(Ci.nsIHttpProtocolHandler).userAgent;
  });

XPCOMUtils.defineLazyGetter(
  this, "DESKTOP_USER_AGENT",
  function() {
    return MOBILE_USER_AGENT
           .replace(/Android \d.+?; [a-zA-Z]+/, "X11; Linux x86_64")
           .replace(/Gecko\/[0-9\.]+/, "Gecko/20100101");
  });

XPCOMUtils.defineLazyGetter(
  this, "VR_USER_AGENT",
  function() {
    return MOBILE_USER_AGENT.replace(/Mobile/, "Mobile VR");
  });
||||||| merged common ancestors
XPCOMUtils.defineLazyGetter(
  this, "MOBILE_USER_AGENT",
  function() {
    return Cc["@mozilla.org/network/protocol;1?name=http"]
           .getService(Ci.nsIHttpProtocolHandler).userAgent;
  });

XPCOMUtils.defineLazyGetter(
  this, "DESKTOP_USER_AGENT",
  function() {
    return MOBILE_USER_AGENT
           .replace(/Android \d.+?; [a-zA-Z]+/, "X11; Linux x86_64")
           .replace(/Gecko\/[0-9\.]+/, "Gecko/20100101");
  });

XPCOMUtils.defineLazyGetter(
  this, "VR_USER_AGENT",
  function() {
    return MOBILE_USER_AGENT
           .getService(Ci.nsIHttpProtocolHandler).userAgent
           .replace(/Android \d+; [a-zA-Z]+/, "VR");
  });
=======
XPCOMUtils.defineLazyGetter(this, "VR_USER_AGENT", function() {
  return MOBILE_USER_AGENT.replace(/Mobile/, "Mobile VR");
});
>>>>>>> upstream-releases

// This needs to match GeckoSessionSettings.java
const USER_AGENT_MODE_MOBILE = 0;
const USER_AGENT_MODE_DESKTOP = 1;
const USER_AGENT_MODE_VR = 2;

// Handles GeckoView settings including:
// * multiprocess
// * user agent override
class GeckoViewSettings extends GeckoViewModule {
  onInit() {
    debug`onInit`;
    this._userAgentMode = USER_AGENT_MODE_MOBILE;
    this._userAgentOverride = null;
    // Required for safe browsing and tracking protection.

    this.registerListener(["GeckoView:GetUserAgent"]);
  }

  onEvent(aEvent, aData, aCallback) {
    debug`onEvent ${aEvent} ${aData}`;

    switch (aEvent) {
      case "GeckoView:GetUserAgent": {
        aCallback.onSuccess(this.userAgent);
      }
    }
  }

  onSettingsUpdate() {
    const settings = this.settings;
    debug`onSettingsUpdate: ${settings}`;

    this.displayMode = settings.displayMode;
    this.userAgentMode = settings.userAgentMode;
    this.userAgentOverride = settings.userAgentOverride;
  }

  get useMultiprocess() {
    return this.browser.isRemoteBrowser;
  }

<<<<<<< HEAD
  observe(aSubject, aTopic, aData) {
    debug `observer`;

    let channel = aSubject.QueryInterface(Ci.nsIHttpChannel);

    if (this.browser.outerWindowID !== channel.topLevelOuterContentWindowId) {
      return;
    }

    if (this.userAgentOverride !== null ||
        this.userAgentMode === USER_AGENT_MODE_DESKTOP ||
        this.userAgentMode === USER_AGENT_MODE_VR) {
      channel.setRequestHeader("User-Agent", this.userAgent, false);
    }
  }

||||||| merged common ancestors
  observe(aSubject, aTopic, aData) {
    debug `observer`;

    let channel = aSubject.QueryInterface(Ci.nsIHttpChannel);

    if (this.browser.outerWindowID !== channel.topLevelOuterContentWindowId) {
      return;
    }

    if (this.userAgentMode === USER_AGENT_MODE_DESKTOP ||
        this.userAgentMode === USER_AGENT_MODE_VR) {
      channel.setRequestHeader("User-Agent", this.userAgent, false);
    }
  }

=======
>>>>>>> upstream-releases
  get userAgent() {
    if (this.userAgentOverride !== null) {
      return this.userAgentOverride;
    }
    if (this.userAgentMode === USER_AGENT_MODE_DESKTOP) {
      return DESKTOP_USER_AGENT;
    }
    if (this.userAgentMode === USER_AGENT_MODE_VR) {
      return VR_USER_AGENT;
    }
    return MOBILE_USER_AGENT;
  }

  get userAgentMode() {
    return this._userAgentMode;
  }

  set userAgentMode(aMode) {
    if (this.userAgentMode === aMode) {
      return;
    }
<<<<<<< HEAD
    this.updateUserAgentObserver(this._userAgentOverride, aMode);
||||||| merged common ancestors
    if (this.userAgentMode === USER_AGENT_MODE_MOBILE) {
      Services.obs.addObserver(this, "http-on-useragent-request");
    } else if (aMode === USER_AGENT_MODE_MOBILE) {
      Services.obs.removeObserver(this, "http-on-useragent-request");
    }
=======
>>>>>>> upstream-releases
    this._userAgentMode = aMode;
  }

<<<<<<< HEAD
  get userAgentOverride() {
    return this._userAgentOverride;
  }

  set userAgentOverride(aUserAgent) {
    this.updateUserAgentObserver(aUserAgent, this._userAgentMode);
    this._userAgentOverride = aUserAgent;
  }

  updateUserAgentObserver(aUserAgent, aMode) {
    const wasAdded = this.userAgentOverride !== null || this.userAgentMode !== USER_AGENT_MODE_MOBILE;
    const shouldAdd = aUserAgent !== null || aMode !== USER_AGENT_MODE_MOBILE;

    try {
      if (wasAdded && !shouldAdd) {
        Services.obs.removeObserver(this, "http-on-useragent-request");
      } else if (!wasAdded && shouldAdd) {
        Services.obs.addObserver(this, "http-on-useragent-request");
      }
    } catch (e) {
      warn `Caught exception while adding/removing "http-on-useragent-request" observer: ${e.message}`;
    }
  }

||||||| merged common ancestors
=======
  get userAgentOverride() {
    return this._userAgentOverride;
  }

  set userAgentOverride(aUserAgent) {
    this._userAgentOverride = aUserAgent;
  }

>>>>>>> upstream-releases
  get displayMode() {
    return this.window.docShell.displayMode;
  }

  set displayMode(aMode) {
    this.window.docShell.displayMode = aMode;
  }
}

const { debug, warn } = GeckoViewSettings.initLogging("GeckoViewSettings"); // eslint-disable-line no-unused-vars
