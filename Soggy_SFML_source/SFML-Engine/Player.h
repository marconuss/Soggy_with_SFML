#pragma once
#include "Scene.h"

class CPlayer
{
private:
	//variables
	Texture*		pTexture;
	Sprite*			pSprite;

	int				m_animFrameCount;
	int				m_animationFrames;
	int				m_lineCount;
	int				m_animationState;

	Text			myText;
	RectangleShape	textBox;

		
	//initialize functions
	void initText();
	void checkCollisions(int state);

public:
	CScene*			pCurrentScene;
	Vector2f		m_playerPos;
	vector<CText*>	m_dialogueLines;
	float			m_animationTime;
	int				m_sceneNum;
	int				m_playerSpeed;
	int				m_tileSize;
	bool			m_isInDialogue;
	bool			m_moving;

	void initTexture(string path);
	void initSprite();

	//constructor
	CPlayer();
	//deconstructor
	~CPlayer();

	//functions

	void animator();

	void updatePlayer(RenderTarget& target);
	void updateCharacter(RenderTarget& target);

	void Start(string path, xml_node<>* pNode);
};
