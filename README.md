# Ghostery Browser for Android

This is the next generation Ghostery Browser for Android based on Firefox.

## Building

To build this project follow these [instructions][1]. Please, notice that the
modified **Mozilla Firefox source code is inside the `mozilla-release` folder**:
before running any command from the link above, you should enter the
`mozilla-release` folder (`cd <project_root_folder>/mozilla-release`).
The build process is divided in 5 main steps:

1. Dependencies installation

   Performed by the `./mach bootstrap` command. It is a one time operation and
   it will install all the extra software you need to properly compile the
   software.

2. Config file creation

   `<project-root>/mozilla-release/mozconfig` is the file where you can specify
   all the build options. An example of how to configure the Firefox build
   system to pick up a folder can be found [here][2] and in
   [`<project-root>/mozconfigs`][3].

3. Build

   Performed by the `./mach build` command. You have to run this every time you
   change the `mozconfig` file.

4. Package

   Performed by the `./mach package` command. It prepares (and translates) the
   resources to be included in the final APK and build it. You have to run this
   every time you change a resource file (i.e. translations).

5. Install

   Performed by the `./mach install` command. It rebuilds the APK and pushes it
   to an attached device or emulator.

## Utils

Here is a list of utility scripts you can find inside the `utils` folder:

1. **startemu.sh**

    It makes easy to run an emulator from the command line by presenting a list
    of the already available emulators (i.e. those created using Android
    Studio).

2. **forward.sh**

    It forwards the port 6000 to the Firefox debug socket if it exists. It will
    ask for the branding you want to connect to (i.e. Cliqz, Ghosthery, etc.).

3. **multibuild.sh**

    This script builds a multi locale apk given the branding and the desired
    architecture (i.e. arm, x86).

[1]: https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Build_Instructions/Simple_Firefox_for_Android_build
[2]: https://github.com/ghostery/browser-android/blob/946a61d5a08636c4c945ec36d557394ef0ef5b4a/mozconfigs/stefano.mozconfig#L26
[3]: https://github.com/ghostery/browser-android/tree/master/mozconfigs
