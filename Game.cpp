#include "Game.h"

void Game::Quit()
{
	m_bRunning = false;
}

bool Game::Run(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	std::srand((unsigned)std::time(NULL));
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "SDL Init successful" << endl;
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		assert(m_pWindow != 0);
		if (m_pWindow != 0)
		{
			cout << "Window creation successful" << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		}
		else
		{
			cout << "Window Init fail" << endl;
			return false;
		}
		assert(IMG_Init(IMG_INIT_PNG));
		if (IMG_Init(IMG_INIT_PNG))
		{
			TheTextureManager::Instance()->laserTexture = TheTextureManager::Instance()->Load("Assets/Images/Laser2.png");
			TheTextureManager::Instance()->tankTexture = TheTextureManager::Instance()->Load("Assets/Images/Tank.png");
			TheTextureManager::Instance()->spikeTexture = TheTextureManager::Instance()->Load("Assets/Images/Spike.png");
			TheTextureManager::Instance()->tileTexture[0] = TheTextureManager::Instance()->Load("Assets/Images/tile.png");
			TheTextureManager::Instance()->tileTexture[1] = TheTextureManager::Instance()->Load("Assets/Images/BrickWall.png");
			TheTextureManager::Instance()->tileTexture[2] = TheTextureManager::Instance()->Load("Assets/Images/Gravel.png");
			TheTextureManager::Instance()->tileTexture[3] = TheTextureManager::Instance()->Load("Assets/Images/Water.png");
			TheTextureManager::Instance()->tileTexture[4] = TheTextureManager::Instance()->Load("Assets/Images/Lava.png");
			TheTextureManager::Instance()->rockTexture = TheTextureManager::Instance()->Load("Assets/Images/Rock.png");
			TheTextureManager::Instance()->natureTexture = TheTextureManager::Instance()->Load("Assets/Images/Nature.png");
			TheTextureManager::Instance()->enemyTurret = TheTextureManager::Instance()->Load("Assets/Images/EnemyTurrets.png");
			TheTextureManager::Instance()->mineTexture = TheTextureManager::Instance()->Load("Assets/Images/Mine.png");
			TheTextureManager::Instance()->buttonTexture = TheTextureManager::Instance()->Load("Assets/Images/Buttons.png");
			TheTextureManager::Instance()->explosionSprite = TheTextureManager::Instance()->Load("Assets/Images/ExplosionSprites.png");
			TheTextureManager::Instance()->instructionTexture = TheTextureManager::Instance()->Load("Assets/Images/Instructions.png"); 
			cout << "image init success" << endl;
		}
		else
		{
			cout << "png image init fail" << endl;
			return false; // Window init fail.
		}
		assert(Mix_Init(MIX_INIT_MP3) != 0);
		if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
		{
			Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 8192);
			Mix_AllocateChannels(16);
		}
		else
		{
			cout << "audio init fail" << endl;
			return false; // Window init fail.
		}
		assert(TTF_Init() == 0);
		if (TTF_Init() == 0)
		{
			TheTextureManager::Instance()->gemunuLibreBold = TTF_OpenFont("Assets/Fonts/MotionControl.otf", 35);
			TheTextureManager::Instance()->calibriRegular = TTF_OpenFont("Assets/Fonts/calibri.ttf", 30);
				
		}
		else
		{
			cout << "ttf init fail" << endl;
			return false; // Window init fail.
		}
	}
	else
	{
		cout << "SDL Init fail" << endl;
		return false;
	}
	
	cout << "Init success" << endl;
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	m_bRunning = true;

	m_vSounds.push_back(new Audio("Assets/Sounds/AGamersSong.mp3", 0, 1, 100.0f));      // 100% background music
	m_vSounds.push_back(new Audio("Assets/Sounds/Hero_Immortal.mp3", 1, 1, 50.0f));    // 100% background music
	m_vSounds.push_back(new Audio("Assets/Sounds/Fire.wav", 2, 0, 25.0f));				// 25% fire sound
	m_vSounds.push_back(new Audio("Assets/Sounds/explosion.wav", 3, 0, 50.0f));			// 50% explosion sound
	m_vSounds.push_back(new Audio("Assets/Sounds/HonkSound.mp3", 4, 0, 75.0f));			// 50% car horn sound
	m_vSounds.push_back(new Audio("Assets/Sounds/CarIdle.mp3", 5, 1, 15.0f));			// 50% car horn sound
	m_vSounds.push_back(new Audio("Assets/Sounds/Damage.mp3", 6, 1, 25.0f));			// 50% car horn sound
	m_vSounds.push_back(new Audio("Assets/Sounds/Win.wav", 7, 1, 25.0f));			// 50% car horn sound
	m_pFSM = new FSM();
	GetFSM()->ChangeState(new TitleState());											// Change to the title state

	m_pPlayer = new Player;

	m_iKeystates = SDL_GetKeyboardState(nullptr);
	LoadData();
	while (m_bRunning)
	{
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < delayTime)
		{
			SDL_Delay((int)delayTime - frameTime);
		}
		HandleEvents();
		Update();
		Render();
	}
	Clean();
}

