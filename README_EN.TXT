violetland
----------

1. INFORMATION

Violetland is an opensource crossplatform game similar to crimsonland.

In this game the player should help a girl by name of Violet to struggle with hordes of monsters. For this purpose the various weapon, and also the special abilities of the heroine which are opening with experience can be used. In game there are elements of RPG in the form of strength-agility-vitality and derivatives. Also there is an unique feature: dynamic change of day and night.

http://code.google.com/p/violetland/
http://groups.google.com/group/violetland

2. SYSTEM REQUIREMENTS

Before you enjoy the game make sure that you have a 3D-accelerated video card and latest drivers for your card with 3D-acceleration support.

Currently are supported: windows, linux, freebsd.

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