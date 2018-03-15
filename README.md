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
