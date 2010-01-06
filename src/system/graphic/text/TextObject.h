#include "../Texture.h"
#include "../../Object.h"

#ifndef TEXTOBJECT_H_
#define TEXTOBJECT_H_

class TextObject: public Object {
private:
	Texture* m_texture;
	void drawQuad();
public:
	TextObject(float x, float y, int w, int h, Texture *texture);
	void draw(bool outlined, float x, float y);
	virtual ~TextObject();
};

#endif /* TEXTOBJECT_H_ */
