#include "TextManager.h"

#include <iostream>

TextManager::TextManager(boost::filesystem::path fontPath, int fontSize) {
	m_font = TTF_OpenFont(fontPath.string().c_str(), fontSize);
	if (!m_font) {
		std::cerr << "Couldn't initialize font: " << TTF_GetError() << std::endl;
		exit(5);
	}
	m_height = TTF_FontHeight(m_font);
	m_ident = (int) (m_height * 0.5);
}

void TextManager::draw(const std::string& textBuf, float x, float y,
		TextHAlignFlag halign, TextVAlignFlag valign) {
	if (textBuf.size() == 0)
		return;

	TextObject *textObject = TextManager::getObject(textBuf, x, y, halign,
			valign);

	textObject->draw(true, textObject->X, textObject->Y);
	delete textObject;
}

TextObject* TextManager::getObject(const std::string& textBuf, float x, float y,
		TextHAlignFlag halign, TextVAlignFlag valign) {

	SDL_Surface *textImage = ImageUtility::createImage(m_font, 255, 255, 255,
			255, 0, 0, 0, 255, textBuf, ImageUtility::blended);

	if (textImage) {
		switch (halign) {
		case LEFT:
			x += textImage->w / 2;
			break;
		case RIGHT:
			x -= textImage->w / 2;
			break;
		case CENTER:
			break;
		}

		switch (valign) {
		case TOP:
			y += textImage->h / 2;
			break;
		case BOTTOM:
			y -= textImage->h / 2;
			break;
		case MIDDLE:
			break;
		}

		Texture *tex = new Texture(textImage, 0x84F5, GL_LINEAR, true);

		TextObject *textObject = new TextObject(x, y, tex->getWidth(),
				tex->getHeight(), tex);

		return textObject;
	}

	return NULL;
}

TextManager::~TextManager() {
	TTF_CloseFont( m_font);
}
