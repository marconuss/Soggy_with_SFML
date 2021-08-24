#pragma once
//#include "AssetManager.h"
#include "Player.h"


class CParser
{
private:

	CScene*			pScene;

public:

	CPlayer*		pPlayer;
	CPlayer*	    pCharacter;
	CAssetManager	assetManager;
	vector<CScene*> scenes;

	CParser();
	~CParser();

	bool LoadGameData(string path);

};
