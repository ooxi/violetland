#!/bin/bash
set -e



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_SCRIPT_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIRECTORY_OF_SCRIPT_SH}/../travis-ci.sh"



# Build
CMAKE=cmake
CXX=clang++

mkdir "${BUILD_DIRECTORY}" && cd "${BUILD_DIRECTORY}" && $CMAKE -DCMAKE_INSTALL_PREFIX=${DIST_DIRECTORY} "${ROOT_DIRECTORY}" && make install



# Deploy
deploy_as_tarbz2
