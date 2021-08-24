#include "XMLParser.h"


CParser::CParser()
{
	pScene = NULL;
	assetManager.m_assetPath = "";
}

CParser::~CParser()
{
}

bool CParser::LoadGameData(string path)
{
	ifstream myFile(path, ios::binary);
	if (myFile.fail()) {
		return false;
	}
	//storing the file data
	vector<char> buffer((istreambuf_iterator<char>(myFile)), istreambuf_iterator<char>());
	//making shure the data is '0' terminating for the parser
	buffer.push_back('\0');
	myFile.close();

	xml_document<> doc;


	//parse the data from the beginning
	doc.parse<0>(&buffer[0]);
	
	//get the root node
	xml_node<>*			pRootNode = doc.first_node();
	xml_node<>*			pNode = pRootNode;
	xml_attribute<>*	pAttr;

	//read assets data
	pNode = FindChildNode(pRootNode, "assets");
	if (pNode != NULL) {
		pAttr = FindAttribute(pNode, "path");
		if (pAttr != NULL) {
			//save the assets path
			assetManager.m_assetPath = pAttr->value();
		}
		pAttr = FindAttribute(pNode, "scale");
		if (pAttr != NULL) {
			assetManager.m_gameScale = atoi(pAttr->value());
		}
		//read the spritesheets data
		xml_node<>* pNextNode = FindChildNode(pNode, "spritesheets");
		if (pNextNode != NULL) {

			pAttr = FindAttribute(pNextNode, "tileSize");
			if (pAttr != NULL) {
				assetManager.m_tileSize = atoi(pAttr->value());
			}
			for (xml_node<>* pChild = pNextNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
				if (strcmp(pChild->name(), "spritesheet") == 0) {
					CSpritesheet* pAsset = new CSpritesheet();
					pAsset->Start(pChild, assetManager.m_assetPath);
					assetManager.m_AssetList.push_back(pAsset);
				}
			}
		}
		// read the sounds data
		pNextNode = FindChildNode(pNode, "sounds");
		if (pNextNode != NULL) {
			for (xml_node<>* pChild = pNextNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
				if (strcmp(pChild->name(), "sound") == 0) {
					CSound* pSound = new CSound();
					pSound->Start(pChild, assetManager.m_assetPath);
					assetManager.m_soundList.push_back(pSound);
				}
			}
		}
	}

	//player setup
	pNode = FindChildNode(pRootNode, "player");
	if(pNode != NULL){
		pPlayer = new CPlayer();
		pPlayer->Start(assetManager.m_assetPath, pNode);
		pPlayer->m_tileSize = assetManager.m_tileSize;
	}
	//other character setup
	pNode = FindChildNode(pRootNode, "character");
	if(pNode != NULL){
		pCharacter = new CPlayer();
		pCharacter->Start(assetManager.m_assetPath, pNode);
		for (int i = 0; i < assetManager.m_AssetList.size(); i++)
		{
			if (assetManager.m_AssetList[i]->m_role == "character") {
				pCharacter->m_tileSize = assetManager.m_tileSize;
				pCharacter->initTexture(assetManager.m_AssetList[i]->m_assetPath);
				pCharacter->initSprite();
			}
		}
		pCharacter->m_sceneNum = 0;
	}

	//dialogue data
	pNode = FindChildNode(pRootNode, "dialogues");
	if(pNode!= NULL){
		Font* pFont = new Font();
		int	characterSize = 0;
		
		pAttr = FindAttribute(pNode, "font");
		if (pAttr != NULL) {
			pFont->loadFromFile(assetManager.m_assetPath + pAttr->value());
		}
		pAttr = FindAttribute(pNode, "size");
		if (pAttr != NULL) {
			characterSize = atoi(pAttr->value());
		}
		for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
			if (strcmp(pChild->name(), "line") == 0) {
				CText* myText = new CText();
				myText->Start(pChild, assetManager.m_assetPath);
				myText->m_text.setFont(*pFont);
				myText->m_text.setCharacterSize(characterSize);
				assetManager.m_dialogueLines.push_back(myText);
			}
		}
	}

	//scene data
	pNode = FindChildNode(pRootNode, "scenes");
	if (pNode != NULL) {
		for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
			if (strcmp(pChild->name(), "scene") == 0) {
				pScene = new CScene();
				pScene->Start(assetManager.m_assetPath, pChild);
				pScene->m_tileSize = assetManager.m_tileSize;
				scenes.push_back(pScene);
			}
		}
	}

	
	return true;
}
