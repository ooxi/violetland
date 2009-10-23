#include <vector>
#include <iostream>
#include <cmath>
#include "../system/StaticObject.h"

#ifndef TERRAIN_H_
#define TERRAIN_H_

class Terrain {
private:
	int m_viewport[4];
	vector<StaticObject*> m_tiles;
	int m_tileWidth, m_tileHeight;
	int m_xTilesCount, m_yTilesCount;
	int m_gameAreaSize;
	GLuint m_tileDList;
	void drawOnTile(int tileX, int tileY, StaticObject *piece);
public:
	Terrain(SDL_Surface *base, vector<SDL_Surface*> tiles, int gameAreaSize);
	void beginDrawOn();
	void drawOn(StaticObject *piece);
	void endDrawOn();
	void draw();
	const int getTileWidth();
	const int getTileHeight();
	~Terrain();
};

#endif /* TERRAIN_H_ */
