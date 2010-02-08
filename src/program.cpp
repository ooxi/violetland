#ifdef _WIN32
#pragma comment(lib, "SDL")
#pragma comment(lib, "SDLmain")
#pragma comment(lib, "SDL_image")
#pragma comment(lib, "SDL_ttf")
#pragma comment(lib, "SDL_mixer")
#pragma comment(lib, "opengl32")
#define _USE_MATH_DEFINES
#include <windows.h>
#include <winbase.h>
#include <time.h>
#endif //_WIN32
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "system/utility/ImageUtility.h"
#include "system/InputHandler.h"
#include "system/graphic/text/TextManager.h"
#include "system/utility/FileUtility.h"
#ifdef __APPLE__
#include "system/utility/macBundlePath.h"
#endif
#include "system/graphic/Texture.h"
#include "system/graphic/Aim.h"
#include "system/graphic/Camera.h"
#include "system/graphic/Particle.h"
#include "system/sound/SoundManager.h"
#include "system/graphic/Window.h"
#include "system/graphic/VideoManager.h"
#include "system/Configuration.h"
#include "system/GameState.h"
#include "game/MonsterFactory.h"
#include "game/Enemy.h"
#include "game/Player.h"
#include "game/Powerup.h"
#include "game/Terrain.h"
#include "game/MusicManager.h"
#include "game/WeaponManager.h"
#include "game/Highscores.h"
#include "game/Explosion.h"
#include "windows/MainMenuWindow.h"
#include "windows/HelpWindow.h"

const string PROJECT = "violetland";
const string VERSION = "0.2.8";

GameState* gameState;
Configuration* config;
VideoManager* videoManager;
Camera* cam;

int framesCount;
int fpsCountingStart;
int fps = 0;

MonsterFactory* monsterFactory;

Configuration* tempConfig;

Sound* playerKilledSound;
vector<Sound*> playerHitSounds;
int playerHitSndPlaying = 0;

Texture* medikitTex;
Texture* grenadeTex;
Texture* freezeTex;
StaticObject* crystal;

Sprite* playerLegsSprite;

Sprite* grenadeSprite;

vector<Texture*> bloodTex;
vector<Texture*> explTex;

Aim* aim;

map<string, LifeForm*> lifeForms;

Player* player;

vector<StaticObject*> bloodStains;

vector<Explosion*> explosions;

vector<ParticleSystem*> particleSystems;

vector<Sound*> explosionSounds;

WeaponManager* weaponManager;

vector<Powerup*> powerups;
vector<Bullet*> bullets;

map<string, Window*> windows;

vector<TextObject*> msgQueue;

int currentTime = 0;
int deltaTime = 0;

Terrain* terrain;

FileUtility* fileUtility;
InputHandler* input;
SoundManager* sndManager;
MusicManager* musicManager;

StaticObject* splash;

void clearWindows() {
	std::map<std::string, Window*>::const_iterator iter;
	for (iter = windows.begin(); iter != windows.end(); ++iter) {
		delete iter->second;
	}
	windows.clear();
}

void clearBloodStains() {
	for (unsigned int i = 0; i < bloodStains.size(); i++) {
		delete bloodStains[i];
	}
	bloodStains.clear();
}

void clearLifeForms() {
	map<string, LifeForm*>::const_iterator iter;
	for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
		delete iter->second;
	}
	lifeForms.clear();
}

void clearBullets() {
	for (unsigned int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	bullets.clear();
}

void clearMessages() {
	for (unsigned int i = 0; i < msgQueue.size(); i++) {
		delete msgQueue[i];
	}
	msgQueue.clear();
}

void clearExplosions() {
	for (unsigned int i = 0; i < explosions.size(); i++) {
		delete explosions[i];
	}
	explosions.clear();
}

void clearParticleSystems() {
	for (unsigned int i = 0; i < particleSystems.size(); i++) {
		delete particleSystems[i];
	}
	particleSystems.clear();
}

void clearPowerups() {
	for (unsigned int i = 0; i < powerups.size(); i++) {
		delete powerups[i];
	}
	powerups.clear();
}

void createTerrain() {
	if (terrain)
		delete terrain;

	printf("Forming terrain...\n");

	int baseTexCount = fileUtility->getFilesCountFromDir(
			fileUtility->getFullPath(FileUtility::image, "terrain"));
	int baseTex = (rand() % (baseTexCount * 100 - 1) / 100);

	string tilesDir = fileUtility->getFullPath(FileUtility::image, "terrain");
	tilesDir.append("/%i");
	char *buf;
	sprintf(buf = new char[2000], tilesDir.c_str(), baseTex);
	int tilesCount = fileUtility->getFilesCountFromDir(buf);
	delete[] buf;

	sprintf(buf = new char[100], "terrain/base_%i.png", baseTex);
	SDL_Surface *terrainSurface = ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, buf), 1);
	delete[] buf;

	vector<SDL_Surface*> tiles;
	for (int i = 0; i < tilesCount; i++) {
		sprintf(buf = new char[100], "terrain/%i/%i.png", baseTex, i);
		SDL_Surface *tile = ImageUtility::loadImage(fileUtility->getFullPath(
				FileUtility::image, buf), 1);
		tiles.push_back(tile);
		delete[] buf;
	}

	terrain = new Terrain(terrainSurface, tiles, config->GameAreaSize);

	SDL_FreeSurface(terrainSurface);
	for (int i = 0; i < tilesCount; i++) {
		SDL_FreeSurface(tiles[i]);
	}
	tiles.clear();
}

void spawnEnemy(float r, int lvl) {
	float spawnAngle = (rand() % 6300) / 1000.0;

	Enemy* newMonster = monsterFactory->create(player->Level, lvl);

	newMonster->X = r * cos(spawnAngle);
	newMonster->Y = r * sin(spawnAngle);

	lifeForms.insert(map<string, LifeForm*>::value_type(newMonster->Id,
			newMonster));
}

void startSurvival() {
	glClear(GL_COLOR_BUFFER_BIT);

	cam->X = cam->Y = 0.0f;

	cam->applyGLOrtho();

	splash->draw(false, false);

	videoManager->RegularText->draw("Please wait...", 0, 0,
			TextManager::CENTER, TextManager::MIDDLE);

	SDL_GL_SwapBuffers();

	gameState->startSurvival();

	clearBloodStains();
	clearPowerups();
	clearLifeForms();
	clearBullets();
	clearMessages();
	clearExplosions();

	player = new Player(0, 0, playerLegsSprite);
	player->setWeapon(weaponManager->getWeaponByName("PM"));
	player->HitR = 0.28f;
	player->Acceleration = 0.0004f;

	lifeForms.insert(map<string, LifeForm*>::value_type(player->Id, player));

	msgQueue.push_back(videoManager->RegularText->getObject(
			"Try to survive as long as you can.", 0, 0, TextManager::LEFT,
			TextManager::MIDDLE));
	msgQueue.push_back(videoManager->RegularText->getObject(
			"Shoot monsters to receive experience and other bonuses.", 0, 0,
			TextManager::LEFT, TextManager::MIDDLE));
	msgQueue.push_back(videoManager->RegularText->getObject(
			"Press F1 at any moment to get additional instructions.", 0, 0,
			TextManager::LEFT, TextManager::MIDDLE));

	createTerrain();

	SDL_ShowCursor(0);

	for (unsigned int i = 0; i < config->MonstersAtStart; i++) {
		spawnEnemy(cam->getW(), 1);
	}

	windows["mainmenu"]->CloseFlag = true;
}

char *getProjectTitle() {
	char *buf;
	sprintf(buf = new char[PROJECT.size() + VERSION.size() + 4], "%s v%s",
			PROJECT.c_str(), VERSION.c_str());
	return buf;
}

void printVersion() {
	char* pr = getProjectTitle();
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
	fprintf(stdout, "%s @ %s\n", pr, env.c_str());
	delete[] pr;
}

