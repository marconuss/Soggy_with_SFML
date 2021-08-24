#include "Player.h"


void CPlayer::initTexture(string path) 
{
	pTexture = new Texture();
	if (!pTexture->loadFromFile(path)) {
		cout << "Could not load texture file." << endl;
	}
	//in the spritesheet the frames for one animation are on the same line
	//so by dividing the width of the texture I know how many frames take to do one animation
	//in particular for the main player it's 4 
	//but for the armory guy it'ss 3
	m_animationFrames = pTexture->getSize().x /m_tileSize;
}

void CPlayer::initSprite()
{
	pSprite = new Sprite();
	pSprite->setTexture(*pTexture);
	pSprite->setTextureRect(IntRect(m_tileSize, m_tileSize, m_tileSize, m_tileSize));
	pSprite->setOrigin(m_tileSize/2, m_tileSize/2);
	pSprite->setPosition(m_playerPos);
}

//this text is to signal when you can interact with the armory guy
void CPlayer::initText()
{
	Font* pFont = new Font();
	pFont->loadFromFile("assets\\arial.ttf");
	myText.setFont(*pFont);
	myText.setString("press enter to talk");
	myText.setCharacterSize(15);
	myText.setFillColor(Color(255, 255, 255, 255));
	myText.setOrigin(0, 0);
	myText.setPosition(Vector2f(320, 400));
	
	//it also has a small box around it
	textBox.setSize(Vector2f(myText.getLocalBounds().width + 10, myText.getLocalBounds().height*2));
	textBox.setFillColor(Color(50, 50, 50, 255));
	textBox.setOutlineThickness(5);
	textBox.setOutlineColor(Color(23,75,80,255));
	textBox.setOrigin(0, 0);
	textBox.setPosition(myText.getPosition());
	
}

//construcor
CPlayer::CPlayer()
{
	pCurrentScene = NULL;
	m_animationTime = 0.f;
	m_animationState = 0;
	m_playerPos = { 0,0 };
	m_playerSpeed = 0;
	m_sceneNum = 0;
	m_isInDialogue = false;
	m_dialogueLines = {};
	m_lineCount = 0;
	m_moving = true;
	initText();
}

//deconstructor
CPlayer::~CPlayer()
{
	delete pSprite;
	delete pTexture;
}


//  functions:
// 
//	animator plays the right animation according to the state,
//	for example idle or walking up or down or left, right
void CPlayer::animator()
{
	switch (m_animationState)
	{
	case 1:
		//up animation
		//the animation time is deltatime bound
		if (m_animationTime > 0.15) {
			pSprite->setTextureRect(IntRect(m_animFrameCount * m_tileSize, 0, m_tileSize, m_tileSize));
			m_animFrameCount++;
			
			//animation loops when it hits the last frame
			if (m_animFrameCount == m_animationFrames)
				m_animFrameCount = 0;
			m_animationTime = 0.0f;
		}
	case 2:
		//down animation
		if (m_animationTime > 0.15) {
			pSprite->setTextureRect(IntRect(m_animFrameCount * m_tileSize, m_tileSize, m_tileSize, m_tileSize));
			m_animFrameCount++;

			if (m_animFrameCount == m_animationFrames)
				m_animFrameCount = 0;
			m_animationTime = 0.0f;
		}
	case 3:
		//left animation
		if (m_animationTime > 0.15) {
			pSprite->setTextureRect(IntRect(m_animFrameCount * m_tileSize, m_tileSize * 3, m_tileSize, m_tileSize));
			m_animFrameCount++;

			if (m_animFrameCount == m_animationFrames)
				m_animFrameCount = 0;
			m_animationTime = 0.0f;
		}
	case 4:
		//right animation
		if (m_animationTime > 0.15) {
			pSprite->setTextureRect(IntRect(m_animFrameCount * m_tileSize, m_tileSize * 4, m_tileSize, m_tileSize));
			m_animFrameCount++;

			if (m_animFrameCount == m_animationFrames)
				m_animFrameCount = 0;
			m_animationTime = 0.0f;
		}
	default:
		//idle
		if (m_animationTime > 0.15) {
			pSprite->setTextureRect(IntRect(m_animFrameCount * m_tileSize, m_tileSize * 2, m_tileSize, m_tileSize));
			m_animFrameCount++;

			if (m_animFrameCount == m_animationFrames)
				m_animFrameCount = 0;
			m_animationTime = 0.0f;
		}
		break;
	}
}

