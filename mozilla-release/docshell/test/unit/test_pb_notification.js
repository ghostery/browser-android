function destroy_transient_docshell() {
<<<<<<< HEAD
  let windowlessBrowser = Services.appShell.createWindowlessBrowser(false);
  windowlessBrowser.docShell.setOriginAttributes({privateBrowsingId: 1});
  windowlessBrowser.close();
||||||| merged common ancestors
  var docshell = Cc["@mozilla.org/docshell;1"].createInstance(Ci.nsIDocShell);
  docshell.setOriginAttributes({privateBrowsingId : 1});
=======
  let windowlessBrowser = Services.appShell.createWindowlessBrowser(false);
  windowlessBrowser.docShell.setOriginAttributes({ privateBrowsingId: 1 });
  windowlessBrowser.close();
>>>>>>> upstream-releases
  do_test_pending();
  do_timeout(0, Cu.forceGC);
}

function run_test() {
  var obs = {
    observe(aSubject, aTopic, aData) {
      Assert.equal(aTopic, "last-pb-context-exited");
      do_test_finished();
    },
  };
  Services.obs.addObserver(obs, "last-pb-context-exited");
  destroy_transient_docshell();
}
