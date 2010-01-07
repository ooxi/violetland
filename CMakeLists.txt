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
set(srcBulletsDir ${srcGameDir}/bullets)
set(srcSystemDir ${srcDir}/system)
set(srcGraphicDir ${srcSystemDir}/graphic)
set(srcTextDir ${srcGraphicDir}/text)
set(srcSoundDir ${srcSystemDir}/sound)
set(srcUtilityDir ${srcSystemDir}/utility)

set(srcFileList	
	${srcDir}/program.cpp
	${srcGameDir}/Weapon.cpp 
	${srcGameDir}/Powerup.cpp 
	${srcGameDir}/Player.cpp 
	${srcGameDir}/Enemy.cpp 
	${srcGameDir}/Terrain.cpp
	${srcGameDir}/MusicManager.cpp
	${srcGameDir}/LifeForm.cpp
	${srcGameDir}/Highscores.cpp
	${srcGameDir}/Explosion.cpp
	${srcGameDir}/MonsterFactory.cpp
	${srcBulletsDir}/Bullet.cpp
	${srcBulletsDir}/StandardBullet.cpp
	${srcBulletsDir}/LaserBullet.cpp
	${srcBulletsDir}/GrenadeBullet.cpp
	${srcSystemDir}/Object.cpp
	${srcSystemDir}/InputHandler.cpp
	${srcSystemDir}/ConfigFile.cpp
	${srcSystemDir}/Configuration.cpp
	${srcSystemDir}/GameState.cpp
	${srcUtilityDir}/FileUtility.cpp
	${srcUtilityDir}/ImageUtility.cpp
	${srcGraphicDir}/Texture.cpp
	${srcGraphicDir}/Sprite.cpp 
	${srcGraphicDir}/StaticObject.cpp
	${srcGraphicDir}/DynamicObject.cpp
	${srcGraphicDir}/Aim.cpp
	${srcGraphicDir}/Camera.cpp
	${srcGraphicDir}/Window.cpp
	${srcGraphicDir}/Particle.cpp
	${srcGraphicDir}/ParticleSystem.cpp
	${srcTextDir}/TextManager.cpp 
	${srcTextDir}/TextObject.cpp 
	${srcSoundDir}/Sound.cpp
	${srcSoundDir}/SoundManager.cpp
)

add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

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
	weapons
	README_EN.TXT
	README_RU.TXT
	icon-light.png
DESTINATION ${relResPath})
install(FILES fonts/harabara.ttf DESTINATION ${relResPath}/fonts)
install(FILES
	sounds/ak-47_reload.ogg
	sounds/ak-47_shot.ogg
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
	sounds/laser_reload.ogg
	sounds/laser_shot.ogg
	sounds/m32_shot.ogg
	sounds/explode-0.ogg
	sounds/explode-1.ogg
DESTINATION ${relResPath}/sounds)
install(FILES
	images/blood_0.png
	images/blood_1.png
	images/blood_2.png
	images/player_top.png 
	images/medikit.png
	images/grenade.png
	images/crystal.png
	images/freeze.png
	images/pm.png
	images/laser.png
	images/ak.png
	images/uzi.png
	images/remington.png
	images/m32.png
	images/splash_0.png
	images/splash_1.png 
	images/expl_0.png
	images/expl_1.png
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
	images/anima/grenade/0.png
	images/anima/grenade/1.png
	images/anima/grenade/2.png
	images/anima/grenade/3.png
	images/anima/grenade/4.png
	images/anima/grenade/5.png
	images/anima/grenade/6.png
	images/anima/grenade/7.png
	images/anima/grenade/8.png
	images/anima/grenade/9.png
	images/anima/grenade/10.png
	images/anima/grenade/11.png
DESTINATION ${relResPath}/images/anima/grenade)
install (FILES
	monsters/0/walk/0.png
	monsters/0/walk/1.png
	monsters/0/walk/2.png
	monsters/0/walk/3.png
	monsters/0/walk/4.png
	monsters/0/walk/5.png
	monsters/0/walk/6.png
	monsters/0/walk/7.png
	monsters/0/walk/8.png
	monsters/0/walk/9.png
	monsters/0/walk/10.png
	monsters/0/walk/11.png
	monsters/0/walk/12.png
	monsters/0/walk/13.png
	monsters/0/walk/14.png
	monsters/0/walk/15.png
	monsters/0/walk/16.png
	monsters/0/walk/17.png
	monsters/0/walk/18.png
	monsters/0/walk/19.png
	monsters/0/walk/20.png
	monsters/0/walk/21.png
	monsters/0/walk/22.png
	monsters/0/walk/23.png
DESTINATION ${relResPath}/monsters/0/walk)
install (FILES
	monsters/0/death/0.png
	monsters/0/death/1.png
	monsters/0/death/2.png
	monsters/0/death/3.png
	monsters/0/death/4.png
	monsters/0/death/5.png
	monsters/0/death/6.png
	monsters/0/death/7.png
	monsters/0/death/8.png
	monsters/0/death/9.png
	monsters/0/death/10.png
	monsters/0/death/11.png
	monsters/0/death/12.png
	monsters/0/death/13.png
	monsters/0/death/14.png
	monsters/0/death/15.png
	monsters/0/death/16.png
	monsters/0/death/17.png
	monsters/0/death/18.png
	monsters/0/death/19.png
	monsters/0/death/20.png
	monsters/0/death/21.png
	monsters/0/death/22.png
	monsters/0/death/23.png
DESTINATION ${relResPath}/monsters/0/death)
install (FILES
	monsters/0/hit.ogg
DESTINATION ${relResPath}/monsters/0)
install (FILES
	monsters/1/walk/0.png
	monsters/1/walk/1.png
	monsters/1/walk/2.png
	monsters/1/walk/3.png
	monsters/1/walk/4.png
	monsters/1/walk/5.png
	monsters/1/walk/6.png
	monsters/1/walk/7.png
	monsters/1/walk/8.png
	monsters/1/walk/9.png
	monsters/1/walk/10.png
	monsters/1/walk/11.png
	monsters/1/walk/12.png
	monsters/1/walk/13.png
	monsters/1/walk/14.png
	monsters/1/walk/15.png
	monsters/1/walk/16.png
	monsters/1/walk/17.png
	monsters/1/walk/18.png
	monsters/1/walk/19.png
	monsters/1/walk/20.png
	monsters/1/walk/21.png
	monsters/1/walk/22.png
	monsters/1/walk/23.png
DESTINATION ${relResPath}/monsters/1/walk)
install (FILES
	monsters/1/death/0.png
	monsters/1/death/1.png
	monsters/1/death/2.png
	monsters/1/death/3.png
	monsters/1/death/4.png
	monsters/1/death/5.png
	monsters/1/death/6.png
	monsters/1/death/7.png
	monsters/1/death/8.png
	monsters/1/death/9.png
	monsters/1/death/10.png
	monsters/1/death/11.png
	monsters/1/death/12.png
	monsters/1/death/13.png
	monsters/1/death/14.png
	monsters/1/death/15.png
	monsters/1/death/16.png
	monsters/1/death/17.png
	monsters/1/death/18.png
	monsters/1/death/19.png
	monsters/1/death/20.png
	monsters/1/death/21.png
	monsters/1/death/22.png
	monsters/1/death/23.png
DESTINATION ${relResPath}/monsters/1/death)
install (FILES
	monsters/1/hit.ogg
DESTINATION ${relResPath}/monsters/1)
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
install (FILES
	music/01.ogg
	music/02.ogg
	music/03.ogg
	music/04.ogg
	music/05.ogg
DESTINATION ${relResPath}/music)
