#!/bin/bash
set -e


# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_THIS_FILE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIRECTORY_OF_THIS_FILE}/../travis-ci.sh"


# Build
export CMAKE=/usr/lib/mxe/usr/bin/x86_64-w64-mingw32.static-cmake
export CXX=g++

mkdir -p "${BUILD_DIRECTORY}" && cd "${BUILD_DIRECTORY}" && $CMAKE -DCMAKE_INSTALL_PREFIX=${DIST_DIRECTORY} "${ROOT_DIRECTORY}" && make install

