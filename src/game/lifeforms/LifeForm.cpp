#include <sstream>

#include "LifeForm.h"

using namespace violet;

LifeForm::LifeForm(float x, float y, int w, int h) :
	Object(x, y, w, h) {
	std::ostringstream buf;
	buf << UidGenerator::getInstance()->getId();
	Id = buf.str();
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
	Weight = 1.0f;
	m_health = MaxHealth();
	State = LIFEFORM_STATE_ALIVE;
	m_lastAttackTime = SDL_GetTicks();
	TargetX = TargetY = 0.0f;
	Poisoned = false;
	m_walking = false;
	Frozen = 0;
	Burning = false;
	Level = 1;
	Name = "Unknown lifeform";
}

void LifeForm::draw() {
	// nothing
}

Sound* LifeForm::hit(float damage, bool poison) {
	setHealth(getHealth() - damage);

	if (getHealth() == 0 && State == LIFEFORM_STATE_ALIVE) {
		State = LIFEFORM_STATE_SMITTEN;
	}

	return NULL;
}

void LifeForm::process(int deltaTime) {
	if (Frozen > 0) {
		Frozen -= deltaTime;
		if (Frozen < 0)
			Frozen = 0;
	}

	if (State == LIFEFORM_STATE_ALIVE && Frozen == 0) {
		setHealth(getHealth() + HealthRegen() * deltaTime);

		if (Poisoned)
			setHealth(getHealth() - 0.0004f * deltaTime);

		if (Burning) {
			RMask -= 0.02 / MaxHealth();
			if (RMask < 0)
				RMask = 0;
			GMask -= 0.02 / MaxHealth();
			if (GMask < 0)
				GMask = 0;
			BMask -= 0.02 / MaxHealth();
			if (BMask < 0)
				BMask = 0;
			AMask += 0.01 / MaxHealth();
			if (AMask > 1)
				AMask = 1;

			setHealth(getHealth() - 0.0005f * deltaTime);
		}

		if (!m_walking) {
			Speed -= Acceleration * deltaTime;
			if (Speed < 0)
				Speed = 0;
		}

		m_walking = false;

		Object::move(deltaTime);
	}
}

void LifeForm::move(float direction, int deltaTime) {

	if ( fabs(Speed) > 2.0f * MaxSpeed() )
		Speed *= 0.5f;

	if (m_walkDelay > 0)
		return;

	m_walking = true;

	//bla: Collisions or pushes can cause Speed > MaxSpeed, so deceleration is needed.
	//bla: If Speed close to MaxSpeed, set it to MaxSpeed. Speed cap is 2 * MaxSpeed.
	if ( Speed != MaxSpeed() ) {
		if ( Speed < MaxSpeed() )
			Speed += Acceleration * deltaTime;
		if ( Speed - Acceleration * deltaTime < MaxSpeed() )
			Speed = MaxSpeed();
		else
			Speed -= Acceleration * deltaTime;
	}

	turn(direction, MaxSpeed(), deltaTime);
}

void LifeForm::collisionPush(LifeForm* lf) {

	//bla: Overlapping distance seperated by axis, collision angle and ratio.
	const float ratio = 0.5f * ( getWeight() / (getWeight() + lf->getWeight())
			+ getStrength() / (getStrength() + lf->getStrength()) );
	const float collAngle = calc_angle(X, Y, lf->X, lf->Y);
	const float diffX = fabs(sin(collAngle * M_PI / 180.0f)) * (
			HitR * Scale * m_width + lf->HitR * lf->Scale * lf->m_width
			- calc_dist(X, Y, lf->X, lf->Y) );
	const float diffY = fabs(cos(collAngle * M_PI / 180.0f)) * (
			HitR * Scale * m_width + lf->HitR * lf->Scale * lf->m_width
			- calc_dist(X, Y, lf->X, lf->Y) );

	//bla: Resolve X, Y.
	if ( X > lf->X ) {
		X		+= (1.0f - ratio) * diffX;
		lf->X	-= ratio * diffX;
	} else {
		X		-= (1.0f - ratio) * diffX;
		lf->X	+= ratio * diffX;
	}
	if ( Y > lf->Y ) {
		Y		+= (1.0f - ratio) * diffY;
		lf->Y	-= ratio * diffY;
	} else {
		Y		-= (1.0f - ratio) * diffY;
		lf->Y	+= ratio * diffY;
	}

	//bla: Exceptions for Speed/Angle changes.
	if ( Frozen > 0 || lf->Frozen > 0)
		return;

	//bla: Speed transfer.
	const float newSpeed1 = (1.0f - ratio) * fabs((lf->Speed+Speed)/2.0f) * cos((collAngle - Angle) * M_PI / 180.0f);
	const float newSpeed2 = ratio * fabs((Speed+lf->Speed)/2.0f) * cos((collAngle - lf->Angle - 180.0f) * M_PI / 180.0f);
	lf->Speed -= 0.9f * newSpeed2;
	Speed -= 0.9f * newSpeed1;
}

const float LifeForm::MaxHealth() const {
	return ( getVitality() + getStrength() ) > 0.6f ?
			( 4 * getVitality() + getStrength() ) / ( getStrength() + getVitality() + 3.0f )
			: 0.2f;
}

const float LifeForm::ChanceToEvade() const {
	float chance = ( getAgility() + getStrength() - 2.0f ) / ( getAgility() + getStrength() + 2.0f );
	if (chance < 0) chance = 0;
	return chance;
}

const bool LifeForm::Attack() {
	int now = SDL_GetTicks();

	if (now - m_lastAttackTime > AttackDelay()) {
		m_lastAttackTime = now;
		return true;
	} else {
		return false;
	}
}

const float LifeForm::Damage() const {
	return getStrength() / ( getStrength() + 6.0f );
}

const int LifeForm::AttackDelay() const {
	return (1.0f - (getAgility() - 1.0f) / 2.0f) * 1000;
}

const float LifeForm::MaxSpeed() const {
	return 1.5f * getAgility() / ( getAgility() * getWeight() + 7.5f );
}

const float LifeForm::HealthRegen() const {
	return getVitality() > 1.0f ? (getVitality() - 1.0f) * 0.000006f : 0.0f;
}

const float LifeForm::ReloadSpeedMod() const {
	return 2.0f / 2.0f * getAgility();
}

const float LifeForm::WeaponRetForceMod() const {
	return 1.0f - ( getStrength() - 1.0f ) / ( getStrength() + 1.0f );
}

const float LifeForm::fixHealth(float health) const {
	if (health > MaxHealth())
		return MaxHealth();
	else if (health < 0)
		return 0;
	else
		return health;
}

LifeForm::~LifeForm()
{
	// nothing
}
