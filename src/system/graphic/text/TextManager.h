#ifndef TEXTMANAGER_H_
#define TEXTMANAGER_H_

#include <boost/filesystem.hpp>

#include "TextObject.h"

using namespace boost;

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
	TextManager(filesystem::path fontPath, int fontSize);
	void draw(const std::string& textBuf, float x, float y, TextHAlignFlag halign,
			TextVAlignFlag valign);
	const int getHeight();
	const int getIndent();
	TextObject* getObject(const std::string& textBuf, float x, float y,
			TextHAlignFlag halign, TextVAlignFlag valign);
	virtual ~TextManager();
};

#endif /* TEXTMANAGER_H_ */
