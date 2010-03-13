#ifndef AIM_H_
#define AIM_H_

#include "StaticObject.h"
#include "../Configuration.h"

class Aim {
private:
	GLuint m_aimDListId;
	GLuint m_pointDListId;
	void drawCircle(float r);
	void drawFilledCircle(float r);
public:
	Aim(Configuration* config);
	void draw(float x, float y, float scale, float pointScale);
	virtual ~Aim();
};

#endif /* AIM_H_ */
