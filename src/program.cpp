// MS Windows compatibility
#ifdef _WIN32
#pragma comment(lib, "SDL")
#pragma comment(lib, "SDLmain")
#pragma comment(lib, "SDL_image")
#pragma comment(lib, "SDL_ttf")
#pragma comment(lib, "SDL_mixer")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "libintl")
#define _USE_MATH_DEFINES
#include <windows.h>
#include <winbase.h>
#include <time.h>
#else
#include "pwd.h"
#endif

// Apple Mac OS X compatibility
#ifdef __APPLE__
#include "system/utility/macBundlePath.h"
#endif

// SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"

//~ Internationalization Dependances
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)
#define TRANSLATION_PATH 	"./po"

// The Game
#include "system/Configuration.h"
#include "system/InputHandler.h"
#include "system/utility/ImageUtility.h"
#include "system/utility/FileUtility.h"
#include "system/graphic/text/TextManager.h"
#include "system/graphic/Texture.h"
#include "system/graphic/Aim.h"
#include "system/graphic/Camera.h"
#include "system/graphic/Particle.h"
#include "system/graphic/Window.h"
#include "system/graphic/VideoManager.h"
#include "system/graphic/Explosion.h"
#include "system/sound/SoundManager.h"
#include "game/GameState.h"
#include "game/Resources.h"
#include "game/lifeforms/MonsterFactory.h"
#include "game/lifeforms/Monster.h"
#include "game/lifeforms/Player.h"
#include "game/Powerup.h"
#include "game/Terrain.h"
#include "game/MusicManager.h"
#include "game/WeaponManager.h"
#include "game/Highscores.h"
#include "game/HUD.h"
#include "windows/MainMenuWindow.h"
#include "windows/HelpWindow.h"
#include "windows/CharStatsWindow.h"

#include <sstream>

using namespace std;
using namespace violetland;

const string PROJECT = "violetland";
const string VERSION = "0.3.2";
const string DEFAULT_CHAR_NAME = "Violet";

Configuration* config;
Configuration* tempConfig;
VideoManager* videoManager;
Camera* cam;

Aim* aim;
HUD* hud;
StaticObject* splash;

FileUtility* fileUtility;
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
string playerId;

map<string, Window*> windows;

HighscoresEntry* highscore;

GameMode gameMode;

bool roulette(float eventProbability) {
	return (float) (rand() % 100000) < eventProbability * 100000.0f;
}

// Creation of clear squares of an earth surface
void createTerrain() {
	if (terrain)
		delete terrain;

	cout << "Forming terrain..." << endl;

	int baseTexCount = fileUtility->getFilesCountFromDir(
			fileUtility->getFullPath(FileUtility::image, "terrain"));
	int baseTex = (rand() % baseTexCount);

	string tilesDir = fileUtility->getFullPath(FileUtility::image, "terrain");
	ostringstream oss;
	oss << baseTex;
	int tilesCount = fileUtility->getFilesCountFromDir(tilesDir + "/"+oss.str());

	oss.str("");
	oss << "terrain/base_" << baseTex << ".png";
	SDL_Surface *terrainSurface = ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, oss.str()), 1);

	vector<SDL_Surface*> tiles;
	for (int i = 0; i < tilesCount; i++) {
		oss.str("");
		oss << "terrain/" << baseTex << '/' << i << ".png";
		SDL_Surface *tile = ImageUtility::loadImage(fileUtility->getFullPath(
				FileUtility::image, oss.str()), 1);
		tiles.push_back(tile);
	}

	terrain = new Terrain(terrainSurface, tiles, gameState->GameAreaSize);

	SDL_FreeSurface(terrainSurface);
	for (int i = 0; i < tilesCount; i++) {
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
		Player* player = (Player*) gameState->getLifeForm(playerId);

		newMonster->TargetX = player->X;
		newMonster->TargetY = player->Y;
		break;
	}

	gameState->lifeForms.insert(map<string, LifeForm*>::value_type(
			newMonster->Id, newMonster));
}

// The beginning of new game in selected mode
void startGame(std::string elementName) {
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

	gameState->start(gameMode);

	clearVector<StaticObject*> (&bloodStains);
	clearVector<ParticleSystem*> (&particleSystems);

	Player* player = new Player(0, 0, resources->PlayerWalkSprite,
			resources->PlayerDeathSprites[(rand()
					% (int) resources->PlayerDeathSprites.size())],
			resources->PlayerHitSounds, resources->PlayerDeathSound);
	player->setWeapon(weaponManager->getWeaponByName("PM"));
	player->HitR = 0.28f;

	gameState->lifeForms.insert(map<string, LifeForm*>::value_type(player->Id,
			player));

	playerId = player->Id;

	hud->addMessage(_("Try to survive as long as you can."));
	hud->addMessage(
			_("Shoot monsters to receive experience and other bonuses."));

	createTerrain();

	SDL_ShowCursor(0);

	for (unsigned int i = 0; i < config->MonstersAtStart; i++) {
		spawnEnemy(0, 0, (float) cam->getW(), 1, 1);
	}

	windows["mainmenu"]->CloseFlag = true;
}

// Creation of a string for the window title
inline string getProjectTitle() {
	return PROJECT + " v" + VERSION;
}

// Outputs the information on the program and the runtime environment
void printVersion() {
	string env = "UNKNOWN";
#ifdef _WIN32
	env = "WINDOWS";
#endif //_WIN32W
#ifdef linux
	env = "GNU/LINUX";
#endif //linux
#ifdef __FreeBSD__
	env = "BSD";
#endif //__FreeBSD__
#ifdef __APPLE__
	env = "MAC";
#endif //__APPLE__
	cout << getProjectTitle() << " @ " << env << endl;
}

