#include <algorithm>

using namespace std;

#include "Terrain.h"

Terrain::Terrain(SDL_Surface *surface, std::vector<SDL_Surface*> tiles,
		int gameAreaSize) : m_gameAreaSize(gameAreaSize),
		m_tileWidth(surface->w), m_tileHeight(surface->h) {
	m_xTilesCount = (unsigned) ceil(m_gameAreaSize * 2.0 / m_tileWidth);
	m_yTilesCount = (unsigned) ceil(m_gameAreaSize * 2.0 / m_tileHeight);

	int tilesStartX = -m_gameAreaSize + m_tileWidth / 2;
	int tilesStartY = -m_gameAreaSize + m_tileHeight / 2;

	std::cout << "Creating terrain tiles from the base texture..." << std::endl;

	for (unsigned i = 0; i < m_xTilesCount; ++i) {
		for (unsigned j = 0; j < m_yTilesCount; ++j) {
			Texture* baseTex = new Texture(surface, GL_TEXTURE_2D, GL_NEAREST,
					false);
			StaticObject *newTile = new StaticObject(
					tilesStartX + int(i * m_tileWidth), 
					tilesStartY + int(j * m_tileHeight), 
					256, 256, baseTex, true);
			m_tiles.push_back(newTile);
		}
	}
	m_tileDList = m_tiles[0]->createComplexFace(16);

	std::cout << "Painting terrain tiles with debris textures..." << std::endl;

	Terrain::beginDrawOn();

	for (unsigned int j = 0; j < tiles.size(); j++) {
		Texture* tileTex = new Texture(tiles[j], GL_TEXTURE_2D, GL_NEAREST,
				false);
		StaticObject *piece = new StaticObject(0, 0, 128, 128, tileTex, true);
		for (int i = 0; i < m_gameAreaSize / 4; i++) {
			piece->X = (float) (rand() % (int) (m_gameAreaSize * 2))
					- m_gameAreaSize;
			piece->Y = (float) (rand() % (int) (m_gameAreaSize * 2))
					- m_gameAreaSize;
			piece->AMask = 1.0 - (rand() % 50) / 100.0;
			piece->Angle = (rand() % 360);

			Terrain::drawOn(piece);
		}
		delete piece;
	}

	Terrain::endDrawOn();

	std::cout << "Terrain has been generated successfully." << std::endl;
}

void Terrain::beginDrawOn() {
	glGetIntegerv(GL_VIEWPORT, (GLint*) m_viewport);
	glViewport(0, 0, m_tileWidth, m_tileHeight);
}

void Terrain::drawOn(StaticObject *piece) {
	const float right = piece->getRight();
	const float left = piece->getLeft();
	const float bottom = piece->getBottom();
	const float top = piece->getTop();
	
	if (top < -m_gameAreaSize || left < -m_gameAreaSize || 
		bottom > m_gameAreaSize || right > m_gameAreaSize)
		return;
	
	unsigned i0 = max(left+m_gameAreaSize, 0.0f)/m_tileWidth;
	unsigned i1 = min(int(ceil((right+m_gameAreaSize)/m_tileWidth)), m_xTilesCount);
	unsigned j0 = max(top+m_gameAreaSize, 0.0f)/m_tileHeight;
	unsigned j1 = min(int(ceil((bottom+m_gameAreaSize)/m_tileHeight)), m_yTilesCount);
	const unsigned halfTileWidth = m_tileWidth / 2;
	const unsigned halfTileHeight = m_tileHeight / 2;
	
	for (unsigned i = i0; i < i1; ++i)
		for (unsigned j = j0; j < j1; ++j) {
			StaticObject* tile = m_tiles[i*m_yTilesCount+j];
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(tile->X - halfTileWidth, tile->X + halfTileWidth, 
					tile->Y + halfTileHeight, tile->Y - halfTileHeight, 
					-1.0, 1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			tile->draw(false, true);

			float pieceY = piece->Y;
			piece->Y = 2*tile->Y - pieceY;
			piece->draw(true, false);
			piece->Y = pieceY;

			Texture* tex = tile->getTexture();
			glBindTexture(tex->getType(), tex->getTextureId());
			glCopyTexImage2D(tex->getType(), 0, GL_RGB, 0, 0, m_tileWidth,
					m_tileHeight, 0);
		}
}

void Terrain::endDrawOn() {
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void Terrain::draw(Camera* cam) {
	const float right = cam->X + cam->getHalfW();
	const float left = cam->X - cam->getHalfW();
	const float bottom = cam->Y + cam->getHalfH();
	const float top = cam->Y - cam->getHalfH();
	
	unsigned i0 = (left+m_gameAreaSize)/m_tileWidth;
	unsigned i1 = ceil((right+m_gameAreaSize)/m_tileWidth);
	unsigned j0 = (top+m_gameAreaSize)/m_tileHeight;
	unsigned j1 = ceil((bottom+m_gameAreaSize)/m_tileHeight);
	
	for (unsigned i = i0; i < i1; ++i)
		for (unsigned j = j0; j < j1; ++j)
			m_tiles[i*m_yTilesCount+j]->draw(m_tileDList);
}

Terrain::~Terrain() {
	for (unsigned i = 0; i < m_tiles.size(); ++i) {
		delete m_tiles[i];
	}
	m_tiles.clear();
	glDeleteLists(m_tileDList, 1);
}
