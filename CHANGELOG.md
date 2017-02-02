Legend
======

 * `[+]` a feature was added
 * `[*]` some changes in existing features were made
 * `[!]` a bug was fixed


Upcoming release
----------------

 * `[!]` [Fixed gettext detection](https://github.com/ooxi/violetland/pull/137)
 * `[!]` [Removed `#pragma comment` compiler warnings](https://github.com/ooxi/violetland/pull/135)
 * `[*]` [Violet will speed up slower](https://github.com/ooxi/violetland/pull/120)
 * `[*]` [Use standard libintl detection module](https://github.com/ooxi/violetland/pull/134)
 * `[+]` [Local preview of GitHub flavoured Markdown](https://github.com/ooxi/violetland/pull/127)
 * `[*]` [Violet will level up faster](https://github.com/ooxi/violetland/pull/115)
 * `[+]` [Windows executable uses GUI subsystem](https://github.com/ooxi/violetland/pull/111) thus not spawning a console
 * `[+]` [Icon for Windows executables](https://github.com/ooxi/violetland/pull/109)
 * `[+]` [Isolated build environment](https://github.com/ooxi/violetland/pull/107) using [Vagrant](https://www.vagrantup.com/)
 * `[+]` [Automatic Windows builds](https://github.com/ooxi/violetland/pull/105), deploying Windows development snapshots to [Bintray](https://dl.bintray.com/ooxi/violetland/travis-ci/)
 * `[*]` [Improved M-32 reload sound](https://github.com/ooxi/violetland/pull/101)
 * `[*]` [Window subsystem refactored](https://github.com/ooxi/violetland/pull/96), moved window creation from `program.cpp` to individual subclasses
 * `[+]` [bla-rs](https://github.com/bla-rs) added [weapon documentation](https://github.com/ooxi/violetland/pull/98)
 * `[*]` [Clearified `ConfigFile.h` origin and license](https://github.com/ooxi/violetland/commit/ebb4081312dbbfc076fe02f698cf89c057526b44)
 * `[*]` [Unified namespace `violet` and include guard prefix `VIOLET_`](https://github.com/ooxi/violetland/pull/95)
 * `[!]` [Ensured all files except `start.cmd` use UNIX line endings](https://github.com/ooxi/violetland/pull/94)
 * `[*]` [Improved `CMakeLists.txt` readability](https://github.com/ooxi/violetland/pull/93)
 * `[*]` [Refactored window handlers to use `boost::function`](https://github.com/ooxi/violetland/pull/91)
 * `[+]` [Automatic Windows build using Travis CI and MXE](https://github.com/ooxi/violetland/pull/90)
 * `[*]` [Misc code cleanup](https://github.com/ooxi/violetland/pull/87)
 * `[!]` [Fixed minor bugs](https://github.com/ooxi/violetland/pull/83) enabling Violetland to be build by both GCC and Clang with extra warnings enabled
 * `[!]` [Changing the font to Xolonium-Regular](https://github.com/ooxi/violetland/pull/82) fixed [display issues of german umlauts](https://github.com/ooxi/violetland/issues/81)
 * `[*]` [Changing the font to Xolonium-Regular](https://github.com/ooxi/violetland/pull/82) resolved [probable licensing issues](https://github.com/ooxi/violetland/issues/73)
 * `[+]` [Added german translation](https://github.com/ooxi/violetland/pull/80)

Release 0.5 (2016-01-04)
------------------------

 * `[!]` Fixed game startup crash when `USER` environment variable is not found in [issue 71](https://github.com/ooxi/violetland/issues/71)
 * `[!]` Improved OpenBSD Support
 * `[+]` As of [issue 64](https://github.com/ooxi/violetland/pull/64) the control style can be toggled between "classic" and "modern"
 * `[*]` Added more available video modes in [issue 63](https://github.com/ooxi/violetland/pull/63), since auto detection doesn't seam to work
 * `[!]` Fixed i18n support in [issue 62](https://github.com/ooxi/violetland/pull/62)

Release 0.4.3 (2011-10-10)
--------------------------

 * `[+]` integrated inventory sidebar (additional to HUD)
 * `[!]` image of nuke bonus

Release 0.4.2
-------------

 * `[*]` significant changes in strength-agility-vitality balance
 * `[*]` widesight now fairy increase sight area by 20%
 * `[*]` poison perk is 2 times stronger
 * `[!]` few major bugz

Release 0.4.1
-------------

 * `[+]` new perk "magneto"
 * `[*]` game messages are written to stdout
 * `[*]` telekinesis perk now behaves like in original crimsonland, a status (percentage) can be seen on top-center of the screen
 * `[*]` more realistic laser aim will be helpful at night
 * `[!]` bug with changing the controls
 * `[!]` few errors in game messages

Release 0.4.0
-------------

 * `[+]` new graphics for flamethrower and sawed-off shotgun
 * `[*]` moved to boost library
 * `[*]` small performance improvements
 * `[!]` [issue 49](https://github.com/ooxi/violetland/issues/49) (changing bindings is broken)
 * `[!]` [issue 51](https://github.com/ooxi/violetland/issues/51) (vitality does not affect health regeneration)
 * `[!]` [issue 52](https://github.com/ooxi/violetland/issues/52) (controls definitions not loads when starting program)
 * `[!]` [issue 53](https://github.com/ooxi/violetland/issues/53) (wrong values in highscores screen)
 * `[!]` issue with path to resources after install with cmake

Release 0.3.2
-------------

 * `[+]` support of internationalization: english, french, russian, galician, spain
 * `[+]` new game mode "attack waves"
 * `[+]` new weapon "flamethrower"
 * `[+]` new perk "wide sight"
 * `[*]` new font
 * `[*]` more animations: teleportation, nuclear explosion
 * `[*]` some monsters have now the parameter "stamina" - they can't run all the time
 * `[*]` default player name is the system user name on linux platform
 * `[*]` [issue 19](https://github.com/ooxi/violetland/issues/19) (xp and item will drop out immediately after kill monster)
 * `[!]` [issue 36](https://github.com/ooxi/violetland/issues/36) and other fixes around movements of player and monsters
 * `[!]` [issue 38](https://github.com/ooxi/violetland/issues/38) (light is placed wrong for sawed-off shotgun)
 * `[!]` [issue 40](https://github.com/ooxi/violetland/issues/40) (blue cursor after death in teleportation mode)
 * `[!]` issues [45](https://github.com/ooxi/violetland/issues/45), [48](https://github.com/ooxi/violetland/issues/48) and other undocumented fixes to improve performance and reliability

Release 0.3.1
-------------

 * `[+]` new weapon "SawedOff-Shotgun"
 * `[+]` game controls are changeable from game menu
 * `[+]` new perk "looting"
 * `[+]` support of 1366*768 resolution
 * `[*]` strength affects maximum health
 * `[*]` new HUD animation

Release 0.3.0
-------------

 * `[+]` bonuses timers
 * `[!]` invalid volume of sound at options screen
 * `[!]` extra shot after teleportation

Release 0.2.10
--------------

 * `[+]` new perk "night vision"
 * `[+]` new bonus "nuke"
 * `[+]` new bonus "strength boost"
 * `[+]` new bonus "agility boost"
 * `[+]` new bonus "vitality boost"
 * `[+]` new bonus "teleport" and a possibility to switch action mode (fire/teleport)
 * `[+]` possibility to change some input binding (now in test mode, see comment for the commit r198)
 * `[*]` new HUD (Head-Up Display)
 * `[!]` fixed [issue 32](https://github.com/ooxi/violetland/issues/32) (pressing C when the player is dead will crash the game)
 * `[!]` fixed issues [29](https://github.com/ooxi/violetland/issues/29) and [33](https://github.com/ooxi/violetland/issues/33) (problems with adding of new weapons)

Release 0.2.9
-------------

 * `[+]` new bonus "penetration bullets"
 * `[+]` death animations of player
 * `[+]` explantation of perks on char stats screen
 * `[*]` loading screen between pressing "new survival" and starting the game
 * `[*]` view of flashlight
 * `[!]` last entry of high scores table was always zero
 * `[!]` death animations of monsters were played above player
 * `[!]` rendering of main menu on wide resolutions

Release 0.2.8
-------------

 * `[+]` mac os x support (10.5 or later)
 * `[+]` possibility to change graphic settings from options screen (there are issues on windows - see README)
 * `[+]` new graphics - image of weapon in the arms will change, animation of shells
 * `[*]` structure of a weapon store
 * `[*]` monsters can have any number of sounds of death
 * `[!]` fixed sound issues by explicit sound mixing
 * `[!]` issues of loading resources in *nix systems

Release 0.2.7
-------------

 * `[*]` improved animation of monsters
 * `[!]` error on updating highscores

Release 0.2.6
-------------

 * `[+]` new monster animation (spider)
 * `[+]` in-game information line about monster which cursor points to
 * `[*]` parameters of monsters are loaded from files
 * `[*]` names of monsters directories are names of monsters
 * `[*]` redesign of some game images
 * `[!]` sound issues are fixed by temporary turning off music fading

Release 0.2.5
-------------

 * `[+]` new freeze bonus
 * `[+]` animation of monsters death
 * `[+]` corpses of monsters don't disappear
 * `[+]` master sound volume can be adjusted at options screen
 * `[+]` the resume item to main menu
 * `[*]` new separate directory tree for monsters
 * `[!]` game window couldn't be closed with the window close button
 * `[!]` issues of game installing in *nix systems

Release 0.2.4
-------------

 * `[+]` hand grenades were added in addition to main weapon
 * `[+]` ability to select any directory for data at installation
 * `[+]` this file is added to the package
 * `[+]` new weapon M32 (sort of grenade launcher)
 * `[+]` ability to adjusting whether explosions can damage player (friendly fire)
 * `[*]` telekinesis is now more visible and funny
 * `[*]` medical kit can be small, medium or large
 * `[*]` music changer algorithm is changed, so music playback should be more smooth
 * `[*]` game window has cool icon on title bar
 * `[*]` bonuses animation

Release 0.2.3 and earlier
-------------------------

Ancient history was once available at
[Google Code](http://code.google.com/p/violetland/updates/list) and
[nike.homelinux.com](http://nike.homeunix.com/violetland/) but are lost by now.

