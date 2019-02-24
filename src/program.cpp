// MS Windows compatibility
#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <windows.h>
#include <winbase.h>
#include <time.h>
#else
#include "pwd.h"
#endif

#include <sstream>

// Apple Mac OS X compatibility
#ifdef __APPLE__
#include "system/utility/macBundlePath.h"
#endif

// SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"

// Boost
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

//~ Internationalization Dependances
#include <libintl.h>
#include <locale.h>
#define _(STRING)			gettext(STRING)

// The Game
#include "game/Game.h"
#include "system/Configuration.h"
#include "system/InputHandler.h"
#include "system/utility/ImageUtility.h"
#include "system/utility/FileUtility.h"
#include "system/graphic/text/TextManager.h"
#include "system/graphic/Texture.h"
#include "system/graphic/Aim.h"
#include "system/graphic/Camera.h"
#include "system/graphic/Particle.h"
#include "system/graphic/VideoManager.h"
#include "system/graphic/Explosion.h"
#include "system/sound/SoundManager.h"
#include "game/GameState.h"
#include "game/Powerup.h"
#include "game/Resources.h"
#include "game/lifeforms/MonsterFactory.h"
#include "game/lifeforms/Monster.h"
#include "game/lifeforms/Player.h"
#include "game/Terrain.h"
#include "game/MusicManager.h"
#include "game/WeaponManager.h"
#include "game/Highscores.h"
#include "game/HUD.h"
#include "windows/ChangeInputBindingWindow.h"
#include "windows/CharStatsWindow.h"
#include "windows/ControlsMenuWindow.h"
#include "windows/HighscoresWindow.h"
#include "windows/MainMenuWindow.h"
#include "windows/OptionsWindow.h"

using namespace std;
using namespace violet;

const string PROJECT = "violetland";
const string VERSION = VIOLETLAND_VERSION;
const string DEFAULT_CHAR_NAME = "Violet";
const int MOUSEORBIT = 200;

Configuration* config;
Configuration* tempConfig;
VideoManager* videoManager;
Camera* cam;

Aim* aim;
HUD* hud;
StaticObject* splash;

FileUtility* fileUtility = NULL;
InputHandler* input;
SoundManager* sndManager;
Resources* resources;
MusicManager* musicManager;
WeaponManager* weaponManager;
MonsterFactory* monsterFactory;

vector<ParticleSystem*> particleSystems;
vector<StaticObject*> bloodStains;
Terrain* terrain;

GameState* gameState;
Game* game;
string playerId;

map<string, Window*> windows;

bool roulette(float eventProbability) {
	return (float) (rand() % 100000) < eventProbability * 100000.0f;
}

// Creation of clear squares of an earth surface
void createTerrain() {
	if (terrain)
		delete terrain;

	cout << "Forming terrain..." << endl;

	boost::filesystem::path tilesDir = fileUtility->getFullPath(
			FileUtility::image, "terrain");
	unsigned baseTex = rand() % fileUtility->getFilesCountFromDir(tilesDir);

	ostringstream oss;
	oss << baseTex;
	unsigned tilesCount = fileUtility->getFilesCountFromDir(
			boost::filesystem::path(tilesDir) /= oss.str());

	oss.str("");
	oss << "base_" << baseTex << ".png";
	SDL_Surface *terrainSurface = ImageUtility::loadImage(
			boost::filesystem::path(tilesDir) /= oss.str());

	vector<SDL_Surface*> tiles;
	for (unsigned i = 0; i < tilesCount; i++) {
		oss.str("");
		oss << baseTex << '/' << i << ".png";
		SDL_Surface *tile = ImageUtility::loadImage(
				boost::filesystem::path(tilesDir) /= oss.str());
		tiles.push_back(tile);
	}

	terrain = new Terrain(terrainSurface, tiles, gameState->GameAreaSize);

	SDL_FreeSurface(terrainSurface);
	for (unsigned int i = 0; i < tilesCount; i++) {
		SDL_FreeSurface(tiles[i]);
	}
	tiles.clear();
}

// Creation of a new monster
void spawnEnemy(float x, float y, float r, int baseLvl, int lvl) {
	float spawnAngle = (rand() % 6300) / 1000.0f;

	Monster* newMonster = monsterFactory->create(baseLvl, lvl);

	newMonster->X = x + r * cos(spawnAngle);
	newMonster->Y = y + r * sin(spawnAngle);

	switch (gameState->Mode) {
	case GAMEMODE_SURVIVAL:
	case GAMEMODE_WAVES:
		LifeForm* player = gameState->getLifeForm(playerId);

		newMonster->TargetX = player->X;
		newMonster->TargetY = player->Y;
		break;
	}

	gameState->lifeForms.insert(
			map<string, LifeForm*>::value_type(newMonster->Id, newMonster));
}

// Start the game in selected mode
void MainMenuWindow::onStartClick() {
	cam->setW(1600);

	hud->reset();

	// TODO: encapsulate the aim in the HUD
	if (aim)
		delete aim;
	aim = new Aim(config);

	glClear(GL_COLOR_BUFFER_BIT);

	cam->X = cam->Y = 0.0f;

	cam->applyGLOrtho();

	splash->draw(false, false);

	videoManager->RegularText->draw(_("Please wait..."), 0, 0,
			TextManager::CENTER, TextManager::MIDDLE);

	SDL_GL_SwapBuffers();

	gameState->start();

	clearVector<StaticObject*> (&bloodStains);
	clearVector<ParticleSystem*> (&particleSystems);

	Player* player = new Player(
			0,
			0,
			resources->PlayerWalkSprite,
			resources->PlayerDeathSprites[(rand()
					% (int) resources->PlayerDeathSprites.size())],
			resources->PlayerShieldSprite, 
			resources->PlayerHitSounds, resources->PlayerDeathSound);

	// TODO: add "default weapon" parameter to config file
	player->setWeapon(weaponManager->getWeaponByName("PM"));
	player->HitR = 0.28f;

	gameState->lifeForms.insert(
			map<string, LifeForm*>::value_type(player->Id, player));

	playerId = player->Id;

	createTerrain();

	SDL_ShowCursor(0);

	for (unsigned int i = 0; i < config->MonstersAtStart; i++) {
		spawnEnemy(0, 0, (float) cam->getW(), 1, 1);
	}

	CloseFlag = true;

	hud->addMessage(_("Try to survive as long as you can."));
	hud->addMessage(
			_("Shoot monsters to receive experience and other bonuses."));
}

// Creation of a string for the window title
inline string getProjectTitle() {
	return PROJECT + " v" + VERSION;
}

// Outputs the inboost::formation on the program and the runtime environment
void printVersion() {
	string env = "UNKNOWN";
#ifdef _WIN32
	env = "WINDOWS";
#endif //_WIN32W
#ifdef __linux__
	env = "GNU/LINUX";
#endif //__linux__
#ifdef __FreeBSD__
	env = "FREEBSD";
#endif //__FreeBSD__
#ifdef __NetBSD__
	env = "NETBSD";
#endif //__NetBSD__
#ifdef __APPLE__
	env = "MAC";
#endif //__APPLE__
	cout << getProjectTitle() << " @ " << env << endl;
}

string getDefaultName() {
#ifdef _WIN32
	return DEFAULT_CHAR_NAME;
#else
	
	// Try to read username from system environment
	char* user = getenv("USER");
	
	// `user' can be null if `USER' environment property not found
	//
	// @see http://linux.die.net/man/3/getenv
	// @see https://github.com/ooxi/violetland/issues/71
	if (!user) {
		return DEFAULT_CHAR_NAME;
	}
	
	string name = user;
	if (name.empty()) {
		struct passwd *p;
		uid_t uid;

		uid = geteuid();
		if ((p = getpwuid(uid)))
			name = p->pw_name;

		if (name.empty()) {
			return DEFAULT_CHAR_NAME;
		}
	}

	return name;
#endif
}

