#include "Terrain.h"

Terrain::Terrain(SDL_Surface *surface, std::vector<SDL_Surface*> tiles,
		int gameAreaSize) {
	m_tileWidth = surface->w;
	m_tileHeight = surface->h;

	m_gameAreaSize = gameAreaSize;

	m_xTilesCount = (int) ceil(m_gameAreaSize * 2.0 / m_tileWidth);
	m_yTilesCount = (int) ceil(m_gameAreaSize * 2.0 / m_tileHeight);

	int tilesStartX = -m_gameAreaSize + m_tileWidth / 2;
	int tilesStartY = -m_gameAreaSize + m_tileHeight / 2;

	for (int i = 0; i < m_xTilesCount; i++) {
		for (int j = 0; j < m_yTilesCount; j++) {
			Texture* baseTex = new Texture(surface, GL_TEXTURE_2D, GL_NEAREST,
					false);
			StaticObject *newTile = new StaticObject(tilesStartX + i
					* m_tileWidth, tilesStartY + j * m_tileHeight, 256, 256,
					baseTex, true);

			if (i == 0 && j == 0)
				m_tileDList = newTile->createComplexFace(16);

			m_tiles.push_back(newTile);
		}
	}

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
}

void Terrain::beginDrawOn() {
	glGetIntegerv(GL_VIEWPORT, (GLint*) m_viewport);
	glViewport(0, 0, m_tileWidth, m_tileHeight);
}

void Terrain::drawOn(StaticObject *piece) {
	int tileX = (piece->X + m_gameAreaSize) / m_tileWidth;
	int tileY = (piece->Y + m_gameAreaSize) / m_tileHeight;

	int tileIndex = tileX * m_yTilesCount + tileY;

	drawOnTile(tileX, tileY, piece);

	if (tileIndex < 0 || tileIndex > (signed) m_tiles.size() - 1)
		return;

	if (piece->getLeft() < m_tiles[tileIndex]->getLeft()) {
		drawOnTile(tileX - 1, tileY, piece);
		if (piece->getTop() < m_tiles[tileIndex]->getTop()) {
			drawOnTile(tileX - 1, tileY - 1, piece);
		}
		if (piece->getBottom() > m_tiles[tileIndex]->getBottom()) {
			drawOnTile(tileX - 1, tileY + 1, piece);
		}
	}
	if (piece->getRight() > m_tiles[tileIndex]->getRight()) {
		drawOnTile(tileX + 1, tileY, piece);
		if (piece->getTop() < m_tiles[tileIndex]->getTop()) {
			drawOnTile(tileX + 1, tileY - 1, piece);
		}
		if (piece->getBottom() > m_tiles[tileIndex]->getBottom()) {
			drawOnTile(tileX + 1, tileY + 1, piece);
		}
	}
	if (piece->getTop() < m_tiles[tileIndex]->getTop()) {
		drawOnTile(tileX, tileY - 1, piece);
	}
	if (piece->getBottom() > m_tiles[tileIndex]->getBottom()) {
		drawOnTile(tileX, tileY + 1, piece);
	}
}

void Terrain::endDrawOn() {
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void Terrain::draw(Camera* cam) {
	float right = cam->X + cam->getHalfW();
	float left = cam->X - cam->getHalfW();
	float bottom = cam->Y + cam->getHalfH();
	float top = cam->Y - cam->getHalfH();
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		if (m_tiles[i]->getLeft() < right
				&& m_tiles[i]->getRight() > left
				&& m_tiles[i]->getTop() < bottom
				&& m_tiles[i]->getBottom() > top)
			m_tiles[i]->draw(m_tileDList);
	}
}

void Terrain::drawOnTile(int tileX, int tileY, StaticObject *piece) {
	if (tileX < 0 || tileX > m_xTilesCount - 1 || tileY < 0 || tileY
			> m_yTilesCount - 1)
		return;

	int tileIndex = tileX * m_yTilesCount + tileY;

	Texture* tex = new Texture(m_tileWidth, m_tileHeight, GL_TEXTURE_2D,
			GL_NEAREST);

	int halfTileWidth = m_tileWidth / 2;
	int halfTileHeight = m_tileHeight / 2;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_tiles[tileIndex]->X - halfTileWidth, m_tiles[tileIndex]->X
			+ halfTileWidth, m_tiles[tileIndex]->Y + halfTileHeight,
			m_tiles[tileIndex]->Y - halfTileHeight, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_tiles[tileIndex]->draw(false, true);

	float pieceY = piece->Y;
	float inTilePositionY = (piece->Y + m_gameAreaSize) - tileY * m_tileHeight;
	piece->Y = (m_tiles[tileIndex]->Y + halfTileHeight - inTilePositionY);
	piece->draw(true, false);
	piece->Y = pieceY;

	glBindTexture(tex->getType(), tex->getTextureId());
	glCopyTexImage2D(tex->getType(), 0, GL_RGB, 0, 0, m_tileWidth,
			m_tileHeight, 0);
	m_tiles[tileIndex]->setTexture(tex, true);
}

Terrain::~Terrain() {
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		delete m_tiles[i];
	}
	m_tiles.clear();
	glDeleteLists(m_tileDList, 1);
}
