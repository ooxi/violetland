#ifndef DYNAMICOBJECT_H_
#define DYNAMICOBJECT_H_

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "../Object.h"
#include "Sprite.h"

class DynamicObject: public Object {
private:
	int m_lastFrameRollTime;
public:
	DynamicObject(float x, float y, Sprite *sprite);
	void rollFrame(bool forward);
	void draw();
	void draw(float x, float y, float angle, float scale, float rMask,
			float gMask, float bMask, float aMask);
	Texture* getFrame() const {
		return AnimSprite->getFrame(Frame);
	}
	void process(int deltaTime) {
		Object::move(deltaTime);
	}
	virtual ~DynamicObject();
	int Frame;
	Sprite* AnimSprite;
};

#endif /* DYNAMICOBJECT_H_ */
