<<<<<<< HEAD
/* eslint-env mozilla/frame-script */
||||||| merged common ancestors
=======
/* eslint-env mozilla/frame-script */
// eslint-disable-next-line mozilla/reject-importGlobalProperties
>>>>>>> upstream-releases
Cu.importGlobalProperties(["File", "Directory"]);
<<<<<<< HEAD
ChromeUtils.import("resource://gre/modules/Services.jsm");
||||||| merged common ancestors

=======
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");
>>>>>>> upstream-releases
var tmpFile, tmpDir;

<<<<<<< HEAD
addMessageListener("entries.open", function(e) {
  tmpFile = Services.dirsvc.QueryInterface(Ci.nsIProperties).get("TmpD", Ci.nsIFile);
  tmpFile.append("file.txt");
  tmpFile.createUnique(Ci.nsIFile.FILE_TYPE, 0o600);
||||||| merged common ancestors
addMessageListener("entries.open", function (e) {
  tmpFile = Cc["@mozilla.org/file/directory_service;1"]
              .getService(Ci.nsIDirectoryService)
              .QueryInterface(Ci.nsIProperties)
              .get('TmpD', Ci.nsIFile)
  tmpFile.append('file.txt');
  tmpFile.createUnique(Ci.nsIFile.FILE_TYPE, 0o600);
=======
addMessageListener("entries.open", function(e) {
  tmpFile = Services.dirsvc
    .QueryInterface(Ci.nsIProperties)
    .get("TmpD", Ci.nsIFile);
  tmpFile.append("file.txt");
  tmpFile.createUnique(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);
>>>>>>> upstream-releases

<<<<<<< HEAD
  tmpDir = Services.dirsvc.QueryInterface(Ci.nsIProperties).get("TmpD", Ci.nsIFile);
||||||| merged common ancestors
  tmpDir = Cc["@mozilla.org/file/directory_service;1"]
              .getService(Ci.nsIDirectoryService)
              .QueryInterface(Ci.nsIProperties)
              .get('TmpD', Ci.nsIFile)
=======
  tmpDir = Services.dirsvc
    .QueryInterface(Ci.nsIProperties)
    .get("TmpD", Ci.nsIFile);
>>>>>>> upstream-releases

  tmpDir.append("dir-test");
  tmpDir.createUnique(Ci.nsIFile.DIRECTORY_TYPE, 0o700);

  var file1 = tmpDir.clone();
  file1.append("foo.txt");
  file1.create(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);

  var dir1 = tmpDir.clone();
  dir1.append("subdir");
  dir1.create(Ci.nsIFile.DIRECTORY_TYPE, 0o700);

  var file2 = dir1.clone();
  file2.append("bar..txt"); // Note the double ..
  file2.create(Ci.nsIFile.NORMAL_FILE_TYPE, 0o600);

  var dir2 = dir1.clone();
  dir2.append("subsubdir");
  dir2.create(Ci.nsIFile.DIRECTORY_TYPE, 0o700);

  File.createFromNsIFile(tmpFile).then(function(file) {
    sendAsyncMessage("entries.opened", {
<<<<<<< HEAD
      data: [ new Directory(tmpDir.path), file ],
||||||| merged common ancestors
      data: [ new Directory(tmpDir.path), file ]
=======
      data: [new Directory(tmpDir.path), file],
>>>>>>> upstream-releases
    });
  });
});

addMessageListener("entries.delete", function(e) {
  tmpFile.remove(true);
  tmpDir.remove(true);
  sendAsyncMessage("entries.deleted");
});
