# Cliqz Browser for Android

This is the experimental Cliqz Browser for Android based on Firefox.

## Building

**Read the entire paragraph before proceding**

First thing you need a signed copy of the *Cliqz Extension* and the *Ghostery
Extension* specific to the *Cliqz Browser for Android*, instructions about how
to get them will be added later. They must be copied in a folder with the
following structure:

```
<distribution_directory>
└── assets
    └── distribution
        └── extensions
            ├── firefox@ghostery.com.xpi
            └── search@cliqz.com.xpi
```

The `<distribution_directory>` can be anywhere on your system, but we generally
prefer to use a folder named `cliqz` inside the project root folder (i.e. the
full path of the *Cliqz Extension* will be
`<project_root_folder>/cliqz/assets/distribution/extensions/search@cliqz.com.xpi`
). An example of how to configure the Firefox build system to pick up a folder
can be found [here][2].

To build this project follow these [instructions][1]. Please, notice that the
original **Mozilla Firefox source code is inside the `mozilla-release`
folder**: before running any command from the link above, you should enter the
`mozilla-release` folder (`cd <project_root_folder>/mozilla-release`).
The build process is divided in 5 main steps:

1. Dependencies installation

   Performed by the `./mach bootstrap` command. It is a one time operation and
   it will install all the extra software you need to properly compile the
   software.

2. Config file creation

   `<project-root>/mozilla-release/mozconfig` is the file where you can specify
   all the build options. We have a set of examples and real user
   configurations available in `<project-root>/mozconfigs`.

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

## Disable addon signature checks

Add the following line to your `mozconfig` file:

```bash
CLIQZ_DEBUG=1
```

Alternatively, declare and export the `CLIQZ_DEBUG` variable:

```bash
$> export CLIQZ_DEBUG=1
```

## Utils

Here is a list of utility scripts you can find inside the `utils` folder:

1. **startemu.sh**

    It makes easy to run an emulator from the command line by presenting a list
    of the already available emulators (i.e. those created using Android
    Studio).
2. **forward.sh**

    It forwards the port 6000 to the Firefox debug socket if it exists. If no
    argument is given, it will use `com.cliqz.browser` as the app id, otherwise
    it will append a dot arg to the app id (i.e. `./forward.sh alpha` will use
    `com.cliqz.browser.alpha` as app id).

[1]: https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Build_Instructions/Simple_Firefox_for_Android_build
[2]: https://github.com/cliqz-oss/cliqz-android/blob/946a61d5a08636c4c945ec36d557394ef0ef5b4a/mozconfigs/stefano.mozconfig#L26
