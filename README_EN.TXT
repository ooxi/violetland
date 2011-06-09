	
Violetland

1. INFORMATION

Violetland is an open source cross-platform game similar to Crimsonland.
In this game the player should help a girl by name of Violet to struggle with hordes of monsters.
For this purpose the various weapon, and also the special abilities of the heroine which are opening with experience can be used.

Features:
 - graphic is based on opengl.
 - dynamic change of day and night.
 - not disappearing corpses.
 - improves with experience ñharacter.
 - firearms, hand grenades, teleports, bonuses.
 - languages: english, french, russian, galician, spain.
 - works on gnu/linux, freebds, windows, mac.

irc.freenode.net, #violetland. 
http://code.google.com/p/violetland/
http://groups.google.com/group/violetland

2. SYSTEM REQUIREMENTS

Before you enjoy the game make sure that you have a 3D-accelerated video card and latest drivers for your card with 3D-acceleration support.

Currently are supported: windows, gnu/linux, freebsd, mac os x.

3. WINDOWS

CAUTION! Now a hot switching of video modes from options screen isn't finalized.
When you will apply changes in the options screen, the program will shut down. You must manually restart the program.

The executable was created with help of MSVC++ 2010, thus the Microsoft Visual C++ 2010 Redistributable Package may be needed.
Download it at http://www.microsoft.com/downloads/en/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84&displaylang=en.

The program can't be started (because it can't load resources) if the path to the executable contains cyrillic symbols.

There is a workaround for this problem. Run the program by the command shell script called start.cmd from the root program folder.

4. LINUX

Check for violetland package in main repositories of your linux distro.
If there is nothing, check related wiki page  http://code.google.com/p/violetland/wiki/ThirdParyPackages.

An universal way to play violetland on linux is to build the program with help of the cmake build system.
You probably can install it from the main repository of your linux distribution.
Also your system must have the SDL packages (SDL, SDL_image, SDL_ttf, SDL_mixer), the same devel packages and gcc.

NOTE FOR UBUNTU USERS
You should execute command like "sudo apt-get install cmake libsdl-ttf2.0-dev libsdl-mixer1.2-dev libsdl-image1.2-dev" to install required packages.

Besides, some additional steps are required.

First download and unpack the distribution package or get files from the SVN.

Next go to the root distribution directory and make the program using cmake (the default install prefix is /usr/local):

$ mkdir build
$ cd build
$ cmake ..
$ sudo make install 

Then run the program:

$ violetland

Instead of installing the program to /usr/local you can install it in your home folder or everywhere you want.
Then probably the root password to access /usr/local will be not necessary. To do that you can use next example:

$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=../dist ..
$ make install
$ cd ../dist/bin/
$ ./violetland

In addition you can install the executable and the game data to the separate directory trees:

$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DDATA_INSTALL_DIR=/usr/share/games/violetland ..
$ sudo make install

5. Controls

w - Up
a - Left
s - Down
d - Right
q - Enter/leave teleport mode
space - Grenade
f - Toggle Flashlight
g - Toggle laser sight
c - Abilities menu pause
esc - Main menu pause
right mouse click - Fire
left mouse click - Reload