void initInternationlization() {
	//~
	//~ Internationalization Initialization
	//~

	//~ i18n: initializes the entire current locale of the program as per environment variables set by the user
	setlocale(LC_ALL, "");

	//~ i18n: Indicate the path of the i18n catalog file
	bindtextdomain("violetland", LOCALE_DIR);

	//~ i18n: sets the message domain
	textdomain("violetland");

	InputHandler::initEventNames();
}

// Creation of system objects and their customization
void initSystem() {
	std::cout << (boost::format(_("Name found : %s")) % getDefaultName())
			<< std::endl;

	initInternationlization();

	srand((unsigned) time(NULL));

	TTF_Init();
	atexit(TTF_Quit);
	atexit(SDL_Quit);

	cout << "SDL_Init..." << endl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cerr << "Couldn't initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	videoManager = new VideoManager(fileUtility);

	cam = new Camera();
	videoManager->setMode(config->Screen, cam);

	cout << _("Preparing window...") << endl;

	SDL_WM_SetCaption(getProjectTitle().c_str(), NULL);

	SDL_Surface* icon = ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::common, "icon-light.png"));
	SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);

	SDL_EnableUNICODE(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	cout << _("Drawing splash screen...") << endl;

	ostringstream oss;
	oss << "splash_" << rand() % 2 << ".png";
	Texture* tex = new Texture(
			ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::image, oss.str())),
			GL_TEXTURE_2D, GL_LINEAR, true);

	splash = new StaticObject(0, 0, tex->getWidth(), tex->getHeight(), tex,
			true);

	glClear(GL_COLOR_BUFFER_BIT);

	cam->X = cam->Y = 0.0f;

	cam->applyGLOrtho();

	splash->draw(false, false);

	videoManager->RegularText->draw(_("Please wait..."), 0, 0,
			TextManager::CENTER, TextManager::MIDDLE);

	SDL_GL_SwapBuffers();

	cout << _("Preparing sound systems...") << endl;

	sndManager = new SoundManager(fileUtility, config);
	musicManager = new MusicManager(fileUtility, sndManager, config);

	musicManager->play();

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// flashlight
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.6f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0001f);

	GLfloat nvColor[] = { 0.3f, 1.0f, 0.3f, 1.0f };

	// night vision perk
	glLightfv(GL_LIGHT1, GL_DIFFUSE, nvColor);
	glLightfv(GL_LIGHT1, GL_AMBIENT, nvColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, nvColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.00001f);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	input = new InputHandler(config->PlayerInputBinding);

	gameState = new GameState();
	game = new Game(input, hud, config, gameState, resources, particleSystems);
}

void loseGame(Player* player) {
	gameState->Lost = true;

	hud->addMessage(_("You are gobbled up."));

	Highscores s(fileUtility);
	gameState->HighScore = s.isHighscore(player->Xp);

	if (gameState->HighScore) {
		gameState->PlayerName = getDefaultName();
		input->setInputModeText(true, gameState->PlayerName);
	}

	SDL_ShowCursor(1);
}

// Switches a pause mode
void switchGamePause() {
	gameState->Paused = !gameState->Paused;
	if (gameState->Paused)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

void refreshCharStatsWindow() {
	CharStatsWindow* charStats =
			(CharStatsWindow*) windows.find("charstats")->second;
	charStats->refresh();
}

void createCharStatWindow() {
	CharStatsWindow *charStats = new CharStatsWindow(config, videoManager,
			(Player*) gameState->getLifeForm(playerId));
	windows["charstats"] = charStats;
}

void shutdownSystem() {
	clearVector<ParticleSystem*> (&particleSystems);
	clearVector<StaticObject*> (&bloodStains);

	delete terrain;
	delete videoManager;
	delete gameState;
	delete musicManager;
	delete sndManager;
	delete input;
	delete fileUtility;
	delete splash;
}

void OptionsWindow::onAutoReloadClick() {
	config->AutoReload = !config->AutoReload;
	refresh(tempConfig);
}

void OptionsWindow::onAutoWeaponPickupClick() {
	config->AutoWeaponPickup = !config->AutoWeaponPickup;
	refresh(tempConfig);
}

void OptionsWindow::onFreeCursorMoveClick() {
	config->FreeCursorMove = !config->FreeCursorMove;
	refresh(tempConfig);
}

void OptionsWindow::onFriendlyFireClick() {
	config->FriendlyFire = !config->FriendlyFire;
	refresh(tempConfig);
}

void OptionsWindow::onFullscreenClick() {
	config->Screen.Full = !config->Screen.Full;
	refresh(tempConfig);
}

void OptionsWindow::switchVolumeDown(std::string elementName) {
	if (elementName == "musicvolume") {
		if (config->MusicVolume > 0) {
			config->MusicVolume--;
			Mix_VolumeMusic(config->MusicVolume * 12);
		} else {
			config->MusicVolume = 10;
			Mix_VolumeMusic(config->MusicVolume * 12);
		}
	} else if (elementName == "soundvolume") {
		if (config->SoundVolume > 0) {
			config->SoundVolume--;
			for (unsigned int a = 1; a <= 8; a++) {
				Mix_Volume(a, config->SoundVolume * 12);
			}
		} else {
			config->SoundVolume = 10;
			for (unsigned int a = 1; a <= 8; a++) {
				Mix_Volume(a, config->SoundVolume * 12);
			}
		}
	} else
		return;

	refresh(tempConfig);
}

void OptionsWindow::switchVolumeUp(std::string elementName) {
	if (elementName == "musicvolume") {
		if (config->MusicVolume <= 9) {
			config->MusicVolume++;
			Mix_VolumeMusic(config->MusicVolume * 12);
		} else {
			config->MusicVolume = 0;
			Mix_VolumeMusic(0);
		}
	} else if (elementName == "soundvolume") {
		if (config->SoundVolume <= 9) {
			config->SoundVolume++;
			for (unsigned int a = 1; a <= 8; a++) {
				Mix_Volume(a, config->SoundVolume * 12);
			}
		} else {
			config->SoundVolume = 0;
			for (unsigned int a = 1; a <= 8; a++) {
				Mix_Volume(a, 0);
			}
		}
	} else
		return;

	refresh(tempConfig);
}

void OptionsWindow::onResolutionDownClick() {
	vector<SDL_Rect> modes = videoManager->GetAvailableModes();

	bool set = false;
	for (int i = modes.size() - 1; i > 0; i--) {
		if (tempConfig->Screen.Width == modes[i].w && tempConfig->Screen.Height
				== modes[i].h) {
			tempConfig->Screen.Width = modes[i - 1].w;
			tempConfig->Screen.Height = modes[i - 1].h;
			set = true;
			break;
		}
	}
	if (!set) {
		tempConfig->Screen.Width = modes[modes.size() - 1].w;
		tempConfig->Screen.Height = modes[modes.size() - 1].h;
	}

	refresh(tempConfig);
}

void OptionsWindow::onResolutionUpClick() {
	vector<SDL_Rect> modes = videoManager->GetAvailableModes();

	bool set = false;
	for (unsigned int i = 0; i < modes.size() - 1; i++) {
		if (tempConfig->Screen.Width == modes[i].w && tempConfig->Screen.Height
				== modes[i].h) {
			tempConfig->Screen.Width = modes[i + 1].w;
			tempConfig->Screen.Height = modes[i + 1].h;
			set = true;
			break;
		}
	}
	if (!set) {
		tempConfig->Screen.Width = modes[0].w;
		tempConfig->Screen.Height = modes[0].h;
	}

	refresh(tempConfig);
}

void ControlsMenuWindow::onControlStyleClick() {
	enum ControlStyle style = GetNextControlStyle(config->Control);
	config->Control = style;
	
	std::cout << (boost::format(_("Changed control style to %s.")) % ControlStyleToString(style)) << std::endl;
	config->write();

	refresh();
}

void ControlsMenuWindow::onControlPresetClick() {
    config->ControlPreset++;
    if(config->ControlPreset > VIOLET_CONTROL_PRESET_NUMBER)
	config->ControlPreset = 1;

    std::cout << "Changed control preset to " << config->ControlPreset << "." << std::endl;
    config->write();

    refresh();
}

void drawWindows() {
	if (!windows.empty()) {
		std::map<std::string, Window*>::iterator win, victim;
		for (win = windows.begin(); win != windows.end(); ++win) {
			Window* w = win->second;
			w->draw();
			w->process(input);
		}
		win = windows.begin();
		while (win != windows.end()) {
			if (win->second->CloseFlag) {
				victim = win++;
				windows.erase(victim);
			} else {
				win++;
			}
		}
	}
}

void ControlsMenuWindow::onEventClick(std::string elementName) {
	windows["pressakey"] = new ChangeInputBindingWindow(config, videoManager->RegularText);

	drawWindows();
	SDL_GL_SwapBuffers();

	int key = InputHandler::getEventNumber(elementName);

	input->resetMouseButtons();

	SDL_Event sdlEvent;

	bool ok = false;
	while (!ok) {
		SDL_WaitEvent(&sdlEvent);

		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			config->PlayerInputBinding[key].Type[config->ControlPreset-1] = InputHandler::Keyboard;
			config->PlayerInputBinding[key].Value[config->ControlPreset-1] = sdlEvent.key.keysym.sym;
			ok = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			config->PlayerInputBinding[key].Type[config->ControlPreset-1] = InputHandler::Mouse;
			config->PlayerInputBinding[key].Value[config->ControlPreset-1] = sdlEvent.button.button;
			ok = true;
			break;
		case SDL_QUIT:
			ok = true;
			gameState->end();
			break;
		}
	}

	cout << (boost::format(_("Bind action %s to key %s.")) % elementName
		 % InputHandler::getKeyName(config->PlayerInputBinding[key].Type[config->ControlPreset-1],config->PlayerInputBinding[key].Value[config->ControlPreset-1]))
			<< endl;

	config->write();

	windows["pressakey"]->CloseFlag = true;

	refresh();
}

