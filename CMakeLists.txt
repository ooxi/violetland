PROJECT(violetland)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR) 

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OpenGL REQUIRED)

set(incDirList  ${SDL_INCLUDE_DIR}  ${SDLIMAGE_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
set(libList ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY} ${SDLMIXER_LIBRARY} ${OPENGL_LIBRARIES})

include_directories(${incDirList})

set(srcDir src)
set(srcGameDir ${srcDir}/game)
set(srcSystemDir ${srcDir}/system)

set(srcFileList	
	${srcDir}/program.cpp
	${srcGameDir}/Weapon.cpp 
	${srcGameDir}/Powerup.cpp 
	${srcGameDir}/Player.cpp 
	${srcGameDir}/Enemy.cpp 
	${srcGameDir}/Bullet.cpp
	${srcGameDir}/Terrain.cpp
	${srcSystemDir}/FileUtility.cpp 
	${srcSystemDir}/TextManager.cpp 
	${srcSystemDir}/InputHandler.cpp 
	${srcSystemDir}/ImageUtility.cpp
	${srcSystemDir}/Texture.cpp
	${srcSystemDir}/Sprite.cpp
	${srcSystemDir}/Object.cpp
	${srcSystemDir}/LiveObject.cpp
	${srcSystemDir}/StaticObject.cpp
	${srcSystemDir}/DynamicObject.cpp
	${srcSystemDir}/TextObject.cpp
	${srcSystemDir}/Aim.cpp
	${srcSystemDir}/Camera.cpp
	${srcSystemDir}/Sound.cpp
	${srcSystemDir}/SoundManager.cpp
)

add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

add_executable(violetland ${srcFileList})

target_link_libraries(violetland ${libList})

set(relResPath share/violetland)

install(TARGETS violetland DESTINATION bin)
install(FILES
	weapons
	README_EN.TXT
	README_RU.TXT
DESTINATION ${relResPath})
install(FILES fonts/harabara.ttf DESTINATION ${relResPath}/fonts)
install(FILES
	sounds/ak-47_reload.ogg
	sounds/ak-47_shot.ogg
	sounds/ambient_zombie.ogg
	sounds/player_hit_0.ogg
	sounds/player_hit_1.ogg
	sounds/player_hit_2.ogg
	sounds/player_killed.ogg
	sounds/pm_reload.ogg
	sounds/pm_shot.ogg
	sounds/remington_shot.ogg
	sounds/remington_reload.ogg
	sounds/uzi_reload.ogg
	sounds/uzi_shot.ogg
	sounds/zombie_hit_1.ogg
	sounds/zombie_hit_2.ogg
DESTINATION ${relResPath}/sounds)
install(FILES
	images/blood_0.png
	images/blood_1.png
	images/blood_2.png
	images/player_top.png 
	images/pistol_bullet.png 
	images/medikit.png
	images/pm.png
	images/ak.png
	images/uzi.png
	images/remington.png 
DESTINATION ${relResPath}/images)
install (FILES
	images/terrain/base_0.png
	images/terrain/base_1.png
	images/terrain/base_2.png
	images/terrain/base_3.png
DESTINATION ${relResPath}/images/terrain)
install (FILES
	images/terrain/0/0.png
	images/terrain/0/1.png
DESTINATION ${relResPath}/images/terrain/0)
install (FILES
	images/terrain/1/0.png
	images/terrain/1/1.png
DESTINATION ${relResPath}/images/terrain/1)
install (FILES
	images/terrain/2/0.png
	images/terrain/2/1.png
	images/terrain/2/2.png
DESTINATION ${relResPath}/images/terrain/2)
install (FILES
	images/terrain/3/0.png
	images/terrain/3/1.png
DESTINATION ${relResPath}/images/terrain/3)
install (FILES
	images/anima/bleed/bleed-0.png
	images/anima/bleed/bleed-1.png
	images/anima/bleed/bleed-2.png
	images/anima/bleed/bleed-3.png
	images/anima/bleed/bleed-4.png
	images/anima/bleed/bleed-5.png
	images/anima/bleed/bleed-6.png
	images/anima/bleed/bleed-7.png
	images/anima/bleed/bleed-8.png
	images/anima/bleed/bleed-9.png
	images/anima/bleed/bleed-10.png
	images/anima/bleed/bleed-11.png
	images/anima/bleed/bleed-12.png
DESTINATION ${relResPath}/images/anima/bleed)
install (FILES
	images/anima/monsters/0/move-0.png
	images/anima/monsters/0/move-1.png
	images/anima/monsters/0/move-2.png
	images/anima/monsters/0/move-3.png
	images/anima/monsters/0/move-4.png
	images/anima/monsters/0/move-5.png
	images/anima/monsters/0/move-6.png
	images/anima/monsters/0/move-7.png
	images/anima/monsters/0/move-8.png
	images/anima/monsters/0/move-9.png
	images/anima/monsters/0/move-10.png
	images/anima/monsters/0/move-11.png
	images/anima/monsters/0/move-12.png
	images/anima/monsters/0/move-13.png
	images/anima/monsters/0/move-14.png
	images/anima/monsters/0/move-15.png
	images/anima/monsters/0/move-16.png
	images/anima/monsters/0/move-17.png
	images/anima/monsters/0/move-18.png
	images/anima/monsters/0/move-19.png
	images/anima/monsters/0/move-20.png
	images/anima/monsters/0/move-21.png
	images/anima/monsters/0/move-22.png
	images/anima/monsters/0/move-23.png
	images/anima/monsters/0/move-24.png
DESTINATION ${relResPath}/images/anima/monsters/0)
install (FILES
	images/anima/monsters/1/move-0.png
	images/anima/monsters/1/move-1.png
	images/anima/monsters/1/move-2.png
	images/anima/monsters/1/move-3.png
	images/anima/monsters/1/move-4.png
	images/anima/monsters/1/move-5.png
	images/anima/monsters/1/move-6.png
	images/anima/monsters/1/move-7.png
	images/anima/monsters/1/move-8.png
	images/anima/monsters/1/move-9.png
	images/anima/monsters/1/move-10.png
	images/anima/monsters/1/move-11.png
	images/anima/monsters/1/move-12.png
	images/anima/monsters/1/move-13.png
	images/anima/monsters/1/move-14.png
	images/anima/monsters/1/move-15.png
	images/anima/monsters/1/move-16.png
	images/anima/monsters/1/move-17.png
	images/anima/monsters/1/move-18.png
	images/anima/monsters/1/move-19.png
	images/anima/monsters/1/move-20.png
	images/anima/monsters/1/move-21.png
	images/anima/monsters/1/move-22.png
	images/anima/monsters/1/move-23.png
	images/anima/monsters/1/move-24.png
DESTINATION ${relResPath}/images/anima/monsters/1)
install (FILES
	images/anima/player/legs-0.png
	images/anima/player/legs-1.png
	images/anima/player/legs-2.png
	images/anima/player/legs-3.png
	images/anima/player/legs-4.png
	images/anima/player/legs-5.png
	images/anima/player/legs-6.png
	images/anima/player/legs-7.png
	images/anima/player/legs-8.png
	images/anima/player/legs-9.png
	images/anima/player/legs-10.png
	images/anima/player/legs-11.png
	images/anima/player/legs-12.png
	images/anima/player/legs-13.png
	images/anima/player/legs-14.png
	images/anima/player/legs-15.png
	images/anima/player/legs-16.png
	images/anima/player/legs-17.png
	images/anima/player/legs-18.png
	images/anima/player/legs-19.png
	images/anima/player/legs-20.png
	images/anima/player/legs-21.png
	images/anima/player/legs-22.png
	images/anima/player/legs-23.png
	images/anima/player/legs-24.png
DESTINATION ${relResPath}/images/anima/player)
