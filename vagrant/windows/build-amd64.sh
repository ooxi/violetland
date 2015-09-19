#!/bin/bash

TARGET="windows-amd64"


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

cmake -DCMAKE_TOOLCHAIN_FILE=/home/vagrant/mxe/usr/x86_64-w64-mingw32.static/share/cmake/mxe-conf.cmake -DCMAKE_INSTALL_PREFIX="../${TARGET}-tmp" ".." || exit 1
make install || exit 1


# Move files into windows directory structure
cd ..
mkdir "${TARGET}"

cp "${TARGET}-tmp/bin/violetland.exe" "${TARGET}" || exit 1
cp -r ${TARGET}-tmp/share/violetland/* "${TARGET}" || exit 1


# Package generated binaries and export to synced folder
zip -r "violetland-${TARGET}.zip" "${TARGET}" || exit 1
cp "violetland-${TARGET}.zip" "/vagrant/"
