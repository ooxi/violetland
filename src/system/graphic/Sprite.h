#ifndef VIOLET_SPRITE_H_
#define VIOLET_SPRITE_H_

#include <vector>
#include "Texture.h"

namespace violet {

class Sprite {
private:
	std::vector<Texture*> m_frames;
public:
	Sprite(std::vector<SDL_Surface*> surfaces);
	Texture* getFrame(int index) const {
		return m_frames[index];
	}
	const int getFramesCount() const {
		return m_frames.size();
	}
	virtual ~Sprite();
};
}

#endif /* VIOLET_SPRITE_H_ */
