#!/bin/bash
set -e



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_SCRIPT_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


# Resolve directories
ROOT_DIRECTORY="${DIRECTORY_OF_SCRIPT_SH}/../../.."
BUILD_DIRECTORY="${ROOT_DIRECTORY}/build"
DIST_DIRECTORY="${ROOT_DIRECTORY}/dist"


# Build
CMAKE=cmake
CXX=clang++

mkdir "${BUILD_DIRECTORY}" && cd "${BUILD_DIRECTORY}" && $CMAKE -DCMAKE_INSTALL_PREFIX=${DIST_DIRECTORY} "${ROOT_DIRECTORY}" && make install


# Deploy
source "${DIRECTORY_OF_SCRIPT_SH}/../travis-ci.sh"
tar -cjvf "${BUILD_DIRECTORY}/${ARTIFACT_NAME}" "${DIST_DIRECTORY}"

du -s --si "${BUILD_DIRECTORY}/${ARTIFACT_NAME}"

