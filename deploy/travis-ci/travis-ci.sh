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
