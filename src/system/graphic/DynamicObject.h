#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "../Object.h"
#include "Sprite.h"
#include <cmath>

#ifndef DYNAMICOBJECT_H_
#define DYNAMICOBJECT_H_

class DynamicObject: public Object {
private:
	int m_lastFrameRollTime;
	Sprite *m_sprite;
public:
	DynamicObject(float x, float y, Sprite *sprite);
	void rollFrame(bool forward);
	void draw();
	void draw(float x, float y, float angle, float scale, float rMask,
			float gMask, float bMask, float aMask);
	void drawQuad();
	void process(int deltaTime);
	virtual ~DynamicObject();
	int Frame;
};

#endif /* DYNAMICOBJECT_H_ */
