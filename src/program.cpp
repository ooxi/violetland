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
#endif //_WIN32W
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "system/ImageUtility.h"
#include "system/InputHandler.h"
#include "system/TextManager.h"
#include "game/Enemy.h"
#include "game/Bullet.h"
#include "game/Player.h"
#include "game/Powerup.h"
#include "system/FileUtility.h"
#include "game/Terrain.h"
#include "system/Texture.h"
#include "system/Aim.h"

const string PROJECT = "violetland";
const string VERSION = "0.2.0";
const int GAME_AREA_SIZE = 2048;
const int SCREEN_COLOR = 16;

int vScrWidth = 1600;
int vScrHeight = 1200;

int screenWidth = 800;
int screenHeight = 600;
float widthK = 1;
float heightK = 1;
float aspect = 1 / 3;

int framesCount;
int fpsCountingStart;
int fpsLimit = 10;
int fps = 0;
bool showFps = false;

bool autoReload = true;
int masterVolume = 30;
unsigned int spawnMonstersAtStart = 2;

bool fullScreen = false;
float camX = 0;
float camY = 0;

double gameHardness;
bool game;
int gameTime;
bool lose;
bool gamePaused;
bool showChar;
bool showHelp;

float dayLight = 1.0;

Mix_Chunk *playerKilledSound;
vector<Mix_Chunk*> playerHitSounds;
int playerHitSoundChannel = -1;

Texture *medikitTex;
Texture *playerArmsTex;
Sprite *playerLegsSprite;
vector<Texture*> bloodTex;

vector<Mix_Chunk*> enemyHitSounds;
vector<Sprite*> enemySprites;
Sprite* bleedSprite;

int aimColorDark = 0x000000;
int aimColorLight = 0xFFFFFF;
Aim* aim;

Player *player;
vector<StaticObject*> bloodStains;
vector<StaticObject*> bulletLoops;

vector<Weapon*> weapons;

vector<Powerup*> powerups;
vector<Enemy*> enemies;
vector<Bullet*> bullets;

vector<StaticObject*> msgQueue;

TextManager *text;

int currentTime = 0;
int deltaTime = 0;

Terrain *terrain;

FileUtility *fileUtility;
InputHandler *input;

void clearBloodStains() {
	for (unsigned int i = 0; i < bloodStains.size(); i++) {
		delete bloodStains[i];
	}
	bloodStains.clear();
}

