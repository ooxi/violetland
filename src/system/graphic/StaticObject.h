#include "SDL_opengl.h"
#include "../utility/ImageUtility.h"
#include "../Object.h"
#include "Texture.h"

#ifndef STATICOBJECT_H_
#define STATICOBJECT_H_

class StaticObject: public Object {
private:
	int m_past;
	Texture* m_texture;
	bool m_takeCareOfTexture;
protected:
	StaticObject(float x, float y, int w, int h);
public:
	StaticObject(float x, float y, int w, int h, Texture *texture,
			bool takeCareOfTexture);
	GLuint createComplexFace(int facesCount);
	void draw(bool hreflect, bool vreflect);
	void draw(bool hreflect, bool vreflect, float x, float y, float angle,
			float scale);
	void draw(GLuint dListId);
	void setTexture(Texture* texture, bool takeCareOfTexture);
	Texture* getTexture();
	virtual ~StaticObject();
};

#endif /* STATICOBJECT_H_ */
