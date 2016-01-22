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
if [ "${TRAVIS_SECURE_ENV_VARS}" == "true" ]; then
	source "${DIRECTORY_OF_SCRIPT_SH}/../travis-ci.sh"

	tar -cjvf "${BUILD_DIRECTORY}/${ARTIFACT_NAME}.tar.bz2" "${DIST_DIRECTORY}"

	curl -T "${BUILD_DIRECTORY}/${ARTIFACT_NAME}.tar.bz2" "-u${MY_SECRET_ENV}" "https://api.bintray.com/content/ooxi/violetland/travis-ci/${BUILD_DATE}-b${TRAVIS_BUILD_NUMBER}/${ARTIFACT_NAME}.tar.bz2"
fi

