/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// exercise nsIHttpResponse.setStatusLine, ensure its atomicity, and ensure the
// specified behavior occurs if it's not called

XPCOMUtils.defineLazyGetter(this, "URL", function() {
  return "http://localhost:" + srv.identity.primaryPort;
});

var srv;

function run_test() {
  srv = createServer();

  srv.registerPathHandler("/no/setstatusline", noSetstatusline);
  srv.registerPathHandler("/http1_0", http1_0);
  srv.registerPathHandler("/http1_1", http1_1);
  srv.registerPathHandler("/invalidVersion", invalidVersion);
  srv.registerPathHandler("/invalidStatus", invalidStatus);
  srv.registerPathHandler("/invalidDescription", invalidDescription);
  srv.registerPathHandler("/crazyCode", crazyCode);
  srv.registerPathHandler("/nullVersion", nullVersion);

  srv.start(-1);

  runHttpTests(tests, testComplete(srv));
}

<<<<<<< HEAD

/** ***********
||||||| merged common ancestors

/*************
=======
/** ***********
>>>>>>> upstream-releases
 * UTILITIES *
 *************/

<<<<<<< HEAD
function checkStatusLine(channel, httpMaxVer, httpMinVer, httpCode, statusText) {
||||||| merged common ancestors
function checkStatusLine(channel, httpMaxVer, httpMinVer, httpCode, statusText)
{
=======
function checkStatusLine(
  channel,
  httpMaxVer,
  httpMinVer,
  httpCode,
  statusText
) {
>>>>>>> upstream-releases
  Assert.equal(channel.responseStatus, httpCode);
  Assert.equal(channel.responseStatusText, statusText);

  var respMaj = {},
    respMin = {};
  channel.getResponseVersion(respMaj, respMin);
  Assert.equal(respMaj.value, httpMaxVer);
  Assert.equal(respMin.value, httpMinVer);
}

<<<<<<< HEAD

/** *******
||||||| merged common ancestors

/*********
=======
/** *******
>>>>>>> upstream-releases
 * TESTS *
 *********/

XPCOMUtils.defineLazyGetter(this, "tests", function() {
  return [
    new Test(URL + "/no/setstatusline", null, startNoSetStatusLine, stop),
    new Test(URL + "/http1_0", null, startHttp1_0, stop),
    new Test(URL + "/http1_1", null, startHttp1_1, stop),
    new Test(URL + "/invalidVersion", null, startPassedTrue, stop),
    new Test(URL + "/invalidStatus", null, startPassedTrue, stop),
    new Test(URL + "/invalidDescription", null, startPassedTrue, stop),
    new Test(URL + "/crazyCode", null, startCrazy, stop),
    new Test(URL + "/nullVersion", null, startNullVersion, stop),
  ];
});

// /no/setstatusline
<<<<<<< HEAD
function noSetstatusline(metadata, response) {
}
function startNoSetStatusLine(ch, cx) {
||||||| merged common ancestors
function noSetstatusline(metadata, response)
{
}
function startNoSetStatusLine(ch, cx)
{
=======
function noSetstatusline(metadata, response) {}
function startNoSetStatusLine(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 200, "OK");
}
<<<<<<< HEAD
function stop(ch, cx, status, data) {
||||||| merged common ancestors
function stop(ch, cx, status, data)
{
=======
function stop(ch, status, data) {
>>>>>>> upstream-releases
  Assert.ok(Components.isSuccessCode(status));
}

// /http1_0
function http1_0(metadata, response) {
  response.setStatusLine("1.0", 200, "OK");
}
<<<<<<< HEAD
function startHttp1_0(ch, cx) {
||||||| merged common ancestors
function startHttp1_0(ch, cx)
{
=======
function startHttp1_0(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 0, 200, "OK");
}

// /http1_1
function http1_1(metadata, response) {
  response.setStatusLine("1.1", 200, "OK");
}
<<<<<<< HEAD
function startHttp1_1(ch, cx) {
||||||| merged common ancestors
function startHttp1_1(ch, cx)
{
=======
function startHttp1_1(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 200, "OK");
}

// /invalidVersion
function invalidVersion(metadata, response) {
  try {
    response.setStatusLine(" 1.0", 200, "FAILED");
  } catch (e) {
    response.setHeader("Passed", "true", false);
  }
}
<<<<<<< HEAD
function startPassedTrue(ch, cx) {
||||||| merged common ancestors
function startPassedTrue(ch, cx)
{
=======
function startPassedTrue(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 200, "OK");
  Assert.equal(ch.getResponseHeader("Passed"), "true");
}

// /invalidStatus
function invalidStatus(metadata, response) {
  try {
    response.setStatusLine("1.0", 1000, "FAILED");
  } catch (e) {
    response.setHeader("Passed", "true", false);
  }
}

// /invalidDescription
function invalidDescription(metadata, response) {
  try {
    response.setStatusLine("1.0", 200, "FAILED\x01");
  } catch (e) {
    response.setHeader("Passed", "true", false);
  }
}

// /crazyCode
function crazyCode(metadata, response) {
  response.setStatusLine("1.1", 617, "Crazy");
}
<<<<<<< HEAD
function startCrazy(ch, cx) {
||||||| merged common ancestors
function startCrazy(ch, cx)
{
=======
function startCrazy(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 617, "Crazy");
}

// /nullVersion
function nullVersion(metadata, response) {
  response.setStatusLine(null, 255, "NULL");
}
<<<<<<< HEAD
function startNullVersion(ch, cx) {
||||||| merged common ancestors
function startNullVersion(ch, cx)
{
=======
function startNullVersion(ch) {
>>>>>>> upstream-releases
  // currently, this server implementation defaults to 1.1
  checkStatusLine(ch, 1, 1, 255, "NULL");
}
