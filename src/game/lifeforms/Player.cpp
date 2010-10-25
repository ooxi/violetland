#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Player.h"

violetland::Player::Player() :
	LifeForm(0, 0, 128, 128) {
	Id = "20-" + Id;
	Empty = true;
	Xp = 0;
	LastLevelXp = 0;
	NextLevelXp = 100;
	Kills = 0;
	LevelPoints = 0;
	ActionMode = PLAYER_ACT_MODE_FIRE;
	AccuracyDeviation = 0.0f;
	Grenades = 2;
	Teleports = 1;
	Acceleration = 0.0004f;
	Type = LIFEFORM_PLAYER;
	m_hitSndPlaying = 0;
	m_walkTime = 0;
	m_walkDelay = 0;

	m_weapon = NULL;

	m_light = m_laser = false;

	Unstoppable = PoisonBullets = BigCalibre = Telekinesis = NightVision
			= Looting = false;

	for (int i = PLAYER_BONUS_FIRST; i < PLAYER_BONUS_COUNT; i++)
		bonusTimes[i] = 0;
}

violetland::Player::Player(float x, float y, Sprite *legsSprite,
		Sprite *deathSprite, std::vector<Sound*> hitSounds, Sound* dyingSound) :
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

float violetland::Player::getStrength() {
	return Strength * ((bonusTimes[PLAYER_BONUS_STRENGTHBOOST] > 0) ? 1.2f
			: 1.0f);
}

float violetland::Player::getAgility() {
	return Agility
			* ((bonusTimes[PLAYER_BONUS_AGILITYBOOST] > 0) ? 1.2f : 1.0f);
}

float violetland::Player::getVitality() {
	return Vitality * ((bonusTimes[PLAYER_BONUS_VITALITYBOOST] > 0) ? 1.2f
			: 1.0f);
}

Sound* violetland::Player::hit(float damage, bool poison) {
	LifeForm::hit(damage, poison);

	setMask(1.0f, 0.0f, 0.0f, 1.0f);

	if (!Unstoppable)
		Speed = 0.0f;

	if (!m_hitSounds[m_hitSndPlaying]->isPlaying()) {
		m_hitSndPlaying = (getHealth() < MaxHealth() ? getHealth()
				: getHealth() - 0.01f) / MaxHealth() * m_hitSounds.size();
		return m_hitSounds[m_hitSndPlaying];
	}

	return NULL;
}

std::vector<Bullet*>* violetland::Player::fire() {
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
					&& bonusTimes[PLAYER_BONUS_PENBULLETS] > 0;
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

Bullet* violetland::Player::throwGrenade(Sprite* grenadeSprite) {
	Bullet* newBullet =
			new GrenadeBullet(X, Y, TargetX, TargetY, grenadeSprite);

	newBullet->Damage = 5.0f;
	newBullet->Speed = MaxSpeed() * 5.0f;
	newBullet->MaxRange = 1500;

	Grenades--;

	return newBullet;
}

void violetland::Player::reload() {
	if (m_weapon->reload(ReloadSpeedMod()))
		AccuracyDeviation = 0;
}

void violetland::Player::toggleLight() {
	m_light = !m_light;
}

void violetland::Player::toggleLaser() {
	m_laser = !m_laser;
}

const bool violetland::Player::getLight() {
	return m_light;
}

const bool violetland::Player::getLaser() {
	return m_laser;
}

void violetland::Player::processState(int deltaTime) {
	if (State == LIFEFORM_STATE_ALIVE) {
		m_arms->X = m_body->X = X;
		m_arms->Y = m_body->Y = Y;
		m_body->Angle = Angle;

		if (Speed > MaxSpeed() / 4) {
			m_body->rollFrame(true);
		} else {
			if (!(m_body->Frame == 8 || m_body->Frame == 20)) {
				if ((m_body->Frame > 8 && m_body->Frame < 16) || m_body->Frame
						> 20)
					m_body->rollFrame(false);
				else
					m_body->rollFrame(true);
			}
		}
	}

	if (State == LIFEFORM_STATE_DYING) {
		if (m_body->Frame == m_body->AnimSprite->getFramesCount() - 1)
			State = LIFEFORM_STATE_DIED;

		m_body->rollFrame(true);
	}

	if (State == LIFEFORM_STATE_SMITTEN) {
		const float angle = m_body->Angle;
		m_body = new DynamicObject(X, Y, m_deathSprite);
		m_body->Angle = angle;
		State = LIFEFORM_STATE_DYING;
		if (m_hitSounds[m_hitSndPlaying]->isPlaying())
			m_hitSounds[m_hitSndPlaying]->stop(0);

		m_dyingSound->play(5, 0, 0);
	}
}

void violetland::Player::processArms(int deltaTime) {
	m_arms->Angle = Object::calc_angle(X, Y, TargetX, TargetY);
	m_weapon->process(deltaTime);
	AccuracyDeviation -= deltaTime * 0.01;
	if (AccuracyDeviation < 0)
		AccuracyDeviation = 0;

}
void violetland::Player::process(int deltaTime) {
	// Base processing
	LifeForm::process(deltaTime);

	// State and animation
	processState(deltaTime);

	// Hit animation
	fadeColor(deltaTime);

	// Weapon and targeting
	processArms(deltaTime);

	// Bonuses
	processBonus(deltaTime);
}

void violetland::Player::processBonus(int deltaTime) {
	for (int i = PLAYER_BONUS_FIRST; i <= PLAYER_BONUS_COUNT; i++) {
		bonusTimes[i] -= deltaTime;
		if (bonusTimes[i] < 0)
			bonusTimes[i] = 0;
	}
}

void violetland::Player::fadeColor(int deltaTime) {
	m_arms->setMask(RMask, GMask, BMask, AMask);
	m_body->setMask(RMask, GMask, BMask, AMask);
	if (RMask < 1.0f)
		RMask += 0.01f;
	if (GMask < 1.0f)
		GMask += 0.01f;
	if (BMask < 1.0f)
		BMask += 0.01f;
	if (AMask < 1.0f)
		AMask += 0.01f;
}

void violetland::Player::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);

	if (State == LIFEFORM_STATE_ALIVE)
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

StaticObject* violetland::Player::getCorpse() {
	StaticObject * corpse = new StaticObject(X, Y, m_width, m_height,
			m_body->getFrame(), false);
	corpse->Scale = m_body->Scale;
	corpse->Angle = Object::fixAngle(180 - m_body->Angle);
	return corpse;
}

const float violetland::Player::getLegsAngle() {
	return m_body->Angle;
}

const float violetland::Player::getArmsAngle() {
	return m_arms->Angle;
}

void violetland::Player::setX(float value) {
	X = m_arms->X = m_body->X = value;
}

void violetland::Player::setY(float value) {
	Y = m_arms->Y = m_body->Y = value;
}

Weapon* violetland::Player::getWeapon() {
	return m_weapon;
}
void violetland::Player::teleport() {
	Teleports--;
	setX(TargetX);
	setY(TargetY);
}
void violetland::Player::setWeapon(Weapon *value) {
	if (m_weapon) {
		delete m_arms;
		delete m_weapon;
	}
	m_weapon = new Weapon(*value);
	m_arms = new StaticObject(X, Y, 128, 128, m_weapon->getPlayerTex(), false);
	AccuracyDeviation = 0;
}

violetland::Player::~Player() {
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
