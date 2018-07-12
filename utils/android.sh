ADB=""
EMULATOR=""

function isUndefined() {
  if [ -z "$1" ]; then
    >&2 echo "${2}"
    exit 1
  fi
}

type "adb" > /dev/null 2>&1 && ADB="adb"
type "emulator" > /dev/null 2>&1 && EMULATOR="emulator"

if [ -z "${ADB}" ] && [ -n "${ANDROID_HOME}" ]; then
  ADB="${ANDROID_HOME}/platform-tools/adb"
fi

if [ -z "${EMULATOR}" ] && [ -n "${ANDROID_HOME}" ]; then
  EMULATOR="${ANDROID_HOME}/emulator/emulator"
fi

isUndefined "${ADB}" "Can't find adb executable"
isUndefined "${EMULATOR}" "Can't find emulator executable"
