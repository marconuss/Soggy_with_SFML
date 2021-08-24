#include "Tilemap.h"

bool CTilemap::load(const string& tileset, Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
	//load the tilset texture, breake if none is found
	if (!m_tileset.loadFromFile(tileset))
		return false;
	
	//the tiles are quads which have 4 vertices each to be stored in the array
	//resize to fit the level
	m_vertices.setPrimitiveType(Quads);
	m_vertices.resize(height * width * 4);

	//populate the vertex array with one quad per tile
	for (unsigned int i = 0; i < width; ++i) {
		for (unsigned int j = 0; j < height; ++j) {
			//get the current tilenumber in the level
			//this number is going to determine which texture it is going to be applied
			//tiles is a 1D array that represents a 2D level
			int tileNumber = tiles[i + (j * width)];

			//uv tiles coordinates in the tileset texture
			//the modulo finds the u coordinate (row)
			//the divide finds the v coordinate (column)
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			//get a pointer to the current tile's quad (with 4 vertices)
			Vertex* quad = &m_vertices[(i + j * width) * 4];

			//define its four corners
			quad[0].position = Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

		}
	}

	return true;
}

void CTilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform();

	//apply the tileset texture
	states.texture = &m_tileset;

	//draw the target vertices
	target.draw(m_vertices, states);
}


CTilemap::CTilemap()
{
}

CTilemap::~CTilemap()
{
}

