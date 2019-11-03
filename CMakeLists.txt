PROJECT(violetland)

# CMake 3.2 is necessary in order to use FindIntl. If you want to compile
# violetland on an older system (like plain Ubuntu 14.04) you can revert
# da80351581f826f0f4c1c2f7b494ed51a6acec51 and it will work most likely down to
# CMake 2.8.12
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules/sdl2)



# SDL
find_package(SDL2 REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDL2_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDL2_LIBRARY})


# SDL_image
find_package(SDL2_image REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDL2_IMAGE_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDL2_IMAGE_LIBRARY})


# SDL_ttf
find_package(SDL2_ttf REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDL2_TTF_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDL2_TTF_LIBRARY})


# SDL_mixer
find_package(SDL2_mixer REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDL2_MIXER_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDL2_MIXER_LIBRARY})


# OpenGL
find_package(OpenGL REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${OPENGL_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${OPENGL_LIBRARIES})


# Boost
find_package(Boost COMPONENTS filesystem system REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${Boost_INCLUDE_DIRS})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${Boost_LIBRARIES})
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)


# Intl
find_package(Intl REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${Intl_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${Intl_LIBRARIES})





# LOCALEDIR must be set up before including po/CMakeLists.txt
if (DEFINED LOCALE_INSTALL_DIR)
	set(LOCALEDIR "${LOCALE_INSTALL_DIR}")
else(DEFINED LOCALE_INSTALL_DIR)
	set(LOCALEDIR "${CMAKE_INSTALL_PREFIX}/locale")
endif(DEFINED LOCALE_INSTALL_DIR)

add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOCALE_DIR="${LOCALEDIR}")

add_subdirectory(po)



# Read build version from `VERSION' file
file(STRINGS "VERSION" VIOLETLAND_VERSION LIMIT_COUNT 1)
add_definitions(-DVIOLETLAND_VERSION="${VIOLETLAND_VERSION}")



# Windows resources
set(VIOLET_RESOURCE_DIR rc)

set(VIOLET_RESOURCES
	# rc/
	${VIOLET_RESOURCE_DIR}/icon.rc
)


# Sources, ordered by
#
#  1) first files, than directories
#  2) alphabetically, case insensitive
#  3) shorter file names first
#
set(VIOLET_SOURCE_DIR src)

