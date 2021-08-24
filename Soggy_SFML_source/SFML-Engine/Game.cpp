#include "Game.h"


//initialize the window
void CGame::initWindow()
{
	pWindow = new RenderWindow(VideoMode(17*m_pParser->assetManager.m_tileSize, 14*m_pParser->assetManager.m_tileSize), "SFML-Engine", Style::Close | Style::Titlebar);
	//set the frame rate limit to have a consistent experience
	pWindow->setFramerateLimit(60);
}

//initialize the current scene
void CGame::initScene(int sceneNum)
{
	for (int i = 0; i < m_pParser->scenes.size(); i++)
	{
		if (m_pParser->scenes[i]->m_sceneNumber == sceneNum) {
			pCurrentScene = m_pParser->scenes[i];
			pCurrentScene->loadScene();
		}
	}
}

//initialize the player
void CGame::initPlayer()
{

	if (pPlayer == NULL) {
		pPlayer = m_pParser->pPlayer;

		for (int i = 0; i < m_pParser->assetManager.m_AssetList.size(); i++)
		{
			if (m_pParser->assetManager.m_AssetList[i]->m_role == "player") {
				
				pPlayer->initTexture(m_pParser->assetManager.m_AssetList[i]->m_assetPath);
				pPlayer->initSprite();
				pPlayer->m_dialogueLines = m_pParser->assetManager.m_dialogueLines;
			}
		}
	}

	//update the player scene attributes
	pPlayer->m_sceneNum = m_currentSceneNum;
	pPlayer->pCurrentScene = pCurrentScene;

}

//displays the dialogue text
void CGame::displayDialogue()
{
	//when the dialogue is over return
	if ((m_lineCount-1) == m_pParser->assetManager.m_dialogueLines.size()) {
		pPlayer->m_isInDialogue = false;
		pPlayer->m_moving = true;
		m_dialogueisActive = false;

		m_lineCount = 0;
		return;
	}
	Text myText = m_pParser->assetManager.m_dialogueLines[m_lineCount - 1]->m_text;
	
	//draw a box around the dialogue
	textBox.setSize(Vector2f(myText.getLocalBounds().width+20, myText.getLocalBounds().height * 2));
	textBox.setFillColor(Color(50,50,50, 255));
	textBox.setOutlineThickness(5);
	textBox.setOutlineColor(Color(23,73,80,255));
	textBox.setOrigin(10, 0);
	textBox.setPosition(myText.getPosition());

	pWindow->draw(textBox);
	pWindow->draw(myText);
}

// constructor
CGame::CGame() 
{
	//load game data
	m_pParser = new CParser();
	m_pParser->LoadGameData("data.xml");

	//start game clock
	pClock = new Clock();
	pDeltaTime = new Time();

	//variables
	m_currentSceneNum = 1;
	m_lineCount = 0;
	m_dialogueisActive = false;
	//functions 
	initWindow();
	initScene(m_currentSceneNum);
	initPlayer();

};

// destructor
CGame::~CGame()
{
	//delete pointers
	delete pWindow;
	delete pPlayer;
	delete pCurrentScene;
	delete m_pParser; 
	delete pClock;
	delete pDeltaTime;
}


void CGame::run()
{
	//update and render are called every frame as long as the game is running
	while (pWindow->isOpen())
	{
		update();
		render();
	}
}

// was considering making it a point and click, than I changed my mind 
/*
void CGame::updateMousePos()
{
	m_mousePosWindow = Mouse::getPosition(*pWindow);
}
*/

void CGame::update()
{
	//!	this represents what happens in each frame
	//!	looks for events and changes in the states of the objects in the game
	
	// restart delta time
	*pDeltaTime = pClock->restart();

	// sets the time for the player animations
	pPlayer->m_animationTime += pDeltaTime->asSeconds();
	m_pParser->pCharacter->m_animationTime += pDeltaTime->asSeconds();

	//checks if the scene has changed
	if (pPlayer->m_sceneNum != m_currentSceneNum) {
		//cout << "change scene" << endl;
		m_pParser->assetManager.m_soundList[0]->m_sound.play();
		int newSceneNum = pPlayer->m_sceneNum;
		m_currentSceneNum = newSceneNum;
		initScene(newSceneNum);
		initPlayer();
	}

	Event event;
	while (pWindow->pollEvent(event))
	{
		//check for the window closing
		if (event.type == Event::Closed)
		{
			pWindow->close();
		}
		//check for enter key press
		if (event.type == sf::Event::KeyPressed)
		{
			//I use the enter key to activate the dialogue
			if (event.key.code == sf::Keyboard::Enter && pPlayer->m_isInDialogue) {
				pPlayer->m_moving = false;
				m_dialogueisActive = true;
				m_lineCount++;
			}
		}
		
	}

	//updateMousePos();
}

void CGame::render() 
{
	//!	steps of the render function:
	//! 1- clear the old frame
	//! 2- render objects
	//! 	- background
	//! 	- player
	//! 	- other
	//! 3- display the new frame

	//1
	pWindow->clear(Color(255,255,255,255));

	//2
	pCurrentScene->updateScene(*pWindow);

	if (pCurrentScene->m_sceneNumber == 2) {
		m_pParser->pCharacter->updateCharacter(*pWindow);
	}

	pPlayer->updatePlayer(*pWindow);

	if (m_dialogueisActive) {
		displayDialogue();
	}

	//3
	pWindow->display();

}
