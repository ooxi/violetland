#!/bin/bash



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_SCRIPT_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

ROOT_DIRECTORY="${DIRECTORY_OF_SCRIPT_SH}/../../.."
BUILD_DIRECTORY="${ROOT_DIRECTORY}/build"
DIST_DIRECTORY="${ROOT_DIRECTORY}/dist"



CMAKE=cmake
CXX=g++

mkdir "${BUILD_DIRECTORY}" && cd "${BUILD_DIRECTORY}" && $CMAKE -DCMAKE_INSTALL_PREFIX=${DIST_DIRECTORY} "${ROOT_DIRECTORY}" && make install



source "${DIRECTORY_OF_SCRIPT_SH}/../travis-ci.sh"
echo "X ${ARTIFACT_NAME} Y"

