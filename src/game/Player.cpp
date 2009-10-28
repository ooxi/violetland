#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Player.h"

Player::Player(float x, float y, Sprite *legsSprite, Texture *armsTex,
		Weapon *weapon) :
	LiveObject(x, y, 128, 128) {

	Xp = 0;
	NextLevelXp = 100;
	Kills = 0;
	Level = 1;
	LevelPoints = 0;

	X = x;
	Y = y;
	TargetX = x;
	TargetY = y;

	m_legs = new DynamicObject(x, y, legsSprite);
	m_arms = new StaticObject(x, y, 128, 128, armsTex, false);

	m_legs->Frame = 8;

	AccuracyDeviation = 0.0;
	Speed = 0.0;

	m_light = m_laser = false;

	m_weapon = new Weapon(*weapon);

	Unstoppable = false;
	PoisonBullets = false;
	BigCalibre = false;
}

void Player::move(char movementX, char movementY, int deltaTime) {
	if (movementX != 0 || movementY != 0) {
		Speed += Acceleration * deltaTime;
		if (Speed > MaxSpeed())
			Speed = MaxSpeed();

		float newAngle = Object::calculateAngle(0, 0, movementX, movementY);

		if (abs(m_legs->Angle - newAngle) < MaxSpeed() * deltaTime)
			m_legs->Angle = newAngle;
		else
			m_legs->turn(newAngle, MaxSpeed(), deltaTime);
	} else {
		Speed -= Acceleration * deltaTime;
		if (Speed < 0)
			Speed = 0;
	}

	if (Speed > 0) {
		m_legs->Speed = Speed;
		m_legs->process(deltaTime);

		X = m_arms->X = m_legs->X;
		Y = m_arms->Y = m_legs->Y;
	}

	if (Speed > MaxSpeed() / 4) {
		m_legs->rollFrame(true);
	} else {
		if (!(m_legs->Frame == 8 || m_legs->Frame == 20)) {
			if ((m_legs->Frame > 8 && m_legs->Frame < 16) || m_legs->Frame > 20)
				m_legs->rollFrame(false);
			else
				m_legs->rollFrame(true);
		}
	}
}

std::vector<Bullet*> *Player::fire() {
	const float rad = (getArmsDirection() - 90) * M_PI / 180;
	std::vector<Bullet*> *newBullets = m_weapon->fire(m_arms->X + 50 * cos(rad), m_arms->Y + 50 * sin(rad));

	if (!newBullets->empty()) {
		std::vector<Bullet*>::iterator it;

		if (AccuracyDeviation < 25)
			AccuracyDeviation
					+= m_weapon->BulletsAtOnce > 1 ? m_weapon->ReturnForce
							: m_weapon->ReturnForce * WeaponRetForceMod();

		for (it = newBullets->begin(); it != (newBullets->end()); it++) {
			Bullet* bullet = *(it);
			bullet->Poisoned = m_weapon->Type == Bullet::standard && PoisonBullets;
			bullet->BigCalibre = m_weapon->Type == Bullet::standard && BigCalibre;
			bullet->Angle = AccuracyDeviation < 1 ? m_arms->Angle
					: m_arms->Angle + (rand() % (int) (AccuracyDeviation * 2))
							- AccuracyDeviation;
		}

		if (m_weapon->BulletsAtOnce > 1)
			AccuracyDeviation = 0;
	}

	return newBullets;
}

void Player::reload() {
	if (m_weapon->reload(ReloadSpeedMod()))
		AccuracyDeviation = 0;
}

const float Player::getReloadState() {
	return m_weapon->getReloadState();
}

void Player::toggleLight() {
	m_light = !m_light;
}

void Player::toggleLaser() {
	m_laser = !m_laser;
}

const bool Player::getLight() {
	return m_light;
}

const bool Player::getLaser() {
	return m_laser;
}

void Player::process(int deltaTime) {
	m_arms->Angle = Object::calculateAngle(X, Y, TargetX, TargetY);

	m_weapon->process(deltaTime);

	setHealth(getHealth() + HealthRegen() * deltaTime);

	AccuracyDeviation -= deltaTime * 0.01;
	if (AccuracyDeviation < 0)
		AccuracyDeviation = 0;
}

void Player::draw() {
	m_legs->draw();
	m_arms->draw(false);
}

const float Player::getMoveDirection() {
	return m_legs->Angle;
}

const float Player::getArmsDirection() {
	return m_arms->Angle;
}

void Player::setX(float value) {
	X = m_arms->X = m_legs->X = value;
}

void Player::setY(float value) {
	Y = m_arms->Y = m_legs->Y = value;
}

const int Player::getAmmo() {
	return m_weapon->Ammo;
}

const int Player::getMaxAmmo() {
	return m_weapon->AmmoClipSize;
}

std::string Player::getWeaponName() {
	return m_weapon->Name;
}

void Player::setWeapon(Weapon *value) {
	delete m_weapon;
	m_weapon = new Weapon(*value);
	AccuracyDeviation = 0;
}

Player::~Player() {
	delete m_legs;
	delete m_arms;
	delete m_weapon;
}
