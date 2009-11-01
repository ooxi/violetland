#include "../system/graphic/DynamicObject.h"
#include "LiveObject.h"
#include <cmath>
#include <vector>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "../system/graphic/Texture.h"
#include "../system/sound/Sound.h"
#include "Bullet.h"

#ifndef ENEMY_H_
#define ENEMY_H_

class Enemy: public LiveObject {
private:
	DynamicObject* m_body;
	Sprite *m_bleedSprite;
	std::vector<DynamicObject*> m_bleeds;
	Sound* m_hitSound;
	int m_hitSoundChannel;
	int m_bleeding;
public:
	Enemy(float x, float y, Sprite *sprite, Sprite *bleedSprite,
			Sound* hitSound);
	void hit(Bullet* bullet, float pX, float pY);
	void process(int deltaTime);
	void draw();
	void setAppearance(float scale, float rMask, float gMask, float bMask,
			float aMask);
	void rollFrame(bool forward);
	bool isBleeding();
	void destroy();
	~Enemy();
	bool DoNotDisturb;
	bool Angry;
	int blowDelay;
	float TargetX, TargetY;
	bool Poisoned;
};

#endif /* ENEMY_H_ */
