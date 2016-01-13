#ifndef IMAGEUTILITY_H_
#define IMAGEUTILITY_H_

#include <iostream>
#include <boost/filesystem.hpp>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_opengl.h"



namespace violet {

class ImageUtility {
public:
	enum FontRenderingFlag {
		solid, shaded, blended
	};
	static SDL_Surface *loadImage(boost::filesystem::path);
	static SDL_Surface *createImage(
			TTF_Font *font,
			Uint8 fgR, Uint8 fgG, Uint8 fgB, Uint8 fgA,
			Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA,
			std::string text, FontRenderingFlag quality);
	static float getColorChR(int color);
	static float getColorChG(int color);
	static float getColorChB(int color);
};
}

#endif /* IMAGEUTILITY_H_ */
