#!/bin/bash
set -e


# @see http://stackoverflow.com/a/246128/2534648
DIRECTORY_OF_THIS_FILE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIRECTORY_OF_THIS_FILE}/../travis-ci.sh"


# Install dependencies
PACKAGES='mxe-x86-64-w64-mingw32.static-gcc cmake mxe-x86-64-w64-mingw32.static-gettext mxe-x86-64-w64-mingw32.static-sdl mxe-x86-64-w64-mingw32.static-sdl-ttf mxe-x86-64-w64-mingw32.static-sdl-mixer mxe-x86-64-w64-mingw32.static-sdl-image mxe-x86-64-w64-mingw32.static-boost'

echo "deb http://pkg.mxe.cc/repos/apt/debian wheezy main" | sudo tee /etc/apt/sources.list.d/mxeapt.list
sudo apt-key adv --keyserver x-hkp://keys.gnupg.net --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB

sudo apt-get update
sudo apt-get install -y $PACKAGES
