# Cliqz Browser for Android

This is the experimental Cliqz Browser for Android based on Firefox.

## Disable addon signature checks

Add the following line to your `mozconfig` file:

```bash
CLIQZ_DEBUG=1
```

Alternatively, declare and export the `CLIQZ_DEBUG` variable:

```
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
