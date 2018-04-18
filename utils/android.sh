function check_cmd() {
    type "$1" > /dev/null 2>&1|| {
        cat >&2 << EOF
Can't find $1, please make sure it is installed and its path is in PATH
EOF
        exit 1
    }
}

check_cmd adb
check_cmd emulator
