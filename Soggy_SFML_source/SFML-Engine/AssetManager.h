#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <list>

#include "rapidxml.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace rapidxml; 
using namespace std;
using namespace sf;

xml_node<>* FindChildNode(rapidxml::xml_node<>* pNode, const char* szName);
xml_attribute<>* FindAttribute(rapidxml::xml_node<>* pNode, const char* szName);
Vector2f ReadVector2f(xml_attribute<>* pAttr);


class CGameAsset 
{
public:
	int		m_id;
	string	m_assetPath;
	string	m_role;
	virtual void Start(xml_node<>* pNode, string path);

	CGameAsset();
	~CGameAsset();
};

class CSpritesheet : public CGameAsset
{
public:

	CSpritesheet();
	~CSpritesheet();

	virtual void Start(xml_node<>* pNode, string path);
};

class CText : public CGameAsset
{
public:

	Text	m_text;

	virtual void Start(xml_node<>* pNode, string path);

	CText();
	~CText();
};

class CSound : public CGameAsset
{
public:
	SoundBuffer sBuffer;
	Sound		m_sound;

	virtual void Start(xml_node<>* pNode, string path);

	CSound();
	~CSound();
};

class CAssetManager
{
public:

	string				m_assetPath;
	vector<CGameAsset*>	m_AssetList;
	vector<CText*>		m_dialogueLines;
	vector<CSound*>		m_soundList;
	int					m_tileSize;
	int					m_gameScale;

	CAssetManager();
	CGameAsset* GetAssetByID(int nID);
};

