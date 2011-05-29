#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <vector>
#include "../system/graphic/StaticObject.h"
#include "../system/graphic/Camera.h"

class Terrain {
private:
	int m_viewport[4];
	std::vector<StaticObject*> m_tiles;
	const int m_gameAreaSize;
	const unsigned m_tileWidth, m_tileHeight;
	unsigned m_xTilesCount, m_yTilesCount;
	GLuint m_tileDList;
public:
	Terrain(SDL_Surface *base, std::vector<SDL_Surface*> tiles,
			int gameAreaSize);
	void beginDrawOn();
	void drawOn(StaticObject *piece);
	void endDrawOn();
	void draw(Camera *cam);
	int getTileWidth() const {
		return m_tileWidth;
	}
	int getTileHeight() const {
		return m_tileHeight;
	}
	~Terrain();
};

#endif /* TERRAIN_H_ */
