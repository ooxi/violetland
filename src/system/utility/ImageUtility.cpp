#include "ImageUtility.h"

SDL_Surface *ImageUtility::loadImage(filesystem::path fileName, float prescale) {
	SDL_Surface *image = IMG_Load(fileName.string().c_str());
	if (image == NULL) {
		// visual studio compiler can't compile [ ... << const std::string ... ] ?!
		std::cerr << "Couldn't load " << fileName << ": " << SDL_GetError() << std::endl;
		throw 1;
	}

	SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(
			image->format, 255, 0, 255));

	SDL_Surface *optimizedImage = SDL_DisplayFormatAlpha(image);
	SDL_FreeSurface(image);

	if (prescale != 1.0)
		return ImageUtility::scaleSurface(optimizedImage, optimizedImage->w
				* prescale, optimizedImage->h * prescale);
	else
		return optimizedImage;
}

SDL_Surface *ImageUtility::createImage(TTF_Font *font, char fgR, char fgG,
		char fgB, char fgA, char bgR, char bgG, char bgB, char bgA,
		std::string text, FontRenderingFlag quality) {
	SDL_Color tmpfontcolor = { fgR, fgG, fgB, fgA };
	SDL_Color tmpfontbgcolor = { bgR, bgG, bgB, bgA };
	SDL_Surface *resulting_text = NULL;

	if (quality == solid)
		resulting_text = TTF_RenderUTF8_Solid(font, text.c_str(), tmpfontcolor);
	else if (quality == shaded)
		resulting_text = TTF_RenderUTF8_Shaded(font, text.c_str(),
				tmpfontcolor, tmpfontbgcolor);
	else if (quality == blended)
		resulting_text = TTF_RenderUTF8_Blended(font, text.c_str(),
				tmpfontcolor);

	SDL_Surface *optimizedImage = SDL_DisplayFormatAlpha(resulting_text);
	SDL_FreeSurface(resulting_text);

	return optimizedImage;
}

SDL_Surface *ImageUtility::scaleSurface(SDL_Surface *Surface, int width,
		int height) {
	SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, width, height,
			Surface->format->BitsPerPixel, Surface->format->Rmask,
			Surface->format->Gmask, Surface->format->Bmask,
			Surface->format->Amask);

	double _stretch_factor_x = (static_cast<double> (width)
			/ static_cast<double> (Surface->w)), _stretch_factor_y =
			(static_cast<double> (height) / static_cast<double> (Surface->h));

	for (int y = 0; y < Surface->h; y++)
		for (int x = 0; x < Surface->w; x++)
			for (int o_y = 0; o_y < _stretch_factor_y; ++o_y)
				for (int o_x = 0; o_x < _stretch_factor_x; ++o_x)
					ImageUtility::drawPixel(_ret,
							static_cast<int> (_stretch_factor_x * x) + o_x,
							static_cast<int> (_stretch_factor_y * y) + o_y,
							ImageUtility::readPixel(Surface, x, y));

	SDL_FreeSurface(Surface);

	return _ret;
}

Uint32 ImageUtility::readPixel(SDL_Surface *surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *) p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *) p;
	default:
		return 0;
	}
}

void ImageUtility::drawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *) p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *) p = pixel;
		break;
	}
}

float ImageUtility::getColorChR(int color) {
	return (color / 65536) / 255.0;
}

float ImageUtility::getColorChG(int color) {
	return ((color - (color / 65536) * 65536) / 256) / 255.0;
}

float ImageUtility::getColorChB(int color) {
	return (color - (color / 65536) * 65536
			- ((color - (color / 65536) * 65536) / 256) * 256) / 255.0;
}
