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

