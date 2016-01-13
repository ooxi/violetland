#ifndef VIOLET_STATICOBJECT_H_
#define VIOLET_STATICOBJECT_H_

#include "../Object.h"
#include "Texture.h"

namespace violet {

class StaticObject: public Object {
private:
	Texture* m_texture;
	bool m_takeCareOfTexture;
protected:
	StaticObject(float x, float y, int w, int h);
public:
	StaticObject(float x, float y, int w, int h, Texture *texture,
			bool takeCareOfTexture);
	GLuint createComplexFace(int facesCount);
	void draw(GLuint dListId);
	void draw(bool hreflect, bool vreflect);
	void draw(bool hreflect, bool vreflect, float x, float y, float angle,
			float scale);
	void setTexture(Texture* texture, bool takeCareOfTexture);
	Texture* getTexture() const {
		return m_texture;
	}
	virtual ~StaticObject();
};
}

#endif /* VIOLET_STATICOBJECT_H_ */