void OptionsWindow::onControlsMenuClick() {
	windows["controls"] = new ControlsMenuWindow(config, videoManager->RegularText);

	CloseFlag = true;
}

void OptionsWindow::onResetControlsClick() {
	Configuration* config_default = new Configuration(fileUtility);
	for (int i = 0; i < InputHandler::GameInputEventsCount; i++)
		config->PlayerInputBinding[i] = config_default->PlayerInputBinding[i];
}

void MainMenuWindow::onOptionsClick() {
	tempConfig = new Configuration(*config);

	OptionsWindow *w = new OptionsWindow(config, videoManager->RegularText);
	windows["options"] = w;
	w->refresh(tempConfig);
	
	CloseFlag = true;
}

void MainMenuWindow::onResumeClick() {
	CloseFlag = true;
	switchGamePause();
}

void createMainMenuWindow() {
	
	/* Remove main window, if existing
	 */
	std::map<std::string, Window*>::iterator it = windows.find("mainmenu");
	
	if (it != windows.end())
		delete it->second;
	
	/* Create and open main window
	 */
	windows["mainmenu"] = new MainMenuWindow(
		config, gameState,
		videoManager->RegularText
	);
}

void HighscoresWindow::onBackClick() {
	CloseFlag = true;
	createMainMenuWindow();
}

void HighscoresWindow::onResetClick() {
	Highscores s(fileUtility);
	s.clear();
	onBackClick();
}

void MainMenuWindow::onHighScoresClick() {
	CloseFlag = true;
	windows["highscores"] = new HighscoresWindow(config, videoManager->RegularText, Highscores(fileUtility));
}

void unloadResources() {
	delete weaponManager;
	delete monsterFactory;
	delete aim;
	delete hud;
	delete resources;

	clearMap<std::string, Window*> (&windows);

	delete config;
}

void OptionsWindow::onSaveAndReturnClick() {
	bool changeVideoMode = config->Screen.Width != tempConfig->Screen.Width
			|| config->Screen.Height != tempConfig->Screen.Height;

	config->Screen.Width = tempConfig->Screen.Width;
	config->Screen.Height = tempConfig->Screen.Height;
	config->write();

	if (changeVideoMode) {
#ifdef _WIN32
		cout << _("Hot video mode changing is not supported on windows now. You should restart the game.");
		unloadResources();
		shutdownSystem();
		exit(0);
#endif //_WIN32
		videoManager->setMode(config->Screen, cam);
	}

	CloseFlag = true;
	createMainMenuWindow();
}

void handleCommonControls() {
	if (input->getPressInput(InputHandler::ShowChar)) {
		map<string, Window*>::iterator it = windows.find("charstats");
		if (it == windows.end()) { // if window not exists, create one
			if (gameState->Begun && !gameState->Lost) { // it is possible to remove second check to show charstats window after player death
				clearMap<std::string, Window*> (&windows); // close all other windows

				createCharStatWindow();
				refreshCharStatsWindow();

				if (!gameState->Paused)
					switchGamePause();
			}
		} else { // if window exists, close it
			Window* w = it->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Menu)) {
		map<string, Window*>::iterator it = windows.find("mainmenu");
		if (it == windows.end()) { // if window not exists, create one
			clearMap<std::string, Window*> (&windows); // close all other windows

			createMainMenuWindow();

			if (!gameState->Paused)
				switchGamePause();
		} else if (gameState->Begun) { // if window exists and game state was begun, close it
			Window* w = it->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Exit)) {
		gameState->end();
	}
}

void addBloodStain(float x, float y, float angle, float scale, bool poisoned) {
	StaticObject *newBloodStain = new StaticObject(x, y, 128, 128,
			resources->BloodTex[(rand() % 3)], false);

	newBloodStain->Scale = scale;
	newBloodStain->Angle = angle;
	if (poisoned) {
		newBloodStain->GMask = 1.0f - (rand() % 200) / 1000.0f;
		newBloodStain->RMask = newBloodStain->BMask = (rand() % 200) / 1000.0f;
	} else {
		newBloodStain->RMask = 1.0f - (rand() % 200) / 1000.0f;
		newBloodStain->GMask = newBloodStain->BMask = (rand() % 200) / 1000.0f;

	}

	bloodStains.push_back(newBloodStain);
}

void addExplosion(float x, float y, float damage, float range) {
	vector<Blood> vBlood = gameState->processExplosion(x, y, damage, range,
			config->FriendlyFire);

	if (!vBlood.empty()) {
		for (unsigned int i = 0; i < vBlood.size(); i++) {
			for (unsigned int k = 0; k < 6; k++) {
				int angleDev = 90 + (rand() % 90) - 45;
				float distance = (rand() % 200);
				float x = vBlood[i].x + distance * -cos(
						(vBlood[i].angle - angleDev) * M_PI / 180.0f);
				float y = vBlood[i].y + distance * -sin(
						(vBlood[i].angle - angleDev) * M_PI / 180.0f);
				addBloodStain(x, y, (rand() % 6300) / 1000.0f, vBlood[i].scale,
						vBlood[i].poisoned);
			}

			ParticleSystem* partSys = new ParticleSystem();
			for (unsigned int k = 0; k < 12; k++) {
				Particle * p = new Particle(vBlood[i].x, vBlood[i].y, 128, 128,
						resources->BloodTex[(rand() % 299) / 100]);
				p->setMask(0.0f, 0.0f, 0.0f, 1.0f);
				if (vBlood[i].poisoned)
					p->GMask = 1.0f - (rand() % 200) / 1000.0f;
				else
					p->RMask = 1.0f - (rand() % 200) / 1000.0f;

				p->Scale = (rand() % 150) / 100.0f * vBlood[i].scale;

				p->Angle = (rand() % 6300) / 1000.0f;

				int angleDev = 90 + (rand() % 90) - 45;

				p->XSpeed = -cos((vBlood[i].angle - angleDev) * M_PI / 180.0f)
						* 1 / p->Scale * 0.2;
				p->YSpeed = -sin((vBlood[i].angle - angleDev) * M_PI / 180.0f)
						* 1 / p->Scale * 0.2;

				p->AMod = -0.0015 * 1 / p->Scale;
				partSys->Particles.push_back(p);
			}

			particleSystems.push_back(partSys);
		}
	}
}

