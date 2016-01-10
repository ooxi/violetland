PROJECT(violetland)

# CMake 2.8.10 documents the `CMAKE_<LANG>_COMPILER_ID' variable necessary to
# identify Clang, otherwise we can go back to 2.6.0
#
# @see https://cmake.org/cmake/help/v2.8.10/cmake.html#variable:CMAKE_LANG_COMPILER_ID
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Gettext REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)
add_definitions(-DBOOST_FILESYSTEM_VERSION=3)

# Searching for libintl using bundled CMake script
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/fcitx/cmake")
include(FindLibintl)

if(${MINGW})
	set(LIBINTL_LIBRARIES intl)
endif(${MINGW})

set(incDirList  ${SDL_INCLUDE_DIR}  ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${LIBINTL_INCLUDE_DIR})
set(libList ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} ${SDLMIXER_LIBRARY} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES} ${LIBINTL_LIBRARIES})

if (DEFINED LOCALE_INSTALL_DIR)
	set(LOCALEDIR "${LOCALE_INSTALL_DIR}")
else(DEFINED LOCALE_INSTALL_DIR)
	set(LOCALEDIR "${CMAKE_INSTALL_PREFIX}/locale")
endif(DEFINED LOCALE_INSTALL_DIR)

add_subdirectory(po)
include_directories(${incDirList})

# Read build version from `VERSION' file
file(STRINGS "VERSION" VIOLETLAND_VERSION LIMIT_COUNT 1)
add_definitions(-DVIOLETLAND_VERSION="${VIOLETLAND_VERSION}")

set(VS_SOURCE src)

set(srcFileList	
	${VS_SOURCE}/program.cpp
	${VS_SOURCE}/game/Weapon.cpp 
	${VS_SOURCE}/game/WeaponManager.cpp
	${VS_SOURCE}/game/Powerup.cpp 
	${VS_SOURCE}/game/Terrain.cpp
	${VS_SOURCE}/game/MusicManager.cpp
	${VS_SOURCE}/game/Highscores.cpp
	${VS_SOURCE}/game/Resources.cpp
	${VS_SOURCE}/game/GameState.cpp
	${VS_SOURCE}/game/HUD.cpp
	${VS_SOURCE}/game/lifeforms/LifeForm.cpp
	${VS_SOURCE}/game/lifeforms/Player.cpp 
	${VS_SOURCE}/game/lifeforms/Monster.cpp
	${VS_SOURCE}/game/lifeforms/MonsterFactory.cpp
	${VS_SOURCE}/game/lifeforms/MonsterTemplate.cpp
	${VS_SOURCE}/game/bullets/Bullet.cpp
	${VS_SOURCE}/game/bullets/StandardBullet.cpp
	${VS_SOURCE}/game/bullets/LaserBullet.cpp
	${VS_SOURCE}/game/bullets/GrenadeBullet.cpp
	${VS_SOURCE}/game/bullets/Flame.cpp
	${VS_SOURCE}/system/Object.cpp
	${VS_SOURCE}/system/InputHandler.cpp
	${VS_SOURCE}/system/ConfigFile.cpp
	${VS_SOURCE}/system/Configuration.cpp
	${VS_SOURCE}/system/ControlStyle.cpp
	${VS_SOURCE}/system/utility/FileUtility.cpp
	${VS_SOURCE}/system/utility/ImageUtility.cpp
	${VS_SOURCE}/system/utility/UidGenerator.cpp
	${VS_SOURCE}/system/graphic/Texture.cpp
	${VS_SOURCE}/system/graphic/Sprite.cpp 
	${VS_SOURCE}/system/graphic/StaticObject.cpp
	${VS_SOURCE}/system/graphic/DynamicObject.cpp
	${VS_SOURCE}/system/graphic/Aim.cpp
	${VS_SOURCE}/system/graphic/Camera.cpp
	${VS_SOURCE}/system/graphic/Particle.cpp
	${VS_SOURCE}/system/graphic/ParticleSystem.cpp
	${VS_SOURCE}/system/graphic/Explosion.cpp
	${VS_SOURCE}/system/graphic/VideoManager.cpp
	${VS_SOURCE}/system/graphic/text/TextManager.cpp 
	${VS_SOURCE}/system/graphic/text/TextObject.cpp 
	${VS_SOURCE}/system/sound/Sound.cpp
	${VS_SOURCE}/system/sound/SoundManager.cpp
	${VS_SOURCE}/windows/Window.cpp
	${VS_SOURCE}/windows/MainMenuWindow.cpp
	${VS_SOURCE}/windows/CharStatsWindow.cpp
)

add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOCALE_DIR="${LOCALEDIR}")


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


add_executable(violetland ${srcFileList})

target_link_libraries(violetland ${libList})

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
