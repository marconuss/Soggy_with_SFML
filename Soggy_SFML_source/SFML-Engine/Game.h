#pragma once
#include "XMLParser.h"

class CGame
{
private:
	//variables
	CPlayer*		pPlayer;
	RenderWindow*	pWindow;
	CParser*		m_pParser;
	int				m_lineCount;
	bool			m_dialogueisActive;

	RectangleShape	textBox;

	void initWindow();
	void initScene(int sceneNum);
	void initPlayer();
	void displayDialogue();

public:
	// constructor and destructor
	CGame();
	~CGame();

	//delta time
	Clock*			pClock;
	Time*			pDeltaTime;
	CScene*			pCurrentScene;

	//Mouse Positios
	Vector2i		m_mousePosWindow;

	//current scene
	int				m_currentSceneNum;

	// functions:
	void run();

	// update mouse positions
	void updateMousePos();

	// update data for each frame
	void update();
	// show window
	void render();

};