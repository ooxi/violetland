#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Player.h"

Player::Player() :
	LifeForm(0, 0, 128, 128) {
	Id = "20-" + Id;
	Empty = true;
	Xp = 0;
	LastLevelXp = 0;
	NextLevelXp = 100;
	Kills = 0;
	LevelPoints = 0;
	AccuracyDeviation = 0.0f;
	Grenades = 2;
	Type = LifeForm::player;
	HudInfo = "";
	m_hitSndPlaying = 0;

	m_weapon = NULL;

	m_light = m_laser = false;

	Unstoppable = PoisonBullets = BigCalibre = Telekinesis = NightVision
			= false;
	PenBullets = 0;
}

Player::Player(float x, float y, Sprite *legsSprite, Sprite *deathSprite,
		std::vector<Sound*> hitSounds, Sound* dyingSound) :
	LifeForm(x, y, 128, 128) {
	*this = Player();

	X = x;
	Y = y;
	TargetX = x;
	TargetY = y;

	m_body = new DynamicObject(x, y, legsSprite);
	m_body->Frame = 8;
	m_deathSprite = deathSprite;
	m_hitSounds = hitSounds;
	m_dyingSound = dyingSound;

	Empty = false;
}

void Player::hit() {
	if (!m_hitSounds[m_hitSndPlaying]->isPlaying()) {
		m_hitSndPlaying = (getHealth() < MaxHealth() ? getHealth()
				: getHealth() - 0.01f) / MaxHealth() * m_hitSounds.size();
		m_hitSounds[m_hitSndPlaying]->play(6, 0, 0);
	}
}

void Player::move(char movementX, char movementY, int deltaTime) {
	if (movementX != 0 || movementY != 0) {
		Speed += Acceleration * deltaTime;
		if (Speed > MaxSpeed())
			Speed = MaxSpeed();

		float newAngle = Object::calculateAngle(0, 0, movementX, movementY);

		if (abs(m_body->Angle - newAngle) < MaxSpeed() * deltaTime)
			m_body->Angle = newAngle;
		else
			m_body->turn(newAngle, MaxSpeed(), deltaTime);
	} else {
		Speed -= Acceleration * deltaTime;
		if (Speed < 0)
			Speed = 0;
	}

	if (Speed > 0) {
		m_body->Speed = Speed;
		m_body->process(deltaTime);

		X = m_arms->X = m_body->X;
		Y = m_arms->Y = m_body->Y;
	}

	if (Speed > MaxSpeed() / 4) {
		m_body->rollFrame(true);
	} else {
		if (!(m_body->Frame == 8 || m_body->Frame == 20)) {
			if ((m_body->Frame > 8 && m_body->Frame < 16) || m_body->Frame > 20)
				m_body->rollFrame(false);
			else
				m_body->rollFrame(true);
		}
	}
}

std::vector<Bullet*> *Player::fire() {
	const float rad = (getArmsAngle() - 90) * M_PI / 180;
	std::vector<Bullet*> *newBullets = m_weapon->fire(m_arms->X, m_arms->Y,
			m_arms->X + m_weapon->XDiff * cos(rad) + m_weapon->YDiff
					* sin(-rad), m_arms->Y + m_weapon->XDiff * sin(rad)
					+ m_weapon->YDiff * cos(-rad));

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
			if (BigCalibre) {
				bullet->Damage *= 1.1;
			}
			bullet->Penetrating = m_weapon->Type == Bullet::standard
					&& PenBullets > 0;
			bullet->Angle = AccuracyDeviation < 1 ? m_arms->Angle
					: m_arms->Angle + (rand() % (int) (AccuracyDeviation * 2))
							- AccuracyDeviation;
			bullet->OwnerId = Id;
		}

		if (m_weapon->BulletsAtOnce > 1)
			AccuracyDeviation = 0;

		DynamicObject* shell = new DynamicObject(X, Y, m_weapon->ShellSprite);
		shell->Angle = m_arms->Angle;
		m_shells.push_back(shell);
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

	if (State == LifeForm::dying) {
		if (m_body->Frame == m_body->AnimSprite->getFramesCount() - 1)
			State = LifeForm::died;
	}

	if (State == LifeForm::dying) {
		m_body->rollFrame(true);
	}

	if (State == LifeForm::smitten) {
		const float angle = m_body->Angle;
		m_body = new DynamicObject(X, Y, m_deathSprite);
		m_body ->Angle = angle;
		State = LifeForm::dying;
		if (m_hitSounds[m_hitSndPlaying]->isPlaying())
			m_hitSounds[m_hitSndPlaying]->stop(0);

		m_dyingSound->play(5, 0, 0);
	}

	m_arms->Angle = Object::calculateAngle(X, Y, TargetX, TargetY);

	m_weapon->process(deltaTime);

	AccuracyDeviation -= deltaTime * 0.01;
	if (AccuracyDeviation < 0)
		AccuracyDeviation = 0;

	PenBullets -= deltaTime;
	if (PenBullets < 0)
		PenBullets = 0;
}

void Player::draw() {
	m_body->draw();

	if (State == LifeForm::alive)
		m_arms->draw(false, false);

	for (int i = m_shells.size() - 1; i >= 0; i--) {
		m_shells[i]->draw();
		if (m_shells[i]->Frame == m_shells[i]->AnimSprite->getFramesCount() - 1) {
			delete m_shells[i];
			m_shells .erase(m_shells.begin() + i);
			continue;
		}
		m_shells[i]->rollFrame(true);
	}
}

StaticObject* Player::getCorpse() {
	StaticObject * corpse = new StaticObject(X, Y, m_width, m_height,
			m_body->getFrame(), false);
	corpse->Scale = m_body->Scale;
	corpse->Angle = Object::fixAngle(180 - m_body->Angle);
	return corpse;
}

const float Player::getLegsAngle() {
	return m_body->Angle;
}

const float Player::getArmsAngle() {
	return m_arms->Angle;
}

void Player::setX(float value) {
	X = m_arms->X = m_body->X = value;
}

void Player::setY(float value) {
	Y = m_arms->Y = m_body->Y = value;
}

Weapon* Player::getWeapon() {
	return m_weapon;
}

void Player::setWeapon(Weapon *value) {
	if (m_weapon) {
		delete m_arms;
		delete m_weapon;
	}
	m_weapon = new Weapon(*value);
	m_arms = new StaticObject(X, Y, 128, 128, m_weapon->getPlayerTex(), false);
	AccuracyDeviation = 0;
}

Player::~Player() {
	if (!Empty) {
		delete m_body;
		delete m_arms;
		delete m_weapon;
	}
	for (unsigned int i = 0; i < m_shells.size(); i++) {
		delete m_shells[i];
	}
	m_shells.clear();
}
