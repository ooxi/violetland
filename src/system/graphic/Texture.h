#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../../system/utility/ImageUtility.h"

class Texture {
private:
	GLuint m_textureId;
	const GLenum m_type;
	int m_width, m_height;
public:
	Texture(int width, int height, GLenum type, GLint filter);
	Texture(SDL_Surface *surface, GLenum type, GLint filter,
			bool takeCareOfSurface);
	const int getWidth() const {
		return m_width;
	}
	const int getHeight() const {
		return m_height;
	}
	const GLuint getTextureId() const {
		return m_textureId;
	}
	const GLenum getType() const {
		return m_type;
	}

	virtual ~Texture();
};

#endif /* TEXTURE_H_ */
