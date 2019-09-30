/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

var EXPORTED_SYMBOLS = ["LightweightThemeConsumer"];

const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");
const { LightweightThemeManager } = ChromeUtils.import(
  "resource://gre/modules/LightweightThemeManager.jsm"
);
const { ExtensionUtils } = ChromeUtils.import(
  "resource://gre/modules/ExtensionUtils.jsm"
);

<<<<<<< HEAD
ChromeUtils.defineModuleGetter(this, "EventDispatcher",
                               "resource://gre/modules/Messaging.jsm");
ChromeUtils.defineModuleGetter(this, "LightweightThemePersister",
  "resource://gre/modules/addons/LightweightThemePersister.jsm");
||||||| merged common ancestors
ChromeUtils.defineModuleGetter(this, "EventDispatcher",
                               "resource://gre/modules/Messaging.jsm");
=======
ChromeUtils.defineModuleGetter(
  this,
  "EventDispatcher",
  "resource://gre/modules/Messaging.jsm"
);
>>>>>>> upstream-releases

const DEFAULT_THEME_ID = "default-theme@mozilla.org";

let RESOLVE_PROPERTIES = ["headerURL"];

<<<<<<< HEAD
  this._update(LightweightThemeManager.currentThemeWithFallback);
||||||| merged common ancestors
  this._update(LightweightThemeManager.currentThemeForDisplay);
=======
let handlers = new ExtensionUtils.DefaultMap(proto => {
  try {
    return Cc[`@mozilla.org/network/protocol;1?name=${proto}`].getService(
      Ci.nsISubstitutingProtocolHandler
    );
  } catch (e) {
    return null;
  }
});

// The Java front-end code cannot understand internal protocols like
// resource:, so resolve them to their underlying file: or jar: URIs
// when possible.
function maybeResolveURL(url) {
  try {
    let uri = Services.io.newURI(url);
    let handler = handlers.get(uri.scheme);
    if (handler) {
      return handler.resolveURI(uri);
    }
  } catch (e) {
    Cu.reportError(e);
  }
  return url;
>>>>>>> upstream-releases
}

class LightweightThemeConsumer {
  constructor(aDocument) {
    this._doc = aDocument;
    Services.obs.addObserver(this, "lightweight-theme-styling-update");

    this._update(LightweightThemeManager.currentThemeWithFallback);
  }

  observe(aSubject, aTopic, aData) {
    if (aTopic == "lightweight-theme-styling-update") {
<<<<<<< HEAD
      let parsedData = JSON.parse(aData);
      if (!parsedData) {
        parsedData = { theme: null };
      }
      this._update(parsedData.theme);
    } else if (aTopic == "lightweight-theme-apply") {
      this._update(LightweightThemeManager.currentThemeWithFallback);
||||||| merged common ancestors
      let parsedData = JSON.parse(aData);
      if (!parsedData) {
        parsedData = { theme: null };
      }
      this._update(parsedData.theme);
    } else if (aTopic == "lightweight-theme-apply") {
      this._update(LightweightThemeManager.currentThemeForDisplay);
=======
      this._update(aSubject.wrappedJSObject.theme);
>>>>>>> upstream-releases
    }
  }

  destroy() {
    Services.obs.removeObserver(this, "lightweight-theme-styling-update");
    this._doc = null;
  }

  _update(aData) {
    let active = aData && aData.id !== DEFAULT_THEME_ID;
<<<<<<< HEAD
    let msg = active ? { type: "LightweightTheme:Update" } :
                       { type: "LightweightTheme:Disable" };

    if (active) {
      LightweightThemePersister.persistImages(aData, () => {
        msg.data = LightweightThemePersister.getPersistedData(aData);
        EventDispatcher.instance.sendRequest(msg);
      });
    } else {
      EventDispatcher.instance.sendRequest(msg);
    }
  },
};
||||||| merged common ancestors
    if (!aData) {
      aData = {};
    }

    let msg = active ? { type: "LightweightTheme:Update", data: aData } :
                       { type: "LightweightTheme:Disable" };
    EventDispatcher.instance.sendRequest(msg);
  },
};
=======
    let msg = {
      type: active ? "LightweightTheme:Update" : "LightweightTheme:Disable",
    };

    if (active) {
      msg.data = { ...aData };
      for (let prop of RESOLVE_PROPERTIES) {
        if (msg.data[prop]) {
          msg.data[prop] = maybeResolveURL(msg.data[prop]);
        }
      }
    }
    EventDispatcher.instance.sendRequest(msg);
  }
}
>>>>>>> upstream-releases