void handleParticles() {
	if (!particleSystems.empty()) {
		for (int i = particleSystems.size() - 1; i >= 0; i--) {
			particleSystems[i]->process(videoManager->getFrameDeltaTime());

			if (particleSystems[i]->isEmpty()) {
				delete particleSystems[i];
				particleSystems.erase(particleSystems.begin() + i);
			}
		}
	}
}

void handleMonster(LifeForm* lf) {
	LifeForm* player = gameState->getLifeForm(playerId);
	Monster* enemy = (Monster*) lf;

	// HUD related
	if (!gameState->Lost && enemy->detectCollide(player->TargetX,
			player->TargetY)) {
		ostringstream oss;
		oss << enemy->Name << " (" << enemy->Level << ')';
		hud->Info = oss.str();
	}

	// Visual related

	if (lf->Frozen <= 0) {
		if (enemy->isBleeding() && bloodStains.size() < 12) {
			addBloodStain(enemy->X, enemy->Y, enemy->Angle,
					(rand() % 10) / 50.0f + 0.1f, enemy->Poisoned);
		}
	}

	if (lf->Burning) {
		ParticleSystem* partSys = new ParticleSystem();
		for (unsigned int k = 0; k < 5; k++) {
			Particle * p = new Particle(enemy->X + (rand() % 50) - 25,
					enemy->Y + (rand() % 50) - 25, 128, 128,
					resources->ExplTex[0]);
			p->RMask = 1.0;
			p->GMask = (float) (rand() % 50) / 100 + 0.4;
			p->BMask = 0.3;
			p->AMask = 0.7;
			p->Scale = (rand() % 45) / 100.0f * enemy->Scale;
			p->YSpeed = -((rand() % 400) - 150) / 1000.0f;
			p->AMod = -0.002;
			partSys->Particles.push_back(p);
		}

		particleSystems.push_back(partSys);
	}

	// AI

	// Check if current target exist and is in range
	if (enemy->targetId != "ambient") {
		LifeForm* targetLifeForm = gameState->getLifeForm(enemy->targetId);

		if (targetLifeForm == NULL)
			enemy->targetId = "ambient";
		else if (targetLifeForm->State != LIFEFORM_STATE_ALIVE)
			enemy->targetId = "ambient";
		else {
			float range = Object::calc_dist(enemy->X, enemy->Y,
					targetLifeForm->X, targetLifeForm->Y);

			if (range > 800)
				enemy->targetId = "ambient";
		}
	}

	// Check for new targets in range
	if (enemy->targetId == "ambient") {
		LifeForm* lifeForm = gameState->getLifeForm(playerId);

		if (lifeForm->State == LIFEFORM_STATE_ALIVE) {
			float range = Object::calc_dist(enemy->X, enemy->Y, lifeForm->X,
					lifeForm->Y);
			if (range < 800)
				enemy->targetId = lifeForm->Id;
		}
	}

	if (enemy->targetId == "ambient") {
		// Hang around

		float range = Object::calc_dist(enemy->X, enemy->Y, enemy->TargetX,
				enemy->TargetY);

		if (range < enemy->getWidth() * enemy->Scale * enemy->HitR * 5) {
			enemy->TargetX = (rand() % (gameState->GameAreaSize * 2))
					- gameState->GameAreaSize;
			enemy->TargetY = (rand() % (gameState->GameAreaSize * 2))
					- gameState->GameAreaSize;
		}
	} else {
		// Attack target

		LifeForm* targetLifeForm = gameState->getLifeForm(enemy->targetId);

		float range = Object::calc_dist(enemy->X, enemy->Y, targetLifeForm->X,
				targetLifeForm->Y);

		if (range < 400 || targetLifeForm->Speed == 0 || enemy->Speed == 0) {
			enemy->TargetX = targetLifeForm->X;
			enemy->TargetY = targetLifeForm->Y;
		} else {
			enemy->TargetX = targetLifeForm->X - cos(
					(targetLifeForm->Angle + 90) * M_PI / 180) * range / 2.0f
					/ enemy->Speed * targetLifeForm->Speed;
			enemy->TargetY = targetLifeForm->Y - sin(
					(targetLifeForm->Angle + 90) * M_PI / 180) * range / 2.0f
					/ enemy->Speed * targetLifeForm->Speed;
		}

		if (enemy->detectCollide(targetLifeForm)) {
			if (enemy->Frozen == 0 && enemy->Attack()) {
				if (rand() % 100 > targetLifeForm->ChanceToEvade() * 100) {
					Sound* hitSound = targetLifeForm->hit(enemy->Damage(),
							false);
					if (hitSound != NULL)
						//TODO: There should be more channels for shouts
						hitSound->play(6, 0, 0);
				}
			}

			if (targetLifeForm->Frozen == 0 && targetLifeForm->Attack()) {
				if ((rand() % 100) > enemy->ChanceToEvade() * 100) {
					Sound* hitSound = enemy->hit(targetLifeForm->Damage(),
							false, true);
					if (hitSound != NULL)
						//TODO: There should be more channels for shouts
						hitSound->play(7, 0, 0);
				}
			}

			enemy->Speed = 0.0f;

			return;
		}
	}

	if (enemy->Frozen == 0) {
		float direction = Object::calc_angle(enemy->X, enemy->Y,
				enemy->TargetX, enemy->TargetY);

		enemy->move(direction, videoManager->getFrameDeltaTime());

		if (enemy->Speed > enemy->MaxSpeed() / 4)
			enemy->rollFrame(true);

	}
}

void levelUp(Player* player) {
	if (gameState->Mode == GAMEMODE_SURVIVAL)
		spawnEnemy(0, 0, gameState->GameAreaSize * 1.5f, player->Level,
				player->Level * 2.0f + 15);

	player->LastLevelXp = player->NextLevelXp;
	player->NextLevelXp *= 1.3;

	player->Level += 1;
	player->LevelPoints += 1;

	hud->addMessage(_("You have reached new level."));

	player->setHealth(player->MaxHealth());
}





/**
 * Classic controller where the player's direction is absolute (press arrow up
 * and Violet will go north)
 */
static void handlePlayerClassicStyle(Player* player) {
	char movementX = 0;
	char movementY = 0;

	if (input->getDownInput(InputHandler::MoveDown))
		movementY = 1;
	if (input->getDownInput(InputHandler::MoveUp))
		movementY = -1;
	if (input->getDownInput(InputHandler::MoveLeft))
		movementX = -1;
	if (input->getDownInput(InputHandler::MoveRight))
		movementX = 1;

	float movementDirection = Object::calc_angle(0, 0, movementX, movementY);

	if (movementX != 0 || movementY != 0)
		player->move(movementDirection, videoManager->getFrameDeltaTime());
}


/**
 * New controls where the directions are relative to the player's looking
 * direction
 *
 * @warning All angles are in degree with 0 equals north, 180 = -180 equals
 *     south, -90 equals west and 90 equals east
 */
static void handlePlayerModernStyle(Player* player) {

	/* Movement in the direction where the player is looking
	 */
	int parallel = 0;

	if (input->getDownInput(InputHandler::MoveUp)) {
		parallel += 1;
	}
	if (input->getDownInput(InputHandler::MoveDown)) {
		parallel -= 1;
	}


	/* Movement perpendicular to the direction where the player is looking
	 * (negative means left, positive means right -- just as in politics, as
	 * my physics prof used to say ;)
	 */
	int perpendicular = 0;

	if (input->getDownInput(InputHandler::MoveLeft)) {
		perpendicular -= 1;
	}
	if (input->getDownInput(InputHandler::MoveRight)) {
		perpendicular += 1;
	}


	/* Calculate angle relative to the direction, the player is
	 * looking at
	 */
	if (parallel != 0 || perpendicular != 0) {

		/* Base direction is always where the player is looking at
		 */
		float direction = player->getArmsAngle();

		/* Backwards means the opposite direction of where the user is
		 * going
		 */
		if (-1 == parallel) {
			direction = Object::fixAngle(direction + 180.f);
		}
		
		/* Strafe left
		 */
		if (-1 == perpendicular) {
			direction = Object::fixAngle(direction - 90.f);
		} else if (1 == perpendicular) {
			direction = Object::fixAngle(direction + 90.f);
		}
			
		/* Go in the desired direction :-)
		 */
		player->move(
			direction,
			videoManager->getFrameDeltaTime()
		);
	}
}



