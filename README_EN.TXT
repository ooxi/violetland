violetland
----------

1. INFORMATION

Violetland is an open source cross platform crimsonland game clone. Except reimplementing the original game we are adding some other cool features such as dynamic changing of day and night, network play and more.

http://nike.homeunix.com/violetland
svn://nike.homeunix.com:119/svn_rep/cpp/violetland/trunk

2. SYSTEM REQUIREMENTS

Before you enjoy the game make sure that you have a 3D-accelerated video card and latest drivers for your card with 3D-acceleration support.

Only windows and linux operation systems are currently supported.

3. WINDOWS

The executable was created with help of MSVC++ 2008, thus the Microsoft Visual C++ 2008 Redistributable Package may be needed.

The program can't be started (because it can't load resources) if the path to the executable contains cyrillic symbols.

There is a workaround for this problem. Run the program by the command shell script called start.cmd from the root program folder.

4. LINUX

Currently an easiest way to play violetland on linux is to build the program with help of the cmake build system. You probably can install it from the main repository of your linux distribution. Also your system must have the SDL packages (SDL, SDL_image, SDL_ttf, SDL_mixer), the same devel packages and gcc.

Besides, some additional steps are required.

First download and unzip the distribution package or get files from the SVN.

Next go to the root distribution directory and make the program using cmake:

$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
$ sudo make install 

Then run the program:

$ violetland

Instead of installing the program to /usr/local you can install it in your home folder or everywhere you want. Then probably the root password to access /usr/local will be not necessary. To do that you can use next example:

$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../dist ..
$ make install
$ cd ../dist/bin/
$ ./violetland