void initSystem() {
	srand((unsigned) time(NULL));

	TTF_Init();
	atexit(TTF_Quit);
	atexit(SDL_Quit);

	printf("SDL_Init...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	videoManager = new VideoManager(fileUtility);

	cam = new Camera();
	videoManager->setMode(config, cam);

	printf("Preparing window...\n");

	char* buf;
	buf = getProjectTitle();
	SDL_WM_SetCaption(buf, NULL);
	delete[] buf;

	SDL_Surface* icon = ImageUtility::loadImage(fileUtility->getFullPath(
			FileUtility::common, "icon-light.png"), 1.0f);
	SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	printf("Drawing splash screen...\n");

	sprintf(buf = new char[100], "splash_%i.png", (rand() % 199) / 100);
	Texture* tex = new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, buf)), GL_TEXTURE_2D,
			GL_LINEAR, true);
	delete[] buf;

	splash = new StaticObject(0, 0, tex->getWidth(), tex->getHeight(), tex,
			true);

	glClear(GL_COLOR_BUFFER_BIT);

	cam->X = cam->Y = 0.0f;

	cam->applyGLOrtho();

	splash->draw(false, false);

	videoManager->RegularText->draw("Please wait...", 0, 0,
			TextManager::CENTER, TextManager::MIDDLE);

	SDL_GL_SwapBuffers();

	printf("Preparing sound systems...\n");

	sndManager = new SoundManager(fileUtility, config);
	musicManager = new MusicManager(fileUtility, sndManager, config);

	musicManager->play();

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//flashlight
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.8f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0008f);

	//selflight
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001f);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	input = new InputHandler();

	gameState = new GameState();
}

void loseGame(Player* player) {
	gameState->Lost = true;
	if (playerHitSounds[playerHitSndPlaying]->isPlaying())
		playerHitSounds[playerHitSndPlaying]->stop(0);

	playerKilledSound->play(5, 0, 0);

	msgQueue.push_back(videoManager->RegularText->getObject("Player is dead.",
			0, 0, TextManager::LEFT, TextManager::BOTTOM));

	Highscores s(fileUtility);
	HighscoresEntry* h = new HighscoresEntry();
	h->Agility = player->Agility;
	h->Strength = player->Strength;
	h->Vitality = player->Vitality;
	h->Time = gameState->Time;
	h->Xp = player->Xp;
	s.add(h);

	SDL_ShowCursor(1);
}