SDL_Renderer* Game::GetRenderer()
{
	return m_pRenderer;
}


void Game::LoadData()
{
	XMLDocument xmlDoc;
	XMLElement* pElement;
	XMLNode* pRoot;
	xmlDoc.LoadFile("GameData.xml");
	// Create a Root to store all data
	pRoot = xmlDoc.FirstChildElement("Root");
	// Elements for fastest times
	pElement = pRoot->FirstChildElement("Player");
	pElement->QueryIntAttribute("Kills", &PlayerStats::kills);
	pElement->QueryIntAttribute("Deaths", &PlayerStats::deaths);

	pElement = pRoot->FirstChildElement("BestTimes");
	pElement->QueryIntAttribute("count", &PlayerStats::time_size);

	XMLElement* pTimeChildElement = pElement->FirstChildElement("Time");
	while (pTimeChildElement != nullptr)
	{
		float childElement;
		pTimeChildElement->QueryFloatText(&childElement);
		pTimeChildElement = pTimeChildElement->NextSiblingElement("Time");
		PlayerStats::m_vHighestTimes.push_back(childElement);
	}
	// Elements for best accuracy

	pElement = pRoot->FirstChildElement("BestAccuracy");
	pElement->QueryIntAttribute("count", &PlayerStats::accuracy_size);

	pTimeChildElement = pElement->FirstChildElement("Accuracy");
	while (pTimeChildElement != nullptr)
	{
		float childElement;
		pTimeChildElement->QueryFloatText(&childElement);
		pTimeChildElement = pTimeChildElement->NextSiblingElement("Accuracy");
		PlayerStats::m_vAccuracy.push_back(childElement);
	}
	cout << PlayerStats();

}

