#ifndef STATICOBJECT_H_
#define STATICOBJECT_H_

#include "../Object.h"
#include "Texture.h"

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

#endif /* STATICOBJECT_H_ */