void switchGamePause() {
	gameState->Paused = !gameState->Paused;
	if (gameState->Paused)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

void refreshCharStatsWindow() {
	const int l = config->ScreenWidth * 0.1f;
	const int r = config->ScreenWidth * 0.6f;

	Window* charStats = windows.find("charstats")->second;

	char *buf;
	sprintf(buf = new char[100], "Current player level: %i",
			(int) ((player->Level)));
	charStats->addElement("level", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 4.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "Available improvement points: %i",
			(int) ((player->LevelPoints)));
	charStats->addElement("availpoints", videoManager->RegularText->getObject(
			buf, l, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "Strength: %i",
			(int) ((player->Strength * 100)));
	charStats->addElement("strength", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Agility: %i", (int) ((player->Agility * 100)));
	charStats->addElement("agility", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Vitality: %i",
			(int) ((player->Vitality * 100)));
	charStats->addElement("vitality", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 9.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "HP: %i / Max HP: %i",
			(int) ((player->getHealth() * 100)), (int) ((player->MaxHealth()
					* 100)));
	charStats->addElement("hp", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 11.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Melee damage: %i", (int) ((player->Damage()
			* 100)));
	charStats->addElement("melee", videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 12.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Chance of block: %i%%",
			(int) ((player->ChanceToEvade() * 100)));
	charStats->addElement("chanceblock", videoManager->RegularText->getObject(
			buf, l, videoManager->RegularText->getHeight() * 13.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Reloading speed modifier: %i%%",
			(int) ((player->ReloadSpeedMod() * 100)));
	charStats->addElement("reloadingspeed",
			videoManager->RegularText->getObject(buf, l,
					videoManager->RegularText->getHeight() * 14.0f,
					TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Accuracy deviation modifier: %i%%",
			(int) ((player->WeaponRetForceMod() * 100)));
	charStats->addElement("accuracy", videoManager->RegularText->getObject(buf,
			l, videoManager->RegularText->getHeight() * 15.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Health regeneration: %.2f/min",
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
}

void increaseStrength() {
	if (player->LevelPoints > 0) {
		player->Strength += 0.1;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void increaseAgility() {
	if (player->LevelPoints > 0) {
		player->Agility += 0.1;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void increaseVitality() {
	if (player->LevelPoints > 0) {
		float h = player->getHealth() / player->MaxHealth();
		player->Vitality += 0.1;
		player->setHealth(h * player->MaxHealth());
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void takePoisonBullets() {
	if (!player->PoisonBullets && player->LevelPoints > 0) {
		player->PoisonBullets = true;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void takeUnstoppable() {
	if (!player->Unstoppable && player->LevelPoints > 0) {
		player->Unstoppable = true;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void takeBigCalibre() {
	if (!player->BigCalibre && player->LevelPoints > 0) {
		player->BigCalibre = true;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void takeTelekinesis() {
	if (!player->Telekinesis && player->LevelPoints > 0) {
		player->Telekinesis = true;
		player->LevelPoints--;
		refreshCharStatsWindow();
	}
}

void showDetailsUnstoppable() {
	windows["charstats"]->addElement(
			"explantation",
			videoManager->SmallText->getObject(
					"Unstoppable: enemies can't block your movement any more, but they still can hurt you.",
					config->ScreenWidth / 2,
					videoManager->RegularText->getHeight() * 1.0f,
					TextManager::CENTER, TextManager::MIDDLE));
}

void showDetailsPoisonBullets() {
	windows["charstats"]->addElement(
			"explantation",
			videoManager->SmallText->getObject(
					"Poison bullets: after getting hit by your bullet, enemies slowly lose health until they die.",
					config->ScreenWidth / 2,
					videoManager->RegularText->getHeight() * 1.0f,
					TextManager::CENTER, TextManager::MIDDLE));
}

void showDetailsBigCalibre() {
	windows["charstats"]->addElement(
			"explantation",
			videoManager->SmallText->getObject(
					"Big calibre: your bullets can wound a few monsters in a row.",
					config->ScreenWidth / 2,
					videoManager->RegularText->getHeight() * 1.0f,
					TextManager::CENTER, TextManager::MIDDLE));
}

void showDetailsTelekinesis() {
	windows["charstats"]->addElement("explantation",
			videoManager->SmallText->getObject(
					"Telekinesis: useful things slowly move towards you.",
					config->ScreenWidth / 2,
					videoManager->RegularText->getHeight() * 1.0f,
					TextManager::CENTER, TextManager::MIDDLE));
}

void createCharStatWindow() {
	Window *charStats = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int r = config->ScreenWidth * 0.6f;

	charStats->addElement("perks", videoManager->RegularText->getObject(
			"Perks:", r, videoManager->RegularText->getHeight() * 2.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("unstoppable", videoManager->RegularText->getObject(
			"Unstoppable", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 4.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	charStats->addElement("poisonbullets",
			videoManager->RegularText->getObject("Poison bullets", r
					+ videoManager->RegularText->getHeight() * 2.0f,
					videoManager->RegularText->getHeight() * 5.0f,
					TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("bigcalibre", videoManager->RegularText->getObject(
			"Big calibre", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 6.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	charStats->addElement("telekinesis", videoManager->RegularText->getObject(
			"Telekinesis", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 7.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	charStats->addElement("explantation", videoManager->SmallText->getObject(
			"Move mouse over text to get explantation.", config->ScreenWidth
					/ 2, videoManager->RegularText->getHeight() * 1.0f,
			TextManager::CENTER, TextManager::MIDDLE));

	charStats->addHandler(Window::hdl_lclick, "strength", increaseStrength);
	charStats->addHandler(Window::hdl_lclick, "agility", increaseAgility);
	charStats->addHandler(Window::hdl_lclick, "vitality", increaseVitality);

	charStats->addHandler(Window::hdl_lclick, "unstoppable", takeUnstoppable);
	charStats->addHandler(Window::hdl_move, "unstoppable",
			showDetailsUnstoppable);
	charStats->addHandler(Window::hdl_lclick, "poisonbullets",
			takePoisonBullets);
	charStats->addHandler(Window::hdl_move, "poisonbullets",
			showDetailsPoisonBullets);
	charStats->addHandler(Window::hdl_lclick, "bigcalibre", takeBigCalibre);
	charStats->addHandler(Window::hdl_move, "bigcalibre", showDetailsBigCalibre);
	charStats->addHandler(Window::hdl_lclick, "telekinesis", takeTelekinesis);
	charStats->addHandler(Window::hdl_move, "telekinesis",
			showDetailsTelekinesis);

	windows["charstats"] = charStats;
}

void shutdownSystem() {
	delete videoManager;
	delete gameState;
	delete splash;
	delete musicManager;
	delete sndManager;
	delete input;
	delete fileUtility;
}

void backFromHighScores();
void backFromOptionsAndSave();

void createHighscoresWindow() {
	Window *scoresWin = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;
	const int r2 = l * 2.0f;
	const int r3 = l * 4.0f;

	scoresWin->addElement("highscores", videoManager->RegularText->getObject(
			"Highscores", l, videoManager->RegularText->getHeight() * 3.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addElement("headerXp", videoManager->RegularText->getObject(
			"XP", l, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	scoresWin->addElement("headerParams", videoManager->RegularText->getObject(
			"Str/Agil/Vital", r2,
			videoManager->RegularText->getHeight() * 5.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	scoresWin->addElement("headerTime", videoManager->RegularText->getObject(
			"Time", r3, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	Highscores s(fileUtility);
	vector<HighscoresEntry*> highscores = s.getData();

	if (!highscores.empty())
		for (unsigned int i = 0; i < highscores.size(); i++) {
			char* label;
			char* line;
			sprintf(label = new char[30], "xp%i", i);
			sprintf(line = new char[30], "%i", highscores[i]->Xp);
			scoresWin->addElement(label, videoManager->RegularText->getObject(
					line, l, videoManager->RegularText->getHeight()
							* (6.0f + i), TextManager::LEFT,
					TextManager::MIDDLE));
			delete[] label;
			delete[] line;

			sprintf(label = new char[30], "params%i", i);
			sprintf(line = new char[30], "%i/%i/%i",
					(int) (highscores[i]->Strength * 100),
					(int) (highscores[i]->Agility * 100),
					(int) (highscores[i]->Vitality * 100));
			scoresWin->addElement(label, videoManager->RegularText->getObject(
					line, r2, videoManager->RegularText->getHeight() * (6.0f
							+ i), TextManager::LEFT, TextManager::MIDDLE));
			delete[] label;
			delete[] line;

			const int minutes = highscores[i]->Time / 60000;
			const int seconds = (highscores[i]->Time - minutes * 60000) / 1000;

			sprintf(label = new char[30], "time%i", i);
			sprintf(line = new char[30], "%im %is", minutes, seconds);
			scoresWin->addElement(label, videoManager->RegularText->getObject(
					line, r3, videoManager->RegularText->getHeight() * (6.0f
							+ i), TextManager::LEFT, TextManager::MIDDLE));
			delete[] label;
			delete[] line;
		}

	scoresWin->addElement("back", videoManager->RegularText->getObject(
			"Back to main menu", l, videoManager->RegularText->getHeight()
					* 18.0f, TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addHandler(Window::hdl_lclick, "back", backFromHighScores);

	windows["highscores"] = scoresWin;
}

void refreshOptionsWindow() {
	const int l = config->ScreenWidth * 0.1f;
	const int r = config->ScreenWidth * 0.6f;

	Window* w = windows.find("options")->second;

	if (config->AutoReload)
		w->addElement("+autoreload", videoManager->RegularText->getObject("+",
				l, videoManager->RegularText->getHeight() * 7.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autoreload", false);

	if (config->AutoWeaponPickup)
		w->addElement("+autopickup", videoManager->RegularText->getObject("+",
				l, videoManager->RegularText->getHeight() * 8.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autopickup", false);

	if (config->FriendlyFire)
		w->addElement("+friendlyfire", videoManager->RegularText->getObject(
				"+", l, videoManager->RegularText->getHeight() * 9.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+friendlyfire", false);

	if (config->FullScreen)
		w->addElement("+fullscreen", videoManager->RegularText->getObject("+",
				r, videoManager->RegularText->getHeight() * 7.0f,
				TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+fullscreen", false);

	char *buf;
	sprintf(buf = new char[15], "%ix%i", tempConfig->ScreenWidth,
			tempConfig->ScreenHeight);
	TextObject* resInfo = videoManager->RegularText->getObject(buf, r
			+ videoManager->RegularText->getHeight() * 7.0f,
			videoManager->RegularText->getHeight() * 8.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	delete[] buf;
	w->addElement("+resolution", resInfo);

	float snd = config->SoundVolume * 10;
	sprintf(buf = new char[30], "%.0f%%", snd);
	TextObject* sndInd = videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 13.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	delete[] buf;
	w->addElement("+soundvolume", sndInd);

	float mus = config->MusicVolume * 10;
	sprintf(buf = new char[30], "%.0f%%", mus);
	TextObject * musInd = videoManager->RegularText->getObject(buf, l,
			videoManager->RegularText->getHeight() * 14.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	delete[] buf;
	w->addElement("+musicvolume", musInd);
}

void switchAutoReload() {
	config->AutoReload = !config->AutoReload;
	refreshOptionsWindow();
}

void switchSoundVolumeDown() {
	if (config->SoundVolume > 0) {
		config->SoundVolume--;
		for (unsigned int a = 1; a <= 8; a++) {
			Mix_Volume(a, config->SoundVolume * 12);
		}
	} else {
		config->SoundVolume = 10;
		for (unsigned int a = 1; a <= 8; a++) {
			Mix_Volume(a, 0);
		}
	}
	refreshOptionsWindow();
}

void switchSoundVolumeUp() {
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
	refreshOptionsWindow();
}

void switchMusicVolumeDown() {
	if (config->MusicVolume > 0) {
		config->MusicVolume--;
		Mix_Volume(0, config->MusicVolume * 12);
	} else {
		config->MusicVolume = 10;
		Mix_Volume(0, 0);
	}
	refreshOptionsWindow();
}

void switchMusicVolumeUp() {
	if (config->MusicVolume <= 9) {
		config->MusicVolume++;
		Mix_Volume(0, config->MusicVolume * 12);
	} else {
		config->MusicVolume = 0;
		Mix_Volume(0, 0);
	}
	refreshOptionsWindow();
}

void switchFriendlyFire() {
	config->FriendlyFire = !config->FriendlyFire;
	refreshOptionsWindow();
}

void switchAutoPickup() {
	config->AutoWeaponPickup = !config->AutoWeaponPickup;
	refreshOptionsWindow();
}

void switchFullScreen() {
	config->FullScreen = !config->FullScreen;
	refreshOptionsWindow();
}

void switchResolutionDown() {
	vector<SDL_Rect> modes = videoManager->GetAvailableModes(config);

	bool set = false;
	for (int i = modes.size() - 1; i > 0; i--) {
		if (tempConfig->ScreenWidth == modes[i].w && tempConfig->ScreenHeight
				== modes[i].h) {
			tempConfig->ScreenWidth = modes[i - 1].w;
			tempConfig->ScreenHeight = modes[i - 1].h;
			set = true;
			break;
		}
	}
	if (!set) {
		tempConfig->ScreenWidth = modes[modes.size() - 1].w;
		tempConfig->ScreenHeight = modes[modes.size() - 1].h;
	}

	refreshOptionsWindow();
}

void switchResolutionUp() {
	vector<SDL_Rect> modes = videoManager->GetAvailableModes(config);

	bool set = false;
	for (unsigned int i = 0; i < modes.size() - 1; i++) {
		if (tempConfig->ScreenWidth == modes[i].w && tempConfig->ScreenHeight
				== modes[i].h) {
			tempConfig->ScreenWidth = modes[i + 1].w;
			tempConfig->ScreenHeight = modes[i + 1].h;
			set = true;
			break;
		}
	}
	if (!set) {
		tempConfig->ScreenWidth = modes[0].w;
		tempConfig->ScreenHeight = modes[0].h;
	}

	refreshOptionsWindow();
}

void createOptionsWindow() {
	tempConfig = new Configuration(*config);

	Window *w = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;
	const int r = config->ScreenWidth * 0.6f;

	w->addElement("options", videoManager->RegularText->getObject("Options", l,
			videoManager->RegularText->getHeight() * 3.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("sectiongame", videoManager->RegularText->getObject(
			"Gameplay", l, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("autoreload", videoManager->RegularText->getObject(
			"Weapon autoreloading", l + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	w->addElement("autopickup", videoManager->RegularText->getObject(
			"Weapon autotaking", l + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	w->addElement("friendlyfire", videoManager->RegularText->getObject(
			"Friendly fire", l + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 9.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("sectiongraphics", videoManager->RegularText->getObject(
			"Graphics", r, videoManager->RegularText->getHeight() * 5.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("fullscreen", videoManager->RegularText->getObject(
			"Fullscreen", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 7.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	w->addElement("resolution", videoManager->RegularText->getObject(
			"Resolution", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 8.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addElement("sectionsound", videoManager->RegularText->getObject("Sound",
			l, videoManager->RegularText->getHeight() * 11.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("soundvolume", videoManager->RegularText->getObject(
			"Sound volume", l + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 13.0f, TextManager::LEFT,
			TextManager::MIDDLE));
	w->addElement("musicvolume", videoManager->RegularText->getObject(
			"Music volume", l + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 14.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	w->addHandler(Window::hdl_lclick, "autoreload", switchAutoReload);
	w->addHandler(Window::hdl_lclick, "autopickup", switchAutoPickup);
	w->addHandler(Window::hdl_lclick, "friendlyfire", switchFriendlyFire);
	w->addHandler(Window::hdl_lclick, "soundvolume", switchSoundVolumeUp);
	w->addHandler(Window::hdl_rclick, "soundvolume", switchSoundVolumeDown);
	w->addHandler(Window::hdl_lclick, "musicvolume", switchMusicVolumeUp);
	w->addHandler(Window::hdl_rclick, "musicvolume", switchMusicVolumeDown);
	w->addHandler(Window::hdl_lclick, "fullscreen", switchFullScreen);
	w->addHandler(Window::hdl_lclick, "resolution", switchResolutionUp);
	w->addHandler(Window::hdl_rclick, "resolution", switchResolutionDown);

	w->addElement("savereturn", videoManager->RegularText->getObject(
			"Save and return", l, videoManager->RegularText->getHeight()
					* 17.0f, TextManager::LEFT, TextManager::MIDDLE));
	w->addHandler(Window::hdl_lclick, "savereturn", backFromOptionsAndSave);

	windows["options"] = w;

	refreshOptionsWindow();
}

void showHighScores() {
	windows["mainmenu"]->CloseFlag = true;
	createHighscoresWindow();
}

void showOptions() {
	windows["mainmenu"]->CloseFlag = true;
	createOptionsWindow();
}

void resumeGame() {
	Window* w = windows.find("mainmenu")->second;
	w->CloseFlag = true;
	switchGamePause();
}

void endGame() {
	gameState->Works = false;
}

void createMainMenuWindow() {
	Window *mainMenu = new MainMenuWindow(config, gameState,
			videoManager->RegularText);

	mainMenu->addHandler(Window::hdl_lclick, "resume", resumeGame);
	mainMenu->addHandler(Window::hdl_lclick, "survival", startSurvival);
	mainMenu->addHandler(Window::hdl_lclick, "options", showOptions);
	mainMenu->addHandler(Window::hdl_lclick, "highscores", showHighScores);
	mainMenu->addHandler(Window::hdl_lclick, "exit", endGame);

	windows["mainmenu"] = mainMenu;
}

void unloadResources() {
	delete crystal;
	delete weaponManager;
	delete monsterFactory;
	delete playerKilledSound;
	delete aim;
	delete playerLegsSprite;
	delete grenadeSprite;
	delete terrain;
	delete medikitTex;
	for (unsigned int i = 0; i < bloodTex.size(); i++) {
		delete bloodTex[i];
	}
	bloodTex.clear();
	clearBloodStains();
	clearPowerups();
	clearLifeForms();
	clearBullets();
	clearMessages();
	clearWindows();
	clearExplosions();
	clearParticleSystems();

	for (unsigned int i = 0; i < playerHitSounds.size(); i++) {
		delete playerHitSounds[i];
	}
	playerHitSounds.clear();
	delete config;
}

void backFromOptionsAndSave() {
	bool changeVideoMode = config->ScreenWidth != tempConfig->ScreenWidth
			|| config->ScreenHeight != tempConfig->ScreenHeight;

	config->ScreenWidth = tempConfig->ScreenWidth;
	config->ScreenHeight = tempConfig->ScreenHeight;
	config->write();

	if (changeVideoMode) {
#ifdef _WIN32
		fprintf(stdout,"Hot video mode changing is not supported on windows now. You should restart the game.");
		unloadResources();
		shutdownSystem();
		exit(0);
#endif //_WIN32
		videoManager->setMode(config, cam);
	}
	windows["options"]->CloseFlag = true;
	createMainMenuWindow();
}

void backFromHighScores() {
	windows["highscores"]->CloseFlag = true;
	createMainMenuWindow();
}

void createHelpWindow() {
	Window *help = new HelpWindow(config, videoManager->RegularText);
	windows["helpscreen"] = help;
}

void handleCommonControls() {
	if (input->getPressInput(InputHandler::ShowChar)) {
		if (gameState->Begun && !gameState->Lost && windows.count("charstats")
				== 0) {
			clearWindows();

			createCharStatWindow();
			refreshCharStatsWindow();

			if (!gameState->Paused)
				switchGamePause();
		} else {
			Window* w = windows.find("charstats")->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Help)) {
		if (windows.count("helpscreen") == 0) {
			clearWindows();

			createHelpWindow();

			if (!gameState->Paused)
				switchGamePause();
		} else {
			Window* w = windows.find("helpscreen")->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Menu)) {
		if (windows.count("mainmenu") == 0) {
			clearWindows();

			createMainMenuWindow();

			if (!gameState->Paused)
				switchGamePause();
		} else if (gameState->Begun) {
			Window* w = windows.find("mainmenu")->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Exit)) {
		gameState->Works = false;
	}
}

void handleExplosions() {
	if (!explosions.empty()) {
		for (int i = explosions.size() - 1; i >= 0; i--) {
			explosions[i]->process(deltaTime);

			if (explosions[i]->Active && !lifeForms.empty()) {
				map<string, LifeForm*>::const_iterator iter;
				for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
					LifeForm* lf = iter->second;
					if (lf->Type == LifeForm::player && !config->FriendlyFire)
						continue;

					float d = explosions[i]->calcDamage(lf);
					if (d > 0) {
						lf->setHealth(lf->getHealth() - d);
					}
				}
			}

			explosions[i]->Active = false;

			if (explosions[i]->isEmpty()) {
				delete explosions[i];
				explosions.erase(explosions.begin() + i);
			}
		}
	}
}

void handleParticles() {
	if (!particleSystems.empty()) {
		for (int i = particleSystems.size() - 1; i >= 0; i--) {
			particleSystems[i]->process(deltaTime);

			if (particleSystems[i]->isEmpty()) {
				delete particleSystems[i];
				particleSystems.erase(particleSystems.begin() + i);
			}
		}
	}
}

void addBloodStain(float x, float y, float angle, float scale, bool poisoned) {
	StaticObject *newBloodStain = new StaticObject(x, y, 128, 128,
			bloodTex[(rand() % 299) / 100], false);

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

void handleMonster(LifeForm* lf) {
	float x = lf->X;
	float y = lf->Y;

	Enemy* enemy = (Enemy*) lf;

	if (!gameState->Lost && enemy->detectCollide(player->TargetX,
			player->TargetY)) {
		char *buf;
		sprintf(buf = new char[255], "%s (%i)", enemy->Name.c_str(),
				enemy->Level);
		player->HudInfo = buf;
		delete[] buf;
	}

	if (lf->Frozen > 0)
		return;

	if (enemy->isBleeding() && bloodStains.size() < 12) {
		addBloodStain(enemy->X, enemy->Y, enemy->Angle, (rand() % 10) / 50.0f
				+ 0.1f, enemy->Poisoned);
	}

	float rangeToPlayer = sqrt(pow(-enemy->X + player->X, 2) + pow(enemy->Y
			- player->Y, 2));

	if (enemy->DoNotDisturb) {
		bool reach = true;
		if (enemy->X < enemy->TargetX - enemy->Speed * 60 || enemy->X
				> enemy->TargetX + enemy->Speed * 60 || enemy->Y
				< enemy->TargetY - enemy->Speed * 60 || enemy->Y
				> enemy->TargetY + enemy->Speed * 60)
			reach = false;
		if (reach)
			enemy->DoNotDisturb = false;
	}

	if ((rangeToPlayer < 400 || enemy->Angry) && !gameState->Lost) {
		enemy->TargetX = player->X;
		enemy->TargetY = player->Y;
	} else if (rangeToPlayer < 800 && !gameState->Lost) {
		enemy->TargetX = player->X - cos((player->getLegsAngle() + 90) * M_PI
				/ 180) * rangeToPlayer / 2.0f / enemy->Speed * player->Speed;
		enemy->TargetY = player->Y - sin((player->getLegsAngle() + 90) * M_PI
				/ 180) * rangeToPlayer / 2.0f / enemy->Speed * player->Speed;
	} else if (!enemy->DoNotDisturb) {
		enemy->TargetX = (rand() % (config->GameAreaSize * 2))
				- config->GameAreaSize;
		enemy->TargetY = (rand() % (config->GameAreaSize * 2))
				- config->GameAreaSize;
		enemy->DoNotDisturb = true;
	}

	float newAngle = Object::calculateAngle(enemy->X, enemy->Y, enemy->TargetX,
			enemy->TargetY);
	enemy->turn(newAngle, enemy->MaxSpeed(), deltaTime);

	enemy->move(deltaTime);

	if (!gameState->Lost && player->detectCollide(enemy)) {
		if (enemy->Attack()) {
			if (rand() % 100 > player->ChanceToEvade() * 100) {
				player->setHealth(player->getHealth() - enemy->Damage());
				if (!playerHitSounds[playerHitSndPlaying]->isPlaying()) {
					playerHitSndPlaying = (player->getHealth()
							< player->MaxHealth() ? player->getHealth()
							: player->getHealth() - 0.01f)
							/ player->MaxHealth() * playerHitSounds.size();
					playerHitSounds[playerHitSndPlaying]->play(6, 0, 0);
				}
			}

			if (!player->Unstoppable)
				player->Speed = 0.0f;
		}

		if (player->Attack() && rand() % 100 > enemy->ChanceToEvade() * 100)
			enemy->setHealth(player->getHealth() - player->Damage());

		enemy->X = x;
		enemy->Y = y;
	} else {
		enemy->rollFrame(true);
	}
}

void levelUp(Player* player) {
	spawnEnemy(config->GameAreaSize * 1.5f, player->Level * 2.0f + 10);

	player->NextLevelXp *= 2;

	player->Level += 1;
	player->LevelPoints += 1;

	msgQueue.push_back(videoManager->RegularText->getObject(
			"The player has reached new level.", 0, 0, TextManager::LEFT,
			TextManager::MIDDLE));

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

	player->move(movementX, movementY, deltaTime);

	if (lf->X < -config->GameAreaSize)
		player->setX(-config->GameAreaSize);
	if (lf->X > config->GameAreaSize)
		player->setX(config->GameAreaSize);
	if (lf->Y < -config->GameAreaSize)
		player->setY(-config->GameAreaSize);
	if (lf->Y > config->GameAreaSize)
		player->setY(config->GameAreaSize);

	lf->TargetX = input->mouseX / videoManager->WK - cam->getHalfW() + cam->X;
	lf->TargetY = input->mouseY / videoManager->HK - cam->getHalfH() + cam->Y;

	if (input->getDownInput(InputHandler::Fire)) {
		std::vector<Bullet*> *newBullets = player->fire();
		if (!newBullets->empty()) {
			bullets.insert(bullets.end(), newBullets->begin(),
					newBullets->end());
			delete newBullets;
		}
		if (player->getWeapon()->Ammo == 0 && config->AutoReload)
			player->reload();
	}

	if (input->getPressInput(InputHandler::ToggleLight))
		player->toggleLight();

	if (input->getPressInput(InputHandler::ToggleLaser))
		player->toggleLaser();

	if (input->getDownInput(InputHandler::Reload))
		player->reload();

	if (input->getPressInput(InputHandler::ThrowGrenade) && player->Grenades
			> 0) {
		bullets.push_back(player->throwGrenade(grenadeSprite));
	}
}

void dropPowerup(float x, float y) {
	bool powerupDropped = false;
	Powerup *newPowerup;

	if (!powerupDropped && rand() % 1000 >= 950) {
		newPowerup = new Powerup(x, y, medikitTex);
		newPowerup->Scale = 0.3f;
		newPowerup->Type = Powerup::medikit;
		newPowerup->Object = new float(0.1f);
		newPowerup->RMask = newPowerup->BMask = 0.2f;
		powerupDropped = true;
	}

	if (!powerupDropped && rand() % 1000 >= 975) {
		newPowerup = new Powerup(x, y, medikitTex);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = Powerup::medikit;
		newPowerup->Object = new float(0.2f);
		newPowerup->RMask = newPowerup->GMask = 0.4f;
		powerupDropped = true;
	}

	if (!powerupDropped && rand() % 1000 >= 990) {
		newPowerup = new Powerup(x, y, medikitTex);
		newPowerup->Scale = 0.5f;
		newPowerup->Type = Powerup::medikit;
		newPowerup->Object = new float(0.6f);
		newPowerup->BMask = newPowerup->GMask = 0.2f;
		powerupDropped = true;
	}

	if (!powerupDropped && rand() % 1000 >= 970) {
		newPowerup = new Powerup(x, y, grenadeTex);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = Powerup::grenades;
		newPowerup->Object = new int(1);
		powerupDropped = true;
	}

	if (!powerupDropped && rand() % 1000 >= 970) {
		newPowerup = new Powerup(x, y, freezeTex);
		newPowerup->Scale = 0.4f;
		newPowerup->Type = Powerup::freeze;
		newPowerup->Object = new int(10000);
		powerupDropped = true;
	}

	if (rand() % 1000 >= 975 || player->Kills == 0) {
		int weaponIndex = (rand() % (weaponManager->Weapons.size() - 1)) + 1;
		newPowerup = new Powerup(x, y,
				weaponManager->Weapons[weaponIndex]->getDroppedTex());
		newPowerup->Type = Powerup::weapon;
		newPowerup->Object = weaponManager->Weapons[weaponIndex];
		newPowerup->HitR = 0.5f;
		powerupDropped = true;
	}

	if (powerupDropped)
		powerups.push_back(newPowerup);
}

void handleLifeForms() {
	if (!gameState->Lost) {
		for (int i = 0; i < deltaTime; i++) {
			if (rand() % 10000 > gameState->Hardness) {
				int lvl = player->Level * 0.5f + player->Level * pow((rand()
						% 100) / 125.0f, 2);
				if (lvl < 1)
					lvl = 1;
				spawnEnemy(config->GameAreaSize * 1.5, lvl);
			}
		}
	}

	if (!lifeForms.empty()) {
		map<string, LifeForm*>::const_iterator iter;
		for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
			LifeForm* lf = iter->second;

			lf->process(deltaTime);

			if (lf->Type == LifeForm::player) {
				if (!gameState->Lost) {
					if (lf->isDead())
						loseGame(player);

					handlePlayer(lf);
				}
			}

			if (lf->Type == LifeForm::monster) {
				if (!lf->isDead())
					handleMonster(lf);
			}
		}
	}
}

void handleBullets() {
	if (!bullets.empty()) {
		for (int i = bullets.size() - 1; i >= 0; i--) {
			bullets[i]->process(deltaTime);

			if (bullets[i]->isActive() && !lifeForms.empty()) {
				map<string, LifeForm*>::const_iterator iter;
				for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
					LifeForm* lf = iter->second;

					if (lf->Type == LifeForm::player || lf->isDead())
						continue;

					Enemy* enemy = (Enemy*) lf;

					if (bullets[i]->checkHit(enemy)) {
						if (bloodStains.size() < 9) {
							for (int k = 0; k < 3; k++) {
								int angleDev = 90 + (rand() % 60) - 30;
								float distance = (rand() % 100);
								float bX = enemy->X - cos((bullets[i]->Angle
										+ angleDev) * M_PI / 180.0f) * distance;
								float bY = enemy->Y - sin((bullets[i]->Angle
										+ angleDev) * M_PI / 180.0f) * distance;

								addBloodStain(bX, bY, enemy->Angle,
										enemy->Scale * 0.5f, enemy->Poisoned);
							}
						}

						if (enemy->Frozen > 0) {
							ParticleSystem* partSys = new ParticleSystem();
							for (int k = 0; k < 6; k++) {
								Particle* p = new Particle(enemy->X + (rand()
										% 50) - 25, enemy->Y + (rand() % 50)
										- 25, 128, 128, crystal->getTexture());
								p->RMask = p->GMask = p->BMask = 1.0f;
								p->AMask = 1.0f;
								p->Scale = (rand() % 50) / 100.0f
										* enemy->Scale;
								p->XSpeed = ((rand() % 400) - 200) / 1000.0f;
								p->YSpeed = ((rand() % 400) - 200) / 1000.0f;
								p->AMod = -0.002;
								partSys->Particles.push_back(p);
							}
							particleSystems.push_back(partSys);
						} else {
							ParticleSystem* partSys = new ParticleSystem();
							for (int k = 0; k < 25; k++) {
								Particle* p = new Particle(enemy->X + (rand()
										% 50) - 25, enemy->Y + (rand() % 50)
										- 25, 128, 128, bloodTex[(rand() % 299)
										/ 100]);
								p->RMask = p->GMask = p->BMask = 0.0f;
								if (enemy->Poisoned)
									p->GMask = 1.0f - (rand() % 200) / 1000.0f;
								else
									p->RMask = 1.0f - (rand() % 200) / 1000.0f;
								p->AMask = 1.0f;
								p->Scale = (rand() % 15) / 100.0f
										* enemy->Scale;
								int angleDev = 90 + (rand() % 60) - 30;
								p->XSpeed = -cos((bullets[i]->Angle + angleDev)
										* M_PI / 180.0f) / 3.0f;
								p->YSpeed = -sin((bullets[i]->Angle + angleDev)
										* M_PI / 180.0f) / 3.0f;
								p->AMod = -0.004;
								partSys->Particles.push_back(p);
							}
							particleSystems.push_back(partSys);
						}

						bool bypassDirectDamage = false;
						if (bullets[i]->Type == Bullet::standard) {
							if (((StandardBullet*) bullets[i])->isExplosive()) {
								bullets[i]->deactivate();
								Explosion* expl = new Explosion(bullets[i]->X,
										bullets[i]->Y, 100.0f, explTex[0],
										explTex[1], explosionSounds[1]);
								expl->Damage = bullets[i]->Damage;
								explosions.push_back(expl);
								bypassDirectDamage = true;
							}
						}

						if (!bypassDirectDamage) {
							float damageLoss = enemy->getHealth();
							enemy->hit(bullets[i], player->X, player->Y);

							if (bullets[i]->BigCalibre) {
								bullets[i]->Damage -= damageLoss;
								if (bullets[i]->Damage <= 0) {
									bullets[i]->deactivate();
								}
							}
						}
					}
				}
			}

			if (bullets[i]->isReadyToRemove() && bullets[i]->Type
					== Bullet::grenade) {
				Explosion* expl = new Explosion(bullets[i]->X, bullets[i]->Y,
						150.0f, explTex[0], explTex[1], explosionSounds[0]);
				expl->Damage = bullets[i]->Damage;
				explosions.push_back(expl);
			}

			if (bullets[i]->isReadyToRemove()) {
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
			}
		}
	}
}

void drawMessagesQueue() {
	if (!msgQueue.empty()) {
		int s = msgQueue.size();
		for (int i = s - 1; i >= 0; i--) {
			msgQueue[i]->draw(true, msgQueue[i]->X
					+ videoManager->RegularText->getIndent(),
					config->ScreenHeight - s
							* videoManager->RegularText->getHeight() + i
							* videoManager->RegularText->getHeight());
			msgQueue[i]->AMask -= 0.0001f * deltaTime;

			if (msgQueue[i]->AMask <= 0) {
				delete msgQueue[i];
				msgQueue.erase(msgQueue.begin() + i);
			}
		}
	}
}

void setGuiCameraMode() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, config->ScreenWidth, config->ScreenHeight, 0.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawHud() {
	const int minutes = gameState->Time / 60000;
	const int seconds = (gameState->Time - minutes * 60000) / 1000;

	char *buf;

	float health = player->getHealth() / player->MaxHealth() * 100.0f;
	sprintf(buf = new char[30], "Health: %.2f%%", health);
	TextObject* healthMsg = videoManager->RegularText->getObject(buf,
			videoManager->RegularText->getIndent(),
			videoManager->RegularText->getIndent(), TextManager::LEFT,
			TextManager::TOP);
	delete[] buf;
	if (health < 34)
		healthMsg->GMask = healthMsg->BMask = 0.0f;
	healthMsg->draw(true, healthMsg->X, healthMsg->Y);
	delete healthMsg;

	sprintf(buf = new char[30], "%s: %d/%d", player->getWeapon()->Name.c_str(),
			player->getWeapon()->Ammo, player->getWeapon()->AmmoClipSize);
	videoManager->RegularText->draw(buf,
			videoManager->RegularText->getIndent(),
			videoManager->RegularText->getIndent()
					+ videoManager->RegularText->getHeight(),
			TextManager::LEFT, TextManager::TOP);
	delete[] buf;

	sprintf(buf = new char[30], "Grenades: %i", player->Grenades);
	videoManager->RegularText->draw(buf,
			videoManager->RegularText->getIndent(),
			videoManager->RegularText->getIndent()
					+ videoManager->RegularText->getHeight() * 2.0f,
			TextManager::LEFT, TextManager::TOP);
	delete[] buf;

	sprintf(buf = new char[30], "Time: %dm %ds", minutes, seconds);
	videoManager->RegularText->draw(buf, config->ScreenWidth
			- videoManager->RegularText->getIndent(),
			videoManager->RegularText->getIndent(), TextManager::RIGHT,
			TextManager::TOP);
	delete[] buf;

	sprintf(buf = new char[30], "Xp: %d (%d)", player->Xp, player->NextLevelXp);
	videoManager->RegularText->draw(buf, config->ScreenWidth
			- videoManager->RegularText->getIndent(),
			videoManager->RegularText->getIndent()
					+ videoManager->RegularText->getHeight(),
			TextManager::RIGHT, TextManager::TOP);
	delete[] buf;

	if (!gameState->Lost)
		if (player->HudInfo != "")
			videoManager->RegularText->draw(player->HudInfo.c_str(),
					config->ScreenWidth / 2,
					videoManager->RegularText->getIndent(),
					TextManager::CENTER, TextManager::TOP);

	if (config->ShowFps) {
		sprintf(buf = new char[30], "FPS: %i", fps);
		videoManager->RegularText->draw(buf, config->ScreenWidth
				- videoManager->RegularText->getIndent(), config->ScreenHeight
				- videoManager->RegularText->getIndent(), TextManager::RIGHT,
				TextManager::BOTTOM);
		delete[] buf;
	}

	if (gameState->Lost && !gameState->Paused)
		videoManager->RegularText->draw("They have overcome...",
				config->ScreenWidth / 2, config->ScreenHeight / 2,
				TextManager::CENTER, TextManager::MIDDLE);

	if (gameState->Paused)
		videoManager->RegularText->draw("PAUSE", config->ScreenWidth / 2,
				config->ScreenHeight / 2, TextManager::CENTER,
				TextManager::MIDDLE);

	drawMessagesQueue();
}

void handlePowerups() {
	for (int i = powerups.size() - 1; i >= 0; i--) {
		bool deletePowerup = false;
		powerups[i]->Time -= deltaTime;
		powerups[i]->AMask = powerups[i]->Time / 15000.0;
		if (powerups[i]->Type == Powerup::medikit || powerups[i]->Type
				== Powerup::grenades || powerups[i]->Type == Powerup::freeze) {

			powerups[i]->Angle += deltaTime * 0.05f * powerups[i]->Dir;

			if (powerups[i]->Angle > 45 && powerups[i]->Dir > 0)
				powerups[i]->Dir = -1;

			if (powerups[i]->Angle < -45 && powerups[i]->Dir < 0)
				powerups[i]->Dir = 1;
		}

		if (powerups[i]->Time <= 0)
			deletePowerup = true;

		if (!gameState->Lost && powerups[i]->detectCollide(player->TargetX,
				player->TargetY)) {

			switch (powerups[i]->Type) {
			case Powerup::medikit:
				player->HudInfo = "a medikit";
				break;
			case Powerup::freeze:
				player->HudInfo = "a nitrogen bomb";
				break;
			case Powerup::grenades:
				player->HudInfo = "a hand grenade";
				break;
			case Powerup::weapon:
				char *buf;
				sprintf(buf = new char[200], "the %s",
						((Weapon*) powerups[i]->Object)->Name.c_str());
				player->HudInfo = buf;
				delete[] buf;
				break;
			}

			if (player->Telekinesis) {
				float a = Object::calculateAngle(powerups[i]->X,
						powerups[i]->Y, player->X, player->Y);
				powerups[i]->X -= cos((a + 90) * M_PI / 180) * deltaTime
						* player->MaxSpeed();
				powerups[i]->Y -= sin((a + 90) * M_PI / 180) * deltaTime
						* player->MaxSpeed();
			}
		}

		if (!gameState->Lost && (powerups[i]->detectCollide(player))) {
			switch (powerups[i]->Type) {
			case Powerup::medikit: {
				msgQueue.push_back(videoManager->RegularText->getObject(
						"You took a medical kit.", 0, 0, TextManager::LEFT,
						TextManager::MIDDLE));
				player->setHealth(player->getHealth()
						+ *(float*) powerups[i]->Object);
				deletePowerup = true;
				break;
			}
			case Powerup::freeze: {
				msgQueue.push_back(videoManager->RegularText->getObject(
						"All has frozen around you.", 0, 0, TextManager::LEFT,
						TextManager::MIDDLE));

				map<string, LifeForm*>::const_iterator iter;
				for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
					LifeForm* lf = iter->second;

					if (lf->Type == LifeForm::player || lf->isDead())
						continue;
					lf->Frozen = *(int*) powerups[i]->Object;
				}

				deletePowerup = true;
				break;
			}
			case Powerup::grenades: {
				msgQueue.push_back(videoManager->RegularText->getObject(
						"You took a grenade.", 0, 0, TextManager::LEFT,
						TextManager::MIDDLE));
				player->Grenades += *(int*) powerups[i]->Object;
				deletePowerup = true;
				break;
			}
			case Powerup::weapon: {
				if (input->getDownInput(InputHandler::Pickup)
						|| config->AutoWeaponPickup) {
					player->setWeapon((Weapon*) powerups[i]->Object);
					char *buf;
					sprintf(buf = new char[200], "You took the %s.",
							player->getWeapon()->Name.c_str());
					msgQueue.push_back(videoManager->RegularText->getObject(
							buf, 0, 0, TextManager::LEFT, TextManager::MIDDLE));
					delete[] buf;
					deletePowerup = true;
					break;
				}
			}
			}
		}

		if (deletePowerup) {
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			continue;
		}
	}
}

void processGame() {
	if (!gameState->Lost) {
		gameState->Hardness -= deltaTime * 0.00012;
		gameState->Time += deltaTime;
	}

	terrain->beginDrawOn();
	{
		for (unsigned int i = 0; i < bloodStains.size(); i++) {
			terrain->drawOn(bloodStains[i]);
		}

		clearBloodStains();

		map<string, LifeForm*>::const_iterator iter;
		for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
			LifeForm* lf = iter->second;

			if (lf->Type == LifeForm::monster) {
				Enemy* enemy = (Enemy*) lf;
				if (enemy->isReasyToDisappear()) {
					terrain ->drawOn(enemy->getCorpse());
					dropPowerup(lf->X, lf->Y);
					player->Kills++;
					player->Xp += (int) ((1.5 - gameState->TimeOfDay * -0.5)
							* (lf->Strength + lf->Agility + lf->Vitality) * 3);
					delete lf;
					lifeForms.erase(iter->first);
				}
			}
		}
	}
	terrain->endDrawOn();

	player->HudInfo = "";

	handleLifeForms();
	handlePowerups();
	handleBullets();
	handleExplosions();
	handleParticles();
}

void drawGame() {
	cam->X = player->X;
	cam->Y = player->Y;

	if (cam->X < -config->GameAreaSize + cam->getHalfW())
		cam->X = -config->GameAreaSize + cam->getHalfW();
	if (cam->X > config->GameAreaSize - cam->getHalfW())
		cam->X = config->GameAreaSize - cam->getHalfW();
	if (cam->Y < -config->GameAreaSize + cam->getHalfH())
		cam->Y = -config->GameAreaSize + cam->getHalfH();
	if (cam->Y > config->GameAreaSize - cam->getHalfH())
		cam->Y = config->GameAreaSize - cam->getHalfH();

	cam->applyGLOrtho();

	glEnable(GL_LIGHTING);

	gameState->TimeOfDay = abs(cos((gameState->Time + 45000) / 180000.0));

	float gawc = gameState->TimeOfDay;
	float v = cos((gameState->Time + 45000) / 45000.0);
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

	if (!gameState->Lost && player->getLight()) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);

		GLfloat light_pos[] = { 0.0, 0.0, 1.0, 1.0 };

		glPushMatrix();
		glTranslatef(player->X, player->Y, 0.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(player->TargetX, player->TargetY, 0.0f);
		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
		glPopMatrix();
	}

	terrain->draw(cam);

	for (unsigned int i = 0; i < powerups.size(); i++) {
		powerups[i]->draw(false, false);
	}

	// TODO: sort lifeforms by isDead() state. alive lifeforms should not overlay dead ones.
	//	std::sort(lifeForms.begin(), lifeForms.end(),
	//			LifeForm::compareByDeadPredicate);

	map<string, LifeForm*>::const_iterator iter;
	for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
		LifeForm* lf = iter->second;

		if (lf->getLeft() < cam->X + cam->getHalfW() && lf->getRight() > cam->X
				- cam->getHalfW() && lf->getTop() < cam->Y + cam->getHalfH()
				&& lf->getBottom() > cam->Y - cam->getHalfH())

			if (gameState->Lost && lf->Type == LifeForm::player)
				continue;

		lf->draw();

		if (lf->Frozen > 0 && !lf->isDead()) {
			crystal->AMask = lf->Frozen / 10000.0f;
			crystal->draw(false, false, lf->X, lf->Y, lf->Angle, lf->Scale);
		}
	}

	for (unsigned int i = 0; i < particleSystems.size(); i++) {
		particleSystems[i]->draw();
	}

	if (!gameState->Lost && player->getLight()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
	}

	glDisable(GL_LIGHTING);

	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw();
	}

	glDisable(GL_TEXTURE_2D);

	if (!gameState->Lost) {
		if (player->getLaser()) {
			glLineWidth(0.5f);
			glBegin(GL_LINES);
			glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
			const float rad = (player->getArmsAngle() - 90) * M_PI / 180;
			glVertex3f(player->X + player->getWeapon()->XDiff * cos(rad)
					+ player->getWeapon()->YDiff * sin(-rad), player->Y
					+ player->getWeapon()->XDiff * sin(rad)
					+ player->getWeapon()->YDiff * cos(-rad), 0);
			glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
			glVertex3f(player->X + cam->getH() * 0.75f * cos(rad), player->Y
					+ cam->getH() * 0.75f * sin(rad), 0);
			glEnd();
		}
		if (player->getLight()) {
			glBegin(GL_TRIANGLES);
			glNormal3f(0.0f, 0.0f, 1.0f);
			const float rad = (player->getArmsAngle() - 90) * M_PI / 180;
			float flash = 1.0 - gameState->TimeOfDay;
			if (flash > 0.5)
				flash = 0.5;
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f * (1.0 - gameState->TimeOfDay));
			glVertex3f(player->X + player->getWeapon()->XDiff * cos(rad)
					+ player->getWeapon()->YDiff * sin(-rad), player->Y
					+ player->getWeapon()->XDiff * sin(rad)
					+ player->getWeapon()->YDiff * cos(-rad), 0.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
			glVertex3f(player->X + cam->getH() * 0.75f * cos(rad - 0.5f),
					player->Y + cam->getH() * 0.75f * sin(rad - 0.5f), 0.0f);
			glVertex3f(player->X + cam->getH() * 0.75f * cos(rad + 0.5f),
					player->Y + cam->getH() * 0.75f * sin(rad + 0.5f), 0.0f);

			glEnd();
		}
	}

	glEnable(GL_TEXTURE_2D);

	for (unsigned int i = 0; i < explosions.size(); i++) {
		explosions[i]->draw();
	}

	glDisable(GL_TEXTURE_2D);

	if (!gameState->Lost && !gameState->Paused) {
		aim->draw(player->TargetX, player->TargetY, 1.0f + tan(
				player->AccuracyDeviation * M_PI / 180)
				* Object::calculateDistance(player->X, player->Y,
						player->TargetX, player->TargetY) / 25.0f,
				player->getWeapon()->getReloadState() > 0 ? 1.2f
						- player->getWeapon()->getReloadState() : 0.2f);
	}

	glEnable(GL_TEXTURE_2D);
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

void runMainLoop() {
	currentTime = SDL_GetTicks();
	fpsCountingStart = currentTime;
	framesCount = 0;
	while (gameState->Works) {
		framesCount++;
		const int now = SDL_GetTicks();
		deltaTime = now - currentTime;
		currentTime = now;

		if (now - fpsCountingStart > 5000) {
			fpsCountingStart = now;
			fps = framesCount / 5;
			framesCount = 0;
		}

		if (config->FrameDelay > 0 && deltaTime < config->FrameDelay)
			SDL_Delay(config->FrameDelay - deltaTime);

		input->process();

		handleCommonControls();

		if (gameState->Begun) {
			musicManager->process(player, gameState);

			if (!gameState->Paused)
				processGame();

			drawGame();

			setGuiCameraMode();

			drawHud();
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
	explosionSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "explode-0.ogg")));
	explosionSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "explode-1.ogg")));

	playerKilledSound = sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_killed.ogg"));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_0.ogg")));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_1.ogg")));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_2.ogg")));

	vector<SDL_Surface*> playerLegsAnimSurfaces;
	for (unsigned i = 0; i < 25; i++) {
		char *buf;
		sprintf(buf = new char[100], "player/legs-%i.png", i);
		SDL_Surface *surface = ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::anima, buf));
		playerLegsAnimSurfaces.push_back(surface);
		delete[] buf;
	}
	playerLegsSprite = new Sprite(playerLegsAnimSurfaces);

	vector<SDL_Surface*> grenadeAnimSurfaces;
	for (unsigned i = 0; i < 12; i++) {
		char *buf;
		sprintf(buf = new char[100], "grenade/%i.png", i);
		SDL_Surface *surface = ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::anima, buf));
		grenadeAnimSurfaces.push_back(surface);
		delete[] buf;
	}
	grenadeSprite = new Sprite(grenadeAnimSurfaces);

	explTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "expl_0.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));
	explTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "expl_1.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));

	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_0.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_1.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_2.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));

	medikitTex
			= new Texture(ImageUtility::loadImage(fileUtility->getFullPath(
					FileUtility::image, "medikit.png")), GL_TEXTURE_2D,
					GL_LINEAR, true);

	grenadeTex
			= new Texture(ImageUtility::loadImage(fileUtility->getFullPath(
					FileUtility::image, "grenade.png")), GL_TEXTURE_2D,
					GL_LINEAR, true);

	freezeTex = new Texture(ImageUtility::loadImage(fileUtility->getFullPath(
			FileUtility::image, "freeze.png")), GL_TEXTURE_2D, GL_LINEAR, true);

	crystal = new StaticObject(0, 0, 128, 128, new Texture(
			ImageUtility::loadImage(fileUtility->getFullPath(
					FileUtility::image, "crystal.png")), GL_TEXTURE_2D,
			GL_LINEAR, true), true);

	aim = new Aim(config);

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
			printf("\nArguments:\n");
			printf("\t--help\t\t\t\tPrint help (this message) and exit\n");
			printf(
					"\t-w <screen_width>\t\tSet screen width to <screen_width>\n");
			printf(
					"\t-h <screen_height>\t\tSet screen height to <screen_height>\n");
			printf("\t-f\t\t\t\tGo to fullscreen at start\n");
			printf("\t-i\t\t\t\tForce windowed mode\n");
			printf("\t--fps <fps_count>\t\tLimit game fps by <fps_count>\n");
			printf("\t\t\t\t\tDefault value of <fps_count> is 100\n");
			printf("\t\t\t\t\tSeting <fps_count> to 0 will disable\n");
			printf("\t\t\t\t\trestriction\n");
			printf("\t--showfps\t\t\tShow fps in game\n");
			printf("\t--monsters <count>\t\tImmediately spawn\n");
			printf("\t\t\t\t\t<count> monsters at start\n");
			printf(
					"\n\nThese and other parametres can be adjusted in a configuration file\n");
			exit(0);
		}

#ifdef __APPLE__
		fileUtility->setFullResPath(getMacBundlePath()+"/Contents/Resources");
#endif

		if (arg.compare("-r") == 0 && i + 1 < argc) {
			fileUtility->setFullResPath(argv[i + 1]);
		}

		if (arg.compare("-f") == 0)
			config->FullScreen = true;

		if (arg.compare("-i") == 0)
			config->FullScreen = false;

		if (arg.compare("-w") == 0 && i + 1 < argc)
			config->ScreenWidth = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("-h") == 0 && i + 1 < argc)
			config->ScreenHeight = strtol(argv[i + 1], NULL, 10);

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
				printf("Number of monsters must be positive.\n");
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
