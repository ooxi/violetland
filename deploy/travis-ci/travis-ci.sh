#!/bin/bash



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_TRAVIS_CI_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"



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
	ARTIFACT_NAME="violetland-${VERSION}-b${TRAVIS_BUILD_NUMBER}-${TARGET}"
fi



#
# Environment when building a different branch
# =============================================
#
# TRAVIS_BRANCH		feature/automatic-windows-deployment
# TRAVIS_PULL_REQUEST	false
#
# ARTIFACT_NAME		violetland-0.5-b167-trusty-amd64-gcc_feature-automatic-windows-deployment
#

if [ "master" != "${TRAVIS_BRANCH}" ] && [ "false" == "${TRAVIS_PULL_REQUEST}" ]; then

	# @see http://serverfault.com/a/348485/207752
	SAFE_BRANCH_NAME=$(echo "${TRAVIS_BRANCH}" | sed -e 's/[^A-Za-z0-9._-]/_/g')
	ARTIFACT_NAME="branch-${SAFE_BRANCH_NAME}-b${TRAVIS_BUILD_NUMBER}"
fi



#
# Environment when building a pull request
# ========================================
#
# TRAVIS_BRANCH		master
# TRAVIS_PULL_REQUEST	104
# 
# ARTIFACT_NAME		violetland-0.5-b167-trusty-amd64-gcc_pull-request-104
#

if [ "false" != "${TRAVIS_PULL_REQUEST}" ]; then

	# @see http://serverfault.com/a/348485/207752
	SAFE_PULL_REQUEST=$(echo "${TRAVIS_PULL_REQUEST}" | sed -e 's/[^A-Za-z0-9._-]/_/g')
	ARTIFACT_NAME="pull-request-${SAFE_PULL_REQUEST}-b${TRAVIS_BUILD_NUMBER}-${TRAVIS_COMMIT:0:7}"
fi

