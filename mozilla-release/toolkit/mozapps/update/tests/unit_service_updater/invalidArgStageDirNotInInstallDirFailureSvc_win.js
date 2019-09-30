/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/* Different install and working directories for a regular update failure */

<<<<<<< HEAD
/* The service cannot safely write update.status for this failure because the
 * check is done before validating the installed updater. */
const STATE_AFTER_RUNUPDATE_BASE =
  STATE_FAILED_INVALID_APPLYTO_DIR_STAGED_ERROR;
const STATE_AFTER_RUNUPDATE_SERVICE = AppConstants.EARLY_BETA_OR_EARLIER
    ? STATE_PENDING_SVC
    : STATE_FAILED_SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR;
const STATE_AFTER_RUNUPDATE = IS_SERVICE_TEST ? STATE_AFTER_RUNUPDATE_SERVICE
                                              : STATE_AFTER_RUNUPDATE_BASE;

function run_test() {
||||||| merged common ancestors
const STATE_AFTER_RUNUPDATE =
  IS_SERVICE_TEST ? STATE_FAILED_SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR
                  : STATE_FAILED_INVALID_APPLYTO_DIR_STAGED_ERROR;

function run_test() {
=======
async function run_test() {
>>>>>>> upstream-releases
  if (!setupTestCommon()) {
    return;
  }
  const STATE_AFTER_RUNUPDATE = gIsServiceTest
    ? STATE_FAILED_SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR
    : STATE_FAILED_INVALID_APPLYTO_DIR_STAGED_ERROR;
  gTestFiles = gTestFilesCompleteSuccess;
  gTestDirs = gTestDirsCompleteSuccess;
  setTestFilesAndDirsForFailure();
  await setupUpdaterTest(FILE_COMPLETE_MAR, false);
  let path = getApplyDirFile("..", false).path;
  runUpdate(STATE_AFTER_RUNUPDATE, true, 1, true, null, null, path, null);
  standardInit();
  checkPostUpdateRunningFile(false);
  checkFilesAfterUpdateFailure(getApplyDirFile);
  await waitForUpdateXMLFiles();
  if (gIsServiceTest) {
    checkUpdateManager(
      STATE_NONE,
      false,
      STATE_FAILED,
      SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR,
      1
    );
  } else {
    checkUpdateManager(
      STATE_NONE,
      false,
      STATE_FAILED,
      INVALID_APPLYTO_DIR_STAGED_ERROR,
      1
    );
  }

<<<<<<< HEAD
/**
 * Called after the call to waitForUpdateXMLFiles finishes.
 */
function waitForUpdateXMLFilesFinished() {
  if (IS_SERVICE_TEST) {
    if (AppConstants.EARLY_BETA_OR_EARLIER) {
      checkUpdateManager(STATE_NONE, false, STATE_PENDING_SVC, 0, 1);
    } else {
    checkUpdateManager(STATE_NONE, false, STATE_FAILED,
                       SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR, 1);
    }
  } else {
    checkUpdateManager(STATE_NONE, false, STATE_FAILED,
                       INVALID_APPLYTO_DIR_STAGED_ERROR, 1);
  }

||||||| merged common ancestors
/**
 * Called after the call to waitForUpdateXMLFiles finishes.
 */
function waitForUpdateXMLFilesFinished() {
  let errorCode = IS_SERVICE_TEST ? SERVICE_INVALID_APPLYTO_DIR_STAGED_ERROR
                                  : INVALID_APPLYTO_DIR_STAGED_ERROR;
  checkUpdateManager(STATE_NONE, false, STATE_FAILED, errorCode, 1);
=======
>>>>>>> upstream-releases
  waitForFilesInUse();
}
