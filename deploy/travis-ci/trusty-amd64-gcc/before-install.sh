#!/bin/bash
set -e


# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_THIS_FILE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIRECTORY_OF_THIS_FILE}/../travis-ci.sh"


# Install dependencies
PACKAGES='g++ cmake gettext libegl1-mesa-dev libgles2-mesa-dev libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev libboost-all-dev'

sudo apt-get update
sudo apt-get install -y $PACKAGES