void clearBulletLoops() {
	for (unsigned int i = 0; i < bulletLoops.size(); i++) {
		delete bulletLoops[i];
	}
	bulletLoops.clear();
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

	int baseTexCount = fileUtility->getFilesCountFromDir(
			fileUtility->getFullImagePath("terrain"));
	int baseTex = (rand() % (baseTexCount * 100 - 1) / 100);

	string tilesDir = fileUtility->getFullImagePath("terrain");
	tilesDir.append("/%i");
	char *buf;
	sprintf(buf = new char[2000], tilesDir.c_str(), baseTex);
	int tilesCount = fileUtility->getFilesCountFromDir(buf);
	delete[] buf;

	sprintf(buf = new char[100], "terrain/base_%i.png", baseTex);
	SDL_Surface *terrainSurface = ImageUtility::loadImage(
			fileUtility->getFullImagePath(buf), 1);
	delete[] buf;

	vector<SDL_Surface*> tiles;
	for (int i = 0; i < tilesCount; i++) {
		sprintf(buf = new char[100], "terrain/%i/%i.png", baseTex, i);
		SDL_Surface *tile = ImageUtility::loadImage(
				fileUtility->getFullImagePath(buf), 1);
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

void spawnEnemy(float r) {
	float spawnAngle = (rand() % 6300) / 1000.0;

	float scale = pow((rand() % 100) / (double) 130, 3);
	scale = (rand() % 100) > 50 ? 1 + scale : 1 - scale;

	Enemy *newEnemy = new Enemy(r * cos(spawnAngle), r * sin(spawnAngle), scale
			< 1.0 ? enemySprites[1] : enemySprites[0], bleedSprite,
			enemyHitSounds[rand() % enemyHitSounds.size()]);
	newEnemy->HitR = 0.3;

	if (scale < 0.8f || scale > 1.2f) {
		newEnemy->GMask = (rand() % 50) / 100.0 + 0.5;
		newEnemy->RMask = (rand() % 50) / 100.0 + 0.5;
		newEnemy->BMask = (rand() % 50) / 100.0 + 0.5;
	} else {
		newEnemy->GMask = 1.0f;
		newEnemy->RMask = 1.0f;
		newEnemy->BMask = 0.7f;
	}

	newEnemy->Scale = scale;
	newEnemy->Strength = scale * 0.5f;
	newEnemy->Agility = 0.5f / scale;
	newEnemy->Vitality = scale * 0.8f;
	newEnemy->setHealth(newEnemy->MaxHealth());
	newEnemy->Speed = newEnemy->MaxSpeed();
	enemies.push_back(newEnemy);
}

void startGame() {
	printf("Starting new game...\n");
	if (player)
		delete player;
	player = new Player(0, 0, playerLegsSprite, playerArmsTex, weapons[0]);
	player->HitR = 0.3;
	player->Acceleration = 0.0004;

	gameTime = 0;
	gameHardness = 9995.0;
	lose = false;
	gamePaused = false;
	showChar = false;
	showHelp = false;

	clearBloodStains();
	clearPowerups();
	clearBulletLoops();
	clearEnemies();
	clearBullets();
	clearMessages();
	msgQueue.push_back(text->getObject("Try to survive as long as you can.", 0,
			0, TextManager::LEFT));
	msgQueue.push_back(text->getObject(
			"Shoot monsters to receive experience and other bonuses.", 0, 0,
			TextManager::LEFT));
	msgQueue.push_back(text->getObject(
			"Press F1 at any moment to get additional instructions.", 0, 0,
			TextManager::LEFT));

	createTerrain();

	game = true;
	SDL_ShowCursor(0);

	for (unsigned int i = 0; i < spawnMonstersAtStart; i++) {
		spawnEnemy(vScrWidth);
	}
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
	printVersion();

	atexit(TTF_Quit);
	atexit(SDL_Quit);

	printf("\nSDL_Init...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	printf("SDL_GL_SetAttribute SDL_GL_DOUBLEBUFFER...\n");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	char *buf;
	sprintf(buf = new char[50], "SDL_SetVideoMode %ix%i (%c)...\n",
			screenWidth, screenHeight, fullScreen ? 'f' : 'w');
	printf(buf);
	delete[] buf;

	SDL_Surface *screen =
			SDL_SetVideoMode(screenWidth, screenHeight, SCREEN_COLOR,
					fullScreen ? SDL_OPENGL | SDL_FULLSCREEN : SDL_OPENGL);

	aspect = (float) screenWidth / screenHeight;
	vScrHeight = vScrWidth / aspect;
	widthK = (float) screenWidth / vScrWidth;
	heightK = (float) screenHeight / vScrHeight;

	if (screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(2);
	}

	TTF_Init();

	buf = getProjectTitle();
	SDL_WM_SetCaption(buf, NULL);
	delete[] buf;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//flashlight
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001);

	//selflight
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);

	glEnable(GL_LIGHT2);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	printf("glViewport...\n");
	glViewport(0, 0, screenWidth, screenHeight);

	printf("Mix_OpenAudio...\n");
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(3);
	}

	sprintf(buf = new char[30], "Mix_Volume (master) %i...\n", masterVolume);
	printf(buf);
	delete[] buf;
	Mix_Volume(-1, masterVolume);

	printf("InputHandler...\n");
	input = new InputHandler();
}

