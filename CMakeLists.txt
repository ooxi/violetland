PROJECT(violetland)

# CMake 2.8.10 documents the `CMAKE_<LANG>_COMPILER_ID' variable necessary to
# identify Clang, otherwise we can go back to 2.6.0
#
# @see https://cmake.org/cmake/help/v2.8.10/cmake.html#variable:CMAKE_LANG_COMPILER_ID
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)


# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)


# SDL
find_package(SDL REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDL_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDL_LIBRARY})


# SDL_image
find_package(SDL_image REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDLIMAGE_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDLIMAGE_LIBRARY})


# SDL_ttf
find_package(SDL_ttf REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDLTTF_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDLTTF_LIBRARY})


# SDL_mixer
find_package(SDL_mixer REQUIRED)
set(VIOLET_INCLUDE_DIRS	${VIOLET_INCLUDE_DIRS}	${SDLMIXER_INCLUDE_DIR})
set(VIOLET_LIBRARIES	${VIOLET_LIBRARIES}	${SDLMIXER_LIBRARY})


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





add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOCALE_DIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")

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



# Compiler diagnostics are most useful
add_compile_options(-Wall -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# clang++ does not understand boost_static_assert_typedef's unused
	# attribute
	add_compile_options(-Wno-unused-local-typedef)
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
		add_compile_options(-mwindows)
	endif()

endif()



# Build executable
include_directories(${VIOLET_INCLUDE_DIRS})
add_executable(violetland ${VIOLET_RESOURCES} ${VIOLET_SOURCES})
target_link_libraries(violetland ${VIOLET_LIBRARIES})



# Install executable and assets
add_definitions(-DDATA_INSTALL_DIR="${CMAKE_INSTALL_FULL_DATADIR}/violetland/")

install(TARGETS violetland DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES
	icon-light.png
DESTINATION ${CMAKE_INSTALL_DATADIR}/violetland)
install(FILES
	README.md
	README_RU.TXT
DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY
	fonts
	sounds
	images
	monsters
	weapon
	music
DESTINATION ${CMAKE_INSTALL_DATADIR}/violetland
PATTERN ".*" EXCLUDE
PATTERN "*~" EXCLUDE)
install(FILES
    violetland.desktop
DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES
    icon-light.png
DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pixmaps
RENAME violetland.png)
