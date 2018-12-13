# Automation scripts branch
-------------------------

## Building the Android Browser

### build-system/magic.sh
Run the script from the <code>/path/to/repo/android-browser</code> folder.

    sh /path/to/automation-branch/build-system/magic.sh


#### Arguments for magic.sh:
    -a | --arch     : 'x86' or 'arm' (Default: 'x86')
    -b | --brand    : 'cliqz', 'cliqz-alpha', 'ghostery', 'ghostery-alpha' (Default: 'ghostery')
    -c | --channel  : Provide the required Channel (Default: 'MA99')
    -l | --locale   : 'en-US' or 'multi' (Default: 'en-US')
    -t | --type     : 'nightly', 'release' or '' (Default: CI Build)