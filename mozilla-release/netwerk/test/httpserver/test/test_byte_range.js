/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// checks if a byte range request and non-byte range request retrieve the
// correct data.

var srv;
XPCOMUtils.defineLazyGetter(this, "PREFIX", function() {
  return "http://localhost:" + srv.identity.primaryPort;
});

XPCOMUtils.defineLazyGetter(this, "tests", function() {
  return [
<<<<<<< HEAD
   new Test(PREFIX + "/range.txt",
            init_byterange, start_byterange, stop_byterange),
   new Test(PREFIX + "/range.txt",
            init_byterange2, start_byterange2),
   new Test(PREFIX + "/range.txt",
            init_byterange3, start_byterange3, stop_byterange3),
   new Test(PREFIX + "/range.txt",
            init_byterange4, start_byterange4),
   new Test(PREFIX + "/range.txt",
            init_byterange5, start_byterange5, stop_byterange5),
   new Test(PREFIX + "/range.txt",
            init_byterange6, start_byterange6, stop_byterange6),
   new Test(PREFIX + "/range.txt",
            init_byterange7, start_byterange7, stop_byterange7),
   new Test(PREFIX + "/range.txt",
            init_byterange8, start_byterange8, stop_byterange8),
   new Test(PREFIX + "/range.txt",
            init_byterange9, start_byterange9, stop_byterange9),
   new Test(PREFIX + "/range.txt",
            init_byterange10, start_byterange10),
   new Test(PREFIX + "/range.txt",
            init_byterange11, start_byterange11, stop_byterange11),
   new Test(PREFIX + "/empty.txt",
            null, start_byterange12, stop_byterange12),
   new Test(PREFIX + "/headers.txt",
            init_byterange13, start_byterange13, null),
   new Test(PREFIX + "/range.txt",
            null, start_normal, stop_normal),
   ];
||||||| merged common ancestors
   new Test(PREFIX + "/range.txt",
            init_byterange, start_byterange, stop_byterange),
   new Test(PREFIX + "/range.txt",
            init_byterange2, start_byterange2),
   new Test(PREFIX + "/range.txt",
            init_byterange3, start_byterange3, stop_byterange3),
   new Test(PREFIX + "/range.txt",
            init_byterange4, start_byterange4),
   new Test(PREFIX + "/range.txt",
            init_byterange5, start_byterange5, stop_byterange5),
   new Test(PREFIX + "/range.txt",
            init_byterange6, start_byterange6, stop_byterange6),
   new Test(PREFIX + "/range.txt",
            init_byterange7, start_byterange7, stop_byterange7),
   new Test(PREFIX + "/range.txt",
            init_byterange8, start_byterange8, stop_byterange8),
   new Test(PREFIX + "/range.txt",
            init_byterange9, start_byterange9, stop_byterange9),
   new Test(PREFIX + "/range.txt",
            init_byterange10, start_byterange10),
   new Test(PREFIX + "/range.txt",
            init_byterange11, start_byterange11, stop_byterange11),
   new Test(PREFIX + "/empty.txt",
            null, start_byterange12, stop_byterange12),
   new Test(PREFIX + "/headers.txt",
            init_byterange13, start_byterange13, null),
   new Test(PREFIX + "/range.txt",
            null, start_normal, stop_normal)
   ];
=======
    new Test(
      PREFIX + "/range.txt",
      init_byterange,
      start_byterange,
      stop_byterange
    ),
    new Test(PREFIX + "/range.txt", init_byterange2, start_byterange2),
    new Test(
      PREFIX + "/range.txt",
      init_byterange3,
      start_byterange3,
      stop_byterange3
    ),
    new Test(PREFIX + "/range.txt", init_byterange4, start_byterange4),
    new Test(
      PREFIX + "/range.txt",
      init_byterange5,
      start_byterange5,
      stop_byterange5
    ),
    new Test(
      PREFIX + "/range.txt",
      init_byterange6,
      start_byterange6,
      stop_byterange6
    ),
    new Test(
      PREFIX + "/range.txt",
      init_byterange7,
      start_byterange7,
      stop_byterange7
    ),
    new Test(
      PREFIX + "/range.txt",
      init_byterange8,
      start_byterange8,
      stop_byterange8
    ),
    new Test(
      PREFIX + "/range.txt",
      init_byterange9,
      start_byterange9,
      stop_byterange9
    ),
    new Test(PREFIX + "/range.txt", init_byterange10, start_byterange10),
    new Test(
      PREFIX + "/range.txt",
      init_byterange11,
      start_byterange11,
      stop_byterange11
    ),
    new Test(PREFIX + "/empty.txt", null, start_byterange12, stop_byterange12),
    new Test(
      PREFIX + "/headers.txt",
      init_byterange13,
      start_byterange13,
      null
    ),
    new Test(PREFIX + "/range.txt", null, start_normal, stop_normal),
  ];
