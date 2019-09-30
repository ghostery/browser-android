// -*- indent-tabs-mode: nil; js-indent-level: 2 -*-
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

var EXPORTED_SYMBOLS = ["RemoteSecurityUI"];

function RemoteSecurityUI() {
<<<<<<< HEAD
    this._secInfo = null;
    this._state = 0;
||||||| merged common ancestors
    this._secInfo = null;
    this._oldState = 0;
    this._state = 0;
    this._contentBlockingLogJSON = "";
=======
  this._secInfo = null;
  this._state = 0;
  this._event = 0;
>>>>>>> upstream-releases
}

RemoteSecurityUI.prototype = {
  QueryInterface: ChromeUtils.generateQI([Ci.nsISecureBrowserUI]),

  // nsISecureBrowserUI
<<<<<<< HEAD
  get state() { return this._state; },
  get tooltipText() { return ""; },
  get secInfo() { return this._secInfo; },
||||||| merged common ancestors
  get oldState() { return this._oldState; },
  get state() { return this._state; },
  get contentBlockingLogJSON() { return this._contentBlockingLogJSON; },
  get tooltipText() { return ""; },
  get secInfo() { return this._secInfo; },
=======
  get state() {
    return this._state;
  },
  get contentBlockingEvent() {
    return this._event;
  },
  get secInfo() {
    return this._secInfo;
  },
>>>>>>> upstream-releases

  _update(aSecInfo, aState) {
    this._secInfo = aSecInfo;
    this._state = aState;
<<<<<<< HEAD
||||||| merged common ancestors
    this._contentBlockingLogJSON = aContentBlockingLogJSON;
=======
  },
  _updateContentBlockingEvent(aEvent) {
    this._event = aEvent;
>>>>>>> upstream-releases
  },
};