string getDefaultName() {
#ifdef _WIN32
	return DEFAULT_CHAR_NAME;
#else
	string name = getenv("USER");
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

// Creation of system objects and their customization
void initSystem() {
	printf(_("Name found : %s\n"), getDefaultName().c_str());

	//~
	//~ Internationalization Initialization
	//~

	//~ i18n: initializes the entire current locale of the program as per environment variables set by the user
	setlocale(LC_ALL, "");

	//~ i18n: Indicate the path of the i18n catalog file
	bindtextdomain("violetland", TRANSLATION_PATH);

	//~ i18n: sets the message domain
	textdomain("violetland");

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

	SDL_Surface* icon = ImageUtility::loadImage(fileUtility->getFullPath(
			FileUtility::common, "icon-light.png"), 1.0f);
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
	Texture* tex = new Texture(ImageUtility::loadImage(
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
}

// Operations at destruction of the player:
// update of the list of the best results,
// change of a state of objects
void loseGame(Player* player) {
	gameState->Lost = true;

	hud->addMessage(_("You are gobbled up."));

	Highscores s(fileUtility);
	highscore = new HighscoresEntry(player, gameState->Time);
	gameState->HighScore = s.isHighscore(highscore);

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
	Player* player = (Player*) gameState->getLifeForm(playerId);

	const int l = (int) (config->Screen.Width * 0.1f);
	const int r = (int) (config->Screen.Width * 0.6f);

	Window* charStats = windows.find("charstats")->second;

	char *buf;
	//TODO: what is sprintf_s?
	sprintf(buf = new char[100], _("Current player level: %i"),
			(int) ((player->Level)));
	charStats->addElement("level", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 4.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], _("Available improvement points: %i"),
			(int) ((player->LevelPoints)));
	charStats->addElement("availpoints", videoManager->RegularText->getObject(
			buf, l, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], _("Strength: %i"), (int) ((player->Strength
			* 100)));
	charStats->addElement("strength", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Agility: %i"), (int) ((player->Agility
			* 100)));
	charStats->addElement("agility", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Vitality: %i"), (int) ((player->Vitality
			* 100)));
	charStats->addElement("vitality", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 9.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], _("HP: %i / Max HP: %i"),
			(int) ((player->getHealth() * 100)), (int) ((player->MaxHealth()
					* 100)));
	charStats->addElement("hp", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 11.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Melee damage: %i"),
			(int) ((player->Damage() * 100)));
	charStats->addElement("melee", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 12.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Chance of block: %i%%"),
			(int) ((player->ChanceToEvade() * 100)));
	charStats->addElement("chanceblock", videoManager->RegularText->getObject(
			buf, l, videoManager->RegularText->getHeight() * 13.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Reloading speed modifier: %i%%"),
			(int) ((player->ReloadSpeedMod() * 100)));
	charStats->addElement("reloadingspeed",
			videoManager->RegularText->getObject(buf, l,
					videoManager->RegularText->getHeight() * 14.0f,
					TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Accuracy deviation modifier: %i%%"),
			(int) ((player->WeaponRetForceMod() * 100)));
	charStats->addElement("accuracy", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 15.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], _("Health regeneration: %.2f/min"),
			(player->HealthRegen() * 6000000));
	charStats->addElement("healthregen", videoManager->RegularText->getObject(
			buf, l, videoManager->RegularText->getHeight() * 16.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	if (player->Unstoppable)
		charStats->addElement("+unstoppable",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 4.0f,
						TextManager::CENTER, TextManager::MIDDLE));

	if (player->PoisonBullets)
		charStats->addElement("+poisonbullets",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 5.0f,
						TextManager::CENTER, TextManager::MIDDLE));

	if (player->BigCalibre)
		charStats->addElement("+bigcalibre",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 6.0f,
						TextManager::CENTER, TextManager::MIDDLE));

	if (player->Telekinesis)
		charStats->addElement("+telekinesis",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 7.0f,
						TextManager::CENTER, TextManager::MIDDLE));

	if (player->NightVision)
		charStats->addElement("+nightvision",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 8.0f,
						TextManager::CENTER, TextManager::MIDDLE));

	if (player->Looting)
		charStats->addElement("+looting", videoManager->RegularText->getObject(
				"+", r, videoManager->RegularText->getHeight() * 9.0f,
				TextManager::CENTER, TextManager::MIDDLE));

	if (player->WideSight)
		charStats->addElement("+widesight",
				videoManager->RegularText->getObject("+", r,
						videoManager->RegularText->getHeight() * 10.0f,
						TextManager::CENTER, TextManager::MIDDLE));
}

