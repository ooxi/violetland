#!/bin/bash
set -e



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_TRAVIS_CI_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"



#
# Resolve directories
# ===================
#
ROOT_DIRECTORY="${DIRECTORY_OF_TRAVIS_CI_SH}/../.."
BUILD_DIRECTORY="${ROOT_DIRECTORY}/build"
DIST_DIRECTORY="${ROOT_DIRECTORY}/dist"



#
# Common environment
# ==================
#
# BUILD_DATE		2016-01-22
# TARGET		trusty-amd64-gcc
# VERSION		0.5
# TRAVIS_BUILD_NUMBER	167
# TRAVIS_COMMIT		b6b90f7d2e8b7fdd4d1a62b59c7822ea160f9491
#

BUILD_DATE=`date --iso-8601`
VERSION=`head -n 1 ${DIRECTORY_OF_TRAVIS_CI_SH}/../../VERSION`



#
# Environment when building master
# ================================
# 
# TRAVIS_BRANCH		master
# TRAVIS_PULL_REQUEST	false
#
# ARTIFACT_NAME		violetland-0.5-b167-trusty-amd64-gcc
#

if [ "master" == "${TRAVIS_BRANCH}" ] && [ "false" == "${TRAVIS_PULL_REQUEST}" ]; then
	BINTRAY_VERSION="${TRAVIS_BUILD_NUMBER}"
	BINTRAY_DIRECTORY="travis-ci/master/${BINTRAY_VERSION}"
	BINTRAY_FILE="violetland-${VERSION}-b${TRAVIS_BUILD_NUMBER}-${TARGET}"
fi



#
# Environment when building a different branch
# =============================================
#
# TRAVIS_BRANCH		feature/automatic-windows-deployment
# TRAVIS_PULL_REQUEST	false
#
# ARTIFACT_NAME		branch-feature_automatic-windows-deployment-b167-trusty-amd64-gcc
#

if [ "master" != "${TRAVIS_BRANCH}" ] && [ "false" == "${TRAVIS_PULL_REQUEST}" ]; then

	BINTRAY_VERSION="${TRAVIS_BUILD_NUMBER}"
	BINTRAY_DIRECTORY="travis-ci/branches/${TRAVIS_BRANCH}"
	BINTRAY_FILE="violetland-${VERSION}-b${TRAVIS_BUILD_NUMBER}-${TARGET}-${TRAVIS_COMMIT:0:7}"
fi



#
# Environment when building a pull request
# ========================================
#
# TRAVIS_BRANCH		master
# TRAVIS_PULL_REQUEST	104
# 
# ARTIFACT_NAME		pull-request-104-b167-b6b90f7-trusty-amd64-gcc
#

if [ "false" != "${TRAVIS_PULL_REQUEST}" ]; then

	BINTRAY_VERSION="${TRAVIS_BUILD_NUMBER}"
	BINTRAY_DIRECTORY="travis-ci/pull-requests/${TRAVIS_PULL_REQUEST}"
	BINTRAY_FILE="violetland-${VERSION}-b${TRAVIS_BUILD_NUMBER}-${TARGET}-${TRAVIS_COMMIT:0:7}"
fi





#
# Deploy ${DIST_DIRECTORY} as `.tar.bz2' to Bintray
#
function deploy_as_tarbz2 {
	if [ "${TRAVIS_SECURE_ENV_VARS}" == "true" ]; then
		echo "Will deploy artifact ${ARTIFACT_NAME} to Bintray"
		
		sudo apt-get install -y curl tar bzip2
		mv "${DIST_DIRECTORY}" "${ROOT_DIRECTORY}/${BINTRAY_FILE}"
		tar -cjvf "${BUILD_DIRECTORY}/${BINTRAY_FILE}.tar.bz2" "${ROOT_DIRECTORY}/${BINTRAY_FILE}"

		BINTRAY_RESPONSE=`curl -T "${BUILD_DIRECTORY}/${BINTRAY_FILE}.tar.bz2" "-uooxi:${BINTRAY_DEPLOYMENT_API_KEY}" "https://api.bintray.com/content/ooxi/violetland/travis-ci/${BINTRAY_VERSION}/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.tar.bz2?publish=1"`

		if [ '{"message":"success"}' == "${BINTRAY_RESPONSE}" ]; then
			echo "Artifact published at https://dl.bintray.com/ooxi/violetland/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.tar.bz2"
		else
			echo "Depolyment to Bintray failed with response ${BINTRAY_RESPONSE}"
			exit 1
		fi
	fi
}



#
# Deploy ${DIST_DIRECTORY} as `.zip' to Bintray
#
function deploy_as_zip {
	if [ "${TRAVIS_SECURE_ENV_VARS}" == "true" ]; then
		echo "Will deploy artifact ${ARTIFACT_NAME} to Bintray"
		
		# @see http://stackoverflow.com/a/20545763
		sudo apt-get install -y curl p7zip-full
		7z a -tzip "${BUILD_DIRECTORY}/${BINTRAY_FILE}.zip" -w "${DIST_DIRECTORY}"

		BINTRAY_RESPONSE=`curl -T "${BUILD_DIRECTORY}/${BINTRAY_FILE}.zip" "-uooxi:${BINTRAY_DEPLOYMENT_API_KEY}" "https://api.bintray.com/content/ooxi/violetland/travis-ci/${BINTRAY_VERSION}/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.zip?publish=1"`

		if [ '{"message":"success"}' == "${BINTRAY_RESPONSE}" ]; then
			echo "Artifact published at https://dl.bintray.com/ooxi/violetland/${BINTRAY_DIRECTORY}/${BINTRAY_FILE}.zip"
		else
			echo "Depolyment to Bintray failed with response ${BINTRAY_RESPONSE}"
			exit 1
		fi
	fi
}

