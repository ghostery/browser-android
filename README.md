# Cliqz Browser for Android

This is the experimental Cliqz Browser for Android based on Firefox.

## Building

**Read the entire paragraph before proceding**

To build this project follow these [instructions][1]. Please, notice that the
original **Mozilla Firefox source code is inside the `mozilla-release`
folder**: before running any command from the link above, you should enter the
`mozilla-release` folder (`cd <project_root_folder>/mozilla-release`).
The build process is divided in 5 main steps:
1. Dependencies installation

   Performed by the `./mach install` command. It is a one time operation and it
   will install all the extra software you need to properly compile the
   software.

2. Config file creation

   `<project-root>/mozilla-release/mozconfig` is the file where you can specify
   all the build options. We have a set of examples and real user configurations
   available in `<project-root>/mozconfigs`.

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