void Game::SaveData()
{
	cout << "Saving data..." << endl;
	if (Game::Instance()->wonGame)
	{
		PlayerStats::m_vHighestTimes.push_back(120 - Game::Instance()->inGameTime_Seconds + Game::Instance()->totalElapsed_Seconds);
		PlayerStats::m_vAccuracy.push_back(Game::Instance()->accuracy);

		// Sort the list (Bubble sort)
		for (int i = 0; i < PlayerStats::m_vHighestTimes.size(); i++) {
			for (int i = 0; i < PlayerStats::m_vHighestTimes.size() - 1; i++) {
				float temp;
				if (PlayerStats::m_vHighestTimes[i] > PlayerStats::m_vHighestTimes[i + 1]) {
					temp = PlayerStats::m_vHighestTimes[i];
					PlayerStats::m_vHighestTimes[i] = PlayerStats::m_vHighestTimes[i + 1];
					PlayerStats::m_vHighestTimes[i + 1] = temp;
				}
			}
		}
		for (int i = 0; i < PlayerStats::m_vAccuracy.size(); i++) {
			for (int i = 0; i < PlayerStats::m_vAccuracy.size() - 1; i++) {
				float temp;
				if (PlayerStats::m_vAccuracy[i] < PlayerStats::m_vAccuracy[i + 1]) {
					temp = PlayerStats::m_vAccuracy[i];
					PlayerStats::m_vAccuracy[i] = PlayerStats::m_vAccuracy[i + 1];
					PlayerStats::m_vAccuracy[i + 1] = temp;
				}
			}
		}
	}
	


	XMLDocument xmlDoc;
	XMLElement* pElement;
	XMLNode* pRoot;
	char buffer[50];
	// Create a Root to store all data
	pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertEndChild(pRoot);
	// Elements for fastest times
	pElement = xmlDoc.NewElement("Player");

	pElement->SetAttribute("Kills", PlayerStats::kills);
	pElement->SetAttribute("Deaths", PlayerStats::deaths);
	pRoot->InsertEndChild(pElement);
	
	pElement = xmlDoc.NewElement("BestTimes");
	pElement->SetAttribute("count", PlayerStats::m_vHighestTimes.size());
	pRoot->InsertEndChild(pElement);
	for (int i = 0; i < PlayerStats::m_vHighestTimes.size(); i++)
	{
		XMLElement* pTimeChildElement = xmlDoc.NewElement("Time");
		sprintf_s(buffer, "%.1f", PlayerStats::m_vHighestTimes[i]);
		pTimeChildElement->SetText(buffer);
		pElement->InsertEndChild(pTimeChildElement);
	}
	// Elements for best accuracy

	pElement = xmlDoc.NewElement("BestAccuracy");
	pElement->SetAttribute("count", PlayerStats::m_vAccuracy.size());
	pRoot->InsertEndChild(pElement);
	for (int i = 0; i < PlayerStats::m_vAccuracy.size(); i++)
	{
		XMLElement* pTimeChildElement = xmlDoc.NewElement("Accuracy");
		sprintf_s(buffer, "%.1f", PlayerStats::m_vAccuracy[i]);
		pTimeChildElement->SetText(buffer);
		pElement->InsertEndChild(pTimeChildElement);
	}	
	xmlDoc.SaveFile("GameData.xml");
}