void handlePlayer(LifeForm* lf) {
	Player* player = (Player*) lf;

	if (player->Xp >= player->NextLevelXp) {
		levelUp(player);
	}


	/* Use controller configured by the player
	 */
	switch (config->Control) {
		case E_CONTROL_STYLE_CLASSIC:
			handlePlayerClassicStyle(player);
			break;
		case E_CONTROL_STYLE_MODERN:
			handlePlayerModernStyle(player);
			break;
		default:
			std::cout << (boost::format(_("Unknown control style to %s.")) % ControlStyleToString(config->Control)) << std::endl;
	}


	if (lf->X < -gameState->GameAreaSize)
		player->setX(-gameState->GameAreaSize);
	if (lf->X > gameState->GameAreaSize)
		player->setX(gameState->GameAreaSize);
	if (lf->Y < -gameState->GameAreaSize)
		player->setY(-gameState->GameAreaSize);
	if (lf->Y > gameState->GameAreaSize)
		player->setY(gameState->GameAreaSize);

	lf->TargetX = input->mouseX / videoManager->WK - cam->getHalfW() + cam->X;
	lf->TargetY = input->mouseY / videoManager->HK - cam->getHalfH() + cam->Y;

	if (lf->TargetX < -gameState->GameAreaSize)
		player->TargetX = -gameState->GameAreaSize;
	if (lf->TargetX > gameState->GameAreaSize)
		player->TargetX = gameState->GameAreaSize;
	if (lf->TargetY < -gameState->GameAreaSize)
		player->TargetY = -gameState->GameAreaSize;
	if (lf->TargetY > gameState->GameAreaSize)
		player->TargetY = gameState->GameAreaSize;

	if (player->ActionMode == 0 && input->getDownInput(InputHandler::Fire)) {
		std::vector<Bullet*> *newBullets = player->fire();
		if (!newBullets->empty()) {
			gameState->bullets.insert(gameState->bullets.end(),
					newBullets->begin(), newBullets->end());
			delete newBullets;
		}
		if (player->getWeapon()->Ammo == 0 && config->AutoReload)
			player->reload();
	}

	if (player->ActionMode == 1 && input->getPressInput(InputHandler::Fire)) {
		ParticleSystem* partSys = new ParticleSystem();

		for (int i = 0; i < 50; i++) {
			Particle* spark = new Particle(player->X + (rand() % 200) - 100,
					player->Y + (rand() % 200) - 100, 128, 128,
					resources->ExplTex[0]);
			spark->RMask = spark->GMask = 0.6f;
			spark->BMask = 1.0f;
			spark->AMask = (float) (rand() % 40) / 100 + 0.6f;
			spark->Scale = (float) (rand() % 60) / 300 + 0.05f;
			spark->XSpeed = -(float) ((rand() % 250) - 125) / 1000;
			spark->YSpeed = -(float) ((rand() % 250) - 125) / 1000;
			spark->AMod = -0.0003f;
			partSys->Particles.push_back(spark);
		}

		particleSystems.push_back(partSys);

		player->teleport();
		player->ActionMode = PLAYER_ACT_MODE_FIRE;
		player->setMask(0.0f, 1.0f, 1.0f, 1.0f);

		if (player->WideSight) {
			player->TargetX = player->X;
			player->TargetY = player->Y;
		}

		delete aim;
		aim = new Aim(config);
	}

	if (input->getPressInput(InputHandler::ToggleLight))
		player->toggleLight();

	if (input->getPressInput(InputHandler::ToggleLaser))
		player->toggleLaser();

	if (input->getDownInput(InputHandler::Reload))
		player->reload();

	if (input->getPressInput(InputHandler::SwitchCursorStyle))
	{
		config->FreeCursorMove = !config->FreeCursorMove;
		if(config->FreeCursorMove)
		    hud->addMessage(_("Cursor can be moved freely"));
		else
		    hud->addMessage(_("Cursor orbits you"));
	}

	if (input->getPressInput(InputHandler::Teleport)) {
		if (player->ActionMode != 1 && player->Teleports > 0) {
			player->ActionMode = PLAYER_ACT_MODE_TELEPORT;
			delete aim;
			aim = new Aim(0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f);
		} else if (player->ActionMode == PLAYER_ACT_MODE_TELEPORT) {
			player->ActionMode = PLAYER_ACT_MODE_FIRE;
			delete aim;
			aim = new Aim(config);
		}
	}

	if (input->getPressInput(InputHandler::ThrowGrenade) && player->Grenades
			> 0) {
		gameState->bullets.push_back(
				player->throwGrenade(resources->GrenadeSprite));
	}
}





