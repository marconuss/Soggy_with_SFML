#include "AssetManager.h"

//find the child of a node helper 
xml_node<>* FindChildNode(xml_node<>* pNode, const char* szName)
{
	for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling()) {
		if (strcmp(pChild->name(), szName) == 0) return pChild;
	}
	return NULL;
}
//find attribute of a node helper
xml_attribute<>* FindAttribute(xml_node<>* pNode, const char* szName)
{
	for (xml_attribute<>* pAttribute = pNode->first_attribute(); pAttribute != NULL; pAttribute = pAttribute->next_attribute()) {
		if (strcmp(pAttribute->name(), szName) == 0) return pAttribute;
	}
	return NULL;
}

//helper function to get a set of data separated by ',' 
Vector2f ReadVector2f(xml_attribute<>* pAttr) 
{
	Vector2f result;
	vector<int> pos;
	char* next_pt = NULL;
	char* pt = strtok_s(pAttr->value(), ",", &next_pt);
	while (pt != NULL) {
		pos.push_back(atoi(pt));
		pt = strtok_s(NULL, ",", &next_pt);
	}
	result.x = pos[0];
	result.y = pos[1];

	return result;
}


CGameAsset::CGameAsset()
{
}

CGameAsset::~CGameAsset()
{
}

void CGameAsset::Start(xml_node<>* pNode, string path)
{
}

CSpritesheet::CSpritesheet()
{
}

CSpritesheet::~CSpritesheet()
{
}

//spritesheets have an id and a role 
void CSpritesheet::Start(xml_node<>* pNode, string path)
{
	xml_attribute<>* pAttr;

	pAttr = FindAttribute(pNode, "id");
	if(pAttr != NULL){
		m_id = atoi(pAttr->value());
	}
	pAttr = FindAttribute(pNode, "role");
	if (pAttr != NULL) {
		m_role = pAttr->value();
	}

	m_assetPath = path + pNode->value();

}

//text data has an Id and the value of the node is the actual text
void CText::Start(xml_node<>* pNode, string path)
{
	xml_attribute<>* pAttr;
	pAttr = FindAttribute(pNode, "id");
	if (pAttr != NULL) {
		m_id = atoi(pAttr->value());
	}

	m_text.setString(pNode->value());

	m_text.setFillColor(Color(255, 255, 255, 255));
	m_text.setStyle(Text::Bold);
	m_text.setPosition(Vector2f(50, 270));
}

CText::CText() {

}
CText::~CText() {

}

//sound has an Id and role, plus it needs a buffer to load the file
void CSound::Start(xml_node<>* pNode, string path)
{
	xml_attribute<>* pAttr;
	pAttr = FindAttribute(pNode, "id");
	if (pAttr != NULL) {
		m_id = atoi(pAttr->value());
	}
	pAttr = FindAttribute(pNode, "role");
	if (pAttr != NULL) {
		m_role = pAttr->value();
	}
	sBuffer.loadFromFile(path + pNode->value());
	m_sound.setBuffer(sBuffer);
}

CSound::CSound()
{
}

CSound::~CSound()
{
}

CAssetManager::CAssetManager()
{
}

CGameAsset* CAssetManager::GetAssetByID(int nID)
{
	return nullptr;
}