#include <stdlib.h>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "ImageUtility.h"
#include "Texture.h"
#include "StaticObject.h"

#ifndef TEXTOBJECT_H_
#define TEXTOBJECT_H_

class TextManager {
private:
	TTF_Font *m_font;
public:
	enum TextAlignFlag {
		LEFT, CENTER, RIGHT
	};
	TextManager(string fontPath, int fontSize);
	void draw(const char *textBuf, float x, float y, TextAlignFlag align);
	const int getFontHeight();
	StaticObject* getObject(const char *textBuf, float x, float y,
			TextAlignFlag align);
	virtual ~TextManager();
};

#endif /* TEXTOBJECT_H_ */
