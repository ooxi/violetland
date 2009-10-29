#include "Sprite.h"

Sprite::Sprite(std::vector<SDL_Surface*> surfaces) {
	for (unsigned int i = 0; i < surfaces.size(); i++) {
		Texture *tex = new Texture(surfaces[i], GL_TEXTURE_2D, GL_LINEAR, true);
		m_frames.push_back(tex);
	}
	surfaces.clear();
}

Texture* Sprite::getFrame(int index) {
	return m_frames[index];
}

const int Sprite::getFramesCount() {
	return m_frames.size();
}

Sprite::~Sprite() {
	for (unsigned int i = 0; i < m_frames.size(); i++) {
		m_frames.erase(m_frames.begin() + i);
	}
	m_frames.clear();
}
