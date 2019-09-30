const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");

const URI = Services.io.newURI("http://example.org/");

const cs = Cc["@mozilla.org/cookieService;1"].getService(Ci.nsICookieService);

function run_test() {
  // Allow all cookies.
  Services.prefs.setIntPref("network.cookie.cookieBehavior", 0);

  // Clear cookies.
  Services.cookies.removeAll();

  // Add a new cookie.
  setCookie("foo=bar", {
<<<<<<< HEAD
    type: "added", isSession: true, isSecure: false, isHttpOnly: false,
||||||| merged common ancestors
    type: "added", isSession: true, isSecure: false, isHttpOnly: false
=======
    type: "added",
    isSession: true,
    isSecure: false,
    isHttpOnly: false,
>>>>>>> upstream-releases
  });

  // Update cookie with isHttpOnly=true.
  setCookie("foo=bar; HttpOnly", {
<<<<<<< HEAD
    type: "changed", isSession: true, isSecure: false, isHttpOnly: true,
||||||| merged common ancestors
    type: "changed", isSession: true, isSecure: false, isHttpOnly: true
=======
    type: "changed",
    isSession: true,
    isSecure: false,
    isHttpOnly: true,
>>>>>>> upstream-releases
  });

  // Update cookie with isSecure=true.
  setCookie("foo=bar; Secure", {
<<<<<<< HEAD
    type: "changed", isSession: true, isSecure: true, isHttpOnly: false,
||||||| merged common ancestors
    type: "changed", isSession: true, isSecure: true, isHttpOnly: false
=======
    type: "changed",
    isSession: true,
    isSecure: true,
    isHttpOnly: false,
>>>>>>> upstream-releases
  });

  // Update cookie with isSession=false.
  let expiry = new Date();
  expiry.setUTCFullYear(expiry.getUTCFullYear() + 2);
  setCookie(`foo=bar; Expires=${expiry.toGMTString()}`, {
<<<<<<< HEAD
    type: "changed", isSession: false, isSecure: false, isHttpOnly: false,
||||||| merged common ancestors
    type: "changed", isSession: false, isSecure: false, isHttpOnly: false
=======
    type: "changed",
    isSession: false,
    isSecure: false,
    isHttpOnly: false,
>>>>>>> upstream-releases
  });

  // Reset cookie.
  setCookie("foo=bar", {
<<<<<<< HEAD
    type: "changed", isSession: true, isSecure: false, isHttpOnly: false,
||||||| merged common ancestors
    type: "changed", isSession: true, isSecure: false, isHttpOnly: false
=======
    type: "changed",
    isSession: true,
    isSecure: false,
    isHttpOnly: false,
>>>>>>> upstream-releases
  });
}

function setCookie(value, expected) {
  function setCookieInternal(valueInternal, expectedInternal = null) {
    function observer(subject, topic, data) {
      if (!expectedInternal) {
        do_throw("no notification expected");
        return;
      }

      // Check we saw the right notification.
      Assert.equal(data, expectedInternal.type);

      // Check cookie details.
<<<<<<< HEAD
      let cookie = subject.QueryInterface(Ci.nsICookie2);
      Assert.equal(cookie.isSession, expectedInternal.isSession);
      Assert.equal(cookie.isSecure, expectedInternal.isSecure);
      Assert.equal(cookie.isHttpOnly, expectedInternal.isHttpOnly);
||||||| merged common ancestors
      let cookie = subject.QueryInterface(Ci.nsICookie2);
      Assert.equal(cookie.isSession, expected.isSession);
      Assert.equal(cookie.isSecure, expected.isSecure);
      Assert.equal(cookie.isHttpOnly, expected.isHttpOnly);
=======
      let cookie = subject.QueryInterface(Ci.nsICookie);
      Assert.equal(cookie.isSession, expectedInternal.isSession);
      Assert.equal(cookie.isSecure, expectedInternal.isSecure);
      Assert.equal(cookie.isHttpOnly, expectedInternal.isHttpOnly);
>>>>>>> upstream-releases
    }

    Services.obs.addObserver(observer, "cookie-changed");
    cs.setCookieStringFromHttp(URI, null, null, valueInternal, null, null);
    Services.obs.removeObserver(observer, "cookie-changed");
  }

  // Check that updating/inserting the cookie works.
  setCookieInternal(value, expected);

  // Check that we ignore identical cookies.
  setCookieInternal(value);
}
