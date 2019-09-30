var gNotifications = 0;

ChromeUtils.import("resource://gre/modules/Services.jsm");

var observer = {
<<<<<<< HEAD
  QueryInterface: ChromeUtils.generateQI(["nsIPrivacyTransitionObserver", "nsISupportsWeakReference"]),

  privateModeChanged(enabled) {
||||||| merged common ancestors
  QueryInterface: function(iid) {
    if (Ci.nsIPrivacyTransitionObserver.equals(iid) ||
        Ci.nsISupportsWeakReference.equals(iid) ||
        Ci.nsISupports.equals(iid))
      return this;
    throw Cr.NS_ERROR_NO_INTERFACE;
  },
  
  privateModeChanged: function(enabled) {
=======
  QueryInterface: ChromeUtils.generateQI([
    "nsIPrivacyTransitionObserver",
    "nsISupportsWeakReference",
  ]),

  privateModeChanged(enabled) {
>>>>>>> upstream-releases
    gNotifications++;
  },
};

function run_test() {
<<<<<<< HEAD
  let windowlessBrowser = Services.appShell.createWindowlessBrowser(false);
  windowlessBrowser.docShell.addWeakPrivacyTransitionObserver(observer);
  windowlessBrowser.docShell.setOriginAttributes({privateBrowsingId: 1});
  windowlessBrowser.docShell.setOriginAttributes({privateBrowsingId: 0});
  windowlessBrowser.close();
||||||| merged common ancestors
  var docshell = Cc["@mozilla.org/docshell;1"].createInstance(Ci.nsIDocShell);
  docshell.addWeakPrivacyTransitionObserver(observer);
  docshell.setOriginAttributes({ privateBrowsingId : 1 });
  docshell.setOriginAttributes({ privateBrowsingId : 0 });
=======
  let windowlessBrowser = Services.appShell.createWindowlessBrowser(false);
  windowlessBrowser.docShell.addWeakPrivacyTransitionObserver(observer);
  windowlessBrowser.docShell.setOriginAttributes({ privateBrowsingId: 1 });
  windowlessBrowser.docShell.setOriginAttributes({ privateBrowsingId: 0 });
  windowlessBrowser.close();
>>>>>>> upstream-releases
  Assert.equal(gNotifications, 2);
}
