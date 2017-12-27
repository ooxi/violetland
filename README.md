Violetland [![Build Status](https://travis-ci.org/ooxi/violetland.svg?branch=master)](https://travis-ci.org/ooxi/violetland)
==========

Violetland is an open source cross-platform game similar to Crimsonland. In this game the player should help a girl by name of Violet to struggle with hordes of monsters. For this purpose the various weapon, and also the special abilities of the heroine which are opening with experience can be used.

### Features

 * graphic is based on opengl.
 * dynamic change of day and night.
 * not disappearing corpses.
 * character improves with experience.
 * firearms, hand grenades, teleports, bonuses.
 * [languages](http://www.getlocalization.com/violetland/): english, [french](https://www.getlocalization.com/violetland/editor/8553/), [galician](https://www.getlocalization.com/violetland/editor/8554/), [german](https://www.getlocalization.com/violetland/editor/8549/), [russian](https://www.getlocalization.com/violetland/editor/8551/), [spanish](https://www.getlocalization.com/violetland/editor/8552/).
 * works on gnu/linux, freebds, netbsd, windows, mac.

### Community

 * [Git source repository](https://github.com/ooxi/violetland)
 * [Issue tracker](https://github.com/ooxi/violetland/issues)
 * [Wiki](https://github.com/ooxi/violetland/wiki)
 * [IRC](http://webchat.freenode.net/?channels=violetland) irc.freenode.net #violetland
 * [Google Groups](http://groups.google.com/group/violetland)

SCREENSHOTS
-----------

[![Violetland uses an advanced illumination system](http://violetland.github.com/images/screenshots/thumbnail/violetland-illumination-200x150.png)](http://violetland.github.com/images/screenshots/original/violetland-illumination.png)
[![Violet shooting a laser](http://violetland.github.com/images/screenshots/thumbnail/violetland-laser-200x150.png)](http://violetland.github.com/images/screenshots/original/violetland-laser.png)
[![Death is the only way out ;)](http://violetland.github.com/images/screenshots/thumbnail/violetland-death-200x150.png)](http://violetland.github.com/images/screenshots/original/violetland-death.png)
[![Lighting up the dark](http://violetland.github.com/images/screenshots/thumbnail/violetland-flashlight-200x150.png)](http://violetland.github.com/images/screenshots/original/violetland-flashlight.png)
[![You cannot say manslaughter without laughter](http://violetland.github.com/images/screenshots/thumbnail/violetland-massacre-200x150.jpg)](http://violetland.github.com/images/screenshots/original/violetland-massacre.jpg)

SYSTEM REQUIREMENTS
-------------------

Before you enjoy the game make sure that you have a 3D-accelerated video card and latest drivers for your card with 3D-acceleration support.

Currently are supported: windows, gnu/linux, freebsd, netbsd, mac os x.

WINDOWS
-------

CAUTION! Now a hot switching of video modes from options screen isn't finalized. When you will apply changes in the options screen, the program will shut down. You must manually restart the program.

The executable was created with help of MSVC++ 2010, thus the Microsoft Visual C++ 2010 Redistributable Package may be needed. You can download it from [Microsoft](http://www.microsoft.com/downloads/en/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84&displaylang=en).

The program can't be started (because it can't load resources) if the path to the executable contains cyrillic symbols.

There is a workaround for this problem. Run the program by the command shell script called start.cmd from the root program folder.

LINUX
-----

Check for violetland package in main repositories of your linux distro. If there is nothing, check related wiki page for [third party packages](https://github.com/ooxi/violetland/wiki/Third-Pary-Packages).

An universal way to play violetland on linux is to build the program with help of the cmake build system. You probably can install it from the main repository of your linux distribution.
Also your system must have the SDL packages (`SDL`, `SDL_image`, `SDL_ttf`, `SDL_mixer`), the same devel packages and gcc.


### NOTE FOR UBUNTU USERS
You should execute command like `sudo apt-get install cmake libsdl-ttf2.0-dev libsdl-mixer1.2-dev libsdl-image1.2-dev libboost-all-dev` to install required packages (tested with Ubuntu 12.04).

Besides, some additional steps are required.

First download and unpack the distribution package or get files from the repository.

Next go to the root distribution directory and make the program using cmake (the default install prefix is `/usr/local`):

    $ git submodule init
    $ git submodule update
    $ mkdir build
    $ cd build
    $ cmake ..
    $ sudo make install 

Then run the program:

    $ violetland

Instead of installing the program to `/usr/local` you can install it in your home folder or everywhere you want. Then probably the root password to access `/usr/local` will be not necessary. To do that you can use next example:

    $ git submodule init; git submodule update
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=../dist ..
    $ make install
    $ cd ../dist/bin/
    $ ./violetland

In addition you can install the executable and the game data to the separate directory trees:

    $ git submodule init; git submodule update
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DDATA_INSTALL_DIR=/usr/share/games/violetland -DLOCALE_INSTALL_DIR=/usr/share/locale ..
    $ sudo make install


### NOTE FOR FEDORA USERS

In order to build Violetland on Fedora, you need to install the following packages (tested with Fedora 23, 24, 25, 26 and 27)

    # dnf install boost-devel cmake make gcc-c++ SDL-devel SDL_image-devel SDL_mixer-devel SDL_ttf-devel gettext

The rest of the build and installation procedure is identical to Ubuntu.


OpenBSD
-------

[OpenBSD](http://www.openbsd.org/) support was tested with OpenBSD 5.2

    # pkg_add boost sdl sdl-image sdl-mixer sdl-ttf
    $ git clone https://github.com/ooxi/violetland.git; cd violetland
    $ git submodule init; git submodule update
    $ mkdir build; cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=../dist ..
    $ make install


NetBSD (pkgsrc)
---------------

[NetBSD](http://www.netbsd.org/) ships with violetland in pkgsrc.

The easiest way to install the game is with pkgin:

    pkgin install violetland

Alternatively install from sources:

    cd /usr/pkgsrc/games/violetland && make install


Controls
--------

 * `w` - Up
 * `a` - Left
 * `s` - Down
 * `d` - Right
 * `q` - Enter/leave teleport mode
 * `space` - Grenade
 * `f` - Toggle Flashlight
 * `g` - Toggle laser sight
 * `c` - Abilities menu pause
 * `esc` - Main menu pause
 * `right mouse click` - Fire
 * `left mouse click` - Reload