>>>>>>> upstream-releases
});

function run_test() {
  srv = createServer();
  var dir = do_get_file("data/ranges/");
  srv.registerDirectory("/", dir);

  srv.start(-1);

  runHttpTests(tests, testComplete(srv));
}

<<<<<<< HEAD
function start_normal(ch, cx) {
||||||| merged common ancestors
function start_normal(ch, cx)
{
=======
function start_normal(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 200);
  Assert.equal(ch.getResponseHeader("Content-Length"), "21");
  Assert.equal(ch.getResponseHeader("Content-Type"), "text/plain");
}

<<<<<<< HEAD
function stop_normal(ch, cx, status, data) {
||||||| merged common ancestors
function stop_normal(ch, cx, status, data)
{
=======
function stop_normal(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 21);
  Assert.equal(data[0], 0x54);
  Assert.equal(data[20], 0x0a);
}

function init_byterange(ch) {
  ch.setRequestHeader("Range", "bytes=10-", false);
}

<<<<<<< HEAD
function start_byterange(ch, cx) {
||||||| merged common ancestors
function start_byterange(ch, cx)
{
=======
function start_byterange(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
  Assert.equal(ch.getResponseHeader("Content-Length"), "11");
  Assert.equal(ch.getResponseHeader("Content-Type"), "text/plain");
  Assert.equal(ch.getResponseHeader("Content-Range"), "bytes 10-20/21");
}

<<<<<<< HEAD
function stop_byterange(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange(ch, cx, status, data)
{
=======
function stop_byterange(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 11);
  Assert.equal(data[0], 0x64);
  Assert.equal(data[10], 0x0a);
}

function init_byterange2(ch) {
  ch.setRequestHeader("Range", "bytes=21-", false);
}

<<<<<<< HEAD
function start_byterange2(ch, cx) {
||||||| merged common ancestors
function start_byterange2(ch, cx)
{
=======
function start_byterange2(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 416);
}

function init_byterange3(ch) {
  ch.setRequestHeader("Range", "bytes=10-15", false);
}

<<<<<<< HEAD
function start_byterange3(ch, cx) {
||||||| merged common ancestors
function start_byterange3(ch, cx)
{
=======
function start_byterange3(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
  Assert.equal(ch.getResponseHeader("Content-Length"), "6");
  Assert.equal(ch.getResponseHeader("Content-Type"), "text/plain");
  Assert.equal(ch.getResponseHeader("Content-Range"), "bytes 10-15/21");
}

<<<<<<< HEAD
function stop_byterange3(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange3(ch, cx, status, data)
{
=======
function stop_byterange3(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 6);
  Assert.equal(data[0], 0x64);
  Assert.equal(data[1], 0x20);
  Assert.equal(data[2], 0x62);
  Assert.equal(data[3], 0x65);
  Assert.equal(data[4], 0x20);
  Assert.equal(data[5], 0x73);
}

function init_byterange4(ch) {
  ch.setRequestHeader("Range", "xbytes=21-", false);
}

<<<<<<< HEAD
function start_byterange4(ch, cx) {
||||||| merged common ancestors
function start_byterange4(ch, cx)
{
=======
function start_byterange4(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 400);
}

function init_byterange5(ch) {
  ch.setRequestHeader("Range", "bytes=-5", false);
}

<<<<<<< HEAD
function start_byterange5(ch, cx) {
||||||| merged common ancestors
function start_byterange5(ch, cx)
{
=======
function start_byterange5(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
}

<<<<<<< HEAD
function stop_byterange5(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange5(ch, cx, status, data)
{
=======
function stop_byterange5(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 5);
  Assert.equal(data[0], 0x65);
  Assert.equal(data[1], 0x65);
  Assert.equal(data[2], 0x6e);
  Assert.equal(data[3], 0x2e);
  Assert.equal(data[4], 0x0a);
}

function init_byterange6(ch) {
  ch.setRequestHeader("Range", "bytes=15-12", false);
}

<<<<<<< HEAD
function start_byterange6(ch, cx) {
||||||| merged common ancestors
function start_byterange6(ch, cx)
{
=======
function start_byterange6(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 200);
}

<<<<<<< HEAD
function stop_byterange6(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange6(ch, cx, status, data)
{
=======
function stop_byterange6(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 21);
  Assert.equal(data[0], 0x54);
  Assert.equal(data[20], 0x0a);
}

function init_byterange7(ch) {
  ch.setRequestHeader("Range", "bytes=0-5", false);
}

<<<<<<< HEAD
function start_byterange7(ch, cx) {
||||||| merged common ancestors
function start_byterange7(ch, cx)
{
=======
function start_byterange7(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
  Assert.equal(ch.getResponseHeader("Content-Length"), "6");
  Assert.equal(ch.getResponseHeader("Content-Type"), "text/plain");
  Assert.equal(ch.getResponseHeader("Content-Range"), "bytes 0-5/21");
}

<<<<<<< HEAD
function stop_byterange7(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange7(ch, cx, status, data)
{
=======
function stop_byterange7(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 6);
  Assert.equal(data[0], 0x54);
  Assert.equal(data[1], 0x68);
  Assert.equal(data[2], 0x69);
  Assert.equal(data[3], 0x73);
  Assert.equal(data[4], 0x20);
  Assert.equal(data[5], 0x73);
}

function init_byterange8(ch) {
  ch.setRequestHeader("Range", "bytes=20-21", false);
}

<<<<<<< HEAD
function start_byterange8(ch, cx) {
||||||| merged common ancestors
function start_byterange8(ch, cx)
{
=======
function start_byterange8(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
  Assert.equal(ch.getResponseHeader("Content-Range"), "bytes 20-20/21");
}

<<<<<<< HEAD
function stop_byterange8(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange8(ch, cx, status, data)
{
=======
function stop_byterange8(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 1);
  Assert.equal(data[0], 0x0a);
}

function init_byterange9(ch) {
  ch.setRequestHeader("Range", "bytes=020-021", false);
}

<<<<<<< HEAD
function start_byterange9(ch, cx) {
||||||| merged common ancestors
function start_byterange9(ch, cx)
{
=======
function start_byterange9(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
}

<<<<<<< HEAD
function stop_byterange9(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange9(ch, cx, status, data)
{
=======
function stop_byterange9(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 1);
  Assert.equal(data[0], 0x0a);
}

function init_byterange10(ch) {
  ch.setRequestHeader("Range", "bytes=-", false);
}

<<<<<<< HEAD
function start_byterange10(ch, cx) {
||||||| merged common ancestors
function start_byterange10(ch, cx)
{
=======
function start_byterange10(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 400);
}

function init_byterange11(ch) {
  ch.setRequestHeader("Range", "bytes=-500", false);
}

<<<<<<< HEAD
function start_byterange11(ch, cx) {
||||||| merged common ancestors
function start_byterange11(ch, cx)
{
=======
function start_byterange11(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 206);
}

<<<<<<< HEAD
function stop_byterange11(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange11(ch, cx, status, data)
{
=======
function stop_byterange11(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 21);
  Assert.equal(data[0], 0x54);
  Assert.equal(data[20], 0x0a);
}

<<<<<<< HEAD
function start_byterange12(ch, cx) {
||||||| merged common ancestors
function start_byterange12(ch, cx)
{
=======
function start_byterange12(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 200);
  Assert.equal(ch.getResponseHeader("Content-Length"), "0");
}

<<<<<<< HEAD
function stop_byterange12(ch, cx, status, data) {
||||||| merged common ancestors
function stop_byterange12(ch, cx, status, data)
{
=======
function stop_byterange12(ch, status, data) {
>>>>>>> upstream-releases
  Assert.equal(data.length, 0);
}

function init_byterange13(ch) {
  ch.setRequestHeader("Range", "bytes=9999999-", false);
}

<<<<<<< HEAD
function start_byterange13(ch, cx) {
||||||| merged common ancestors
function start_byterange13(ch, cx)
{
=======
function start_byterange13(ch) {
>>>>>>> upstream-releases
  Assert.equal(ch.responseStatus, 416);
  Assert.equal(ch.getResponseHeader("X-SJS-Header"), "customized");
}
