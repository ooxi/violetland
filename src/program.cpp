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
#endif //_WIN32W
#include <stdlib.h>
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
#include "system/graphic/Texture.h"
#include "system/graphic/Aim.h"
#include "system/graphic/Camera.h"
#include "system/sound/SoundManager.h"
#include "system/graphic/Window.h"
#include "system/Configuration.h"
#include "game/Enemy.h"
#include "game/Player.h"
#include "game/Powerup.h"
#include "game/Terrain.h"
#include "game/MusicManager.h"
#include "game/Highscores.h"

const string PROJECT = "violetland";
const string VERSION = "0.2.3";
const int GAME_AREA_SIZE = 2048;

Configuration* config;
Camera* cam;

float widthK = 1;
float heightK = 1;
float aspect = 1 / 3;

int framesCount;
int fpsCountingStart;
int fps = 0;

double gameHardness;
bool game;
bool lose;
bool gameStarted;
bool gamePaused;

float dayLight = 1.0;

Sound* playerKilledSound;
vector<Sound*> playerHitSounds;
int playerHitSndPlaying = 0;

Texture *medikitTex;
Texture *playerArmsTex;
Sprite *playerLegsSprite;
vector<Texture*> bloodTex;

vector<Sound*> enemyHitSounds;
vector<Sprite*> enemySprites;
Sprite* bleedSprite;

Aim* aim;

Player *player;
vector<StaticObject*> bloodStains;

vector<Weapon*> weapons;

vector<Powerup*> powerups;
vector<Enemy*> enemies;
vector<Bullet*> bullets;

map<string, Window*> windows;

vector<TextObject*> msgQueue;

int currentTime = 0;
int deltaTime = 0;

Terrain* terrain;

TextManager* text;
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

void clearEnemies() {
	for (unsigned int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
	enemies.clear();
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

	terrain = new Terrain(terrainSurface, tiles, GAME_AREA_SIZE);

	SDL_FreeSurface(terrainSurface);
	for (int i = 0; i < tilesCount; i++) {
		SDL_FreeSurface(tiles[i]);
	}
	tiles.clear();
}

void spawnEnemy(float r, int lvl, float* param) {
	float spawnAngle = (rand() % 6300) / 1000.0;

	float scale = pow((float) lvl / player->Level, 0.2f);

	if (lvl > 1)
		for (int i = 0; i < lvl; i++) {
			int s = (rand() % 299) / 100;
			param[s] += 0.1f;
		}

	float hi = 0.0f;
	for (int i = 0; i < 3; i++) {
		if (param[i] > hi)
			hi = param[i];
	}

	Enemy *newEnemy =
			new Enemy(r * cos(spawnAngle), r * sin(spawnAngle), param[1] + 0.3f
					> (param[0] + param[1] + param[2]) / 3.0f ? enemySprites[1]
					: enemySprites[0], bleedSprite, enemyHitSounds[rand()
					% enemyHitSounds.size()]);

	newEnemy->Strength = param[0];
	newEnemy->Agility = param[1];
	newEnemy->Vitality = param[2];

	newEnemy->HitR = 0.3;

	newEnemy->RMask = newEnemy->Vitality / hi;
	newEnemy->GMask = newEnemy->Strength / hi;
	newEnemy->BMask = newEnemy->Agility / hi * 0.7f;
	newEnemy->Scale = scale;

	newEnemy->setHealth(newEnemy->MaxHealth());
	newEnemy->Speed = newEnemy->MaxSpeed();
	enemies.push_back(newEnemy);
}

void startSurvival() {
	if (player)
		delete player;
	player = new Player(0, 0, playerLegsSprite, playerArmsTex, weapons[0]);
	player->HitR = 0.28f;
	player->Acceleration = 0.0004f;

	gameHardness = 9995.0;
	lose = false;
	gamePaused = false;
	gameStarted = true;

	clearBloodStains();
	clearPowerups();
	clearEnemies();
	clearBullets();
	clearMessages();

	msgQueue.push_back(text->getObject("Try to survive as long as you can.", 0,
			0, TextManager::LEFT, TextManager::MIDDLE));
	msgQueue.push_back(text->getObject(
			"Shoot monsters to receive experience and other bonuses.", 0, 0,
			TextManager::LEFT, TextManager::MIDDLE));
	msgQueue.push_back(text->getObject(
			"Press F1 at any moment to get additional instructions.", 0, 0,
			TextManager::LEFT, TextManager::MIDDLE));

	createTerrain();

	SDL_ShowCursor(0);

	for (unsigned int i = 0; i < config->MonstersAtStart; i++) {
		float param[3] = { 0.8f, 0.5f, 1.0f };
		spawnEnemy(cam->getW(), 1, param);
	}

	windows["mainmenu"]->CloseFlag = true;
}

void endGame() {
	game = false;
}

char *getProjectTitle() {
	char *buf;
	sprintf(buf = new char[PROJECT.size() + VERSION.size() + 4], "%s v%s",
			PROJECT.c_str(), VERSION.c_str());
	return buf;
}

void printVersion() {
	char *buf = getProjectTitle();
	cout << buf << endl;
	delete[] buf;
}

void initSystem() {
	srand((unsigned) time(NULL));

	printVersion();

#ifdef _WIN32
	printf("Assuming WINDOWS environment...\n");
#endif //_WIN32W
#ifdef linux
	printf("Assuming LINUX environment...\n");
#endif //linux
#ifdef __FreeBSD__
	printf("Assuming BSD environment...\n");
#endif //__FreeBSD__
	atexit(TTF_Quit);
	atexit(SDL_Quit);

	printf("SDL_Init...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	printf("SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER...\n");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// seems that this code is supported only in windows
	// printf("SDL_GL_SetAttribute SDL_GL_SWAP_CONTROL...\n");
	// SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	fprintf(stdout, "SDL_SetVideoMode %ix%i (%c)...\n", config->ScreenWidth,
			config->ScreenHeight, config->FullScreen ? 'f' : 'w');

	SDL_Surface *screen = SDL_SetVideoMode(config->ScreenWidth,
			config->ScreenHeight, config->ScreenColor,
			config->FullScreen ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);

	cam = new Camera();
	aspect = (float) config->ScreenWidth / config->ScreenHeight;
	cam->setH(cam->getW() / aspect);
	widthK = (float) config->ScreenWidth / cam->getW();
	heightK = (float) config->ScreenHeight / cam->getH();

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(2);
	}

	char* buf;
	buf = getProjectTitle();
	SDL_WM_SetCaption(buf, NULL);
	delete[] buf;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	printf("glViewport...\n");
	glViewport(0, 0, config->ScreenWidth, config->ScreenHeight);

	sprintf(buf = new char[100], "splash_%i.png", (rand() % 199) / 100);
	Texture* tex = new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, buf)), GL_TEXTURE_2D,
			GL_LINEAR, true);
	delete[] buf;

	splash = new StaticObject(0, 0, tex->getWidth(), tex->getHeight(), tex,
			true);

	glClear(GL_COLOR_BUFFER_BIT);

	cam->X = cam->Y = 0.0f;

	cam->apply();

	splash->draw(false);

	SDL_GL_SwapBuffers();

	sndManager = new SoundManager(fileUtility, config);
	musicManager = new MusicManager(fileUtility, sndManager, config);

	musicManager->play();

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//flashlight
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);

	//selflight
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001f);

	glEnable(GL_LIGHT2);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	TTF_Init();

	input = new InputHandler();
}

