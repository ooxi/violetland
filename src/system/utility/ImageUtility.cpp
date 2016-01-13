#include "ImageUtility.h"

SDL_Surface* violet::ImageUtility::loadImage(boost::filesystem::path fileName) {
	SDL_Surface *image = IMG_Load(fileName.string().c_str());
	if (image == NULL) {
		std::cerr << "Couldn't load " << fileName << ": " << SDL_GetError() << std::endl;
		throw 1;
	}

	SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(
		image->format, 255, 0, 255));

	SDL_Surface *optimizedImage = SDL_DisplayFormatAlpha(image);
	SDL_FreeSurface(image);

	return optimizedImage;
}

SDL_Surface* violet::ImageUtility::createImage(
		TTF_Font *font,
		Uint8 fgR, Uint8 fgG, Uint8 fgB, Uint8 fgA,
		Uint8 bgR, Uint8 bgG, Uint8 bgB, Uint8 bgA,
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

float violet::ImageUtility::getColorChR(int color) {
	return (color / 65536) / 255.0;
}

float violet::ImageUtility::getColorChG(int color) {
	return ((color - (color / 65536) * 65536) / 256) / 255.0;
}

float violet::ImageUtility::getColorChB(int color) {
	return (color - (color / 65536) * 65536
			- ((color - (color / 65536) * 65536) / 256) * 256) / 255.0;
}
