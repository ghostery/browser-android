/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Tests that the seizePower API works correctly.
 */

XPCOMUtils.defineLazyGetter(this, "PORT", function() {
  return srv.identity.primaryPort;
});

var srv;

function run_test() {
  srv = createServer();

  srv.registerPathHandler("/raw-data", handleRawData);
  srv.registerPathHandler("/called-too-late", handleTooLate);
  srv.registerPathHandler("/exceptions", handleExceptions);
  srv.registerPathHandler("/async-seizure", handleAsyncSeizure);
  srv.registerPathHandler("/seize-after-async", handleSeizeAfterAsync);

  srv.start(-1);

  runRawTests(tests, testComplete(srv));
}

<<<<<<< HEAD

function checkException(fun, err, msg) {
  try {
||||||| merged common ancestors

function checkException(fun, err, msg)
{
  try
  {
=======
function checkException(fun, err, msg) {
  try {
>>>>>>> upstream-releases
    fun();
<<<<<<< HEAD
  } catch (e) {
    if (e !== err && e.result !== err)
||||||| merged common ancestors
  }
  catch (e)
  {
    if (e !== err && e.result !== err)
=======
  } catch (e) {
    if (e !== err && e.result !== err) {
>>>>>>> upstream-releases
      do_throw(msg);
    }
    return;
  }
  do_throw(msg);
}

function callASAPLater(fun) {
  gThreadManager.dispatchToMainThread({
    run() {
      fun();
    },
  });
}

<<<<<<< HEAD

/** ***************
||||||| merged common ancestors

/*****************
=======
/** ***************
>>>>>>> upstream-releases
 * PATH HANDLERS *
 *****************/

function handleRawData(request, response) {
  response.seizePower();
  response.write("Raw data!");
  response.finish();
}

function handleTooLate(request, response) {
  response.write("DO NOT WANT");
  var output = response.bodyOutputStream;

  response.seizePower();

  if (response.bodyOutputStream !== output) {
    response.write("bodyOutputStream changed!");
  } else {
    response.write("too-late passed");
  }
  response.finish();
}

function handleExceptions(request, response) {
  response.seizePower();
  checkException(
    function() {
      response.setStatusLine("1.0", 500, "ISE");
    },
    Cr.NS_ERROR_NOT_AVAILABLE,
    "setStatusLine should throw not-available after seizePower"
  );
  checkException(
    function() {
      response.setHeader("X-Fail", "FAIL", false);
    },
    Cr.NS_ERROR_NOT_AVAILABLE,
    "setHeader should throw not-available after seizePower"
  );
  checkException(
    function() {
      response.processAsync();
    },
    Cr.NS_ERROR_NOT_AVAILABLE,
    "processAsync should throw not-available after seizePower"
  );
  var out = response.bodyOutputStream;
  var data = "exceptions test passed";
  out.write(data, data.length);
  response.seizePower(); // idempotency test of seizePower
  response.finish();
  response.finish(); // idempotency test of finish after seizePower
  checkException(
    function() {
      response.seizePower();
    },
    Cr.NS_ERROR_UNEXPECTED,
    "seizePower should throw unexpected after finish"
  );
}

function handleAsyncSeizure(request, response) {
  response.seizePower();
  callLater(1, function() {
    response.write("async seizure passed");
    response.bodyOutputStream.close();
    callLater(1, function() {
      response.finish();
    });
  });
}

function handleSeizeAfterAsync(request, response) {
  response.setStatusLine(request.httpVersion, 200, "async seizure pass");
  response.processAsync();
<<<<<<< HEAD
  checkException(function() { response.seizePower(); },
                 Cr.NS_ERROR_NOT_AVAILABLE,
                 "seizePower should throw not-available after processAsync");
  callLater(1, function() {
||||||| merged common ancestors
  checkException(function() { response.seizePower(); },
                 Cr.NS_ERROR_NOT_AVAILABLE,
                 "seizePower should throw not-available after processAsync");
  callLater(1, function()
  {
=======
  checkException(
    function() {
      response.seizePower();
    },
    Cr.NS_ERROR_NOT_AVAILABLE,
    "seizePower should throw not-available after processAsync"
  );
  callLater(1, function() {
>>>>>>> upstream-releases
    response.finish();
  });
}

<<<<<<< HEAD

/** *************
||||||| merged common ancestors

/***************
=======
/** *************
>>>>>>> upstream-releases
 * BEGIN TESTS *
 ***************/

XPCOMUtils.defineLazyGetter(this, "tests", function() {
  return [
    new RawTest("localhost", PORT, data0, checkRawData),
    new RawTest("localhost", PORT, data1, checkTooLate),
    new RawTest("localhost", PORT, data2, checkExceptions),
    new RawTest("localhost", PORT, data3, checkAsyncSeizure),
    new RawTest("localhost", PORT, data4, checkSeizeAfterAsync),
  ];
});

<<<<<<< HEAD
var data0 = "GET /raw-data HTTP/1.0\r\n" +
       "\r\n";
function checkRawData(data) {
||||||| merged common ancestors
var data0 = "GET /raw-data HTTP/1.0\r\n" +
       "\r\n";
function checkRawData(data)
{
=======
// eslint-disable-next-line no-useless-concat
var data0 = "GET /raw-data HTTP/1.0\r\n" + "\r\n";
function checkRawData(data) {
>>>>>>> upstream-releases
  Assert.equal(data, "Raw data!");
}

<<<<<<< HEAD
var data1 = "GET /called-too-late HTTP/1.0\r\n" +
       "\r\n";
function checkTooLate(data) {
||||||| merged common ancestors
var data1 = "GET /called-too-late HTTP/1.0\r\n" +
       "\r\n";
function checkTooLate(data)
{
=======
// eslint-disable-next-line no-useless-concat
var data1 = "GET /called-too-late HTTP/1.0\r\n" + "\r\n";
function checkTooLate(data) {
>>>>>>> upstream-releases
  Assert.equal(LineIterator(data).next().value, "too-late passed");
}

<<<<<<< HEAD
var data2 = "GET /exceptions HTTP/1.0\r\n" +
       "\r\n";
function checkExceptions(data) {
||||||| merged common ancestors
var data2 = "GET /exceptions HTTP/1.0\r\n" +
       "\r\n";
function checkExceptions(data)
{
=======
// eslint-disable-next-line no-useless-concat
var data2 = "GET /exceptions HTTP/1.0\r\n" + "\r\n";
function checkExceptions(data) {
>>>>>>> upstream-releases
  Assert.equal("exceptions test passed", data);
}

<<<<<<< HEAD
var data3 = "GET /async-seizure HTTP/1.0\r\n" +
       "\r\n";
function checkAsyncSeizure(data) {
||||||| merged common ancestors
var data3 = "GET /async-seizure HTTP/1.0\r\n" +
       "\r\n";
function checkAsyncSeizure(data)
{
=======
// eslint-disable-next-line no-useless-concat
var data3 = "GET /async-seizure HTTP/1.0\r\n" + "\r\n";
function checkAsyncSeizure(data) {
>>>>>>> upstream-releases
  Assert.equal(data, "async seizure passed");
}

<<<<<<< HEAD
var data4 = "GET /seize-after-async HTTP/1.0\r\n" +
       "\r\n";
function checkSeizeAfterAsync(data) {
  Assert.equal(LineIterator(data).next().value, "HTTP/1.0 200 async seizure pass");
||||||| merged common ancestors
var data4 = "GET /seize-after-async HTTP/1.0\r\n" +
       "\r\n";
function checkSeizeAfterAsync(data)
{
  Assert.equal(LineIterator(data).next().value, "HTTP/1.0 200 async seizure pass");
=======
// eslint-disable-next-line no-useless-concat
var data4 = "GET /seize-after-async HTTP/1.0\r\n" + "\r\n";
function checkSeizeAfterAsync(data) {
  Assert.equal(
    LineIterator(data).next().value,
    "HTTP/1.0 200 async seizure pass"
  );
>>>>>>> upstream-releases
}
