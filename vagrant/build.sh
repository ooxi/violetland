#!/bin/bash

TARGET=`cat TARGET`


# Do not taint synced folders
if [ -d violetland ]; then
	rm -rf violetland
fi

cp -r /violetland/ .
cd violetland


# Update dependencies
git submodule init || exit 1
git submodule update || exit 1

mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX="../${TARGET}" ".." || exit 1
make install || exit 1


# Package generated binaries and export to synced folder
tar -czvf "violetland-${TARGET}.tar.gz" "../${TARGET}" || exit 1
cp "violetland-${TARGET}.tar.gz" "/vagrant/"