void increaseVioletParam(std::string elementName) {
	Player* player = (Player*) gameState->getLifeForm(playerId);

	if (player->LevelPoints > 0) {
		if (elementName.compare("strength") == 0)
			player->Strength += 0.1;

		if (elementName.compare("agility") == 0)
			player->Agility += 0.1;

		if (elementName.compare("vitality") == 0) {
			float h = player->getHealth() / player->MaxHealth();
			player->Vitality += 0.1;
			player->setHealth(h * player->MaxHealth());
		}

		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void takePerk(std::string elementName) {
	Player* player = (Player*) gameState->getLifeForm(playerId);

	if (player->LevelPoints > 0) {
		if (elementName.compare("unstoppable") == 0 && !player->Unstoppable) {
			player->Unstoppable = true;
			player->LevelPoints--;
		}

		if (elementName.compare("poisonbullets") == 0 && !player->PoisonBullets) {
			player->PoisonBullets = true;
			player->LevelPoints--;
		}

		if (elementName.compare("bigcalibre") == 0 && !player->BigCalibre) {
			player->BigCalibre = true;
			player->LevelPoints--;
		}

		if (elementName.compare("telekinesis") == 0 && !player->Telekinesis) {
			player->Telekinesis = true;
			player->LevelPoints--;
		}

		if (elementName.compare("nightvision") == 0 && !player->NightVision) {
			player->NightVision = true;
			player->LevelPoints--;
		}

		if (elementName.compare("looting") == 0 && !player->Looting) {
			player->Looting = true;
			player->LevelPoints--;
		}

		if (elementName.compare("widesight") == 0 && !player->WideSight) {
			player->WideSight = true;
			player->LevelPoints--;
		}

		refreshCharStatsWindow();
	}
}

void showPerkDetails(std::string elementName) {
	if (elementName.compare("unstoppable") == 0) {
		windows["charstats"]->addElement(
				"explantation",
				videoManager->SmallText->getObject(
						_("Unstoppable: enemies can't block your movement any more, but they still can hurt you."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("poisonbullets") == 0) {
		windows["charstats"]->addElement(
				"explantation",
				videoManager->SmallText->getObject(
						_("Poison bullets: after getting hit by your bullet, enemies slowly lose health until they die."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("bigcalibre") == 0) {
		windows["charstats"]->addElement(
				"explantation",
				videoManager->SmallText->getObject(
						_("Big calibre: your bullets can wound a few monsters in a row."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("telekinesis") == 0) {
		windows["charstats"]->addElement(
				"explantation",
				videoManager->SmallText->getObject(
						_("Telekinesis: useful things slowly move towards you."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("nightvision") == 0) {
		windows["charstats"]->addElement("explantation",
				videoManager->SmallText->getObject(
						_("Night vision: you can see in the dark."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("looting") == 0) {
		windows["charstats"]->addElement("explantation",
				videoManager->SmallText->getObject(
						_("Looting: Monsters will drop more bonuses."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}

	if (elementName.compare("widesight") == 0) {
		windows["charstats"]->addElement(
				"explantation",
				videoManager->SmallText->getObject(
						_("Wide sight: accessible area for action is much more."),
						config->Screen.Width / 2,
						videoManager->RegularText->getHeight() * 1.0f,
						TextManager::CENTER, TextManager::MIDDLE));
	}
}

void createCharStatWindow() {
	Window *charStats = new CharStatsWindow(config, videoManager);

	charStats->addHandler(Window::hdl_lclick, "strength", increaseVioletParam);
	charStats->addHandler(Window::hdl_lclick, "agility", increaseVioletParam);
	charStats->addHandler(Window::hdl_lclick, "vitality", increaseVioletParam);

	charStats->addHandler(Window::hdl_lclick, "unstoppable", takePerk);
	charStats->addHandler(Window::hdl_move, "unstoppable", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "poisonbullets", takePerk);
	charStats->addHandler(Window::hdl_move, "poisonbullets", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "bigcalibre", takePerk);
	charStats->addHandler(Window::hdl_move, "bigcalibre", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "telekinesis", takePerk);
	charStats->addHandler(Window::hdl_move, "telekinesis", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "nightvision", takePerk);
	charStats->addHandler(Window::hdl_move, "nightvision", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "looting", takePerk);
	charStats->addHandler(Window::hdl_move, "looting", showPerkDetails);
	charStats->addHandler(Window::hdl_lclick, "widesight", takePerk);
	charStats->addHandler(Window::hdl_move, "widesight", showPerkDetails);

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

void backFromOptionsAndSave(std::string elementName);

void refreshOptionsWindow() {
	const int l = config->Screen.Width * 0.1f;
	const int r = config->Screen.Width * 0.6f;

	Window* w = windows.find("options")->second;

	if (config->AutoReload)
		w->addElement("+autoreload", videoManager->RegularText->getObject("+",
				l, videoManager->RegularText->getHeight() * 6.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autoreload", false);

	if (config->AutoWeaponPickup)
		w->addElement("+autopickup", videoManager->RegularText->getObject("+",
				l, videoManager->RegularText->getHeight() * 7.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autopickup", false);

	if (config->FriendlyFire)
		w->addElement("+friendlyfire", videoManager->RegularText->getObject(
				"+", l, videoManager->RegularText->getHeight() * 8.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+friendlyfire", false);

	if (config->Screen.Full)
		w->addElement("+fullscreen", videoManager->RegularText->getObject("+",
				r, videoManager->RegularText->getHeight() * 6.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+fullscreen", false);

	ostringstream oss;
	oss << tempConfig->Screen.Width << 'x' << tempConfig->Screen.Height;
	TextObject* resInfo = videoManager->RegularText->getObject(oss.str(), r
			+ videoManager->RegularText->getHeight() * 8.0f,
			videoManager->RegularText->getHeight() * 7.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	w->addElement("+resolution", resInfo);

	oss.str("");
	oss << config->SoundVolume * 10 << '%';
	TextObject* sndInd = videoManager->RegularText->getObject(oss.str(), l,
			videoManager->RegularText->getHeight() * 12.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	w->addElement("+soundvolume", sndInd);

	oss.str("");
	oss << config->MusicVolume * 10 << '%';
	TextObject * musInd = videoManager->RegularText->getObject(oss.str(), l,
			videoManager->RegularText->getHeight() * 13.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	w->addElement("+musicvolume", musInd);
}

void switchGameOption(std::string elementName) {
	if (elementName.compare("autoreload") == 0)
		config->AutoReload = !config->AutoReload;

	if (elementName.compare("autopickup") == 0)
		config->AutoWeaponPickup = !config->AutoWeaponPickup;

	if (elementName.compare("friendlyfire") == 0)
		config->FriendlyFire = !config->FriendlyFire;

	if (elementName.compare("fullscreen") == 0)
		config->Screen.Full = !config->Screen.Full;

	refreshOptionsWindow();
}

void switchVolumeDown(std::string elementName) {
	if (elementName.compare("musicvolume") == 0) {
		if (config->MusicVolume > 0) {
			config->MusicVolume--;
			Mix_Volume(0, config->MusicVolume * 12);
		} else {
			config->MusicVolume = 10;
			Mix_Volume(0, config->MusicVolume * 12);
		}
	}

	if (elementName.compare("soundvolume") == 0) {
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
	}

	refreshOptionsWindow();
}

void switchVolumeUp(std::string elementName) {
	if (elementName.compare("musicvolume") == 0) {
		if (config->MusicVolume <= 9) {
			config->MusicVolume++;
			Mix_Volume(0, config->MusicVolume * 12);
		} else {
			config->MusicVolume = 0;
			Mix_Volume(0, 0);
		}
	}

	if (elementName.compare("soundvolume") == 0) {
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
	}

	refreshOptionsWindow();
}

void switchResolutionDown(std::string elementName) {
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

	refreshOptionsWindow();
}

void switchResolutionUp(std::string elementName) {
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

	refreshOptionsWindow();
}

void controlsMenuWindowController(std::string elementName);

void refreshControlsMenuWindow() {
	Window* w = windows.find("controls")->second;

	const int col1_l = config->Screen.Width * 0.1f;
	const int col1_r = config->Screen.Width * 0.45f;

	w->addElement("controls", videoManager->RegularText->getObject(
			_("Controls"), col1_l, videoManager->RegularText->getHeight()
					* 2.0f, TextManager::LEFT, TextManager::MIDDLE));

	const int col2_l = config->Screen.Width * 0.55f;
	const int col2_r = config->Screen.Width * 0.9f;

	int col1_items = InputHandler::GameInputEventsCount / 2;
	int col2_items = col1_items;
	if (col1_items + col2_items != InputHandler::GameInputEventsCount)
		col1_items++;

	for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
		int l, r, strN;
		if (i < col1_items) {
			l = col1_l;
			r = col1_r;
			strN = i + 4;
		} else {
			l = col2_l;
			r = col2_r;
			strN = i - col1_items + 4;
		}

		w->addElement(InputHandler::getEventName(i),
				videoManager->RegularText->getObject(
						InputHandler::getEventName(i), l,
						videoManager->RegularText->getHeight() * strN,
						TextManager::LEFT, TextManager::MIDDLE));

		string skey = InputHandler::getEventName(i) + "key";
		w->addElement(skey, videoManager->RegularText->getObject(
				InputHandler::getKeyName(config->PlayerInputBinding[i]), r,
				videoManager->RegularText->getHeight() * strN,
				TextManager::RIGHT, TextManager::MIDDLE));

		w->addHandler(Window::hdl_lclick, InputHandler::getEventName(i),
				controlsMenuWindowController);
	}
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

void controlsMenuWindowController(std::string elementName) {
	Window *w = new Window(0.0f, 0.0f, config->Screen.Width,
			config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f);

	w->addElement("pressakey",
			videoManager->RegularText->getObject(_("Press a key, please..."),
					config->Screen.Width / 2, config->Screen.Height / 2,
					TextManager::CENTER, TextManager::MIDDLE));

	windows["pressakey"] = w;

	drawWindows();
	SDL_GL_SwapBuffers();

	int key = InputHandler::getEventNumber(elementName);

	input->resetMouseButtons();
	SDL_Event event;
	while (event.type != SDL_QUIT && event.type != SDL_KEYDOWN && event.type
			!= SDL_MOUSEBUTTONDOWN) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_KEYDOWN:
			config->PlayerInputBinding[key].Type = InputHandler::Keyboard;
			config->PlayerInputBinding[key].Value = event.key.keysym.sym;
			break;
		case SDL_MOUSEBUTTONDOWN:
			config->PlayerInputBinding[key].Type = InputHandler::Mouse;
			config->PlayerInputBinding[key].Value = event.button.button;
			break;
		case SDL_QUIT:
			gameState->end();
		}
	}

	fprintf(stdout, _("Bind action %s to %s.\n"), elementName.c_str(),
			InputHandler::getKeyName(config->PlayerInputBinding[key]));

	config->write();

	windows["pressakey"]->CloseFlag = true;

	refreshControlsMenuWindow();
}

void drawWindows();
void showHighScores(std::string);

void createControlsMenuWindow(std::string elementName) {
	Window *w = new Window(0.0f, 0.0f, config->Screen.Width,
			config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f);

	windows["controls"] = w;

	refreshControlsMenuWindow();

	windows["options"]->CloseFlag = true;
}

void resetControls(std::string elementName) {
	Configuration* config_default = new Configuration(fileUtility);
	for (int i = 0; i < InputHandler::GameInputEventsCount; i++)
		config->PlayerInputBinding[i] = config_default->PlayerInputBinding[i];
}

void createOptionsWindow() {
	tempConfig = new Configuration(*config);

	Window *w = new Window(0.0f, 0.0f, config->Screen.Width,
			config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->Screen.Width * 0.1f;
	const int r = config->Screen.Width * 0.6f;

	w->addElement("options", videoManager->RegularText->getObject(_("Options"),
			l, videoManager->RegularText->getHeight() * 2.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("sectiongame", videoManager->RegularText->getObject(
			_("Gameplay"), l, videoManager->RegularText->getHeight()
					* 4.0f, TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("autoreload", videoManager->RegularText->getObject(
			_("Weapon autoreloading"), l
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 6.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	w->addElement("autopickup", videoManager->RegularText->getObject(
			_("Weapon autotaking"), l
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 7.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	w->addElement("friendlyfire", videoManager->RegularText->getObject(
			_("Friendly fire"), l
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 8.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("sectiongraphics", videoManager->RegularText->getObject(
			_("Graphics"), r, videoManager->RegularText->getHeight()
					* 4.0f, TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("fullscreen", videoManager->RegularText->getObject(
			_("Fullscreen"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 6.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	w->addElement("resolution", videoManager->RegularText->getObject(
			_("Resolution"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("sectionsound", videoManager->RegularText->getObject(
			_("Sound"), l,
			videoManager->RegularText->getHeight() * 10.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("soundvolume", videoManager->RegularText->getObject(
			_("Sound volume"), l + videoManager->RegularText->getHeight()
					* 3.0f, videoManager->RegularText->getHeight() * 12.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	w->addElement("musicvolume", videoManager->RegularText->getObject(
			_("Music volume"), l + videoManager->RegularText->getHeight()
					* 3.0f, videoManager->RegularText->getHeight() * 13.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("controlstitle", videoManager->RegularText->getObject(
			_("Controls"), r, videoManager->RegularText->getHeight()
					* 10.0f, TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("controlsmenu", videoManager->RegularText->getObject(
			_("Edit Controls"), r
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 12.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("controlsreset", videoManager->RegularText->getObject(
			_("Reset Controls"), r
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 13.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addHandler(Window::hdl_lclick, "autoreload", switchGameOption);
	w->addHandler(Window::hdl_lclick, "autopickup", switchGameOption);
	w->addHandler(Window::hdl_lclick, "friendlyfire", switchGameOption);
	w->addHandler(Window::hdl_lclick, "soundvolume", switchVolumeUp);
	w->addHandler(Window::hdl_rclick, "soundvolume", switchVolumeDown);
	w->addHandler(Window::hdl_lclick, "musicvolume", switchVolumeUp);
	w->addHandler(Window::hdl_rclick, "musicvolume", switchVolumeDown);
	w->addHandler(Window::hdl_lclick, "fullscreen", switchGameOption);
	w->addHandler(Window::hdl_lclick, "resolution", switchResolutionUp);
	w->addHandler(Window::hdl_rclick, "resolution", switchResolutionDown);
	w->addHandler(Window::hdl_lclick, "controlsmenu", createControlsMenuWindow);
	w->addHandler(Window::hdl_lclick, "controlsreset", resetControls);

	w->addElement("savereturn", videoManager->RegularText->getObject(
			_("Save and return"), l,
			videoManager->RegularText->getHeight() * 16.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	w->addHandler(Window::hdl_lclick, "savereturn", backFromOptionsAndSave);

	windows["options"] = w;

	refreshOptionsWindow();
}

void showOptions(std::string elementName) {
	windows["mainmenu"]->CloseFlag = true;
	createOptionsWindow();
}

void resumeGame(std::string elementName) {
	Window* w = windows.find("mainmenu")->second;
	w->CloseFlag = true;
	switchGamePause();
}

void endGame(std::string elementName) {
	gameState->end();
}

void refreshMainMenuWindow() {
	Window* w = windows.find("mainmenu")->second;

	const int r = config->Screen.Width * 0.3f;

	string strGameMode = _("Unknown");
	switch (gameMode) {
	case GAMEMODE_SURVIVAL:
		strGameMode = _("Violetland Survival");
		break;
	case GAMEMODE_WAVES:
		strGameMode = _("Attack waves");
		break;
	}

	w->addElement("gamemode", videoManager->RegularText->getObject(
			strGameMode.c_str(), r, videoManager->RegularText->getHeight()
					* 8.0f, TextManager::LEFT, TextManager::MIDDLE));
}

void switchGameMode(std::string elementName) {
	switch (gameMode) {
	case GAMEMODE_SURVIVAL:
		gameMode = GAMEMODE_WAVES;
		break;
	case GAMEMODE_WAVES:
		gameMode = GAMEMODE_SURVIVAL;
		break;
	}

	refreshMainMenuWindow();
}

void createMainMenuWindow() {
	gameMode = gameState->Mode;

	Window *mainMenu = new MainMenuWindow(config, gameState,
			videoManager->RegularText);

	mainMenu->addHandler(Window::hdl_lclick, "resume", resumeGame);
	mainMenu->addHandler(Window::hdl_lclick, "start", startGame);
	mainMenu->addHandler(Window::hdl_lclick, "gamemode", switchGameMode);
	mainMenu->addHandler(Window::hdl_lclick, "options", showOptions);
	mainMenu->addHandler(Window::hdl_lclick, "highscores", showHighScores);
	mainMenu->addHandler(Window::hdl_lclick, "exit", endGame);

	std::map<std::string, Window*>::iterator it = windows.find("mainmenu");
	if (it != windows.end())
		delete it->second;
	windows["mainmenu"] = mainMenu;

	refreshMainMenuWindow();
}

void highScoresWindowController(std::string elementName) {
	if (elementName.compare("back") == 0) {
		windows["highscores"]->CloseFlag = true;
		createMainMenuWindow();
	}

	if (elementName.compare("reset") == 0) {
		Highscores s(fileUtility);
		s.clear();
		highScoresWindowController("back");
	}
}

void createHighscoresWindow() {
	Window *scoresWin = new Window(0.0f, 0.0f, config->Screen.Width,
			config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->Screen.Width * 0.1f;
	const int r2 = l * 2.0f;
	const int r3 = l * 5.0f;
	const int r4 = l * 7.0f;

	scoresWin->addElement("highscores", videoManager->RegularText->getObject(
			_("Highscores"), l, videoManager->RegularText->getHeight()
					* 2.0f, TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addElement("headerXp", videoManager->RegularText->getObject(
			_("XP"), l, videoManager->RegularText->getHeight() * 4.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	scoresWin->addElement("headerParams", videoManager->RegularText->getObject(
			_("Str/Agil/Vital"), r2,
			videoManager->RegularText->getHeight() * 4.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	scoresWin->addElement("headerTime", videoManager->RegularText->getObject(
			_("Time"), r3, videoManager->RegularText->getHeight() * 4.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	scoresWin->addElement("headerName", videoManager->RegularText->getObject(
			_("Name"), r4, videoManager->RegularText->getHeight() * 4.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	Highscores s(fileUtility);
	vector<HighscoresEntry*> highscores = s.getData();

	if (!highscores.empty())
		for (unsigned int i = 0; i < highscores.size(); i++) {
			ostringstream oss1, oss2;

			oss1 << "xp" << i;
			oss2 << highscores[i]->Xp;
			scoresWin->addElement(
					oss1.str(),
					videoManager->RegularText->getObject(
							oss2.str(),
							l,
							videoManager->RegularText->getHeight() * (5.0f + i),
							TextManager::LEFT, TextManager::MIDDLE));

			oss1.str("");
			oss2.str("");
			oss1 << "params" << i;
			oss2 << (int) (highscores[i]->Strength * 100) << '/'
					<< (int) (highscores[i]->Agility * 100) << '/'
					<< (int) (highscores[i]->Vitality * 100);
			scoresWin->addElement(
					oss1.str(),
					videoManager->RegularText->getObject(
							oss2.str(),
							r2,
							videoManager->RegularText->getHeight() * (5.0f + i),
							TextManager::LEFT, TextManager::MIDDLE));

			const int minutes = highscores[i]->Time / 60000;
			const int seconds = (highscores[i]->Time - minutes * 60000) / 1000;

			oss1.str("");
			oss2.str("");
			oss1 << "time" << i;
			oss2 << minutes << "m " << seconds << 's';
			scoresWin->addElement(
					oss1.str(),
					videoManager->RegularText->getObject(
							oss2.str(),
							r3,
							videoManager->RegularText->getHeight() * (5.0f + i),
							TextManager::LEFT, TextManager::MIDDLE));

			oss1.str("");
			oss1 << "name" << i;
			scoresWin->addElement(
					oss1.str(),
					videoManager->RegularText->getObject(
							highscores[i]->Name->c_str(),
							r4,
							videoManager->RegularText->getHeight() * (5.0f + i),
							TextManager::LEFT, TextManager::MIDDLE));
		}

	scoresWin->addElement("back", videoManager->RegularText->getObject(
			_("Back to main menu"), l,
			videoManager->RegularText->getHeight() * 16.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	scoresWin->addElement("reset", videoManager->RegularText->getObject(
			_("Reset list"), r3, videoManager->RegularText->getHeight()
					* 16.0f, TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addHandler(Window::hdl_lclick, "back",
			highScoresWindowController);

	scoresWin->addHandler(Window::hdl_lclick, "reset",
			highScoresWindowController);

	windows["highscores"] = scoresWin;
}

void showHighScores(std::string elementName) {
	windows["mainmenu"]->CloseFlag = true;
	createHighscoresWindow();
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

void backFromOptionsAndSave(std::string elementName) {
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

	windows["options"]->CloseFlag = true;
	createMainMenuWindow();
}

void createHelpWindow() {
	Window *help = new HelpWindow(config, videoManager->RegularText);
	windows["helpscreen"] = help;
}

void handleCommonControls() {
	if (input->getPressInput(InputHandler::ShowChar)) {
		map<string, Window*>::iterator it = windows.find("charstats");
		if (it == windows.end()) {
			if (gameState->Begun && !gameState->Lost) { // it is possible to remove second check to show charstats window after player death
				clearMap<std::string, Window*> (&windows);

				createCharStatWindow();
				refreshCharStatsWindow();

				if (!gameState->Paused)
					switchGamePause();
			}
		} else {
			Window* w = it->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	/* Currently the help screen is unneeded as it contains only controls tips.
	 * Controls tips are equivalent to controls menu.
	 */

	//	if (input->getPressInput(InputHandler::Help)) {
	//		map<string, Window*>::iterator it = windows.find("helpscreen");
	//		if (it == windows.end()) {
	//			clearMap<std::string, Window*> (&windows);
	//
	//			createHelpWindow();
	//
	//			if (!gameState->Paused)
	//				switchGamePause();
	//		} else {
	//			Window* w = it->second;
	//			w->CloseFlag = true;
	//			switchGamePause();
	//		}
	//	}

	if (input->getPressInput(InputHandler::Menu)) {
		map<string, Window*>::iterator it = windows.find("mainmenu");
		if (it == windows.end()) {
			clearMap<std::string, Window*> (&windows);

			createMainMenuWindow();

			if (!gameState->Paused)
				switchGamePause();
		} else if (gameState->Begun) {
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
				float x = vBlood[i].x + distance * -cos((vBlood[i].angle
						- angleDev) * M_PI / 180.0f);
				float y = vBlood[i].y + distance * -sin((vBlood[i].angle
						- angleDev) * M_PI / 180.0f);
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
	Player* player = (Player*) gameState->getLifeForm(playerId);
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
			addBloodStain(enemy->X, enemy->Y, enemy->Angle, (rand() % 10)
					/ 50.0f + 0.1f, enemy->Poisoned);
		}
	}

	if (lf->Burning) {
		ParticleSystem* partSys = new ParticleSystem();
		for (unsigned int k = 0; k < 5; k++) {
			Particle * p = new Particle(enemy->X + (rand() % 50) - 25, enemy->Y
					+ (rand() % 50) - 25, 128, 128, resources->ExplTex[0]);
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
	if (enemy->targetId.compare("ambient") != 0) {
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
	if (enemy->targetId.compare("ambient") == 0) {
		LifeForm* lifeForm = gameState->getLifeForm(playerId);

		if (lifeForm->State == LIFEFORM_STATE_ALIVE) {
			float range = Object::calc_dist(enemy->X, enemy->Y, lifeForm->X,
					lifeForm->Y);
			if (range < 800)
				enemy->targetId = lifeForm->Id;
		}
	}

	if (enemy->targetId.compare("ambient") == 0) {
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
			enemy->TargetX = targetLifeForm->X - cos((targetLifeForm->Angle
					+ 90) * M_PI / 180) * range / 2.0f / enemy->Speed
					* targetLifeForm->Speed;
			enemy->TargetY = targetLifeForm->Y - sin((targetLifeForm->Angle
					+ 90) * M_PI / 180) * range / 2.0f / enemy->Speed
					* targetLifeForm->Speed;
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
	player->NextLevelXp *= 2;

	player->Level += 1;
	player->LevelPoints += 1;

	hud->addMessage(_("You have reached new level."));

	player->setHealth(player->MaxHealth());
}

void handlePlayer(LifeForm* lf) {
	Player* player = (Player*) lf;

	if (player->Xp >= player->NextLevelXp) {
		levelUp(player);
	}

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

	if (lf->X < -gameState->GameAreaSize)
		player->setX(-gameState->GameAreaSize);
	if (lf->X > gameState->GameAreaSize)
		player->setX(gameState->GameAreaSize);
	if (lf->Y < -gameState->GameAreaSize)
		player->setY(-gameState->GameAreaSize);
	if (lf->Y > gameState->GameAreaSize)
		player->setY(gameState->GameAreaSize);

	if (player->WideSight) {
		lf->TargetX = input->mouseX / videoManager->WK - cam->getHalfW()
				+ player->X;
		lf->TargetY = input->mouseY / videoManager->HK - cam->getHalfH()
				+ player->Y;
	} else {
		lf->TargetX = input->mouseX / videoManager->WK - cam->getHalfW()
				+ cam->X;
		lf->TargetY = input->mouseY / videoManager->HK - cam->getHalfH()
				+ cam->Y;
	}

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
			spark->AMod = -0.0003;
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
		gameState->bullets.push_back(player->throwGrenade(
				resources->GrenadeSprite));
	}
}

//Choice and creation of bonus
void dropPowerup(float x, float y, float chance, bool forceWeapon) {
	bool powerupDropped = false;
	Powerup *newPowerup;

	// Weapon drop - should be first check
	if (forceWeapon || roulette(chance * 2.5)) {
		int weaponIndex;
		if (false) // TODO: "Allow PM drop" to options. true for allow.
			weaponIndex = (rand() % weaponManager->Weapons.size());
		else
			weaponIndex = (rand() % (weaponManager->Weapons.size() - 1));

		newPowerup = new Powerup(x, y,
				weaponManager->Weapons[weaponIndex]->getDroppedTex());
		newPowerup->Type = BONUS_WEAPON;
		newPowerup->Object = weaponManager->Weapons[weaponIndex];
		newPowerup->HitR = 0.5f;
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 5)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_MEDIKIT]);
		newPowerup->Scale = 0.3f;
		newPowerup->Type = BONUS_MEDIKIT;
		newPowerup->Object = new float(0.1f);
		newPowerup->RMask = newPowerup->BMask = 0.2f;
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2.5)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_MEDIKIT]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_MEDIKIT;
		newPowerup->Object = new float(0.2f);
		newPowerup->RMask = newPowerup->GMask = 0.4f;
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_MEDIKIT]);
		newPowerup->Scale = 0.5f;
		newPowerup->Type = BONUS_MEDIKIT;
		newPowerup->Object = new float(0.6f);
		newPowerup->BMask = newPowerup->GMask = 0.2f;
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_GRENADES]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_GRENADES;
		newPowerup->Object = new int(1);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_FREEZE]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_FREEZE;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_NUKE]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_NUKE;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_PENBULLETS]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_PENBULLETS;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y,
				resources->PowerupTex[BONUS_VITALITYROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_VITALITYROIDS;
		newPowerup->RMask = newPowerup->BMask = 0.2f;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y,
				resources->PowerupTex[BONUS_STRENGTHROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_STRENGTHROIDS;
		newPowerup->GMask = newPowerup->BMask = 0.2f;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y,
				resources->PowerupTex[BONUS_AGILITYROIDS]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_AGILITYROIDS;
		newPowerup->RMask = newPowerup->GMask = 0.2f;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (!powerupDropped && roulette(chance * 2)) {
		newPowerup = new Powerup(x, y, resources->PowerupTex[BONUS_TELEPORTS]);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = BONUS_TELEPORTS;
		newPowerup->Object = new int(1);
		powerupDropped = true;
	}

	if (powerupDropped)
		gameState->powerups.push_back(newPowerup);
}

void handleLifeForms() {
	Player* player = (Player*) gameState->getLifeForm(playerId);

	if (!gameState->Lost) {
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
	}

	if (!gameState->lifeForms.empty()) {
		map<string, LifeForm*>::const_iterator iter;
		for (iter = gameState->lifeForms.begin(); iter
				!= gameState->lifeForms.end(); ++iter) {
			LifeForm* lifeForm = iter->second;

			if (lifeForm->Type == LIFEFORM_PLAYER) {
				if (!gameState->Lost) {
					if (lifeForm->State == LIFEFORM_STATE_DIED
							|| lifeForm->State == LIFEFORM_STATE_BURST)
						loseGame(player);

					if (lifeForm->State == LIFEFORM_STATE_ALIVE)
						handlePlayer(lifeForm);
				}
			}

			if (lifeForm->Type == LIFEFORM_MONSTER) {
				if (lifeForm->State == LIFEFORM_STATE_ALIVE)
					handleMonster(lifeForm);
			}

			lifeForm->process(videoManager->getFrameDeltaTime());

			if (lifeForm->State == LIFEFORM_STATE_DIED)
				bloodStains.push_back(lifeForm->getCorpse());

			if (lifeForm->State == LIFEFORM_STATE_DIED || lifeForm->State
					== LIFEFORM_STATE_BURST) {

				if (lifeForm->Type == LIFEFORM_MONSTER) {
					delete lifeForm;
					gameState->lifeForms.erase(iter->first);
				}
			}
		}
	}
}

void collideBulletAndEnemy(Bullet* bullet, Monster* enemy) {
	if (bullet->Type == BULLET_FLAME) {
		if (enemy->Frozen > 0)
			enemy->Frozen = 0;

		enemy->Burning = true;
	}

	Player* player = (Player*) gameState->getLifeForm(bullet->OwnerId);

	if (bullet->Type == BULLET_STANDARD && bloodStains.size() < 10) {
		for (unsigned int k = 0; k < 10 / 3; k++) {
			int angleDev = 90 + (rand() % 60) - 30;
			float distance = (rand() % 100);
			float bX = enemy->X - cos((bullet->Angle + angleDev) * M_PI
					/ 180.0f) * distance;
			float bY = enemy->Y - sin((bullet->Angle + angleDev) * M_PI
					/ 180.0f) * distance;

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
				p->AMod = -0.002;
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

	if (!bypassDirectDamage) {
		float damageLoss = enemy->getHealth();

		Sound* hitSound = enemy->hit(bullet->Damage, bullet->Poisoned,
				bullet->Type != BULLET_FLAME);

		if (hitSound != NULL) {
			hitSound->play(7, 0, 0);
			hitSound->setPos(Object::calc_angle(enemy->X, enemy->Y, player->X,
					player->Y), Object::calc_dist(enemy->X, enemy->Y,
					player->X, player->Y));
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
	if (!gameState->bullets.empty()) {
		for (int i = gameState->bullets.size() - 1; i >= 0; i--) {
			gameState->bullets[i]->process(videoManager->getFrameDeltaTime());

			if (gameState->bullets[i]->isActive()
					&& !gameState->lifeForms.empty()) {
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
				addExplosion(gameState->bullets[i]->X,
						gameState->bullets[i]->Y,
						gameState->bullets[i]->Damage, 150.0f);

				resources->ExplSounds[0]->play(8, 0, 0);

				Explosion* expl = new Explosion(false,
						gameState->bullets[i]->X, gameState->bullets[i]->Y,
						150.0f, resources->ExplTex[0], resources->ExplTex[1]);
				particleSystems.push_back(expl);
			}

			if (gameState->bullets[i]->isReadyToRemove()) {
				delete gameState->bullets[i];
				gameState->bullets.erase(gameState->bullets.begin() + i);
			}
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

	for (int i = gameState->powerups.size() - 1; i >= 0; i--) {
		bool deletePowerup = false;
		gameState->powerups[i]->Time -= videoManager->getFrameDeltaTime();
		gameState->powerups[i]->AMask = gameState->powerups[i]->Time / 15000.0;
		if (gameState->powerups[i]->Type != BONUS_WEAPON) {
			gameState->powerups[i]->Angle += videoManager->getFrameDeltaTime()
					* 0.05f * gameState->powerups[i]->Dir;

			if (gameState->powerups[i]->Angle > 45
					&& gameState->powerups[i]->Dir > 0)
				gameState->powerups[i]->Dir = -1;

			if (gameState->powerups[i]->Angle < -45
					&& gameState->powerups[i]->Dir < 0)
				gameState->powerups[i]->Dir = 1;
		}

		if (gameState->powerups[i]->Time <= 0)
			deletePowerup = true;

		if (!gameState->Lost && gameState->powerups[i]->detectCollide(
				player->TargetX, player->TargetY)) {

			switch (gameState->powerups[i]->Type) {
			case BONUS_MEDIKIT:
				hud->Info = _("a medikit");
				break;
			case BONUS_FREEZE:
				hud->Info = _("a nitrogen bomb");
				break;
			case BONUS_NUKE:
				hud->Info = _("nuke!");
				break;
			case BONUS_GRENADES:
				hud->Info = _("a hand grenade");
				break;
			case BONUS_PENBULLETS:
				hud->Info = _("penetration bullets");
				break;
			case BONUS_VITALITYROIDS:
				hud->Info = _("vitality boost");
				break;
			case BONUS_STRENGTHROIDS:
				hud->Info = _("strength boost");
				break;
			case BONUS_AGILITYROIDS:
				hud->Info = _("agility boost");
				break;
			case BONUS_TELEPORTS:
				hud->Info = _("a teleport");
				break;
			case BONUS_WEAPON:
				hud->Info = ((Weapon*) gameState->powerups[i]->Object)->Name;
				break;
			}

			if (player->Telekinesis) {
				float a = Object::calc_angle(gameState->powerups[i]->X,
						gameState->powerups[i]->Y, player->X, player->Y);
				gameState->powerups[i]->X -= cos((a + 90) * M_PI / 180)
						* videoManager->getFrameDeltaTime()
						* player->MaxSpeed() * 2;
				gameState->powerups[i]->Y -= sin((a + 90) * M_PI / 180)
						* videoManager->getFrameDeltaTime()
						* player->MaxSpeed() * 2;
			}
		}

		if (!gameState->Lost && (gameState->powerups[i]->detectCollide(player))) {
			switch (gameState->powerups[i]->Type) {
			case BONUS_MEDIKIT: {
				if (player->getHealth() < player->MaxHealth()) {
					hud->addMessage(_("You have taken a medical kit."));
					player->setHealth(player->getHealth()
							+ *(float*) gameState->powerups[i]->Object);
					deletePowerup = true;
				}
				break;
			}
			case BONUS_FREEZE: {
				hud->addMessage(_("All have been frozen around you."));

				map<string, LifeForm*>::const_iterator iter;
				for (iter = gameState->lifeForms.begin(); iter
						!= gameState->lifeForms.end(); ++iter) {
					LifeForm* lf = iter->second;

					if (lf->Type == LIFEFORM_PLAYER || lf->State
							!= LIFEFORM_STATE_ALIVE)
						continue;
					lf->Frozen = *(int*) gameState->powerups[i]->Object;
					lf->Burning = false;
					lf->Speed = 0.0f;
				}
				player->bonusTimes[PLAYER_BONUS_FREEZE]
						= *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_NUKE: {
				hud->addMessage(_("Boom!"));
				addExplosion(player->X, player->Y, 12.0f, 400.0f);

				resources->ExplSounds[1]->play(8, 0, 0);

				Explosion * expl = new Explosion(true, player->X, player->Y,
						400.0f, resources->ExplTex[0], resources->ExplTex[1]);
				particleSystems.push_back(expl);

				deletePowerup = true;
				break;
			}
			case BONUS_PENBULLETS: {
				hud->addMessage(_("You got powerful penetration bullets."));
				player->bonusTimes[PLAYER_BONUS_PENBULLETS]
						= *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_VITALITYROIDS: {
				hud->addMessage(_("You got a vitality boost."));
				player->bonusTimes[PLAYER_BONUS_VITALITYBOOST]
						+= *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_STRENGTHROIDS: {
				hud->addMessage(_("You got a strength boost."));
				player->bonusTimes[PLAYER_BONUS_STRENGTHBOOST]
						+= *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_AGILITYROIDS: {
				hud->addMessage(_("You got a agility boost."));
				player->bonusTimes[PLAYER_BONUS_AGILITYBOOST]
						+= *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_GRENADES: {
				hud->addMessage(_("You have taken a grenade."));
				player->Grenades += *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_TELEPORTS: {
				hud->addMessage(_("You have taken a teleport."));
				player->Teleports += *(int*) gameState->powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case BONUS_WEAPON: {
				if (input->getDownInput(InputHandler::Pickup)
						|| config->AutoWeaponPickup) {
					player->setWeapon((Weapon*) gameState->powerups[i]->Object);
					char *buf;
					sprintf(buf = new char[200], _("You have taken the %s."),
							player->getWeapon()->Name.c_str());
					hud->addMessage(buf);
					delete[] buf;
					deletePowerup = true;
				}
				break;
			}
			}
		}

		if (deletePowerup) {
			delete gameState->powerups[i];
			gameState->powerups.erase(gameState->powerups.begin() + i);
			continue;
		}
	}
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
			highscore->Name = new string(gameState->PlayerName);
			input->setInputMode(InputHandler::Direct);
			Highscores s(fileUtility);
			s.add(highscore);
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

	if (player->WideSight) {
		cam->X = (player->TargetX + player->X) / 2;
		cam->Y = (player->TargetY + player->Y) / 2;
	} else {
		cam->X = player->X;
		cam->Y = player->Y;
	}

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

	float left = cam->X + cam->getHalfW();
	float right = cam->X - cam->getHalfW();
	float top = cam->Y + cam->getHalfH();
	float bottom = cam->Y - cam->getHalfH();
	map<string, LifeForm*>::const_iterator iter;
	for (iter = gameState->lifeForms.begin(); iter
			!= gameState->lifeForms.end(); ++iter) {
		LifeForm* lifeForm = iter->second;

		if (lifeForm->getLeft() < left && lifeForm->getRight() > right
				&& lifeForm->getTop() < top && lifeForm->getBottom() > bottom)

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
		const float maxLen = cam->getH() * 0.75f;
		if (player->getLaser()) {
			glLineWidth(0.5f);
			glBegin(GL_LINES);
			glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
			glVertex3f(wpnX, wpnY, 0);
			glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
			glVertex3f(player->X + maxLen * cos(rad), player->Y + maxLen * sin(
					rad), 0);
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
			glVertex3f(player->X + len * cos(rad - width), player->Y + len
					* sin(rad - width), 0.0f);
			glVertex3f(player->X + len * cos(rad + width), player->Y + len
					* sin(rad + width), 0.0f);

			glEnd();
		}
	}

	if (!gameState->Lost && !gameState->Paused) {
		aim->draw(player->TargetX, player->TargetY,
				1.0f + tan(player->AccuracyDeviation * M_PI / 180)
						* Object::calc_dist(player->X, player->Y,
								player->TargetX, player->TargetY) / 25.0f,
				player->getWeapon()->getReloadState() > 0 ? 1.2f
						- player->getWeapon()->getReloadState() : 0.2f);
	}

	glEnable(GL_TEXTURE_2D);
}

void drawFps() {
	char* buf;
	sprintf(buf = new char[30], _("FPS: %i"), videoManager->getFps());
	videoManager->RegularText->draw(buf, config->Screen.Width
			- videoManager->RegularText->getIndent(), config->Screen.Height
			- videoManager->RegularText->getIndent(), TextManager::RIGHT,
			TextManager::BOTTOM);
	delete[] buf;
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

void parsePreferences(int argc, char *argv[]) {
	fileUtility = new FileUtility(argv[0]);
	config = new Configuration(fileUtility);
	config->read();

	for (int i = 0; i < argc; i++) {
		string arg = argv[i];

		if (arg.compare("--help") == 0) {
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
		fileUtility->setFullResPath(getMacBundlePath()+"/Contents/Resources");
#endif

		if (arg.compare("-r") == 0 && i + 1 < argc) {
			fileUtility->setFullResPath(argv[i + 1]);
		}

		if (arg.compare("-f") == 0)
			config->Screen.Full = true;

		if (arg.compare("-i") == 0)
			config->Screen.Full = false;

		if (arg.compare("-w") == 0 && i + 1 < argc)
			config->Screen.Width = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("-h") == 0 && i + 1 < argc)
			config->Screen.Height = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("--fps") == 0 && i + 1 < argc) {
			int lim = strtol(argv[i + 1], NULL, 10);
			config->FrameDelay = lim > 0 ? 1000 / lim : 0;
		}

		if (arg.compare("--showfps") == 0)
			config->ShowFps = true;

		if (arg.compare("--monsters") == 0 && i + 1 < argc) {
			int n = strtol(argv[i + 1], NULL, 10);
			if (n >= 0)
				config->MonstersAtStart = n;
			else
				cout << "Number of monsters must be positive." << endl;
		}
	}
}

int main(int argc, char *argv[]) {
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
