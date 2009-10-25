#include <iostream>
#include "StaticObject.h"
#include "FileUtility.h"

#ifndef AIM_H_
#define AIM_H_

class Aim {
private:
	GLuint m_aimDListId;
	GLuint m_pointDListId;
	void drawCircle(float r);
	void drawFilledCircle(float r);
public:
	Aim(int colorDark, int colorLight);
	void draw(float x, float y, float scale, float pointScale);
	virtual ~Aim();
};

#endif /* AIM_H_ */
