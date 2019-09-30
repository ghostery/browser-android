<<<<<<< HEAD
ChromeUtils.import("resource://gre/modules/Services.jsm");

||||||| merged common ancestors
=======
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");

>>>>>>> upstream-releases
function run_test() {
  var notifications = 0;
  var obs = {
    observe(aSubject, aTopic, aData) {
      Assert.equal(aTopic, "last-pb-context-exited");
      notifications++;
    },
  };
<<<<<<< HEAD
  Services.os.addObserver(obs, "last-pb-context-exited");

  run_test_in_child("../unit/test_pb_notification.js",
                    function() {
                      Assert.equal(notifications, 1);
                      do_test_finished();
                    });
}
||||||| merged common ancestors
  var os = Cc["@mozilla.org/observer-service;1"].getService(Ci.nsIObserverService);
  os.addObserver(obs, "last-pb-context-exited");
 
  run_test_in_child("../unit/test_pb_notification.js",
                    function() {
                      Assert.equal(notifications, 1);
                      do_test_finished();
                    });
}
=======
  Services.os.addObserver(obs, "last-pb-context-exited");

  run_test_in_child("../unit/test_pb_notification.js", function() {
    Assert.equal(notifications, 1);
    do_test_finished();
  });
}
>>>>>>> upstream-releases
