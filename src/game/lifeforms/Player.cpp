#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Player.h"
#include "../../system/utility/Templates.h"

namespace violet {

    Player::Player(float x, float y, Sprite *legsSprite, Sprite *deathSprite,
		   Sprite *shieldSprite, std::vector<Sound*> hitSounds,
		   Sound* dyingSound) :
	LifeForm(x, y, 128, 128) {
	Id = "20-" + Id;
	Xp = 0;
	LastLevelXp = 0;
	NextLevelXp = 200;
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
			= Looting = WideSight = Magneto = false;

	for (int i = PLAYER_BONUS_FIRST; i < PLAYER_BONUS_COUNT; i++)
		bonusTimes[i] = 0;

	processTelekinesis(0, true);

	TargetX = x;
	TargetY = y;

	m_weapon = NULL;
	m_arms = NULL;
	m_body = new DynamicObject(x, y, legsSprite);
	m_body->Frame = 8;
	m_shield = new DynamicObject(x, y, shieldSprite);
	m_deathSprite = deathSprite;
	m_hitSounds = hitSounds;
	m_dyingSound = dyingSound;
}

float Player::getStrength() const {
	return Strength * ((bonusTimes[PLAYER_BONUS_STRENGTHBOOST] > 0) ? 1.2f
			: 1.0f);
}

float Player::getAgility() const {
	return Agility
			* ((bonusTimes[PLAYER_BONUS_AGILITYBOOST] > 0) ? 1.2f : 1.0f);
}

float Player::getVitality() const {
	return Vitality * ((bonusTimes[PLAYER_BONUS_VITALITYBOOST] > 0) ? 1.2f
			: 1.0f);
}

Sound* Player::hit(float damage, bool poison) {
    if(this->bonusTimes[PLAYER_BONUS_SHIELD]>0)
	return NULL;
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

std::vector<Bullet*>* Player::fire() {
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
			bullet->Poisoned = m_weapon->Type == BULLET_STANDARD
					&& PoisonBullets;
			bullet->BigCalibre = m_weapon->Type == BULLET_STANDARD
					&& BigCalibre;
			if (BigCalibre) {
				bullet->Damage *= 1.1f;
			}
			bullet->Penetrating = m_weapon->Type == BULLET_STANDARD
					&& bonusTimes[PLAYER_BONUS_PENBULLETS] > 0;
			bullet->Angle = AccuracyDeviation < 1 ? m_arms->Angle
					: m_arms->Angle + (rand() % (int) (AccuracyDeviation * 2))
							- AccuracyDeviation;
			bullet->OwnerId = Id;
		}

		if (m_weapon->BulletsAtOnce > 1)
			AccuracyDeviation = 0;

		if (m_weapon->ShellSprite)
		{
			DynamicObject* shell = new DynamicObject(X, Y, m_weapon->ShellSprite);
			shell->Angle = m_arms->Angle;
			m_shells.push_back(shell);
		}
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

void Player::processState(int deltaTime) {
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
		delete m_body;
		m_body = new DynamicObject(X, Y, m_deathSprite);
		m_body->Angle = angle;
		State = LIFEFORM_STATE_DYING;
		if (m_hitSounds[m_hitSndPlaying]->isPlaying())
			m_hitSounds[m_hitSndPlaying]->stop(0);

		m_dyingSound->play(5, 0, 0);
	}
}

void Player::processArms(int deltaTime) {
	m_arms->Angle = Object::calc_angle(X, Y, TargetX, TargetY);
	m_weapon->process(deltaTime);
	AccuracyDeviation -= deltaTime * 0.01f;
	if (AccuracyDeviation < 0)
		AccuracyDeviation = 0;

}
void Player::process(int deltaTime) {
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

	// Roll shield state
	m_shield->rollFrame(true);

}

void Player::processBonus(int deltaTime) {
	for (int i = PLAYER_BONUS_FIRST; i < PLAYER_BONUS_COUNT; i++) {
		bonusTimes[i] -= deltaTime;
		if (bonusTimes[i] < 0)
			bonusTimes[i] = 0;
	}
}

void Player::fadeColor(int deltaTime) {
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

void Player::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);

	if (State == LIFEFORM_STATE_ALIVE)
		m_arms->draw(false, false);

	//  a not so elegant solution...
	//  there are 31 identical shield animation image, and the AMask is changed based on the frame count
	if(this->bonusTimes[PLAYER_BONUS_SHIELD] > 0)
	{
	    m_shield->draw(X, Y, 0.0f, Scale, RMask, GMask, BMask, 0.2f + 0.7f / m_shield->AnimSprite->getFramesCount() * m_shield->Frame);
	}

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

void Player::setX(float value) {
	X = m_arms->X = m_body->X = value;
}

void Player::setY(float value) {
	Y = m_arms->Y = m_body->Y = value;
}

void Player::teleport() {
	Teleports--;
	setX(TargetX);
	setY(TargetY);
}

void Player::setWeapon(Weapon *value) {
	float angle = 0;
	if (m_weapon) {
		angle = getArmsAngle();
		delete m_arms;
		delete m_weapon;
	}
	m_weapon = new Weapon(*value);
	m_arms = new StaticObject(X, Y, 128, 128, m_weapon->getPlayerTex(), false);
	m_arms->Angle = angle;
	AccuracyDeviation = 0;
}

// Controls telekinesis ability
// Reset = true to reset timer (obvious!), deltaTime will be ignored, will return 0
// Reset = false and deltaTime >= 0 to evaluate timer, returns percentage of telekinesis process (100 or more to complete)
unsigned Player::processTelekinesis(int deltaTime, bool reset)
{
	if (reset)
	{
		m_telekinesisDelay = TELEKINESIS_DELAY;
		return 0;
	}
	else
	{
		m_telekinesisDelay -= deltaTime;
		return (unsigned)((float)(TELEKINESIS_DELAY - m_telekinesisDelay) / TELEKINESIS_DELAY * 10) * 10;
	}
}

Player::~Player() {
	if (m_body)
		delete m_body;

	if (m_arms)
		delete m_arms;

	if (m_weapon)
		delete m_weapon;

	clearVector<DynamicObject*> (&m_shells);
}

}
