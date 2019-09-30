let cookieBehavior = BEHAVIOR_REJECT_TRACKER;
let blockingByContentBlockingRTUI = false;
let blockingByAllowList = true;
<<<<<<< HEAD
let expectedBlockingNotifications = Ci.nsIWebProgressListener.STATE_COOKIES_BLOCKED_TRACKER;
||||||| merged common ancestors
let expectedBlockingNotifications = true;
=======
let expectedBlockingNotifications =
  Ci.nsIWebProgressListener.STATE_COOKIES_BLOCKED_TRACKER;
>>>>>>> upstream-releases

let rootDir = getRootDirectory(gTestPath);
let jar = getJar(rootDir);
if (jar) {
  let tmpdir = extractJarToTmp(jar);
  rootDir = "file://" + tmpdir.path + "/";
}
/* import-globals-from imageCacheWorker.js */
Services.scriptloader.loadSubScript(rootDir + "imageCacheWorker.js", this);
