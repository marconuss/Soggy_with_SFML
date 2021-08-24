#pragma once
#include "Tilemap.h"
#include "AssetManager.h"

// struct test

struct Tile {
	int			tileIndex;
	Vector2f	tilePos;
	bool		hasCollider;
};

struct Interactable {
	string			name;
	RectangleShape	interShape;
	Vector2f		outPos;
};

class CScene
{
private:

	CTilemap*		m_pTilemap;
	vector<int>		m_collisionTiles;
	//CAssetManager*	m_pAssetManager;

public:
	int				m_sceneNumber;
	string			m_path;
	int				m_width;
	int				m_height;
	int				m_tileSize;
	vector<int>		m_tileIndexes;
	Vector2f		m_colliderSize;
	//Interactable	door;

	vector<Tile>*			m_pTiles;
	vector<Interactable>*	m_pInteractables;
	
	// constructor destructor
	CScene();
	CScene(int sceneNumber, string path, int widht, int height, vector<int> tileIndexes, vector<int> collisionTiles, Vector2f playerStartPos);
	~CScene();

	bool loadScene();
	void updateScene(RenderTarget& target);


	//void Start(CAssetManager* pAssetManger, xml_node<>* pNode);
	void Start(string path, xml_node<>* pNode);
	//void setUpScene(xml_node<>* pNode, string tileset);
};