void writeHighScores() {
	printf("Updating scores...\n");

	vector<int> scores;
	ifstream in;
	in.open(fileUtility->getFullUserPath("highscores").c_str());
	if (in) {
		while (in) {
			int score;
			in >> score;
			scores.push_back(score);
		}
		in.close();
	} else {
		printf("Previous file with scores is not found.\n");
	}

	bool placed = false;
	for (unsigned int i = 0; i < scores.size(); i++) {
		if (player->Xp > scores[i]) {
			scores.insert(scores.begin() + i, player->Xp);
			placed = true;
			break;
		}
	}

	if (!placed)
		scores.push_back(player->Xp);

	ofstream out;
	out.open(fileUtility->getFullUserPath("highscores~").c_str());
	if (out) {
		for (unsigned int i = 0; i < (scores.size() < 10 ? scores.size() : 10); i++) {
			out << scores[i] << endl;
		}
		out.close();

		remove(fileUtility->getFullUserPath("highscores").c_str());

		FileUtility::copyFile(
				fileUtility->getFullUserPath("highscores~").c_str(),
				fileUtility->getFullUserPath("highscores").c_str());

		remove(fileUtility->getFullUserPath("highscores~").c_str());

		printf("Scores was updated.\n");
	} else {
		printf("Can't write scores to file.\n");
	}

	scores.clear();
}

void loseGame() {
	lose = true;
	if (playerHitSoundChannel != -1 && Mix_Playing(playerHitSoundChannel) != 0) {
		Mix_HaltChannel(playerHitSoundChannel);
	}
	Mix_PlayChannel(-1, playerKilledSound, 0);

	msgQueue.push_back(text->getObject("Player is dead.", 0, vScrHeight,
			TextManager::LEFT));

	writeHighScores();

	SDL_ShowCursor(1);
}