// collision are checked against the tiles that have a collider or, a interaction zone or the scene external borders
void CPlayer::checkCollisions(int state)
{
	//
	// if position of the player == position of the tile borders
	// and the tile has a collider 
	// push the player back by the overlap between player and tile
	// 

	vector<Tile>* tiles = pCurrentScene->m_pTiles;

	RectangleShape tile = RectangleShape(pCurrentScene->m_colliderSize);
	tile.setOrigin(0, 0);


	FloatRect playerBounds = pSprite->getGlobalBounds();


	for (int i = 0; i < tiles->size(); i++)
	{
		tile.setPosition(tiles->data()[i].tilePos);
		
		FloatRect tileBounds = tile.getGlobalBounds();

		FloatRect overlap;

		if (playerBounds.intersects(tileBounds, overlap) && tiles->data()[i].hasCollider)
		{
			if (state == 1) 
				pSprite->move(0, overlap.height);

			else if (state == 2) 
				pSprite->move(0, -overlap.height);

			else if (state == 3) 
				pSprite->move(overlap.width, 0);

			else if (state == 4) 
				pSprite->move(-overlap.width, 0);
		}

		//scene boundaries  
		if (pSprite->getPosition().x > (pCurrentScene->m_width * m_tileSize))
			pSprite->setPosition(pCurrentScene->m_width * m_tileSize, m_playerPos.y);
		else if (pSprite->getPosition().x < 0)
			pSprite->setPosition(0, m_playerPos.y);
		else if (pSprite->getPosition().y > (pCurrentScene->m_height * m_tileSize))
			pSprite->setPosition(m_playerPos.x, pCurrentScene->m_height * m_tileSize);
		else if (pSprite->getPosition().y < 0)
			pSprite->setPosition(m_playerPos.x, 0);
		
		//check interactables
		for (int i = 0; i < pCurrentScene->m_pInteractables->size(); i++)
		{

			FloatRect interBounds = pCurrentScene->m_pInteractables->data()[i].interShape.getGlobalBounds();
			//if it's the door change scene
			if (playerBounds.intersects(interBounds) && pCurrentScene->m_pInteractables->data()[i].name == "door") {
				if (pCurrentScene->m_sceneNumber == 1) {
					m_sceneNum = 2;
				}
				if (pCurrentScene->m_sceneNumber == 2) {
					m_sceneNum = 1;
				}
				pSprite->setPosition(pCurrentScene->m_pInteractables->data()[i].outPos);				
			}
			//if it's the character activate dialogue
			if (playerBounds.intersects(interBounds) && pCurrentScene->m_pInteractables->data()[i].name == "character") {
				//cout << "talk to character" << endl;
				m_isInDialogue = true;
				
			}
			else {
				m_isInDialogue = false;
			}
		}

	}
}

void CPlayer::updatePlayer(RenderTarget& target)
{
	//update Position on keypress 
	//arrows or WASD
	if (m_moving) 
	{
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {

			//state = 1 = up
			m_animationState = 1;

			//move player
			pSprite->move(0, -m_playerSpeed);
			
			//check for collisions
			checkCollisions(m_animationState);
				
		}
		//down is positive y
		else if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
			
			//state = 2 = down
			m_animationState = 2;

			pSprite->move(0, m_playerSpeed);
			checkCollisions(m_animationState);
		}
		//left is negative x
		else if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
			
			//state = 3 = left
			m_animationState = 3;

			pSprite->move(-m_playerSpeed, 0);
			checkCollisions(m_animationState);
		}
		//right is positive x
		else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
				
			//state = 4 = right
			m_animationState = 4;

			pSprite->move(m_playerSpeed, 0);
			checkCollisions(m_animationState);
		}
		else {
			//state = 0 = idle
			m_animationState = 0;
		}
	}
	//update player position data
	m_playerPos = pSprite->getPosition();

	//animate player
	animator();
	target.draw(*pSprite);

	//if in the interaction area 
	//show the user that interaction is possible
	if (m_isInDialogue) {
		target.draw(textBox);
		target.draw(myText);
		pSprite->setPosition(m_playerPos);
	}
	
	//for debugging
	/*
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		//cout << "scene one" << endl;
		m_sceneNum = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		//cout << "scene two" << endl;
		m_sceneNum = 2; 
	}	
	*/
}

// not necessary, but a bit less caotic 
void CPlayer::updateCharacter(RenderTarget& target)
{
	m_animationState = 1;
	animator();
	target.draw(*pSprite);
}


//read the start position and the player speed form the xml
void CPlayer::Start(string path, xml_node<>* pNode) 
{
	xml_attribute<>* pAttr;
	pAttr = FindAttribute(pNode, "startPos");
	if (pAttr != NULL) {
		m_playerPos = ReadVector2f(pAttr);
	}
	pAttr = FindAttribute(pNode, "speed");
	if (pAttr != NULL) {
		m_playerSpeed = atoi(pAttr->value());
	}
}



//----------------------