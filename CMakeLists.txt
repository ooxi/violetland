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
	${srcSystemDir}/Object.cpp
	${srcSystemDir}/InputHandler.cpp
	${srcSystemDir}/ConfigFile.cpp
	${srcSystemDir}/Configuration.cpp
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
	${srcWindowsDir}/HelpWindow.cpp
	${srcWindowsDir}/CharStatsWindow.cpp
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
	README_EN.TXT
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