void switchGamePause() {
	gamePaused = !gamePaused;
	if (gamePaused)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

void handleCommonControls() {
	if (input->getPressInput(InputHandler::Restart) && gameTime > 1000)
		startGame();

	if (input->getPressInput(InputHandler::Escape))
		game = false;

	if (input->getPressInput(InputHandler::Pause) && !showChar && !showHelp) {
		switchGamePause();
	}

	if (input->getPressInput(InputHandler::ShowChar)) {
		showChar = !showChar;
		if (showChar)
			showHelp = false;
		if (gamePaused != showChar)
			switchGamePause();
	}

	if (input->getPressInput(InputHandler::Help)) {
		showHelp = !showHelp;
		if (showHelp)
			showChar = false;
		if (gamePaused != showHelp)
			switchGamePause();
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

	player->TargetX = input->mouseX / widthK - vScrWidth / 2 + camX;
	player->TargetY = input->mouseY / heightK - vScrHeight / 2 + camY;

	player->process(deltaTime);

	if (input->getDownInput(InputHandler::Fire)) {
		std::vector<Bullet*> *newBullets = player->fire();
		if (!newBullets->empty()) {
			bullets.insert(bullets.end(), newBullets->begin(),
					newBullets->end());
			delete newBullets;
		}
		if (player->getAmmo() == 0 && autoReload)
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

void handleEnemies() {
	if (!lose)
		for (int i = 0; i < deltaTime; i++) {
			if (rand() % 10000 > gameHardness)
				spawnEnemy(GAME_AREA_SIZE * 1.5);
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
						* rangeToPlayer / enemies[i]->Speed * player->Speed;
				enemies[i]->TargetY = player->Y - sin(
						(player->getMoveDirection() + 90) * M_PI / 180)
						* rangeToPlayer / enemies[i]->Speed * player->Speed;
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
						if (playerHitSoundChannel == -1 || Mix_Playing(
								playerHitSoundChannel) == 0) {
							playerHitSoundChannel
									= Mix_PlayChannel(
											-1,
											playerHitSounds[(player->getHealth()
													< player->MaxHealth() ? player->getHealth()
													: player->getHealth()
													- 0.01f)
											/ player->MaxHealth()
											* playerHitSounds.size()],
											0);
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

			if (enemies[i]->isBleeding() && bloodStains.size() < 3) {
				StaticObject *newBloodStain = new StaticObject(enemies[i]->X,
						enemies[i]->Y, 128, 128,
						bloodTex[(rand() % 299) / 100], false);
				newBloodStain->Scale = (rand() % 10) / 50.0 + 0.1;
				newBloodStain->Angle = enemies[i]->Angle;
				newBloodStain->RMask = newBloodStain->GMask
						= newBloodStain->BMask = 1.0 - (rand() % 200) / 1000.0;
				bloodStains.push_back(newBloodStain);
			}
		}
	}
}

void handleBullets() {
	if (!bullets.empty())
		for (int i = bullets.size() - 1; i >= 0; i--) {
			bool eraseBullet = bullets[i]->isFall();

			if (!enemies.empty()) {
				for (int j = enemies.size() - 1; j >= 0; j--) {
					if (bullets[i]->detectCollide(enemies[j])) {
						player->Xp += (int) ((1.5 - dayLight * -0.5)
								* bullets[i]->Damage * 10);
						enemies[j]->hit(bullets[i], player->X, player->Y);

						if (bloodStains.size() < 3) {
							StaticObject *newBloodStain = new StaticObject(
									enemies[j]->X, enemies[j]->Y, 128, 128,
									bloodTex[(rand() % 299) / 100], false);
							newBloodStain->Scale = enemies[j]->Scale;
							newBloodStain->Angle = enemies[j]->Angle;
							newBloodStain->RMask = newBloodStain->GMask
									= newBloodStain->BMask = 1.0 - (rand()
											% 200) / 1000.0;
							bloodStains.push_back(newBloodStain);
						}

						eraseBullet = true;
						break;
					}
				}
			}

			if (eraseBullet) {
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				continue;
			}

			bullets[i]->process(deltaTime);

			StaticObject *newBulletLoop = new StaticObject(bullets[i]->X,
					bullets[i]->Y, 32, 32, bullets[i]->getTextureRef(), false);
			newBulletLoop->AMask = 0.5;
			newBulletLoop->Angle = bullets[i]->Angle;
			bulletLoops.push_back(newBulletLoop);
		}

	if (!bulletLoops.empty())
		for (int i = bulletLoops.size() - 1; i >= 0; i--) {
			if (bulletLoops[i]->AMask <= 0) {
				delete bulletLoops[i];
				bulletLoops.erase(bulletLoops.begin() + i);
				continue;
			}

			bulletLoops[i]->AMask -= deltaTime * 0.002;
		}
}

void drawCharStats() {
	const int fh = text->getFontHeight();
	const int l = screenWidth * 0.1f;
	const int r = screenWidth * 0.6f;
	char *buf;

	sprintf(buf = new char[100], "Current player level: %i",
			(int) (player->Level));
	text->draw(buf, l, fh * 4.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Available improvement points: %i",
			(int) (player->LevelPoints));
	text->draw(buf, l, fh * 5.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Strength: %i", (int) (player->Strength * 100));
	text->draw(buf, l, fh * 7.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Agility: %i", (int) (player->Agility * 100));
	text->draw(buf, l, fh * 8.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Vitality: %i", (int) (player->Vitality * 100));
	text->draw(buf, l, fh * 9.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "HP: %i / Max HP: %i",
			(int) (player->getHealth() * 100),
			(int) (player->MaxHealth() * 100));
	text->draw(buf, l, fh * 11.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Melee damage: %i", (int) (player->Damage()
			* 100));
	text->draw(buf, l, fh * 12.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Chance of block: %i%%",
			(int) (player->ChanceToEvade() * 100));
	text->draw(buf, l, fh * 13.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Reloading speed modifier: %i%%",
			(int) (player->ReloadSpeedMod() * 100));
	text->draw(buf, l, fh * 14.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Accuracy deviation modifier: %i%%",
			(int) (player->WeaponRetForceMod() * 100));
	text->draw(buf, l, fh * 15.0f, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[100], "Health regeneration: %.2f/min",
			(player->HealthRegen() * 6000000));
	text->draw(buf, l, fh * 16.0f, TextManager::LEFT);
	delete[] buf;

	text->draw("Perks:", r, fh * 4.0f, TextManager::LEFT);

	if (player->Unstoppable)
		text->draw("+", r, fh * 6.0f, TextManager::CENTER);
	text->draw("Unstoppable", r + fh * 2.0f, fh * 6.0f, TextManager::LEFT);
	if (player->PoisonBullets)
		text->draw("+", r, fh * 7.0f, TextManager::CENTER);
	text->draw("Poison bullets", r + fh * 2.0f, fh * 7.0f, TextManager::LEFT);

	if (input->getPressInput(InputHandler::Fire)) {
		float gmx = input->mouseX;
		float gmy = input->mouseY;

		if (gmx > l && gmx < screenWidth / 2 && gmy > fh * 6.5f && gmy < fh
				* 7.5f && player->LevelPoints > 0) {
			player->Strength += 0.1;
			player->LevelPoints--;
		}
		if (gmx > l && gmx < screenWidth / 2 && gmy > fh * 7.5f && gmy < fh
				* 8.5f && player->LevelPoints > 0) {
			player->Agility += 0.1;
			player->LevelPoints--;
		}
		if (gmx > l && gmx < screenWidth / 2 && gmy > fh * 8.5f && gmy < fh
				* 9.5f && player->LevelPoints > 0) {
			float ch = player->getHealth() / player->MaxHealth();
			player->Vitality += 0.1;
			player->LevelPoints--;
			player->setHealth(player->MaxHealth() * ch);
		}
		if (gmx > r && gmx < screenWidth && gmy > fh * 5.5f && gmy < fh * 6.5f
				&& !player->Unstoppable && player->LevelPoints > 0) {
			player->Unstoppable = true;
			player->LevelPoints--;
		}
		if (gmx > r && gmx < screenWidth && gmy > fh * 6.5f && gmy < fh * 7.5f
				&& !player->PoisonBullets && player->LevelPoints > 0) {
			player->PoisonBullets = true;
			player->LevelPoints--;
		}
	}
}

void drawHelp() {
	const int fh = text->getFontHeight();
	const int l = screenWidth * 0.1f;

	text->draw("Game controls:", l, fh * 4, TextManager::LEFT);
	text->draw("Move up: W", l, fh * 6, TextManager::LEFT);
	text->draw("Move left: A", l, fh * 7, TextManager::LEFT);
	text->draw("Move down: S", l, fh * 8, TextManager::LEFT);
	text->draw("Move right: D", l, fh * 9, TextManager::LEFT);
	text->draw("Fire: Left mouse button", l, fh * 10, TextManager::LEFT);
	text->draw("Reload: Right mouse button", l, fh * 11, TextManager::LEFT);
	text->draw("Toggle flashlight: F", l, fh * 12, TextManager::LEFT);
	text->draw("Toggle laser aim: G", l, fh * 13, TextManager::LEFT);
	text->draw("Restart game: Enter", l, fh * 14, TextManager::LEFT);
	text->draw("Quit game: Esc", l, fh * 15, TextManager::LEFT);
	text->draw("Open player char stats: C", l, fh * 16, TextManager::LEFT);
	text->draw("Pause game: P", l, fh * 17, TextManager::LEFT);
}

void drawMessagesQueue() {
	const int fh = text->getFontHeight();

	if (!msgQueue.empty()) {
		int s = msgQueue.size();
		for (int i = s - 1; i >= 0; i--) {
			msgQueue[i]->draw(true, msgQueue[i]->X, screenHeight - s * fh + i
					* fh);
			msgQueue[i]->AMask -= 0.0001 * deltaTime;

			if (msgQueue[i]->AMask <= 0) {
				delete msgQueue[i];
				msgQueue.erase(msgQueue.begin() + i);
			}
		}
	}
}

void drawHud() {
	const int fh = text->getFontHeight();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, screenWidth, screenHeight, 0.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	const int minutes = gameTime / 60000;
	const int seconds = (gameTime - minutes * 60000) / 1000;

	char *buf;

	sprintf(buf = new char[30], "Health: %.2f%%", player->getHealth()
			/ player->MaxHealth() * 100.0f);
	text->draw(buf, 0, fh, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[30], "%s: %d/%d", player->getWeaponName().c_str(),
			player->getAmmo(), player->getMaxAmmo());
	text->draw(buf, 0, fh * 2, TextManager::LEFT);
	delete[] buf;

	sprintf(buf = new char[30], "Time: %dm %ds", minutes, seconds);
	text->draw(buf, screenWidth, fh, TextManager::RIGHT);
	delete[] buf;

	sprintf(buf = new char[30], "Xp: %d (%d)", player->Xp, player->NextLevelXp);
	text->draw(buf, screenWidth, fh * 2, TextManager::RIGHT);
	delete[] buf;

	if (showFps) {
		sprintf(buf = new char[30], "FPS: %i", fps);
		text->draw(buf, screenWidth, screenHeight - fh, TextManager::RIGHT);
		delete[] buf;
	}

	if (lose && !gamePaused)
		text->draw("They have overcome...", screenWidth / 2, screenHeight / 2,
				TextManager::CENTER);

	if (gamePaused)
		text->draw("PAUSE", screenWidth / 2, screenHeight / 2,
				TextManager::CENTER);

	drawMessagesQueue();

	if (showChar) {
		drawCharStats();
	}

	if (showHelp) {
		drawHelp();
	}
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

		if (!lose && powerups[i]->detectCollide(player)) {
			switch (powerups[i]->Type) {
			case Powerup::medikit:
				msgQueue.push_back(text->getObject(
						"Player has taken a medical kit.", 0, vScrHeight,
						TextManager::LEFT));
				player->setHealth(player->getHealth()
						+ *(float*) powerups[i]->Object);
				break;
			case Powerup::weapon:
				player->setWeapon((Weapon*) powerups[i]->Object);
				char *buf;
				sprintf(buf = new char[200], "Player has taken the %s.",
						player->getWeaponName().c_str());
				msgQueue.push_back(text->getObject(buf, 0, vScrHeight,
						TextManager::LEFT));
				delete[] buf;
				break;
			}

			deletePowerup = true;
		}

		if (deletePowerup) {
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			continue;
		}
	}
}

void setCamera() {
	camX = player->X;
	camY = player->Y;

	if (camX < -GAME_AREA_SIZE + vScrWidth * 0.5)
		camX = -GAME_AREA_SIZE + vScrWidth * 0.5;
	if (camX > GAME_AREA_SIZE - vScrWidth * 0.5)
		camX = GAME_AREA_SIZE - vScrWidth * 0.5;
	if (camY < -GAME_AREA_SIZE + vScrHeight * 0.5)
		camY = -GAME_AREA_SIZE + vScrHeight * 0.5;
	if (camY > GAME_AREA_SIZE - vScrHeight * 0.5)
		camY = GAME_AREA_SIZE - vScrHeight * 0.5;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int a = vScrWidth / 2;
	int b = vScrHeight / 2;

	glOrtho(camX - a, camX + a, camY + b, camY - b, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void levelUp() {
	player->NextLevelXp *= 2;

	player->Level += 1;
	player->LevelPoints += 1;

	msgQueue.push_back(text->getObject("The player has reached new level.", 0,
			vScrHeight, TextManager::LEFT));

	player->setHealth(player->MaxHealth());
}

void processGame() {
	if (!lose) {
		gameHardness -= deltaTime * 0.0001;
		gameTime += deltaTime;

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
	setCamera();

	glEnable(GL_LIGHTING);

	dayLight = abs(cos(gameTime / 180000.0));

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

	terrain->draw();

	if (!lose)
		player->draw();

	for (unsigned int i = 0; i < powerups.size(); i++) {
		powerups[i]->draw(false);
	}

	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getLeft() < camX + vScrWidth / 2
				&& enemies[i]->getRight() > camX - vScrWidth / 2
				&& enemies[i]->getTop() < camY + vScrHeight / 2
				&& enemies[i]->getBottom() > camY - vScrHeight / 2)
			enemies[i]->draw();
	}

	if (!lose && player->getLight()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
	}

	glDisable(GL_LIGHTING);

	for (unsigned int i = 0; i < bulletLoops.size(); i++) {
		bulletLoops[i]->draw(false);
	}

	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw(false);
	}

	glBindTexture(GL_TEXTURE_2D, NULL);

	if (!lose && player->getLaser()) {
		glLineWidth(0.5f);
		glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
		const float rad = (player->getArmsDirection() - 90) * M_PI / 180;
		glVertex3f(player->X + 50 * cos(rad), player->Y + 50 * sin(rad), 0);
		glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
		glVertex3f(player->X + vScrHeight * 0.75f * cos(rad), player->Y
				+ vScrHeight * 0.75f * sin(rad), 0);
		glEnd();
	}

	if (!lose && !gamePaused) {
		aim->draw(player->TargetX, player->TargetY, 1.0f + tan(
				player->AccuracyDeviation * M_PI / 180)
				* Object::calculateDistance(player->X, player->Y,
						player->TargetX, player->TargetY) / 25.0f,
				player->getReloadState() > 0 ? 1.1f - player->getReloadState()
						: 0.1f);
	}
}

void runGameLoop() {
	currentTime = SDL_GetTicks();
	fpsCountingStart = currentTime;
	framesCount = 0;
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

		if (fpsLimit > 0 && deltaTime < fpsLimit)
			SDL_Delay(fpsLimit - deltaTime);

		input->process(deltaTime);

		handleCommonControls();

		if (!gamePaused)
			processGame();

		drawGame();

		glDepthRange(0.0, 0.9);
		drawHud();

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
	in.open(fileUtility->getFullResPath("weapons").c_str());
	if (!in) {
		fprintf(stderr, "Couldn't load weapons list.\n");
		exit(4);
	}
	while (1) {
		char bulletPath[2000] = "";
		char droppedImagePath[2000] = "";
		char shotSoundPath[2000] = "";
		char reloadSoundPath[2000] = "";
		char name[30] = "";
		in >> bulletPath;
		if (strlen(bulletPath) < 1)
			break;
		in >> droppedImagePath;
		in >> shotSoundPath;
		in >> reloadSoundPath;
		in >> name;
		Weapon *weapon = new Weapon(fileUtility->getFullImagePath(bulletPath),
				fileUtility->getFullImagePath(droppedImagePath),
				fileUtility->getFullSoundPath(shotSoundPath),
				fileUtility->getFullSoundPath(reloadSoundPath));
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

	enemyHitSounds.push_back(Mix_LoadWAV(fileUtility->getFullSoundPath(
					"zombie_hit_1.ogg").c_str()));
	enemyHitSounds.push_back(Mix_LoadWAV(fileUtility->getFullSoundPath(
					"zombie_hit_2.ogg").c_str()));

	playerKilledSound = Mix_LoadWAV(fileUtility->getFullSoundPath(
					"player_killed.ogg").c_str());
	playerHitSounds.push_back(Mix_LoadWAV(fileUtility->getFullSoundPath(
					"player_hit_0.ogg").c_str()));
	playerHitSounds.push_back(Mix_LoadWAV(fileUtility->getFullSoundPath(
					"player_hit_1.ogg").c_str()));
	playerHitSounds.push_back(Mix_LoadWAV(fileUtility->getFullSoundPath(
					"player_hit_2.ogg").c_str()));

	playerArmsTex = new Texture(ImageUtility::loadImage(
			fileUtility->getFullImagePath("player_top.png")), GL_TEXTURE_2D,
			GL_LINEAR, true);

	vector<SDL_Surface*> playerLegsAnimSurfaces;
	for (unsigned i = 0; i < 25; i++) {
		char *buf;
		sprintf(buf = new char[100], "player/legs-%i.png", i);
		SDL_Surface *surface = ImageUtility::loadImage(
				fileUtility->getFullAnimaPath(buf));
		playerLegsAnimSurfaces.push_back(surface);
		delete[] buf;
	}
	playerLegsSprite = new Sprite(playerLegsAnimSurfaces);

	vector<SDL_Surface*> bleedAnimSurfaces;
	for (unsigned i = 0; i < 13; i++) {
		char *buf;
		sprintf(buf = new char[100], "bleed/bleed-%i.png", i);
		SDL_Surface *surface = ImageUtility::loadImage(
				fileUtility->getFullAnimaPath(buf));
		bleedAnimSurfaces.push_back(surface);
		delete[] buf;
	}
	bleedSprite = new Sprite(bleedAnimSurfaces);

	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullImagePath("blood_0.png")), GL_TEXTURE_2D,
			GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullImagePath("blood_1.png")), GL_TEXTURE_2D,
			GL_LINEAR, true));
	bloodTex.push_back(new Texture(ImageUtility::loadImage(
			fileUtility->getFullImagePath("blood_2.png")), GL_TEXTURE_2D,
			GL_LINEAR, true));

	for (unsigned int j = 0; j < 2; j++) {
		vector<SDL_Surface*> animSurfaces;
		for (unsigned i = 0; i < 25; i++) {
			char *buf;
			sprintf(buf = new char[100], "monsters/%i/move-%i.png", j, i);
			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullAnimaPath(buf));
			animSurfaces.push_back(surface);
			delete[] buf;
		}
		Sprite *monsterSprite = new Sprite(animSurfaces);
		enemySprites.push_back(monsterSprite);
	}

	medikitTex = new Texture(ImageUtility::loadImage(
			fileUtility->getFullImagePath("medikit.png")), GL_TEXTURE_2D,
			GL_LINEAR, true);

	aim = new Aim(aimColorDark, aimColorLight);

	text = new TextManager(fileUtility->getFullResPath("fonts/harabara.ttf"),
			46 * widthK);
}

void unloadResources() {
	Mix_FreeChunk(playerKilledSound);
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
	clearBulletLoops();
	clearEnemies();
	clearBullets();
	clearMessages();

	for (unsigned int i = 0; i < enemyHitSounds.size(); i++) {
		Mix_FreeChunk(enemyHitSounds[i]);
	}
	enemyHitSounds.clear();
	for (unsigned int i = 0; i < playerHitSounds.size(); i++) {
		Mix_FreeChunk(playerHitSounds[i]);
	}
	playerHitSounds.clear();
}

void parsePreferences(int argc, char *argv[]) {
	fileUtility = new FileUtility(argv[0]);

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
			printf(
					"\t-v <volume>\t\t\tSet the master sound volume to <volume>\n");
			printf("\t\t\t\t\t(from 0 to 128; 30 by default)\n");
			printf("\t--noreload\t\t\tDisable automatic reloading\n");
			printf("\t--fps <fps_count>\t\tLimit game fps by <fps_count>\n");
			printf("\t\t\t\t\tDefault value of <fps_count> is 100\n");
			printf("\t\t\t\t\tSeting <fps_count> to 0 will disable\n");
			printf("\t\t\t\t\trestriction\n");
			printf("\t--showfps\t\t\tShow fps in game\n");
			printf("\t--monsters <count>\t\tImmediately spawn\n");
			printf("\t\t\t\t\t<count> monsters at start\n");
			printf("\t--aimca <color>\t\t\tSet first color of aim cursor\n");
			printf("\t--aimcb <color>\t\t\tSet second color of aim cursor\n");
			printf("\t\t\t\t\t<color> must be hex number of\n");
			printf("\t\t\t\t\tRGB sort like 0x000000 for black\n");
			printf("\t\t\t\t\tand 0xFFFFFF for white (this is\n");
			printf("\t\t\t\t\tdefault values for a and b\n");
			printf("\t\t\t\t\tcolors respectively)\n");
			exit(0);
		}

		if (arg.compare("-f") == 0)
			fullScreen = true;

		if (arg.compare("-v") == 0 && i + 1 < argc)
			masterVolume = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("-r") == 0 && i + 1 < argc) {
			fileUtility->setFullResPath(argv[i + 1]);
		}

		if (arg.compare("-w") == 0 && i + 1 < argc)
			screenWidth = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("-h") == 0 && i + 1 < argc)
			screenHeight = strtol(argv[i + 1], NULL, 10);

		if (arg.compare("--aimca") == 0 && i + 1 < argc) {
			aimColorDark = strtol(argv[i + 1], NULL, 16);
		}

		if (arg.compare("--aimcb") == 0 && i + 1 < argc) {
			aimColorLight = strtol(argv[i + 1], NULL, 16);
		}

		if (arg.compare("--fps") == 0 && i + 1 < argc) {
			int lim = strtol(argv[i + 1], NULL, 10);
			fpsLimit = lim > 0 ? 1000 / lim : 0;
		}

		if (arg.compare("--noreload") == 0)
			autoReload = false;

		if (arg.compare("--showfps") == 0)
			showFps = true;

		if (arg.compare("--monsters") == 0 && i + 1 < argc) {
			int n = strtol(argv[i + 1], NULL, 10);
			if (n >= 0)
				spawnMonstersAtStart = n;
			else
				printf("Number of monsters must be positive.\n");
		}
	}
}

int main(int argc, char *argv[]) {
	parsePreferences(argc, argv);

	initSystem();
	loadResources();

	startGame();

	runGameLoop();

	unloadResources();

	Mix_CloseAudio();
	delete input;
	delete fileUtility;

	exit(0);
}
