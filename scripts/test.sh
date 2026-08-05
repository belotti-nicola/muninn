#!/bin/sh
#
# Use:
#   ./build.sh [-t BuildType] [-a] [-d directory] [-n test_name]
#
# Opzioni:
#   -t <BuildType>   Default: Debug
#   -a               all test switch
#   -d <directory>   Recursive directory execution
#   -n <test_name>   One name
#
# Esempi:
#   ./test.sh                          
#   ./test.sh -t Release
#   ./test.sh -d test/<directory>
#   ./test.sh -n test/<test_name>

set -e

BUILD_TYPE="Debug"
MODE="all"
TARGET=""

while getopts "t:d:n:a" opt; do
  case "$opt" in
    t) BUILD_TYPE="$OPTARG" ;;
    d) MODE="dir"; TARGET="$OPTARG" ;;
    n) MODE="name"; TARGET="$OPTARG" ;;
    a) MODE="all" ;;
    *)
      echo "Option is not valid"
      exit 1
      ;;
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

case "$MODE" in
  all)
    echo "Executing all test..."
    ctest --output-on-failure --stop-on-failure
    ;;
  dir)
    echo "Executing test in directory: $TARGET"
    ctest --output-on-failure --stop-on-failure -L "^${TARGET}"
    ;;
  name)
    echo "Executing test named: $TARGET"
    ctest --output-on-failure --stop-on-failure -R "^${TARGET}\$"
    ;;
esac