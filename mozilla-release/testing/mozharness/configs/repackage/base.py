import os

config = {
    "package-name": "firefox",
    "installer-tag": "browser/installer/windows/app.tag",
    "stub-installer-tag": "browser/installer/windows/stub.tag",
<<<<<<< HEAD
    "sfx-stub": "other-licenses/7zstub/firefox/7zSD.sfx",
    "wsx-stub": "browser/installer/windows/msi/installer.wxs",
    "fetch-dir": os.environ.get('MOZ_FETCHES_DIR'),
||||||| merged common ancestors
    "sfx-stub": "other-licenses/7zstub/firefox/7zSD.sfx",
=======
    "wsx-stub": "browser/installer/windows/msi/installer.wxs",
    "fetch-dir": os.environ.get('MOZ_FETCHES_DIR'),
>>>>>>> upstream-releases
}
