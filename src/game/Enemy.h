#include "../system/graphic/DynamicObject.h"
#include "LifeForm.h"
#include <cmath>
#include <vector>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "../system/graphic/Texture.h"
#include "../system/sound/Sound.h"
#include "bullets/Bullet.h"

#ifndef ENEMY_H_
#define ENEMY_H_

class Enemy: public LifeForm {
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
	virtual void process(int deltaTime);
	virtual void draw();

	void hit(Bullet* bullet, float pX, float pY);
	void setAppearance(float scale, float rMask, float gMask, float bMask,
			float aMask);
	void rollFrame(bool forward);
	bool isBleeding();
	void destroy();
	~Enemy();
	bool DoNotDisturb;
	bool Angry;
	int blowDelay;
};

#endif /* ENEMY_H_ */
