/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Request handlers may throw exceptions, and those exception should be caught
// by the server and converted into the proper error codes.

XPCOMUtils.defineLazyGetter(this, "tests", function() {
  return [
    new Test(
      "http://localhost:" + srv.identity.primaryPort + "/throws/exception",
      null,
      start_throws_exception,
      succeeded
    ),
    new Test(
      "http://localhost:" +
        srv.identity.primaryPort +
        "/this/file/does/not/exist/and/404s",
      null,
      start_nonexistent_404_fails_so_400,
      succeeded
    ),
    new Test(
      "http://localhost:" +
        srv.identity.primaryPort +
        "/attempts/404/fails/so/400/fails/so/500s",
      register400Handler,
      start_multiple_exceptions_500,
      succeeded
    ),
  ];
});

var srv;

function run_test() {
  srv = createServer();

  srv.registerErrorHandler(404, throwsException);
  srv.registerPathHandler("/throws/exception", throwsException);

  srv.start(-1);

  runHttpTests(tests, testComplete(srv));
}

// TEST DATA

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
function start_throws_exception(ch, cx) {
||||||| merged common ancestors
function start_throws_exception(ch, cx)
{
=======
function start_throws_exception(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 500, "Internal Server Error");
}

<<<<<<< HEAD
function start_nonexistent_404_fails_so_400(ch, cx) {
||||||| merged common ancestors
function start_nonexistent_404_fails_so_400(ch, cx)
{
=======
function start_nonexistent_404_fails_so_400(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 400, "Bad Request");
}

<<<<<<< HEAD
function start_multiple_exceptions_500(ch, cx) {
||||||| merged common ancestors
function start_multiple_exceptions_500(ch, cx)
{
=======
function start_multiple_exceptions_500(ch) {
>>>>>>> upstream-releases
  checkStatusLine(ch, 1, 1, 500, "Internal Server Error");
}

<<<<<<< HEAD
function succeeded(ch, cx, status, data) {
||||||| merged common ancestors
function succeeded(ch, cx, status, data)
{
=======
function succeeded(ch, status, data) {
>>>>>>> upstream-releases
  Assert.ok(Components.isSuccessCode(status));
}

function register400Handler(ch) {
  srv.registerErrorHandler(400, throwsException);
}

// PATH HANDLERS

// /throws/exception (and also a 404 and 400 error handler)
<<<<<<< HEAD
function throwsException(metadata, response) {
  throw "this shouldn't cause an exit...";
  do_throw("Not reached!"); // eslint-disable-line no-unreachable
||||||| merged common ancestors
function throwsException(metadata, response)
{
  throw "this shouldn't cause an exit...";
  do_throw("Not reached!");
=======
function throwsException(metadata, response) {
  throw new Error("this shouldn't cause an exit...");
  do_throw("Not reached!"); // eslint-disable-line no-unreachable
>>>>>>> upstream-releases
}
