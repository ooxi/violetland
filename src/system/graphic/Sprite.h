#ifndef SPRITE_H_
#define SPRITE_H_

#include <vector>
#include "Texture.h"

class Sprite {
private:
	std::vector<Texture*> m_frames;
public:
	Sprite(std::vector<SDL_Surface*> surfaces);
	Texture* getFrame(int index);
	const int getFramesCount();
	virtual ~Sprite();
};

#endif /* SPRITE_H_ */
