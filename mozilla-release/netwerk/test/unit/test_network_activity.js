// test for networkactivity
//
const { HttpServer } = ChromeUtils.import("resource://testing-common/httpd.js");

var httpserver = new HttpServer();
var results = [];
var prefs = Cc["@mozilla.org/preferences-service;1"].getService(
  Ci.nsIPrefBranch
);

function createChannel() {
  var chan = NetUtil.newChannel({
    uri: "http://localhost:" + httpserver.identity.primaryPort + "/ok",
    loadUsingSystemPrincipal: true,
  });
  var httpChan = chan.QueryInterface(Ci.nsIHttpChannel);
  return httpChan;
}

function handler(metadata, response) {
  var body = "meh";
  response.setHeader("Content-Type", "text/plain", false);
  response.setStatusLine(metadata.httpVersion, 200, "OK");
  response.bodyOutputStream.write(body, body.length);
}

async function checkValueAndTrigger(request, data) {
  // give Firefox 150 ms to send notifications out
  do_timeout(150, doTest);
}

function doTest() {
<<<<<<< HEAD
    ok(results.length > 0);
    ok(results[0].host == "127.0.0.1");
    ok(results[0].rx > 0 || results[0].tx > 0);
    httpserver.stop(do_test_finished);
||||||| merged common ancestors
    prefs.clearUserPref("network.activity.intervalMilliseconds");
    ok(results.length > 0);
    ok(results[0].host == "127.0.0.1");
    ok(results[0].rx > 0 || results[0].tx > 0);
    httpserver.stop(do_test_finished);
=======
  ok(results.length > 0);
  ok(results[0].host == "127.0.0.1");
  ok(results[0].rx > 0 || results[0].tx > 0);
  httpserver.stop(do_test_finished);
>>>>>>> upstream-releases
}

function run_test() {
  // setting up an observer
  let networkActivity = function(subject, topic, value) {
    subject.QueryInterface(Ci.nsIMutableArray);
    for (let data of subject.enumerate()) {
      results.push(data);
    }
  };

<<<<<<< HEAD
  Services.obs.addObserver(networkActivity, 'network-activity');

||||||| merged common ancestors
  Services.obs.addObserver(networkActivity, 'network-activity');

  // send events every 100ms
  prefs.setIntPref("network.activity.intervalMilliseconds", 100);

=======
  Services.obs.addObserver(networkActivity, "network-activity");

>>>>>>> upstream-releases
  // why do I have to do this ??
  Services.obs.notifyObservers(null, "profile-initial-state");

  do_test_pending();
  httpserver.registerPathHandler("/ok", handler);
  httpserver.start(-1);
  var channel = createChannel();
  channel.asyncOpen(new ChannelListener(checkValueAndTrigger, null));
}