set(VIOLET_SOURCES
	# src/
	${VIOLET_SOURCE_DIR}/program.cpp

	# src/game
	${VIOLET_SOURCE_DIR}/game/GameState.cpp
	${VIOLET_SOURCE_DIR}/game/Highscores.cpp
	${VIOLET_SOURCE_DIR}/game/HUD.cpp
	${VIOLET_SOURCE_DIR}/game/MusicManager.cpp
	${VIOLET_SOURCE_DIR}/game/Powerup.cpp
	${VIOLET_SOURCE_DIR}/game/Resources.cpp
	${VIOLET_SOURCE_DIR}/game/Terrain.cpp
	${VIOLET_SOURCE_DIR}/game/Weapon.cpp
	${VIOLET_SOURCE_DIR}/game/WeaponManager.cpp

	# src/game/bullets
	${VIOLET_SOURCE_DIR}/game/bullets/Bullet.cpp
	${VIOLET_SOURCE_DIR}/game/bullets/Flame.cpp
	${VIOLET_SOURCE_DIR}/game/bullets/GrenadeBullet.cpp
	${VIOLET_SOURCE_DIR}/game/bullets/LaserBullet.cpp
	${VIOLET_SOURCE_DIR}/game/bullets/StandardBullet.cpp

	# src/game/lifeforms
	${VIOLET_SOURCE_DIR}/game/lifeforms/LifeForm.cpp
	${VIOLET_SOURCE_DIR}/game/lifeforms/Monster.cpp
	${VIOLET_SOURCE_DIR}/game/lifeforms/MonsterFactory.cpp
	${VIOLET_SOURCE_DIR}/game/lifeforms/MonsterTemplate.cpp
	${VIOLET_SOURCE_DIR}/game/lifeforms/Player.cpp

	# src/system
	${VIOLET_SOURCE_DIR}/system/ConfigFile.cpp
	${VIOLET_SOURCE_DIR}/system/Configuration.cpp
	${VIOLET_SOURCE_DIR}/system/ControlStyle.cpp
	${VIOLET_SOURCE_DIR}/system/InputHandler.cpp
	${VIOLET_SOURCE_DIR}/system/Object.cpp

	# src/system/graphic
	${VIOLET_SOURCE_DIR}/system/graphic/Aim.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/Camera.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/DynamicObject.cpp	
	${VIOLET_SOURCE_DIR}/system/graphic/Explosion.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/Particle.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/ParticleSystem.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/Sprite.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/StaticObject.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/Texture.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/VideoManager.cpp

	# src/system/graphic/text
	${VIOLET_SOURCE_DIR}/system/graphic/text/TextManager.cpp
	${VIOLET_SOURCE_DIR}/system/graphic/text/TextObject.cpp

	# src/system/sound
	${VIOLET_SOURCE_DIR}/system/sound/Sound.cpp
	${VIOLET_SOURCE_DIR}/system/sound/SoundManager.cpp

	# src/system/utility
	${VIOLET_SOURCE_DIR}/system/utility/FileUtility.cpp
	${VIOLET_SOURCE_DIR}/system/utility/ImageUtility.cpp
	${VIOLET_SOURCE_DIR}/system/utility/UidGenerator.cpp

	# src/windows
	${VIOLET_SOURCE_DIR}/windows/ChangeInputBindingWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/CharStatsWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/ControlsMenuWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/HighscoresWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/MainMenuWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/OptionsWindow.cpp
	${VIOLET_SOURCE_DIR}/windows/Window.cpp
	${VIOLET_SOURCE_DIR}/windows/WindowManager.cpp
)



# Specify C++ standard, I would love to use C++11 but at least on Ubuntu boost
# is compiled with C++98 which will result in linking errors. Seems to be fixed
# in boost 1.57 but Ubuntu before wily does not ship that version.
#
# @see https://svn.boost.org/trac/boost/ticket/6779
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

# Compiler diagnostics are most useful
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# clang++ does not understand boost_static_assert_typedef's unused
	# attribute
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedef")
endif()



# Generated executable should use the GUI subsystem on Windows instead of the
# character subsystem (thus not spawning a console window).
#
# @see https://msdn.microsoft.com/en-us/windows/hardware/gg463119.aspx
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

	# GCC's `-mwindows` will instruct the linker to set the PE header subsystem
	# type appropriately
	#
	# @see https://gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/x86-Windows-Options.html#x86-Windows-Options
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
	endif()

endif()



# Build executable
include_directories(${VIOLET_INCLUDE_DIRS})
add_executable(violetland ${VIOLET_RESOURCES} ${VIOLET_SOURCES})
target_link_libraries(violetland ${VIOLET_LIBRARIES})



# Install executable and assets
if(DEFINED DATA_INSTALL_DIR)
	set(relResPath ${DATA_INSTALL_DIR})
	add_definitions(-DDATA_INSTALL_DIR="${DATA_INSTALL_DIR}/")
else(DEFINED DATA_INSTALL_DIR)
	set(relResPath share/violetland/)
endif(DEFINED DATA_INSTALL_DIR)

install(TARGETS violetland DESTINATION bin)
install(FILES
	README.md
	README_RU.TXT
	icon-light.png
DESTINATION ${relResPath})
install(DIRECTORY
	fonts
	sounds
	images
	monsters
	weapon
	music
DESTINATION ${relResPath}
PATTERN ".*" EXCLUDE
PATTERN "*~" EXCLUDE)
install(FILES
    violetland.desktop
DESTINATION share/applications)
install(FILES
    icon-light.png
DESTINATION share/pixmaps
RENAME violetland.png)
