#!/bin/bash



# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_SCRIPT_SH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"





source "${DIRECTORY_OF_SCRIPT_SH}/../travis-ci.sh"
echo "X ${ARTIFACT_NAME} Y"

