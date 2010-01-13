#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Player.h"

Player::Player() :
	LifeForm(0, 0, 128, 128) {
	Empty = true;
	Xp = 0;
	NextLevelXp = 100;
	Kills = 0;
	LevelPoints = 0;
	AccuracyDeviation = 0.0f;
	Time = 0;
	Grenades = 2;
	Type = LifeForm::player;

	m_weapon = NULL;

	m_light = m_laser = false;

	Unstoppable = PoisonBullets = BigCalibre = Telekinesis = false;
}

Player::Player(float x, float y, Sprite *legsSprite, Texture *armsTex) :
	LifeForm(x, y, 128, 128) {
	*this = Player();

	X = x;
	Y = y;
	TargetX = x;
	TargetY = y;

	m_legs = new DynamicObject(x, y, legsSprite);
	m_legs->Frame = 8;

	m_arms = new StaticObject(x, y, 128, 128, armsTex, false);

	Empty = false;
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
	const float rad = (getArmsAngle() - 90) * M_PI / 180;
	std::vector<Bullet*> *newBullets = m_weapon->fire(m_arms->X, m_arms->Y,
			m_arms->X + 50 * cos(rad), m_arms->Y + 50 * sin(rad));

	if (!newBullets->empty()) {
		std::vector<Bullet*>::iterator it;

		if (AccuracyDeviation < 25)
			AccuracyDeviation
					+= m_weapon->BulletsAtOnce > 1 ? m_weapon->ReturnForce
							: m_weapon->ReturnForce * WeaponRetForceMod();

		for (it = newBullets->begin(); it != (newBullets->end()); it++) {
			Bullet* bullet = *(it);
			bullet->Poisoned = m_weapon->Type == Bullet::standard
					&& PoisonBullets;
			bullet->BigCalibre = m_weapon->Type == Bullet::standard
					&& BigCalibre;
			bullet->Angle = AccuracyDeviation < 1 ? m_arms->Angle
					: m_arms->Angle + (rand() % (int) (AccuracyDeviation * 2))
							- AccuracyDeviation;
		}

		if (m_weapon->BulletsAtOnce > 1)
			AccuracyDeviation = 0;
	}

	return newBullets;
}

Bullet* Player::throwGrenade(Sprite* grenadeSprite) {
	Bullet* newBullet =
			new GrenadeBullet(X, Y, TargetX, TargetY, grenadeSprite);

	newBullet->Damage = 5.0f;
	newBullet->Speed = MaxSpeed() * 5.0f;
	newBullet->MaxRange = 1500;

	Grenades--;

	return newBullet;
}

void Player::reload() {
	if (m_weapon->reload(ReloadSpeedMod()))
		AccuracyDeviation = 0;
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
	LifeForm::process(deltaTime);

	m_arms->Angle = Object::calculateAngle(X, Y, TargetX, TargetY);

	m_weapon->process(deltaTime);

	AccuracyDeviation -= deltaTime * 0.01;
	if (AccuracyDeviation < 0)
		AccuracyDeviation = 0;
}

void Player::draw() {
	m_legs->draw();
	m_arms->draw(false, false);
}

const float Player::getLegsAngle() {
	return m_legs->Angle;
}

const float Player::getArmsAngle() {
	return m_arms->Angle;
}

void Player::setX(float value) {
	X = m_arms->X = m_legs->X = value;
}

void Player::setY(float value) {
	Y = m_arms->Y = m_legs->Y = value;
}

Weapon* Player::getWeapon() {
	return m_weapon;
}

void Player::setWeapon(Weapon *value) {
	if (m_weapon)
		delete m_weapon;
	m_weapon = new Weapon(*value);
	AccuracyDeviation = 0;
}

Player::~Player() {
	if (!Empty) {
		delete m_legs;
		delete m_arms;
		delete m_weapon;
	}
}
