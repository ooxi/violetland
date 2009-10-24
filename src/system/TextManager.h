#include <stdlib.h>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "ImageUtility.h"
#include "Texture.h"
#include "TextObject.h"

#ifndef TEXTMANAGER_H_
#define TEXTMANAGER_H_

class TextManager {
private:
	TTF_Font *m_font;
	int m_ident, m_height;
public:
	enum TextHAlignFlag {
		LEFT, CENTER, RIGHT
	};
	enum TextVAlignFlag {
		TOP, MIDDLE, BOTTOM
	};
	TextManager(string fontPath, int fontSize);
	void draw(const char *textBuf, float x, float y, TextHAlignFlag halign,
			TextVAlignFlag valign);
	const int getHeight();
	const int getIndent();
	TextObject* getObject(const char *textBuf, float x, float y,
			TextHAlignFlag halign, TextVAlignFlag valign);
	virtual ~TextManager();
};

#endif /* TEXTMANAGER_H_ */
