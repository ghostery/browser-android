<<<<<<< HEAD
/* eslint-env mozilla/frame-script */
||||||| merged common ancestors
=======
/* eslint-env mozilla/frame-script */
// eslint-disable-next-line mozilla/reject-importGlobalProperties
>>>>>>> upstream-releases
Cu.importGlobalProperties(["File"]);
<<<<<<< HEAD
ChromeUtils.import("resource://gre/modules/Services.jsm");
||||||| merged common ancestors

=======
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");
>>>>>>> upstream-releases
function createProfDFile() {
<<<<<<< HEAD
  return Services.dirsvc.QueryInterface(Ci.nsIProperties).get("ProfD", Ci.nsIFile);
||||||| merged common ancestors
  return Cc["@mozilla.org/file/directory_service;1"]
           .getService(Ci.nsIDirectoryService)
           .QueryInterface(Ci.nsIProperties)
           .get('ProfD', Ci.nsIFile);
=======
  return Services.dirsvc
    .QueryInterface(Ci.nsIProperties)
    .get("ProfD", Ci.nsIFile);
>>>>>>> upstream-releases
}

// Creates a parametric arity directory hierarchy as a function of depth.
// Each directory contains one leaf file, and subdirectories of depth [1, depth).
// e.g. for depth 3:
//
// subdir3
// - file.txt
// - subdir2
//   - file.txt
//   - subdir1
//     - file.txt
// - subdir1
//   - file.txt
//
// Returns the parent directory of the subtree.
function createTreeFile(depth, parent) {
  if (!parent) {
<<<<<<< HEAD
    parent = Services.dirsvc.QueryInterface(Ci.nsIProperties).get("TmpD", Ci.nsIFile);
    parent.append("dir-tree-test");
||||||| merged common ancestors
    parent = Cc["@mozilla.org/file/directory_service;1"]
                .getService(Ci.nsIDirectoryService)
                .QueryInterface(Ci.nsIProperties)
                .get('TmpD', Ci.nsIFile);
    parent.append('dir-tree-test');
=======
    parent = Services.dirsvc
      .QueryInterface(Ci.nsIProperties)
      .get("TmpD", Ci.nsIFile);
    parent.append("dir-tree-test");
>>>>>>> upstream-releases
    parent.createUnique(Ci.nsIFile.DIRECTORY_TYPE, 0o700);
  }

  var nextFile = parent.clone();
  if (depth == 0) {
    nextFile.append("file.txt");
    nextFile.create(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);
  } else {
    nextFile.append("subdir" + depth);
    nextFile.createUnique(Ci.nsIFile.DIRECTORY_TYPE, 0o700);
    // Decrement the maximal depth by one for each level of nesting.
    for (var i = 0; i < depth; i++) {
      createTreeFile(i, nextFile);
    }
  }

  return parent;
}

function createRootFile() {
  var testFile = createProfDFile();

  // Let's go back to the root of the FileSystem
  while (true) {
    var parent = testFile.parent;
    if (!parent) {
      break;
    }

    testFile = parent;
  }

  return testFile;
}

function createTestFile() {
<<<<<<< HEAD
  var tmpFile = Services.dirsvc.QueryInterface(Ci.nsIProperties).get("TmpD", Ci.nsIFile);
  tmpFile.append("dir-test");
||||||| merged common ancestors
  var tmpFile = Cc["@mozilla.org/file/directory_service;1"]
                  .getService(Ci.nsIDirectoryService)
                  .QueryInterface(Ci.nsIProperties)
                  .get('TmpD', Ci.nsIFile)
  tmpFile.append('dir-test');
=======
  var tmpFile = Services.dirsvc
    .QueryInterface(Ci.nsIProperties)
    .get("TmpD", Ci.nsIFile);
  tmpFile.append("dir-test");
>>>>>>> upstream-releases
  tmpFile.createUnique(Ci.nsIFile.DIRECTORY_TYPE, 0o700);

  var file1 = tmpFile.clone();
  file1.append("foo.txt");
  file1.create(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);

  var dir = tmpFile.clone();
  dir.append("subdir");
  dir.create(Ci.nsIFile.DIRECTORY_TYPE, 0o700);

  var file2 = dir.clone();
  file2.append("bar.txt");
  file2.create(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);

  return tmpFile;
}

addMessageListener("dir.open", function(e) {
  var testFile;

  switch (e.path) {
    case "ProfD":
      // Note that files in the profile directory are not guaranteed to persist-
      // see bug 1284742.
      testFile = createProfDFile();
      break;

    case "root":
      testFile = createRootFile();
      break;

    case "test":
      testFile = createTestFile();
      break;

    case "tree":
      testFile = createTreeFile(3);
      break;
  }

  sendAsyncMessage("dir.opened", {
    dir: testFile.path,
    name: testFile.leafName,
  });
});

<<<<<<< HEAD
addMessageListener("file.open", function(e) {
  var testFile = Services.dirsvc.QueryInterface(Ci.nsIProperties).get("ProfD", Ci.nsIFile);
||||||| merged common ancestors
addMessageListener("file.open", function (e) {
  var testFile = Cc["@mozilla.org/file/directory_service;1"]
                   .getService(Ci.nsIDirectoryService)
                   .QueryInterface(Ci.nsIProperties)
                   .get("ProfD", Ci.nsIFile);
=======
addMessageListener("file.open", function(e) {
  var testFile = Services.dirsvc
    .QueryInterface(Ci.nsIProperties)
    .get("ProfD", Ci.nsIFile);
>>>>>>> upstream-releases
  testFile.append("prefs.js");

  File.createFromNsIFile(testFile).then(function(file) {
    sendAsyncMessage("file.opened", { file });
  });
});
