#ifndef IMAGEUTILITY_H_
#define IMAGEUTILITY_H_

#include <iostream>
#include <boost/filesystem.hpp>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_opengl.h"

using namespace boost;

class ImageUtility {
public:
	enum FontRenderingFlag {
		solid, shaded, blended
	};
	static SDL_Surface *loadImage(filesystem::path, float prescale = 1.0);
	static SDL_Surface *createImage(TTF_Font *font, char fgR, char fgG,
			char fgB, char fgA, char bgR, char bgG, char bgB, char bgA,
			std::string text, FontRenderingFlag quality);
	static SDL_Surface *scaleSurface(SDL_Surface *Surface, int width,
			int height);
	static Uint32 readPixel(SDL_Surface *surface, int x, int y);
	static void drawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
	static float getColorChR(int color);
	static float getColorChG(int color);
	static float getColorChB(int color);
};

#endif /* IMAGEUTILITY_H_ */
