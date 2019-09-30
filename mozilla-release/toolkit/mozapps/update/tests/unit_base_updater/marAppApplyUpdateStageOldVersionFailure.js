/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

/**
 * Test a replace request for a staged update with a version file that specifies
 * an older version failure. The same check is used in nsUpdateDriver.cpp for
 * all update types which is why there aren't tests for the maintenance service
 * as well as for other update types.
 */

async function run_test() {
  if (!setupTestCommon()) {
    return;
  }
  const STATE_AFTER_STAGE = STATE_APPLIED;
  gTestFiles = gTestFilesCompleteSuccess;
  gTestDirs = gTestDirsCompleteSuccess;
<<<<<<< HEAD
  setupUpdaterTest(FILE_COMPLETE_MAR, null, "", false);
}

/**
 * Called after the call to setupUpdaterTest finishes.
 */
function setupUpdaterTestFinished() {
  let patchProps = {state: STATE_AFTER_STAGE};
  let patches = getLocalPatchString(patchProps);
  let updateProps = {appVersion: "0.9"};
  let updates = getLocalUpdateString(updateProps, patches);
  writeUpdatesToXMLFile(getLocalUpdatesXMLString(updates), true);
  getUpdateLog(FILE_UPDATE_LOG).create(Ci.nsIFile.NORMAL_FILE_TYPE, PERMS_FILE);
  writeStatusFile(STATE_AFTER_STAGE);
  // Create the version file with an older version to simulate installing a new
  // version of the application while there is an update that has been staged.
  writeVersionFile("0.9");
  // Try to switch the application to the fake staged application.
  runUpdateUsingApp(STATE_AFTER_STAGE);
}

/**
 * Called after the call to runUpdateUsingApp finishes.
 */
function runUpdateFinished() {
||||||| merged common ancestors
  setupUpdaterTest(FILE_COMPLETE_MAR);
}

/**
 * Called after the call to setupUpdaterTest finishes.
 */
function setupUpdaterTestFinished() {
  stageUpdate(false);
}

/**
 * Called after the call to stageUpdate finishes.
 */
function stageUpdateFinished() {
  checkPostUpdateRunningFile(false);
  checkFilesAfterUpdateSuccess(getStageDirFile, true);
  checkUpdateLogContents(LOG_COMPLETE_SUCCESS, true);
  // Change the version file to an older version to simulate installing a new
  // version of the application while there is an update that has been staged.
  writeVersionFile("0.9");
  // Try to switch the application to the staged application that was updated.
  runUpdateUsingApp(STATE_AFTER_STAGE);
}

/**
 * Called after the call to runUpdateUsingApp finishes.
 */
function runUpdateFinished() {
  // Change the active update to an older version to simulate installing a new
  // version of the application while there is an update that has been staged.
  let patchProps = {state: STATE_AFTER_STAGE};
  let patches = getLocalPatchString(patchProps);
  let updateProps = {appVersion: "0.9"};
  let updates = getLocalUpdateString(updateProps, patches);
  getUpdatesXMLFile(true).remove(false);
  writeUpdatesToXMLFile(getLocalUpdatesXMLString(updates), true);
  reloadUpdateManagerData();

=======
  await setupUpdaterTest(FILE_COMPLETE_MAR, null, "", false);
  let patchProps = { state: STATE_AFTER_STAGE };
  let patches = getLocalPatchString(patchProps);
  let updateProps = { appVersion: "0.9" };
  let updates = getLocalUpdateString(updateProps, patches);
  writeUpdatesToXMLFile(getLocalUpdatesXMLString(updates), true);
  getUpdateDirFile(FILE_UPDATE_LOG).create(
    Ci.nsIFile.NORMAL_FILE_TYPE,
    PERMS_FILE
  );
  writeStatusFile(STATE_AFTER_STAGE);
  // Create the version file with an older version to simulate installing a new
  // version of the application while there is an update that has been staged.
  writeVersionFile("0.9");
  // Try to switch the application to the fake staged application.
  await runUpdateUsingApp(STATE_AFTER_STAGE);
>>>>>>> upstream-releases
  standardInit();
  checkPostUpdateRunningFile(false);
  setTestFilesAndDirsForFailure();
<<<<<<< HEAD
  checkFilesAfterUpdateFailure(getApplyDirFile);

  executeSoon(waitForUpdateXMLFiles);
}

/**
 * Called after the call to waitForUpdateXMLFiles finishes.
 */
function waitForUpdateXMLFilesFinished() {
  checkUpdateManager(STATE_NONE, false, STATE_FAILED,
                     ERR_OLDER_VERSION_OR_SAME_BUILD, 1);
||||||| merged common ancestors
  checkFilesAfterUpdateFailure(getApplyDirFile, !IS_MACOSX, false);

  executeSoon(waitForUpdateXMLFiles);
}

/**
 * Called after the call to waitForUpdateXMLFiles finishes.
 */
function waitForUpdateXMLFilesFinished() {
  checkUpdateManager(STATE_NONE, false, STATE_FAILED,
                     ERR_OLDER_VERSION_OR_SAME_BUILD, 1);
=======
  checkFilesAfterUpdateFailure(getApplyDirFile);
  await waitForUpdateXMLFiles();
  checkUpdateManager(
    STATE_NONE,
    false,
    STATE_FAILED,
    ERR_OLDER_VERSION_OR_SAME_BUILD,
    1
  );
>>>>>>> upstream-releases

  let updatesDir = getUpdateDirFile(DIR_PATCH);
  Assert.ok(
    updatesDir.exists(),
    MSG_SHOULD_EXIST + getMsgPath(updatesDir.path)
  );

  let log = getUpdateDirFile(FILE_UPDATE_LOG);
  Assert.ok(!log.exists(), MSG_SHOULD_NOT_EXIST + getMsgPath(log.path));

  log = getUpdateDirFile(FILE_LAST_UPDATE_LOG);
  Assert.ok(log.exists(), MSG_SHOULD_EXIST + getMsgPath(log.path));

  log = getUpdateDirFile(FILE_BACKUP_UPDATE_LOG);
  Assert.ok(!log.exists(), MSG_SHOULD_NOT_EXIST + getMsgPath(log.path));

  waitForFilesInUse();
}
