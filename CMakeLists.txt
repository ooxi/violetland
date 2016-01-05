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

set(srcDir src)
set(srcWindowsDir ${srcDir}/windows)
set(srcGameDir ${srcDir}/game)
set(srcBulletsDir ${srcGameDir}/bullets)
set(srcLifeFormsDir ${srcGameDir}/lifeforms)
set(srcSystemDir ${srcDir}/system)
set(srcGraphicDir ${srcSystemDir}/graphic)
set(srcTextDir ${srcGraphicDir}/text)
set(srcSoundDir ${srcSystemDir}/sound)
set(srcUtilityDir ${srcSystemDir}/utility)

set(srcFileList	
	${srcDir}/program.cpp
	${srcGameDir}/Weapon.cpp 
	${srcGameDir}/WeaponManager.cpp
	${srcGameDir}/Powerup.cpp 
	${srcGameDir}/Terrain.cpp
	${srcGameDir}/MusicManager.cpp
	${srcGameDir}/Highscores.cpp
	${srcGameDir}/Resources.cpp
	${srcGameDir}/GameState.cpp
	${srcGameDir}/HUD.cpp
	${srcLifeFormsDir}/LifeForm.cpp
	${srcLifeFormsDir}/Player.cpp 
	${srcLifeFormsDir}/Monster.cpp
	${srcLifeFormsDir}/MonsterFactory.cpp
	${srcLifeFormsDir}/MonsterTemplate.cpp
	${srcBulletsDir}/Bullet.cpp
	${srcBulletsDir}/StandardBullet.cpp
	${srcBulletsDir}/LaserBullet.cpp
	${srcBulletsDir}/GrenadeBullet.cpp
	${srcBulletsDir}/Flame.cpp
	${srcSystemDir}/Object.cpp
	${srcSystemDir}/InputHandler.cpp
	${srcSystemDir}/ConfigFile.cpp
	${srcSystemDir}/Configuration.cpp
	${srcSystemDir}/ControlStyle.cpp
	${srcUtilityDir}/FileUtility.cpp
	${srcUtilityDir}/ImageUtility.cpp
	${srcUtilityDir}/UidGenerator.cpp
	${srcGraphicDir}/Texture.cpp
	${srcGraphicDir}/Sprite.cpp 
	${srcGraphicDir}/StaticObject.cpp
	${srcGraphicDir}/DynamicObject.cpp
	${srcGraphicDir}/Aim.cpp
	${srcGraphicDir}/Camera.cpp
	${srcGraphicDir}/Window.cpp
	${srcGraphicDir}/Particle.cpp
	${srcGraphicDir}/ParticleSystem.cpp
	${srcGraphicDir}/Explosion.cpp
	${srcGraphicDir}/VideoManager.cpp
	${srcTextDir}/TextManager.cpp 
	${srcTextDir}/TextObject.cpp 
	${srcSoundDir}/Sound.cpp
	${srcSoundDir}/SoundManager.cpp
	${srcWindowsDir}/MainMenuWindow.cpp
	${srcWindowsDir}/CharStatsWindow.cpp
)

add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-DLOCALE_DIR="${LOCALEDIR}")


# Compiler diagnostics are most useful
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# clang++ does not boost_static_assert_typedef's unused attribute
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