void Game::Load()
{

	ifstream inFile;
	inFile.open("Level.txt");
	char temp;
	try
	{
		if (!inFile.is_open())
			throw string("ERROR: Cannot open file for reading!");
	}
	catch (string msgQueue)
	{
		cout << msgQueue << endl;
		m_bRunning = false;
	}


	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			m_pTiles[i][j] = new BGTile();
			inFile >> temp;
			switch (temp)
			{
			case '0':
				m_pTiles[i][j]->SetTexture(Texture::GRASS);
				break;
			case '1':
				m_pTiles[i][j]->SetTexture(Texture::BRICK);
				break;
			case '2':
				m_pTiles[i][j]->SetTexture(Texture::GRAVEL);
				break;
			case '3':
				m_pTiles[i][j]->SetTexture(Texture::WATER);
				break;
			case '4':
				m_pTiles[i][j]->SetTexture(Texture::LAVA);
				break;
			}
			m_pTiles[i][j]->SetTile(i, j);
		}
	}
	m_vObject.push_back(new Spike(1, 2));

	m_vObject.push_back(new Spike(14, 1));
	m_vObject.push_back(new Spike(14, 3));
	m_vObject.push_back(new Spike(14, 5));

	m_vObject.push_back(new Spike(18, 1));
	m_vObject.push_back(new Spike(18, 3));
	m_vObject.push_back(new Spike(18, 5));

	m_vObject.push_back(new Spike(28, 44));
	m_vObject.push_back(new Spike(26, 44));
	m_vObject.push_back(new Spike(28, 42));
	m_vObject.push_back(new Rock(30, 42));
	m_vObject.push_back(new Rock(30, 43));
	m_vObject.push_back(new Rock(30, 44));
	m_vObject.push_back(new Rock(30, 45));
	m_vObject.push_back(new Rock(30, 46));
	m_vObject.push_back(new Rock(29, 46));
	m_vObject.push_back(new Rock(28, 46));
	m_vObject.push_back(new Rock(27, 46));
	m_vObject.push_back(new Rock(26, 46));

	m_vObject.push_back(new Spike(4, 44));
	m_vObject.push_back(new Spike(2, 44));
	m_vObject.push_back(new Spike(2, 42));
	m_vObject.push_back(new Rock(1, 42));
	m_vObject.push_back(new Rock(1, 43));
	m_vObject.push_back(new Rock(1, 44));
	m_vObject.push_back(new Rock(1, 45));
	m_vObject.push_back(new Rock(1, 46));
	m_vObject.push_back(new Rock(2, 46));
	m_vObject.push_back(new Rock(2, 46));
	m_vObject.push_back(new Rock(3, 46));
	m_vObject.push_back(new Rock(4, 46));
	m_vObject.push_back(new Rock(5, 46));

	m_vObject.push_back(new Rock(3, 8));
	m_vObject.push_back(new Rock(4, 8));
	m_vObject.push_back(new Rock(5, 8));

	m_vObject.push_back(new Rock(11, 8));
	m_vObject.push_back(new Rock(12, 8));
	m_vObject.push_back(new Rock(13, 8));

	m_vObject.push_back(new Rock(19, 20));
	m_vObject.push_back(new Rock(19, 21));
	m_vObject.push_back(new Rock(20, 20));
	m_vObject.push_back(new Rock(20, 21));
	m_vObject.push_back(new Rock(20, 22));
	m_vObject.push_back(new Rock(21, 20));
	m_vObject.push_back(new Rock(19, 22));
	m_vObject.push_back(new Rock(19, 22));

	m_vObject.push_back(new Bush(25, 11, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(25, 12, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(25, 13, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(25, 14, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(25, 15, 32, 32, { 0, 0, 36, 36 }));
	
	m_vObject.push_back(new Mine(1, 18));
	m_vObject.push_back(new Mine(3, 18));

	m_vObject.push_back(new Bush(19, 10, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(20, 10, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(21, 10, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(22, 10, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(23, 10, 32, 32, { 0, 0, 36, 36 }));
	m_vObject.push_back(new Bush(24, 10, 32, 32, { 0, 0, 36, 36 }));

	m_vObject.push_back(new Bush(7, 15, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(8, 15, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(9, 15, 32, 32, { 0, 37, 36, 92 - 37 }));

	m_vObject.push_back(new Bush(5, 25, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(7, 25, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(8, 25, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(8, 25, 50, 64, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(9, 15, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(6, 26, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(6, 27, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(7, 27, 32, 32, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(9, 27, 32, 32, { 0, 37, 36, 92 - 37 }));

	m_vObject.push_back(new Bush(22, 28, 50, 64, { 0, 37, 36, 92 - 37 }));
	m_vObject.push_back(new Bush(25, 34, 50, 64, { 0, 37, 36, 92 - 37 }));

	m_vObject.push_back(new Rock(7, 24));
	m_vObject.push_back(new Rock(5, 27));
	m_vObject.push_back(new Rock(10, 24));
	m_vObject.push_back(new Rock(10, 26));
	m_vObject.push_back(new Rock(10, 27));

	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 6, 5));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 28, 13));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 28, 23));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 2, 23));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 25, 42));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 5, 42));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 28, 2));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 16, 2));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 9, 5));
	m_vObject.push_back(new Turret({ 368, 75, 194, 129 }, 2, 10));
}

bool Game::Running()
{
	return m_bRunning;
}

FSM * Game::GetFSM()
{
	return m_pFSM;
}

void Game::SetMousePosition(int x, int y)
{
	m_mousePosition.x = x;
	m_mousePosition.y = y;
}

SDL_Point Game::GetMousePosition()
{
	return m_mousePosition;
}

bool Game::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Game::Render() 
{
	GetFSM()->Render();

	SDL_RenderPresent(m_pRenderer);
	SDL_RenderClear(m_pRenderer);
}

void Game::Update() 
{
	GetFSM()->Update();
}

void Game::HandleEvents() 
{
	GetFSM()->HandleEvents();
}

void Game::Clean()
{

	cout << "Cleaning game" << endl;

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}
