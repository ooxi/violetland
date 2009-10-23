#include "TextManager.h"

TextManager::TextManager(string fontPath, int fontSize) {
	m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (!m_font) {
		fprintf(stderr, "Couldn't initialize font: %s\n", TTF_GetError());
		exit(5);
	}
}

void TextManager::draw(const char *textBuf, float x, float y,
		TextAlignFlag align) {
	StaticObject *textObject = TextManager::getObject(textBuf, x, y, align);
	textObject->draw(true, textObject->X, textObject->Y);
	delete textObject;
}

const int TextManager::getFontHeight() {
	return TTF_FontHeight(m_font);
}

StaticObject* TextManager::getObject(const char *textBuf, float x, float y,
		TextAlignFlag align) {
	SDL_Surface *textImage = ImageUtility::createImage(m_font, 255, 255, 255,
			255, 0, 0, 0, 255, textBuf, ImageUtility::blended);

	if (textImage) {
		switch (align) {
		case LEFT:
			x += textImage->w / 2;
			break;
		case RIGHT:
			x -= textImage->w / 2;
			break;
		case CENTER:
			break;
		}

		Texture *tex = new Texture(textImage, 0x84F5, GL_LINEAR, true);

		StaticObject *textObject = new StaticObject(x, y, tex->getWidth(),
				tex->getHeight(), tex, true);

		return textObject;
	}

	return NULL;
}

TextManager::~TextManager() {
	TTF_CloseFont(m_font);
}
