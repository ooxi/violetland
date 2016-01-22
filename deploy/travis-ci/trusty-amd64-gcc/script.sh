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

	echo "Will artifact ${ARTIFACT_NAME} to Bintray"
	tar -cjvf "${BUILD_DIRECTORY}/${BINTRAY_FILE}.tar.bz2" "${DIST_DIRECTORY}"

	BINTRAY_RESPONSE=`curl -T "${BUILD_DIRECTORY}/${BINTRAY_FILE}.tar.bz2" "-uooxi:${BINTRAY_DEPLOYMENT_API_KEY}" "https://api.bintray.com/content/ooxi/violetland/travis-ci/${BINTRAY_VERSION}/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.tar.bz2?publish=1"`

	if [ '{"message":"success"}' == "${BINTRAY_RESPONSE}" ]; then
		echo "Artifact published at https://dl.bintray.com/ooxi/violetland/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.tar.bz2"
	else
		echo "Upload to Bintray failed with response ${BINTRAY_RESPONSE}"
		exit 1
	fi
fi

