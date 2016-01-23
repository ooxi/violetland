#!/bin/bash
set -e


# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_THIS_FILE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIRECTORY_OF_THIS_FILE}/../travis-ci.sh"


# Install dependencies
PACKAGES='clang-3.6 cmake gettext libsdl-ttf2.0-dev libsdl-mixer1.2-dev libsdl-image1.2-dev libboost-all-dev'

sudo apt-get update
sudo apt-get install -y $PACKAGES
