#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <vector>
#include "../system/graphic/StaticObject.h"
#include "../system/graphic/Camera.h"

class Terrain {
private:
	int m_viewport[4];
	std::vector<StaticObject*> m_tiles;
	int m_tileWidth, m_tileHeight;
	int m_xTilesCount, m_yTilesCount;
	int m_gameAreaSize;
	GLuint m_tileDList;
	void drawOnTile(int tileX, int tileY, StaticObject *piece);
public:
	Terrain(SDL_Surface *base, std::vector<SDL_Surface*> tiles,
			int gameAreaSize);
	void beginDrawOn();
	void drawOn(StaticObject *piece);
	void endDrawOn();
	void draw(Camera *cam);
	const int getTileWidth() const {
		return m_tileWidth;
	}
	const int getTileHeight() const {
		return m_tileHeight;
	}
	~Terrain();
};

#endif /* TERRAIN_H_ */
