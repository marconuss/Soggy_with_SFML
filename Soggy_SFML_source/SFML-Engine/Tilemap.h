#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;
using namespace sf;


class CTilemap : public Drawable, public Transformable
{
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Texture			m_tileset;
	VertexArray		m_vertices;

	//constrctor and deconstructor
	CTilemap();
	~CTilemap ();

	//functions:
	bool load(const string& tileset, Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
};
#pragma once