//Choice and creation of bonus
void dropPowerup(float x, float y, float chance, bool forceWeapon) {
	BasePowerup *newPowerup;

	// Weapon drop - should be first check
	if (forceWeapon || roulette(chance * 2.5)) {
		int weaponIndex;
		if (false) // TODO: "Allow PM drop" to options. true for allow.
			weaponIndex = (rand() % weaponManager->Weapons.size());
		else
			weaponIndex = (rand() % (weaponManager->Weapons.size() - 1));

		newPowerup = new WeaponPowerup(x, y,
				weaponManager->Weapons[weaponIndex]);
		newPowerup->HitR = 0.5f;
	} else if (roulette(chance * 5)) {
		newPowerup = new MedikitPowerup(x, y,
				resources->PowerupTex[BONUS_MEDIKIT], 0.1f);
		newPowerup->Scale = 0.3f;
		newPowerup->RMask = newPowerup->BMask = 0.2f;
	} else if (roulette(chance * 2.5)) {
		newPowerup = new MedikitPowerup(x, y,
				resources->PowerupTex[BONUS_MEDIKIT], 0.2f);
		newPowerup->Scale = 0.4f;
		newPowerup->RMask = newPowerup->GMask = 0.4f;
	} else if (roulette(chance)) {
		newPowerup = new MedikitPowerup(x, y,
				resources->PowerupTex[BONUS_MEDIKIT], 0.6f);
		newPowerup->Scale = 0.5f;
		newPowerup->BMask = newPowerup->GMask = 0.2f;
	} else if (roulette(chance * 2)) {
		newPowerup = new GrenadePowerup(x, y,
				resources->PowerupTex[BONUS_GRENADES]);
		newPowerup->Scale = 0.4f;
	} else if (roulette(chance * 2)) {
		newPowerup = new FreezePowerup(x, y,
				resources->PowerupTex[BONUS_FREEZE]);
		newPowerup->Scale = 0.4f;
	} else if (roulette(chance * 2)) {
		newPowerup = new NukePowerup(x, y, resources->PowerupTex[BONUS_NUKE]);
		newPowerup->Scale = 0.4f;
	} else if (roulette(chance * 2)) {
		newPowerup = new PenetrationBulletsPowerup(x, y,
				resources->PowerupTex[BONUS_PENBULLETS]);
		newPowerup->Scale = 0.4f;
	} else if (roulette(chance * 2)) {
		newPowerup = new VitalityPowerup(x, y,
				resources->PowerupTex[BONUS_VITALITYROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->RMask = newPowerup->BMask = 0.2f;
	} else if (roulette(chance * 2)) {
		newPowerup = new StrengthPowerup(x, y,
				resources->PowerupTex[BONUS_STRENGTHROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->GMask = newPowerup->BMask = 0.2f;
	} else if (roulette(chance * 2)) {
		newPowerup = new AgilityPowerup(x, y,
				resources->PowerupTex[BONUS_AGILITYROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->RMask = newPowerup->GMask = 0.2f;
	} else if (roulette(chance * 2)) {
		newPowerup = new TeleportPowerup(x, y,
				resources->PowerupTex[BONUS_TELEPORTS]);
		newPowerup->Scale = 0.4f;
	} else if (roulette(chance * 2)) {
		newPowerup = new ShieldPowerup(x, y,
				resources->PowerupTex[BONUS_SHIELD]);
		newPowerup->Scale = 0.4f;
	} else
		return;

	gameState->powerups.push_back(newPowerup);
}

void handleLifeForms() {
	LifeForm* player = gameState->getLifeForm(playerId);

	if (!gameState->Lost) {

		// Spawn monsters by the selected game rules

		switch (gameState->Mode) {
		case GAMEMODE_SURVIVAL:
			for (int i = 0; i < videoManager->getFrameDeltaTime(); i++) {
				if (rand() % 10000 > gameState->Hardness) {
					int lvl = player->Level * 0.5f + player->Level * pow(
							(rand() % 100) / 125.0f, 2);
					if (lvl < 1)
						lvl = 1;
					spawnEnemy(0, 0, gameState->GameAreaSize * 1.5,
							player->Level, lvl);
				}
			}
			break;
		case GAMEMODE_WAVES:
			if (gameState->lifeForms.size() < 5) {
				hud->addMessage(_("Get ready to new wave!"));
				for (unsigned int i = 0; i < (10000 - gameState->Hardness) * 5; i++) {
					int lvl = player->Level * 0.5f + player->Level * pow(
							(rand() % 100) / 125.0f, 2);
					if (lvl < 1)
						lvl = 1;

					spawnEnemy(player->X, player->Y, (float) cam->getW(),
							player->Level, 1);
				}
			}
			break;
		}

		// Check state of the user controlled hero

		if (player->State == LIFEFORM_STATE_DIED ||
			player->State == LIFEFORM_STATE_BURST)
		{
			loseGame((Player*)player);
		}
		else if (player->State == LIFEFORM_STATE_ALIVE)
		{
			// Give controls to the user
			handlePlayer(player);
		}
	}

	// Process all lifeforms in the game

	map<string, LifeForm*>::iterator it = gameState->lifeForms.begin();
	while (it != gameState->lifeForms.end()) {
		LifeForm* lifeForm = it->second;

		if (lifeForm->Type == LIFEFORM_MONSTER && lifeForm->State == LIFEFORM_STATE_ALIVE)
			handleMonster(lifeForm);

		lifeForm->process(videoManager->getFrameDeltaTime());

		if (lifeForm->State == LIFEFORM_STATE_DIED)
			bloodStains.push_back(lifeForm->getCorpse());

		if ((lifeForm->State == LIFEFORM_STATE_DIED || lifeForm->State
				== LIFEFORM_STATE_BURST) && lifeForm->Type == LIFEFORM_MONSTER) {
			delete lifeForm;
			gameState->lifeForms.erase(it++);
		} else {
			++it;
		}
	}
}

void collideBulletAndEnemy(Bullet* bullet, Monster* enemy) {
	if (bullet->Type == BULLET_FLAME) {
		enemy->Frozen = 0;
		enemy->Burning = true;
	}

	if (bullet->Type == BULLET_STANDARD && bloodStains.size() < 10) {
		for (unsigned int k = 0; k < 10 / 3; k++) {
			int angleDev = 90 + (rand() % 60) - 30;
			float distance = (rand() % 100);
			float bX = enemy->X - cos(
					(bullet->Angle + angleDev) * M_PI / 180.0f) * distance;
			float bY = enemy->Y - sin(
					(bullet->Angle + angleDev) * M_PI / 180.0f) * distance;

			addBloodStain(bX, bY, enemy->Angle, enemy->Scale * 0.5f,
					enemy->Poisoned);
		}
	}

	if (bullet->Type != BULLET_FLAME) {
		if (enemy->Frozen > 0) {
			ParticleSystem* partSys = new ParticleSystem();
			for (unsigned int k = 0; k < 5; k++) {
				Particle * p = new Particle(enemy->X + (rand() % 50) - 25,
						enemy->Y + (rand() % 50) - 25, 128, 128,
						resources->Crystal->getTexture());
				p->RMask = p->GMask = p->BMask = 1.0f;
				p->AMask = 1.0f;
				p->Scale = (rand() % 50) / 100.0f * enemy->Scale;
				p->XSpeed = ((rand() % 400) - 200) / 1000.0f;
				p->YSpeed = ((rand() % 400) - 200) / 1000.0f;
				p->AMod = -0.002f;
				partSys->Particles.push_back(p);
			}

			particleSystems.push_back(partSys);
		} else {
			ParticleSystem* partSys = new ParticleSystem();
			for (unsigned int k = 0; k < 15; k++) {
				Particle * p = new Particle(enemy->X + (rand() % 50) - 25,
						enemy->Y + (rand() % 50) - 25, 128, 128,
						resources->BloodTex[(rand() % 299) / 100]);
				p->RMask = p->GMask = p->BMask = 0.0f;
				if (enemy->Poisoned)
					p->GMask = 1.0f - (rand() % 200) / 1000.0f;
				else
					p->RMask = 1.0f - (rand() % 200) / 1000.0f;
				p->AMask = 1.0f;
				p->Scale = (rand() % 15) / 100.0f * enemy->Scale;
				int angleDev = 90 + (rand() % 60) - 30;
				p->XSpeed = -cos((bullet->Angle + angleDev) * M_PI / 180.0f)
						/ 3.0f;
				p->YSpeed = -sin((bullet->Angle + angleDev) * M_PI / 180.0f)
						/ 3.0f;
				p->AMod = -0.004;
				partSys->Particles.push_back(p);
			}

			particleSystems.push_back(partSys);
		}
	}

	bool bypassDirectDamage = false;
	if (bullet->Type == BULLET_STANDARD) {
		if (((StandardBullet*) bullet)->isExplosive()) {
			bullet->deactivate();
			addExplosion(bullet->X, bullet->Y, bullet->Damage, 100.0f);
			bypassDirectDamage = true;

			resources->ExplSounds[1]->play(8, 0, 0);

			Explosion * expl = new Explosion(false, bullet->X, bullet->Y,
					100.0f, resources->ExplTex[0], resources->ExplTex[1]);
			particleSystems.push_back(expl);
		}
	}

	Player* player = (Player*) gameState->getLifeForm(bullet->OwnerId);

	if (!bypassDirectDamage) {
		float damageLoss = enemy->getHealth();

		Sound* hitSound = enemy->hit(bullet->Damage, bullet->Poisoned,
				bullet->Type != BULLET_FLAME);

		if (hitSound != NULL) {
			hitSound->play(7, 0, 0);
			hitSound->setPos(
					Object::calc_angle(enemy->X, enemy->Y, player->X, player->Y),
					Object::calc_dist(enemy->X, enemy->Y, player->X, player->Y));
		}

		if (bullet->BigCalibre && !bullet->Penetrating) {
			bullet->Damage -= damageLoss;
			if (bullet->Damage <= 0) {
				bullet->deactivate();
			}
		}
	}

	if (enemy->getHealth() <= 0) {
		float dropPowerupChance = 0.01;
		if (player->Looting)
			dropPowerupChance *= 2;

		// The PM will increase chance of bonus drop
		if (player->getWeapon()->Name == "PM")
			dropPowerupChance *= 4;

		dropPowerup(enemy->X, enemy->Y, dropPowerupChance, player->Kills == 0);
		if (player->State == LIFEFORM_STATE_ALIVE) {
			player->Kills++;
			player->Xp += (int) ((1.5 - gameState->TimeOfDay * -0.5)
					* (enemy->getStrength() + enemy->getAgility()
							+ enemy->getVitality()) * 3);
		}
	}
}

void handleBullets() {
	for (int i = gameState->bullets.size() - 1; i >= 0; i--) {
		gameState->bullets[i]->process(videoManager->getFrameDeltaTime());

		if (gameState->bullets[i]->isActive()) {
			map<string, LifeForm*>::const_iterator iter;
			for (iter = gameState->lifeForms.begin(); iter
					!= gameState->lifeForms.end(); ++iter) {
				LifeForm* lf = iter->second;

				if (lf->Type == LIFEFORM_PLAYER || lf->State
						!= LIFEFORM_STATE_ALIVE)
					continue;

				Monster* enemy = (Monster*) lf;

				if (gameState->bullets[i]->checkHit(enemy)) {
					collideBulletAndEnemy(gameState->bullets[i], enemy);
				}
			}
		}

		if (gameState->bullets[i]->isReadyToRemove()
				&& gameState->bullets[i]->Type == BULLET_GRENADE) {
			addExplosion(gameState->bullets[i]->X, gameState->bullets[i]->Y,
					gameState->bullets[i]->Damage, 150.0f);

			resources->ExplSounds[0]->play(8, 0, 0);

			Explosion* expl = new Explosion(false, gameState->bullets[i]->X,
					gameState->bullets[i]->Y, 150.0f, resources->ExplTex[0],
					resources->ExplTex[1]);
			particleSystems.push_back(expl);
		}

		if (gameState->bullets[i]->isReadyToRemove()) {
			delete gameState->bullets[i];
			gameState->bullets.erase(gameState->bullets.begin() + i);
		}
	}
}

void setGuiCameraMode() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, config->Screen.Width, config->Screen.Height, 0.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handlePowerups() {
	Player* player = (Player*) gameState->getLifeForm(playerId);
	bool looksAtAnyBonus = false;

	for (int i = gameState->powerups.size() - 1; i >= 0; i--) {
		bool deletePowerup = false;
		gameState->powerups[i]->process(videoManager->getFrameDeltaTime());

		if (player->Magneto) {
			float a = Object::calc_angle(gameState->powerups[i]->X,
					gameState->powerups[i]->Y, player->X, player->Y);
			gameState->powerups[i]->X -= cos((a + 90) * M_PI / 180)
					* videoManager->getFrameDeltaTime() * 0.05;
			gameState->powerups[i]->Y -= sin((a + 90) * M_PI / 180)
					* videoManager->getFrameDeltaTime() * 0.05;
		}

		if (gameState->powerups[i]->Time <= 0)
			deletePowerup = true;

		if (!deletePowerup && !gameState->Lost) {
			if (gameState->powerups[i]->detectCollide(player->TargetX, player->TargetY)) {
				looksAtAnyBonus = true;

				ostringstream hudStr;
				hudStr << gameState->powerups[i]->getHudInfo();

				if (player->Telekinesis) {
					unsigned telekinesisState = player->processTelekinesis(videoManager->getFrameDeltaTime(), false);
					hudStr << " (" << telekinesisState << "%)" << endl;

					if (telekinesisState >= 100) {
						player->processTelekinesis(0, true);
						deletePowerup = gameState->powerups[i]->modify(game, player);
					}
				}

				hud->Info = hudStr.str();
			}
		}

		if (!deletePowerup && !gameState->Lost
				&& gameState->powerups[i]->detectCollide(player)) {
			deletePowerup = gameState->powerups[i]->modify(game, player);
		}

		if (deletePowerup) {
			delete gameState->powerups[i];
			gameState->powerups.erase(gameState->powerups.begin() + i);
		}
	}

	if (!looksAtAnyBonus)
		player->processTelekinesis(0, true);
}

void processTerrain() {
	terrain->beginDrawOn();
	{
		for (unsigned int i = 0; i < bloodStains.size(); i++) {
			terrain->drawOn(bloodStains[i]);
		}

		clearVector<StaticObject*> (&bloodStains);
	}
	terrain->endDrawOn();
}

void processGame() {
	gameState->process(videoManager->getFrameDeltaTime());

	hud->Info = "";

	if (gameState->HighScore) {
		if (input->hasBeenValidated()) {
			Player* player = (Player*) gameState->getLifeForm(playerId);
			input->setInputMode(InputHandler::Direct);
			Highscores s(fileUtility);
			s.add(
					HighscoresEntry(player, gameState->PlayerName,
							gameState->Time));
			gameState->HighScore = false;
		} else {
			gameState->PlayerName = input->getTextToShow();
		}
	}

	handleLifeForms();
	handlePowerups();
	handleBullets();
	handleParticles();
}

void drawGame() {
	Player* player = (Player*) gameState->getLifeForm(playerId);

	// Actually enable widesight perk
	if (player->WideSight)
		if (cam->getW() == 1600)
			cam->setW(1920);

	cam->X = player->X;
	cam->Y = player->Y;

	if (cam->X < -gameState->GameAreaSize + cam->getHalfW())
		cam->X = -gameState->GameAreaSize + cam->getHalfW();
	if (cam->X > gameState->GameAreaSize - cam->getHalfW())
		cam->X = gameState->GameAreaSize - cam->getHalfW();
	if (cam->Y < -gameState->GameAreaSize + cam->getHalfH())
		cam->Y = -gameState->GameAreaSize + cam->getHalfH();
	if (cam->Y > gameState->GameAreaSize - cam->getHalfH())
		cam->Y = gameState->GameAreaSize - cam->getHalfH();

	cam->applyGLOrtho();

	glEnable(GL_LIGHTING);

	double tod = cos(gameState->Time / 180000.0);
	gameState->TimeOfDay = abs((float) tod);
	float gawc = gameState->TimeOfDay;

	double v = cos((gameState->Time + 90000) / 90000.0);

	float garc = v >= 0 ? 0 : v / -3;
	float gabc = v > 0 ? v / 3 : 0;
	float r = gawc + garc;
	float b = gawc + gabc;
	if (r > 1)
		r = 1;
	if (b > 1)
		b = 1;

	GLfloat global_ambient[] = { r, gawc, b, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	if (!gameState->Lost) {
		GLfloat light_pos[] = { 0.0, 0.0, 1.0, 1.0 };
		if (player->getLight()) {
			glEnable(GL_LIGHT0);

			glPushMatrix();
			glTranslatef(player->TargetX, player->TargetY, 0.0f);
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
			glPopMatrix();
		}
		if (player->NightVision) {
			glEnable(GL_LIGHT1);

			glPushMatrix();
			glTranslatef(player->X, player->Y, 0.0f);
			glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
			glPopMatrix();
		}
	}

	terrain->draw(cam);

	for (unsigned int i = 0; i < gameState->powerups.size(); i++) {
		gameState->powerups[i]->draw(false, false);
	}

	float right = cam->X + cam->getHalfW();
	float left = cam->X - cam->getHalfW();
	float bottom = cam->Y + cam->getHalfH();
	float top = cam->Y - cam->getHalfH();
	map<string, LifeForm*>::const_iterator iter;
	for (iter = gameState->lifeForms.begin(); iter
			!= gameState->lifeForms.end(); ++iter) {
		LifeForm* lifeForm = iter->second;

		// Explicit check if lifeform is out of screen bounds
		// In fact, such a check is made in opengl
		// Perhaps opengl even does it quickly, I do not know
		if (lifeForm->getLeft() >= right || lifeForm->getRight() <= left
				|| lifeForm->getTop() >= bottom || lifeForm->getBottom() <= top)
			continue;

		// Check if lifeform is dead, we should draw only alive lifeforms
		// Images of corpses are printed on the texture of the terrain
		if (lifeForm->Type == LIFEFORM_PLAYER && (lifeForm->State
				== LIFEFORM_STATE_DIED || lifeForm->State
				== LIFEFORM_STATE_BURST))
			continue;

		lifeForm->draw();

		if (lifeForm->Frozen > 0 && lifeForm->State == LIFEFORM_STATE_ALIVE) {
			resources->Crystal->AMask = lifeForm->Frozen / 10000.0f;
			resources->Crystal->draw(false, false, lifeForm->X, lifeForm->Y,
					lifeForm->Angle, lifeForm->Scale);
		}
	}

	if (!gameState->Lost) {
		if (player->getLight())
			glDisable(GL_LIGHT0);
		if (player->NightVision)
			glDisable(GL_LIGHT1);
	}

	glDisable(GL_LIGHTING);

	for (unsigned int i = 0; i < gameState->bullets.size(); i++) {
		gameState->bullets[i]->draw();
	}

	for (unsigned int i = 0; i < particleSystems.size(); i++) {
		particleSystems[i]->draw();
	}

	glDisable(GL_TEXTURE_2D);

	if (!gameState->Lost) {
		const float rad = (player->getArmsAngle() - 90) * M_PI / 180;
		const float wpnX = player->X + player->getWeapon()->XDiff * cos(rad)
				+ player->getWeapon()->YDiff * sin(-rad);
		const float wpnY = player->Y + player->getWeapon()->XDiff * sin(rad)
				+ player->getWeapon()->YDiff * cos(-rad);

		/* Red laser pointer should be drawn until it collides with
		 * first obstacle
		 */
		if (player->getLaser()) {
			float len = (float)cam->getW();
			float tX = player->X + len * cos(rad);
			float tY = player->Y + len * sin(rad);

			for (iter = gameState->lifeForms.begin(); iter != gameState->lifeForms.end(); ++iter) {
				LifeForm* lf = iter->second;

				if (lf->Type == LIFEFORM_PLAYER || lf->State
						!= LIFEFORM_STATE_ALIVE)
					continue;

				Monster* enemy = (Monster*) lf;

				if (enemy->detectCollide(wpnX, wpnY, tX, tY))
				{
					float nlen = Object::calc_dist(wpnX, wpnY, enemy->X, enemy->Y) + enemy->getWidth() / 2;
					if (nlen < len)
					{
						len = nlen;
						tX = player->X + len * cos(rad);
						tY = player->Y + len * sin(rad);
					}
				}
			}

			glLineWidth(0.5f);
			glBegin(GL_LINES);
			glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
			glVertex3f(wpnX, wpnY, 0);
			glColor4f(1.0f, 0.0f, 0.0f, (cam->getW() - len) / cam->getW() * 0.75f);
			glVertex3f(tX, tY, 0);
			glEnd();
		}
		if (player->getLight()) {
			glBegin(GL_TRIANGLES);
			glNormal3f(0.0f, 0.0f, 1.0f);
			float flash = 1.0 - gameState->TimeOfDay;
			if (flash > 0.3)
				flash = 0.3;
			glColor4f(1.0f, 1.0f, 1.0f, flash);
			glVertex3f(wpnX, wpnY, 0.0f);

			const float len = Object::calc_dist(player->X, player->Y,
					player->TargetX, player->TargetY);
			float width = 0.75 - len / cam->getW();
			if (width < 0.25)
				width = 0.25;
			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glVertex3f(player->X + len * cos(rad - width),
					player->Y + len * sin(rad - width), 0.0f);
			glVertex3f(player->X + len * cos(rad + width),
					player->Y + len * sin(rad + width), 0.0f);

			glEnd();
		}
	}

	if (!gameState->Lost && !gameState->Paused) {
		aim->draw(
				player->TargetX,
				player->TargetY,
				1.0f + tan(player->AccuracyDeviation * M_PI / 180)
						* Object::calc_dist(player->X, player->Y,
								player->TargetX, player->TargetY) / 25.0f,
				player->getWeapon()->getReloadState() > 0 ? 1.2f
						- player->getWeapon()->getReloadState() : 0.2f);
	}

	glEnable(GL_TEXTURE_2D);
}

void drawFps() {
	ostringstream oss;
	oss << boost::format(_("FPS: %i")) % videoManager->getFps();
	videoManager->RegularText->draw(oss.str(),
			config->Screen.Width - videoManager->RegularText->getIndent(),
			config->Screen.Height - videoManager->RegularText->getIndent(),
			TextManager::RIGHT, TextManager::BOTTOM);
}

void runMainLoop() {
	while (gameState->Works) {
		videoManager->countFrame(config->FrameDelay);

		input->process();

		handleCommonControls();

		if (gameState->Begun) {
			if (gameState->JustBegun) {
				videoManager->resetDeltaTime();
				gameState->JustBegun = false;
				continue;
			}

			Player* player = (Player*) gameState->getLifeForm(playerId);

			musicManager->process(player, gameState);

			if (!gameState->Paused) {
				processGame();
				processTerrain();
			}

			drawGame();

			setGuiCameraMode();

			hud->draw(gameState, player);

			if (config->ShowFps)
				drawFps();
		} else {
			musicManager->play();

			glClear(GL_COLOR_BUFFER_BIT);

			cam->X = cam->Y = 0.0f;

			cam->applyGLOrtho();

			splash->draw(false, false);

			setGuiCameraMode();
		}

		drawWindows();

		SDL_GL_SwapBuffers();
	}
}

void loadResources() {
	resources = new Resources(fileUtility, sndManager);

	aim = new Aim(config);
	hud = new HUD(videoManager, resources);

	monsterFactory = new MonsterFactory(fileUtility, sndManager);
	weaponManager = new WeaponManager(fileUtility, sndManager);
}

void parsePreferences(int argc, char** argv) {
	fileUtility = FileUtility::ofOs(argv[0]);
	config = new Configuration(fileUtility);
	config->read();

	for (int i = 1; i < argc; i++) {
		string arg = argv[i];

		if (arg == "--help") {
			printVersion();
			cout << endl << "Arguments:" << endl;
			cout << "\t--help\t\t\t\tPrint help (this message) and exit"
					<< endl;
			cout << "\t-w <screen_width>\t\tSet screen width to <screen_width>"
					<< endl;
			cout
					<< "\t-h <screen_height>\t\tSet screen height to <screen_height>"
					<< endl;
			cout << "\t-f\t\t\t\tGo to fullscreen at start" << endl;
			cout << "\t-i\t\t\t\tForce windowed mode" << endl;
			cout << "\t--fps <fps_count>\t\tLimit game fps by <fps_count>"
					<< endl;
			cout << "\t\t\t\t\tDefault value of <fps_count> is 100" << endl;
			cout << "\t\t\t\t\tSeting <fps_count> to 0 will disable" << endl;
			cout << "\t\t\t\t\trestriction" << endl;
			cout << "\t--showfps\t\t\tShow fps in game" << endl;
			cout << "\t--monsters <count>\t\tImmediately spawn" << endl;
			cout << "\t\t\t\t\t<count> monsters at start" << endl;
			cout << endl << endl
					<< "These and other parametres can be adjusted in a configuration file"
					<< endl;

			exit(0);
		}

#ifdef __APPLE__
		fileUtility->setResourcePath(getMacBundlePath()+"/Contents/Resources");
#endif

		else if (arg == "-r") {
			if (++i < argc)
				fileUtility->setResourcePath(argv[i]);
		} else if (arg == "-f") {
			config->Screen.Full = true;
		} else if (arg == "-i") {
			config->Screen.Full = false;
		} else if (arg == "-w") {
			if (++i < argc)
				config->Screen.Width = strtol(argv[i], NULL, 10);
		} else if (arg == "-h") {
			if (++i < argc)
				config->Screen.Height = strtol(argv[i], NULL, 10);
		} else if (arg == "--fps") {
			if (++i < argc) {
				int lim = strtol(argv[i], NULL, 10);
				config->FrameDelay = lim > 0 ? 1000 / lim : 0;
			}
		} else if (arg == "--showfps") {
			config->ShowFps = true;
		} else if (arg == "--monsters") {
			if (++i < argc) {
				int n = strtol(argv[i], NULL, 10);
				if (n >= 0)
					config->MonstersAtStart = n;
				else
					cout << "Number of monsters must be positive." << endl;
			}
		}
	}
}

int main(int argc, char** argv) {
	printVersion();

	parsePreferences(argc, argv);

	initSystem();

	loadResources();

	createMainMenuWindow();

	runMainLoop();

	unloadResources();

	shutdownSystem();

	exit(0);
}
