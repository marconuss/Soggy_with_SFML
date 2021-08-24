#include "Scene.h"

//constructor
CScene::CScene()
{
	m_path = "";
	m_sceneNumber = 0;
	m_width = m_height = 0;
	m_tileSize = 0;
	m_tileIndexes = {};
	m_collisionTiles = {};

	m_pTilemap = new CTilemap();
	m_pTiles = new vector<Tile>;
	m_pInteractables = new vector<Interactable>;
}

//deconstructor
CScene::~CScene()
{
	delete(m_pTilemap);
	delete(m_pTiles);
	delete(m_pInteractables);
}

bool CScene::loadScene() {
	//fills up the vector of Tiles with the all the required information 

	int* sceneTiles = this->m_tileIndexes.data();
 

	if (!m_pTilemap->load(this->m_path, Vector2u(m_tileSize, m_tileSize), sceneTiles, this->m_width, this->m_height)) {
		cout << "Tilemap loading failed!" << endl;
		return false;
	}

	for (int i = 0; i < m_tileIndexes.size(); i++)
	{
		//Tile is a sctruct that has all the information needed of a tile
		// its size, position, tile-number and if it has a collider 
		Tile newTile;
		//copy the tileindex of the tilemap into the the new Tile
		newTile.tileIndex = m_tileIndexes[i];
		m_pTilemap->m_vertices[i];
		//assign the position of the tile in the scene
		newTile.tilePos.x = (i % m_width) * m_tileSize;
		newTile.tilePos.y = (i / m_width) * m_tileSize;

		//check if the current tile has a collider
		for (int j = 0; j < m_collisionTiles.size(); j++)
		{
			if (m_tileIndexes[i] == m_collisionTiles[j]) {
				newTile.hasCollider = true;
				break;
			}
			else {
				newTile.hasCollider = false;
			}
		}
		m_pTiles->push_back(newTile);

	}
	return true;
}

//draw the scene
void CScene::updateScene(RenderTarget& target)
{
	target.draw(*m_pTilemap);
}

// the scene node needs a lot of data..
void CScene::Start(string path, xml_node<>* pNode)
{	
	m_tileIndexes = {};

	xml_node<>* pChild;
	xml_attribute<>* pAttr;

//  ..from the scene number = id .. 
	pAttr= FindAttribute(pNode, "id");
	if (pAttr != NULL) {
		m_sceneNumber = atoi(pAttr->value());
		m_path = path + "level_"+ pAttr->value() + "_tiles.png";
	}

//  ..to the size of the scene..
	pAttr = FindAttribute(pNode, "width");
	if (pAttr != NULL) 
		m_width = atoi(pAttr->value());

	pAttr = FindAttribute(pNode, "height");
	if (pAttr != NULL) 
		m_height = atoi(pAttr->value());

//  ..the whole tilemap sequence ..
	pChild = FindChildNode(pNode, "tilemap");
	if (pNode != NULL) {
		//the tile numbers are separated by a ',' as some are dobule digit
		char* next_pt = NULL;
		char* pt = strtok_s(pChild->value(), ",", &next_pt);
		while (pt != NULL) {
			m_tileIndexes.push_back(atoi(pt));
			pt = strtok_s(NULL, ",", &next_pt);
		}
	}

	for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
		if (strcmp(pChild->name(), "tile") == 0) {
			//assets have the attribute "type" and "id"
			pAttr = FindAttribute(pChild, "type");
			if (pAttr != NULL) {
			//.. the collision tiles..
				if (strcmp(pAttr->value(), "collider") == 0) {
					char* next_pt = NULL;
					char* pt = strtok_s(pChild->value(), ",", &next_pt);
					while (pt != NULL) {
						m_collisionTiles.push_back(atoi(pt));
						pt = strtok_s(NULL, ",", &next_pt);
					}
				}
			}
			pAttr = FindAttribute(pChild, "colliderSize");
			if (pAttr != NULL) {
				m_colliderSize = ReadVector2f(pAttr);
			}
		}
	}
	for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
		if (strcmp(pChild->name(), "action") == 0) {
			pAttr = FindAttribute(pChild, "type");
			//.. and the interactables
			Interactable inter;
			if (pAttr != NULL) {
				if (strcmp(pAttr->value(), "door") == 0) {
					//add interactable object door
					inter.name = "door";
				}
				if (strcmp(pAttr->value(), "character") == 0) {
					inter.name = "character";
				}
			}
			pAttr = FindAttribute(pChild, "actionArea");
			if (pAttr != NULL) {
				// interactable Area size
				inter.interShape = RectangleShape(ReadVector2f(pAttr));
			}
			pAttr = FindAttribute(pChild, "inPos");
			if (pAttr != NULL) {
				// interactable Area position
				inter.interShape.setPosition(ReadVector2f(pAttr));
			}
			pAttr = FindAttribute(pChild, "outPos");
			if (pAttr != NULL) {
				//new position of the player when exting the interactable
				inter.outPos = ReadVector2f(pAttr);
			}
			m_pInteractables->push_back(inter);
		}
	}
}

