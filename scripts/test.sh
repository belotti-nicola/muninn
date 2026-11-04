#!/bin/sh
#
# Use:
#   ./build.sh [-t BuildType] [-a] [-d directory] [-n test_name] [-f]
#
# Opzioni:
#   -t <BuildType>   Default: Debug
#   -a               all test switch
#   -d <directory>   Recursive directory execution
#   -n <test_name>   One name
#   -f               Skip stress tests
#
# Esempi:
#   ./test.sh                          
#   ./test.sh -t Release
#   ./test.sh -d test/<directory>
#   ./test.sh -n test/<test_name>
#   ./test.sh -f

set -e

BUILD_TYPE="Debug"
MODE="all"
TARGET=""
SKIP_STRESS=0

while getopts "t:d:n:af" opt; do
  case "$opt" in
    t) BUILD_TYPE="$OPTARG" ;;
    d) MODE="dir";  TARGET="$OPTARG" ;;
    n) MODE="name"; TARGET="$OPTARG" ;;
    a) MODE="all" ;;
    f) SKIP_STRESS=1 ;;
    *) echo "Option is not valid: '$opt' (OPTARG='$OPTARG')"; exit 1 ;;
  esac
done

echo "Build type: $BUILD_TYPE"
sleep 1 && echo "\t-3"
sleep 1 && echo "\t-2"
sleep 1 && echo "\t-1"
sleep 1

mkdir -p ../build
rm -rfv ../build/*
cd ../build || exit 1

cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
make

CTEST_ARGS="--output-on-failure --stop-on-failure"
if [ "$SKIP_STRESS" -eq 1 ]; then
  CTEST_ARGS="$CTEST_ARGS -LE stress"
fi

case "$MODE" in
  all)
    echo "Executing all tests..."
    ctest $CTEST_ARGS
    ;;
  dir)
    echo "Executing tests in directory: $TARGET"
    ctest $CTEST_ARGS -L "^${TARGET}"
    ;;
  name)
    echo "Executing test named: $TARGET"
    ctest $CTEST_ARGS -R "^${TARGET}\$"
    ;;
esac