void loseGame() {
	lose = true;
	if (playerHitSounds[playerHitSndPlaying]->isPlaying())
		playerHitSounds[playerHitSndPlaying]->stop(0);

	playerKilledSound->play(0);

	msgQueue.push_back(text->getObject("Player is dead.", 0, 0,
			TextManager::LEFT, TextManager::BOTTOM));

	Highscores s(fileUtility);
	s.add(player);

	SDL_ShowCursor(1);
}

void switchGamePause() {
	gamePaused = !gamePaused;
	if (gamePaused)
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
	charStats->addElement("level", text->getObject(buf, l, text->getHeight()
			* 4.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "Available improvement points: %i",
			(int) ((player->LevelPoints)));
	charStats->addElement("availpoints", text->getObject(buf, l,
			text->getHeight() * 5.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "Strength: %i",
			(int) ((player->Strength * 100)));
	charStats->addElement("strength", text->getObject(buf, l, text->getHeight()
			* 7.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Agility: %i", (int) ((player->Agility * 100)));
	charStats->addElement("agility", text->getObject(buf, l, text->getHeight()
			* 8.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Vitality: %i",
			(int) ((player->Vitality * 100)));
	charStats->addElement("vitality", text->getObject(buf, l, text->getHeight()
			* 9.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	sprintf(buf = new char[100], "HP: %i / Max HP: %i",
			(int) ((player->getHealth() * 100)), (int) ((player->MaxHealth()
					* 100)));
	charStats->addElement("hp", text->getObject(buf, l, text->getHeight()
			* 11.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Melee damage: %i", (int) ((player->Damage()
			* 100)));
	charStats->addElement("melee", text->getObject(buf, l, text->getHeight()
			* 12.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Chance of block: %i%%",
			(int) ((player->ChanceToEvade() * 100)));
	charStats->addElement("chanceblock", text->getObject(buf, l,
			text->getHeight() * 13.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Reloading speed modifier: %i%%",
			(int) ((player->ReloadSpeedMod() * 100)));
	charStats->addElement("reloadingspeed", text->getObject(buf, l,
			text->getHeight() * 14.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Accuracy deviation modifier: %i%%",
			(int) ((player->WeaponRetForceMod() * 100)));
	charStats->addElement("accuracy", text->getObject(buf, l, text->getHeight()
			* 15.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;
	sprintf(buf = new char[100], "Health regeneration: %.2f/min",
			(player->HealthRegen() * 6000000));
	charStats->addElement("healthregen", text->getObject(buf, l,
			text->getHeight() * 16.0f, TextManager::LEFT, TextManager::MIDDLE));
	delete[] buf;

	if (player->Unstoppable)
		charStats->addElement("+unstoppable", text->getObject("+", r,
				text->getHeight() * 6.0f, TextManager::CENTER,
				TextManager::MIDDLE));

	if (player->PoisonBullets)
		charStats->addElement("+poisonbullets", text->getObject("+", r,
				text->getHeight() * 7.0f, TextManager::CENTER,
				TextManager::MIDDLE));

	if (player->BigCalibre)
		charStats->addElement("+bigcalibre", text->getObject("+", r,
				text->getHeight() * 8.0f, TextManager::CENTER,
				TextManager::MIDDLE));

	if (player->Telekinesis)
		charStats->addElement("+telekinesis", text->getObject("+", r,
				text->getHeight() * 9.0f, TextManager::CENTER,
				TextManager::MIDDLE));
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

void createCharStatWindow() {
	Window *charStats = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int r = config->ScreenWidth * 0.6f;

	charStats->addElement("perks", text->getObject("Perks:", r,
			text->getHeight() * 4.0f, TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("unstoppable", text->getObject("Unstoppable", r
			+ text->getHeight() * 2.0f, text->getHeight() * 6.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("poisonbullets", text->getObject("Poison bullets", r
			+ text->getHeight() * 2.0f, text->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("bigcalibre", text->getObject("Big calibre", r
			+ text->getHeight() * 2.0f, text->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	charStats->addElement("telekinesis", text->getObject("Telekinesis", r
			+ text->getHeight() * 2.0f, text->getHeight() * 9.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	charStats->addHandler("strength", increaseStrength);
	charStats->addHandler("agility", increaseAgility);
	charStats->addHandler("vitality", increaseVitality);

	charStats->addHandler("unstoppable", takeUnstoppable);
	charStats->addHandler("poisonbullets", takePoisonBullets);
	charStats->addHandler("bigcalibre", takeBigCalibre);
	charStats->addHandler("telekinesis", takeTelekinesis);

	windows["charstats"] = charStats;
}

void backFromHighScores();
void backFromOptionsAndSave();

void createHighscoresWindow() {
	Window *scoresWin = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;
	const int r2 = l * 2.0f;
	const int r3 = l * 4.0f;

	scoresWin->addElement("highscores", text->getObject("Highscores", l,
			text->getHeight() * 3.0f, TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addElement("headerXp", text->getObject("XP", l,
			text->getHeight() * 5.0f, TextManager::LEFT, TextManager::MIDDLE));
	scoresWin->addElement("headerParams", text->getObject("Str/Agil/Vital", r2,
			text->getHeight() * 5.0f, TextManager::LEFT, TextManager::MIDDLE));
	scoresWin->addElement("headerTime", text->getObject("Time", r3,
			text->getHeight() * 5.0f, TextManager::LEFT, TextManager::MIDDLE));

	Highscores s(fileUtility);
	vector<Player*> highscores = s.getData();

	if (!highscores.empty())
		for (unsigned int i = 0; i < highscores.size(); i++) {
			char* label;
			char* line;
			sprintf(label = new char[30], "xp%i", i);
			sprintf(line = new char[30], "%i", highscores[i]->Xp);
			scoresWin->addElement(label, text->getObject(line, l,
					text->getHeight() * (6.0f + i), TextManager::LEFT,
					TextManager::MIDDLE));
			delete[] label;
			delete[] line;

			sprintf(label = new char[30], "params%i", i);
			sprintf(line = new char[30], "%i/%i/%i",
					(int) (highscores[i]->Strength * 100),
					(int) (highscores[i]->Agility * 100),
					(int) (highscores[i]->Vitality * 100));
			scoresWin->addElement(label, text->getObject(line, r2,
					text->getHeight() * (6.0f + i), TextManager::LEFT,
					TextManager::MIDDLE));
			delete[] label;
			delete[] line;

			const int minutes = highscores[i]->Time / 60000;
			const int seconds = (highscores[i]->Time - minutes * 60000) / 1000;

			sprintf(label = new char[30], "time%i", i);
			sprintf(line = new char[30], "%im %is", minutes, seconds);
			scoresWin->addElement(label, text->getObject(line, r3,
					text->getHeight() * (6.0f + i), TextManager::LEFT,
					TextManager::MIDDLE));
			delete[] label;
			delete[] line;
		}

	scoresWin->addElement("back", text->getObject("Back to main menu", l,
			text->getHeight() * 18.0f, TextManager::LEFT, TextManager::MIDDLE));

	scoresWin->addHandler("back", backFromHighScores);

	windows["highscores"] = scoresWin;
}

void refreshOptionsWindow() {
	const int l = config->ScreenWidth * 0.1f;

	Window* w = windows.find("options")->second;

	if (config->AutoReload)
		w->addElement("+autoreload", text->getObject("+", l, text->getHeight()
				* 7.0f, TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autoreload", false);

	if (config->AutoWeaponPickup)
		w->addElement("+autopickup", text->getObject("+", l, text->getHeight()
				* 8.0f, TextManager::LEFT, TextManager::MIDDLE));
	else
		w->removeElement("+autopickup", false);
}

void switchAutoReload() {
	config->AutoReload = !config->AutoReload;
	refreshOptionsWindow();
}

void switchAutoPickup() {
	config->AutoWeaponPickup = !config->AutoWeaponPickup;
	refreshOptionsWindow();
}

void createOptionsWindow() {
	Window *w = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;

	w->addElement("options", text->getObject("Options", l, text->getHeight()
			* 3.0f, TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("sectiongame", text->getObject("Gameplay", l,
			text->getHeight() * 5.0f, TextManager::LEFT, TextManager::MIDDLE));

	w->addElement("autoreload", text->getObject("Weapon autoreloading", l
			+ text->getHeight() * 2.0f, text->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));
	w->addElement("autopickup", text->getObject("Weapon autotaking", l
			+ text->getHeight() * 2.0f, text->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	w->addHandler("autoreload", switchAutoReload);
	w->addHandler("autopickup", switchAutoPickup);

	w->addElement("savereturn", text->getObject("Save and return", l,
			text->getHeight() * 18.0f, TextManager::LEFT, TextManager::MIDDLE));
	w->addHandler("savereturn", backFromOptionsAndSave);

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

void createMainMenuWindow() {
	Window *mainMenu = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;

	mainMenu->addElement("survival", text->getObject("New survival", l,
			text->getHeight() * 8.0f, TextManager::LEFT, TextManager::MIDDLE));

	mainMenu->addHandler("survival", startSurvival);

	mainMenu->addElement("options", text->getObject("Options", l,
			text->getHeight() * 9.0f, TextManager::LEFT, TextManager::MIDDLE));

	mainMenu->addHandler("options", showOptions);

	mainMenu->addElement("highscores", text->getObject("High scores", l,
			text->getHeight() * 10.0f, TextManager::LEFT, TextManager::MIDDLE));

	mainMenu->addHandler("highscores", showHighScores);

	mainMenu->addElement("exit", text->getObject("Exit", l, text->getHeight()
			* 11.0f, TextManager::LEFT, TextManager::MIDDLE));

	mainMenu->addHandler("exit", endGame);

	windows["mainmenu"] = mainMenu;
}

void backFromOptionsAndSave() {
	config->write();
	windows["options"]->CloseFlag = true;
	createMainMenuWindow();
}

void backFromHighScores() {
	windows["highscores"]->CloseFlag = true;
	createMainMenuWindow();
}

void createHelpWindow() {
	Window *help = new Window(0.0f, 0.0f, config->ScreenWidth,
			config->ScreenHeight, 0.0f, 0.0f, 0.0f, 0.5f);

	const int l = config->ScreenWidth * 0.1f;

	help->addElement("label1", text->getObject("Game controls:", l,
			text->getHeight() * 4, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label3", text->getObject("Move up: W", l,
			text->getHeight() * 6, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label4", text->getObject("Move left: A", l,
			text->getHeight() * 7, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label5", text->getObject("Move down: S", l,
			text->getHeight() * 8, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label6", text->getObject("Move right: D", l,
			text->getHeight() * 9, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label7", text->getObject("Fire: Left mouse button", l,
			text->getHeight() * 10, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label8", text->getObject("Reload: Right mouse button", l,
			text->getHeight() * 11, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label2", text->getObject("Pick up weapon: E", l,
			text->getHeight() * 12, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label9", text->getObject("Toggle flashlight: F", l,
			text->getHeight() * 13, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label10", text->getObject("Toggle laser aim: G", l,
			text->getHeight() * 14, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label13", text->getObject("Open player char stats: C", l,
			text->getHeight() * 15, TextManager::LEFT, TextManager::MIDDLE));
	help->addElement("label12", text->getObject("Main menu: Esc", l,
			text->getHeight() * 16, TextManager::LEFT, TextManager::MIDDLE));

	windows["helpscreen"] = help;
}

void handleGameCommonControls() {
	if (input->getPressInput(InputHandler::ShowChar)) {
		if (windows.count("charstats") == 0) {
			clearWindows();

			createCharStatWindow();
			refreshCharStatsWindow();

			if (!gamePaused)
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

			if (!gamePaused)
				switchGamePause();
		} else {
			Window* w = windows.find("helpscreen")->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}

	if (input->getPressInput(InputHandler::Escape)) {
		if (windows.count("mainmenu") == 0) {
			clearWindows();

			createMainMenuWindow();

			if (!gamePaused)
				switchGamePause();
		} else {
			Window* w = windows.find("mainmenu")->second;
			w->CloseFlag = true;
			switchGamePause();
		}
	}
}

void handlePlayer() {
	if (player->getHealth() == 0)
		loseGame();

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

	if (player->X < -GAME_AREA_SIZE)
		player->setX(-GAME_AREA_SIZE);
	if (player->X > GAME_AREA_SIZE)
		player->setX(GAME_AREA_SIZE);
	if (player->Y < -GAME_AREA_SIZE)
		player->setY(-GAME_AREA_SIZE);
	if (player->Y > GAME_AREA_SIZE)
		player->setY(GAME_AREA_SIZE);

	player->TargetX = input->mouseX / widthK - cam->getHalfW() + cam->X;
	player->TargetY = input->mouseY / heightK - cam->getHalfH() + cam->Y;

	player->process(deltaTime);

	if (input->getDownInput(InputHandler::Fire)) {
		std::vector<Bullet*> *newBullets = player->fire();
		if (!newBullets->empty()) {
			bullets.insert(bullets.end(), newBullets->begin(),
					newBullets->end());
			delete newBullets;
		}
		if (player->getAmmo() == 0 && config->AutoReload)
			player->reload();
	}

	if (input->getPressInput(InputHandler::ToggleLight))
		player->toggleLight();

	if (input->getPressInput(InputHandler::ToggleLaser))
		player->toggleLaser();

	if (input->getDownInput(InputHandler::Reload))
		player->reload();
}

void killEnemy(int index) {
	bool powerupDropped = false;

	if (rand() % 1000 >= 970 || player->Kills == 0) {
		int weaponIndex = (rand() % (weapons.size() - 1)) + 1;
		Powerup *newPowerup = new Powerup(enemies[index]->X, enemies[index]->Y,
				weapons[weaponIndex]->getDroppedTex());
		newPowerup->Type = Powerup::weapon;
		newPowerup->Object = weapons[weaponIndex];
		newPowerup->HitR = 0.5f;
		powerups.push_back(newPowerup);
		powerupDropped = true;
	}

	if (!powerupDropped && rand() % 1000 >= 950) {
		Powerup *newPowerup = new Powerup(enemies[index]->X, enemies[index]->Y,
				medikitTex);
		newPowerup->Scale = 0.5;
		newPowerup->Type = Powerup::medikit;
		newPowerup->Object = new float(0.1);
		powerups.push_back(newPowerup);
		powerupDropped = true;
	}

	player->Kills++;
	delete enemies[index];
	enemies.erase(enemies.begin() + index);
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

void handleEnemies() {
	if (!lose)
		for (int i = 0; i < deltaTime; i++) {
			if (rand() % 10000 > gameHardness) {
				int lvl = player->Level * 0.5f + player->Level * pow((rand()
						% 100) / 125.0f, 2);
				if (lvl < 1)
					lvl = 1;
				float param[3] = { 0.8f, 0.5f, 1.0f };
				spawnEnemy(GAME_AREA_SIZE * 1.5, lvl, param);
			}
		}

	if (!enemies.empty()) {
		for (int i = enemies.size() - 1; i >= 0; i--) {
			if (enemies[i]->getHealth() <= 0) {
				killEnemy(i);
				continue;
			}

			float rangeToPlayer = sqrt(pow(-enemies[i]->X + player->X, 2)
					+ pow(enemies[i]->Y - player->Y, 2));

			if (enemies[i]->DoNotDisturb) {
				bool reach = true;
				if (enemies[i]->X < enemies[i]->TargetX - enemies[i]->Speed
						* 60 || enemies[i]->X > enemies[i]->TargetX
						+ enemies[i]->Speed * 60 || enemies[i]->Y
						< enemies[i]->TargetY - enemies[i]->Speed * 60
						|| enemies[i]->Y > enemies[i]->TargetY
								+ enemies[i]->Speed * 60)
					reach = false;
				if (reach)
					enemies[i]->DoNotDisturb = false;
			}

			if ((rangeToPlayer < 400 || enemies[i]->Angry) && !lose) {
				enemies[i]->TargetX = player->X;
				enemies[i]->TargetY = player->Y;
			} else if (rangeToPlayer < 800 && !lose) {
				enemies[i]->TargetX = player->X - cos(
						(player->getMoveDirection() + 90) * M_PI / 180)
						* rangeToPlayer / 2.0f / enemies[i]->Speed
						* player->Speed;
				enemies[i]->TargetY = player->Y - sin(
						(player->getMoveDirection() + 90) * M_PI / 180)
						* rangeToPlayer / 2.0f / enemies[i]->Speed
						* player->Speed;
			} else if (!enemies[i]->DoNotDisturb) {
				enemies[i]->TargetX = (rand() % (GAME_AREA_SIZE * 2))
						- GAME_AREA_SIZE;
				enemies[i]->TargetY = (rand() % (GAME_AREA_SIZE * 2))
						- GAME_AREA_SIZE;
				enemies[i]->DoNotDisturb = true;
			}

			float x = enemies[i]->X;
			float y = enemies[i]->Y;

			enemies[i]->process(deltaTime);

			if (!lose && player->detectCollide(enemies[i])) {
				if (enemies[i]->Attack()) {
					if (rand() % 100 > player->ChanceToEvade() * 100) {
						player->setHealth(player->getHealth()
								- enemies[i]->Damage());
						if (!playerHitSounds[playerHitSndPlaying]->isPlaying()) {
							playerHitSndPlaying = (player->getHealth()
									< player->MaxHealth() ? player->getHealth()
									: player->getHealth() - 0.01f)
									/ player->MaxHealth()
									* playerHitSounds.size();
							playerHitSounds[playerHitSndPlaying]->play(0);
						}
					}

					if (!player->Unstoppable)
						player->Speed = 0.0f;
				}

				if (player->Attack() && rand() % 100
						> enemies[i]->ChanceToEvade() * 100)
					enemies[i]->setHealth(player->getHealth()
							- player->Damage());

				enemies[i]->X = x;
				enemies[i]->Y = y;
			} else {
				enemies[i]->rollFrame(true);
			}

			if (enemies[i]->isBleeding() && bloodStains.size() < 9) {
				addBloodStain(enemies[i]->X, enemies[i]->Y, enemies[i]->Angle,
						(rand() % 10) / 50.0f + 0.1f, enemies[i]->Poisoned);
			}
		}
	}
}

void handleBullets() {
	if (!bullets.empty()) {
		for (int i = bullets.size() - 1; i >= 0; i--) {
			bullets[i]->process(deltaTime);

			if (bullets[i]->isActive() && !enemies.empty()) {
				for (int j = enemies.size() - 1; j >= 0; j--) {
					if (bullets[i]->checkHit(enemies[j])) {

						if (bloodStains.size() < 9) {
							for (int k = 0; k < 3; k++) {
								int angleDev = (rand() % 90) - 45;
								float distance = (rand() % 100);
								float bX = enemies[j]->X - cos(
										(bullets[i]->Angle + 90 + angleDev)
												* M_PI / 180.0f) * distance;
								float bY = enemies[j]->Y - sin(
										(bullets[i]->Angle + 90 + angleDev)
												* M_PI / 180.0f) * distance;

								addBloodStain(bX, bY, enemies[j]->Angle,
										enemies[j]->Scale * 0.5f,
										enemies[j]->Poisoned);
							}
						}

						float damageLoss = enemies[j]->getHealth();
						enemies[j]->hit(bullets[i], player->X, player->Y);

						player->Xp
								+= (int) ((1.5 - dayLight * -0.5) * damageLoss
										> bullets[i]->Damage ? bullets[i]->Damage
										* 20
										: damageLoss * 20);

						if (bullets[i]->BigCalibre) {
							bullets[i]->Damage -= damageLoss;
							if (bullets[i]->Damage <= 0)
								bullets[i]->deactivate();
						}
					}
				}
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
			msgQueue[i]->draw(true, msgQueue[i]->X + text->getIndent(),
					config->ScreenHeight - s * text->getHeight() + i
							* text->getHeight());
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
	const int minutes = player->Time / 60000;
	const int seconds = (player->Time - minutes * 60000) / 1000;

	char *buf;

	float health = player->getHealth() / player->MaxHealth() * 100.0f;
	sprintf(buf = new char[30], "Health: %.2f%%", health);
	TextObject* healthMsg = text->getObject(buf, text->getIndent(),
			text->getIndent(), TextManager::LEFT, TextManager::TOP);
	delete[] buf;
	if (health < 34)
		healthMsg->GMask = healthMsg->BMask = 0.0f;
	healthMsg->draw(true, healthMsg->X, healthMsg->Y);
	delete healthMsg;

	sprintf(buf = new char[30], "%s: %d/%d", player->getWeaponName().c_str(),
			player->getAmmo(), player->getMaxAmmo());
	text->draw(buf, text->getIndent(), text->getIndent() + text->getHeight(),
			TextManager::LEFT, TextManager::TOP);
	delete[] buf;

	sprintf(buf = new char[30], "Time: %dm %ds", minutes, seconds);
	text->draw(buf, config->ScreenWidth - text->getIndent(), text->getIndent(),
			TextManager::RIGHT, TextManager::TOP);
	delete[] buf;

	sprintf(buf = new char[30], "Xp: %d (%d)", player->Xp, player->NextLevelXp);
	text->draw(buf, config->ScreenWidth - text->getIndent(), text->getIndent()
			+ text->getHeight(), TextManager::RIGHT, TextManager::TOP);
	delete[] buf;

	if (config->ShowFps) {
		sprintf(buf = new char[30], "FPS: %i", fps);
		text->draw(buf, config->ScreenWidth - text->getIndent(),
				config->ScreenHeight - text->getIndent(), TextManager::RIGHT,
				TextManager::BOTTOM);
		delete[] buf;
	}

	if (lose && !gamePaused)
		text->draw("They have overcome...", config->ScreenWidth / 2,
				config->ScreenHeight / 2, TextManager::CENTER,
				TextManager::MIDDLE);

	if (gamePaused)
		text->draw("PAUSE", config->ScreenWidth / 2, config->ScreenHeight / 2,
				TextManager::CENTER, TextManager::MIDDLE);

	drawMessagesQueue();
}

void handlePowerups() {
	for (int i = powerups.size() - 1; i >= 0; i--) {
		bool deletePowerup = false;
		powerups[i]->Time -= deltaTime;
		powerups[i]->AMask = powerups[i]->Time / 15000.0;
		if (powerups[i]->Type == Powerup::medikit)
			powerups[i]->Angle = StaticObject::fixAngle(powerups[i]->Angle
					+ deltaTime * 0.1);

		if (powerups[i]->Time < 0)
			deletePowerup = true;

		if (player->Telekinesis) {
			if (powerups[i]->detectCollide(player->TargetX, player->TargetY))
				powerups[i]->take(deltaTime);
			else
				powerups[i]->resetTaking();
		}

		if (!lose && (powerups[i]->detectCollide(player)
				|| powerups[i]->TakeDelay == 0)) {
			switch (powerups[i]->Type) {
			case Powerup::medikit:
				msgQueue.push_back(text->getObject(
						"Player has taken a medical kit.", 0, 0,
						TextManager::LEFT, TextManager::MIDDLE));
				player->setHealth(player->getHealth()
						+ *(float*) powerups[i]->Object);
				deletePowerup = true;
				break;
			case Powerup::weapon:
				if (input->getDownInput(InputHandler::Pickup)
						|| config->AutoWeaponPickup) {
					player->setWeapon((Weapon*) powerups[i]->Object);
					char *buf;
					sprintf(buf = new char[200], "Player has taken the %s.",
							player->getWeaponName().c_str());
					msgQueue.push_back(text->getObject(buf, 0, 0,
							TextManager::LEFT, TextManager::MIDDLE));
					delete[] buf;
					deletePowerup = true;
					break;
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

void levelUp() {
	float param[3] = { 1.2f, 0.3f, 3.0f };
	spawnEnemy(GAME_AREA_SIZE * 1.5f, player->Level * 1.5f + 10, param);

	player->NextLevelXp *= 2;

	player->Level += 1;
	player->LevelPoints += 1;

	msgQueue.push_back(text->getObject("The player has reached new level.", 0,
			0, TextManager::LEFT, TextManager::MIDDLE));

	player->setHealth(player->MaxHealth());
}

void processGame() {
	if (!lose) {
		gameHardness -= deltaTime * 0.00012;
		player->Time += deltaTime;

		handlePlayer();
	}

	if (player->Xp >= player->NextLevelXp) {
		levelUp();
	}

	if (bloodStains.size() > 0) {
		terrain->beginDrawOn();
		{
			for (unsigned int i = 0; i < bloodStains.size(); i++) {
				terrain->drawOn(bloodStains[i]);
			}

			clearBloodStains();
		}
		terrain->endDrawOn();
	}

	handlePowerups();
	handleEnemies();
	handleBullets();
}

void drawGame() {
	cam->X = player->X;
	cam->Y = player->Y;

	if (cam->X < -GAME_AREA_SIZE + cam->getHalfW())
		cam->X = -GAME_AREA_SIZE + cam->getHalfW();
	if (cam->X > GAME_AREA_SIZE - cam->getHalfW())
		cam->X = GAME_AREA_SIZE - cam->getHalfW();
	if (cam->Y < -GAME_AREA_SIZE + cam->getHalfH())
		cam->Y = -GAME_AREA_SIZE + cam->getHalfH();
	if (cam->Y > GAME_AREA_SIZE - cam->getHalfH())
		cam->Y = GAME_AREA_SIZE - cam->getHalfH();

	cam->apply();

	glEnable(GL_LIGHTING);

	dayLight = abs(cos(player->Time / 180000.0));

	float globalAmbientColor = dayLight / 5.0;
	float directColor = dayLight / 2.0;

	GLfloat global_ambient[] = { globalAmbientColor, globalAmbientColor,
			globalAmbientColor, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	GLfloat day_light[] = { directColor, directColor, directColor, 1.0f };
	glLightfv(GL_LIGHT2, GL_AMBIENT, day_light);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, day_light);
	glLightfv(GL_LIGHT2, GL_SPECULAR, day_light);

	if (!lose && player->getLight()) {
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
		powerups[i]->draw(false);
	}

	if (!lose)
		player->draw();

	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getLeft() < cam->X + cam->getHalfW()
				&& enemies[i]->getRight() > cam->X - cam->getHalfW()
				&& enemies[i]->getTop() < cam->Y + cam->getHalfH()
				&& enemies[i]->getBottom() > cam->Y - cam->getHalfH())
			enemies[i]->draw();
	}

	if (!lose && player->getLight()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
	}

	glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, NULL);

	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw();
	}

	if (!lose && player->getLaser()) {
		glLineWidth(0.5f);
		glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
		const float rad = (player->getArmsDirection() - 90) * M_PI / 180;
		glVertex3f(player->X + 50 * cos(rad), player->Y + 50 * sin(rad), 0);
		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f(player->X + cam->getH() * 0.75f * cos(rad), player->Y
				+ cam->getH() * 0.75f * sin(rad), 0);
		glEnd();
	}

	if (!lose && !gamePaused) {
		aim->draw(player->TargetX, player->TargetY, 1.0f + tan(
				player->AccuracyDeviation * M_PI / 180)
				* Object::calculateDistance(player->X, player->Y,
						player->TargetX, player->TargetY) / 25.0f,
				player->getReloadState() > 0 ? 1.2f - player->getReloadState()
						: 0.2f);
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

void runMainLoop() {
	currentTime = SDL_GetTicks();
	fpsCountingStart = currentTime;
	framesCount = 0;
	gameStarted = false;
	game = true;
	while (game) {
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

		if (gameStarted) {
			musicManager->process(player, enemies, gamePaused);

			handleGameCommonControls();

			if (!gamePaused)
				processGame();

			drawGame();

			setGuiCameraMode();

			drawHud();
		} else {
			musicManager->play();

			glClear(GL_COLOR_BUFFER_BIT);

			cam->X = cam->Y = 0.0f;

			cam->apply();

			splash->draw(false);

			setGuiCameraMode();
		}

		drawWindows();

		SDL_GL_SwapBuffers();
	}
}

void loadWeapons() {
	printf("Loading weapons...\n");

	if (!weapons.empty()) {
		for (unsigned int i = 0; i < weapons.size(); i++) {
			delete weapons[i];
		}
		weapons.clear();
	}

	ifstream in;
	in.open(fileUtility->getFullPath(FileUtility::common, "weapons").c_str());
	if (!in) {
		fprintf(stderr, "Couldn't load weapons list.\n");
		exit(4);
	}
	while (in) {
		int weaponType;
		in >> weaponType;
		char bulletPath[2000] = "";
		char droppedImagePath[2000] = "";
		char shotSound[2000] = "";
		char reloadSound[2000] = "";
		char name[30] = "";
		in >> bulletPath;
		if (strlen(bulletPath) < 1)
			break;
		in >> droppedImagePath;
		in >> shotSound;
		in >> reloadSound;
		Weapon *weapon = new Weapon((Bullet::BulletType) weaponType,
				fileUtility->getFullPath(FileUtility::image, droppedImagePath),
				sndManager->create(fileUtility->getFullPath(FileUtility::sound,
						shotSound)), sndManager->create(
						fileUtility->getFullPath(FileUtility::sound,
								reloadSound)));
		if (weaponType > 1) {
			weapon->setBulletImage(fileUtility->getFullPath(FileUtility::image,
					bulletPath));
		}
		in >> name;
		weapon->Name = name;
		in >> weapon->AmmoClipSize;
		weapon->Ammo = weapon->AmmoClipSize;
		in >> weapon->Damage;
		in >> weapon->FireDelayTime;
		in >> weapon->ReloadTime;
		in >> weapon->FireRange;
		in >> weapon->BulletSpeed;
		in >> weapon->ReturnForce;
		in >> weapon->BulletsAtOnce;

		weapons.push_back(weapon);
	}
	in.close();
}

void loadResources() {
	loadWeapons();

	enemyHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "zombie_hit_1.ogg")));
	enemyHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "zombie_hit_2.ogg")));

	playerKilledSound = sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_killed.ogg"));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_0.ogg")));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_1.ogg")));
	playerHitSounds.push_back(sndManager->create(fileUtility->getFullPath(
			FileUtility::sound, "player_hit_2.ogg")));

	playerArmsTex = new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "player_top.png")),
			GL_TEXTURE_2D, GL_LINEAR, true);

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

	vector<SDL_Surface*> bleedAnimSurfaces;
	for (unsigned i = 0; i < 13; i++) {
		char *buf;
		sprintf(buf = new char[100], "bleed/bleed-%i.png", i);
		SDL_Surface *surface = ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::anima, buf));
		bleedAnimSurfaces.push_back(surface);
		delete[] buf;
	}
	bleedSprite = new Sprite(bleedAnimSurfaces);

	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_0.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_1.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullPath(FileUtility::image, "blood_2.png")),
			GL_TEXTURE_2D, GL_LINEAR, true));

	for (unsigned int j = 0; j < 2; j++) {
		vector<SDL_Surface*> animSurfaces;
		for (unsigned i = 0; i < 25; i++) {
			char *buf;
			sprintf(buf = new char[100], "monsters/%i/move-%i.png", j, i);
			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, buf));
			animSurfaces.push_back(surface);
			delete[] buf;
		}
		Sprite *monsterSprite = new Sprite(animSurfaces);
		enemySprites.push_back(monsterSprite);
	}

	medikitTex
			= new Texture(ImageUtility::loadImage(fileUtility->getFullPath(
					FileUtility::image, "medikit.png")), GL_TEXTURE_2D,
					GL_LINEAR, true);

	aim = new Aim(config);

	text = new TextManager(fileUtility->getFullPath(FileUtility::common,
			"fonts/harabara.ttf"), 46 * widthK);
}

void unloadResources() {
	delete splash;
	delete playerKilledSound;
	delete text;
	delete aim;
	delete playerArmsTex;
	delete playerLegsSprite;
	delete player;
	delete terrain;
	delete bleedSprite;
	delete medikitTex;
	for (unsigned int i = 0; i < bloodTex.size(); i++) {
		delete bloodTex[i];
	}
	bloodTex.clear();
	for (unsigned int i = 0; i < enemySprites.size(); i++) {
		delete enemySprites[i];
	}
	enemySprites.clear();
	for (unsigned int i = 0; i < weapons.size(); i++) {
		weapons[i]->deleteResources();
		delete weapons[i];
	}
	weapons.clear();
	clearBloodStains();
	clearPowerups();
	clearEnemies();
	clearBullets();
	clearMessages();
	clearWindows();

	for (unsigned int i = 0; i < enemyHitSounds.size(); i++) {
		delete enemyHitSounds[i];
	}
	enemyHitSounds.clear();
	for (unsigned int i = 0; i < playerHitSounds.size(); i++) {
		delete playerHitSounds[i];
	}
	playerHitSounds.clear();
	delete config;
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
			printf("\t\t\t\t\tDefault value of <fps_count> is 0\n");
			printf("\t\t\t\t\tSeting <fps_count> to 0 will disable\n");
			printf("\t\t\t\t\trestriction\n");
			printf("\t--showfps\t\t\tShow fps in game\n");
			printf("\t--monsters <count>\t\tImmediately spawn\n");
			printf("\t\t\t\t\t<count> monsters at start\n");
			exit(0);
		}

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
	parsePreferences(argc, argv);

	initSystem();

	loadResources();

	createMainMenuWindow();

	runMainLoop();

	unloadResources();

	delete musicManager;
	delete sndManager;
	delete input;
	delete fileUtility;

	exit(0);
}
