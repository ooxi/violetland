#include "Sprite.h"
#include "../utility/Templates.h"

Sprite::Sprite(std::vector<SDL_Surface*> surfaces) {
	for (unsigned int i = 0; i < surfaces.size(); i++) {
		Texture *tex = new Texture(surfaces[i], GL_TEXTURE_2D, GL_LINEAR, true);
		m_frames.push_back(tex);
	}
	surfaces.clear();
}

Sprite::~Sprite() {
	clearVector(&m_frames);
}
