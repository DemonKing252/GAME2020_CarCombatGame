#include "FSM.h"
#include "Game.h"


void FSM::ChangeState(State * changeTo)
{
	cout << "Changing states . . . " << endl;

	if (m_pState != nullptr)
	{
		m_pState->Exit();
	}
	changeTo->Enter();
	m_pState = changeTo;
	Game::Instance()->m_bMousePressed = false;
}

void FSM::SaveState(State * changeTo)
{
	cout << "Saving states . . . " << endl;
	
	changeTo->Enter();
	m_pState = changeTo;
	Game::Instance()->m_bMousePressed = false;
}

void FSM::ResumeState(State * changeTo)
{
	cout << "Resuming states . . . " << endl;
	
	if (m_pState != nullptr)
	{
		m_pState->Exit();
	}
	m_pState = changeTo;
	Game::Instance()->m_bMousePressed = false;
}

FSM::FSM()
{
	Game::Instance()->m_vSounds[WAV_Chunk::TITLE_MUSIC]->Play();
}


FSM::~FSM()
{
	m_pState->Exit();
	delete m_pState;
	m_pState = nullptr;
}

void FSM::Render()
{
	m_pState->Render();
}

void FSM::Update()
{
	m_pState->Update();
}

void FSM::HandleEvents()
{
	m_pState->HandleEvents();
}

void GameState::Enter()
{
	Game::Instance()->wonGame = false;
	Game::Instance()->m_pPlayer->start_Death = 0;
	Game::Instance()->m_pPlayer->m_bAnimate = false;
	Game::Instance()->m_pPlayer->m_rSrc = { 0, 0, 620, 691 };
	Game::Instance()->accuracy = 0.0f;
	Game::Instance()->shotsFired = 0;
	Game::Instance()->shotsHit = 0;
	Game::Instance()->timeSinceHonk_Start = clock();
	Game::Instance()->m_pPlayer->m_dHealth = 100.0f;
	Game::Instance()->m_pPlayer->m_bisAlive = true;

	Game::Instance()->m_vSounds[WAV_Chunk::TITLE_MUSIC]->Hault();
	Game::Instance()->m_vSounds[WAV_Chunk::GAME_MUSIC]->Play();
	Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->Play();

	Game::Instance()->time = 120;
	Game::Instance()->inGameTime_Minutes = 2;
	Game::Instance()->m_pPlayer->m_dAngle = 0;
	while (!Game::Instance()->m_vObject.empty()) {
		for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
			delete Game::Instance()->m_vObject[i];
			Game::Instance()->m_vObject.erase(Game::Instance()->m_vObject.begin() + i);
		}
	}
	Game::Instance()->Load();
	Game::Instance()->m_vObject.shrink_to_fit();

	Camera::SetPosition(Vector2(512.0f, 384.0f));

	Game::Instance()->m_iNumEnemies = 10;
	Game::Instance()->start = clock();
}

void GameState::Update()
{
	if (Game::Instance()->KeyDown(SDL_SCANCODE_P)) {
		Game::Instance()->GetFSM()->SaveState(new PauseState());
	}
	Game::Instance()->inGameTime_Seconds = ((double)clock() - Game::Instance()->start) / CLOCKS_PER_SEC;
	for (GameObject* object : Game::Instance()->m_vObject) {
		object->Update();
	}
	for (int i = 0; i < Game::Instance()->m_vBullet.size(); i++) {
		Game::Instance()->m_vBullet[i]->Update();
		if (!Game::Instance()->m_vBullet[i]->m_bisAlive) {
			delete Game::Instance()->m_vBullet[i];
			Game::Instance()->m_vBullet.erase(Game::Instance()->m_vBullet.begin() + i);
			Game::Instance()->m_vBullet.shrink_to_fit();
		}
	}
	Game::Instance()->m_pPlayer->Update();
	for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
		if (!Game::Instance()->m_vObject[i]->m_bisAlive) {
			if (Game::Instance()->m_vObject[i]->GetType() == ENEMY) {
				Game::Instance()->m_iNumEnemies--;
				delete Game::Instance()->m_vObject[i];
				Game::Instance()->m_vObject.erase(Game::Instance()->m_vObject.begin() + i);
			}
			else if (Game::Instance()->m_vObject[i]->GetType() == MINE) {
				delete Game::Instance()->m_vObject[i];
				Game::Instance()->m_vObject.erase(Game::Instance()->m_vObject.begin() + i);
			}
		}
	}
	/*static clock_t prevFrame;
	prevFrame = clock();

	timeSinceHonk -= ((double)clock() - prevFrame) / CLOCKS_PER_SEC;
	cout << timeSinceHonk << endl;*/

	

}

void GameState::Render()
{
	if (!Game::Instance()->m_bPauseRunning) {
		clock_t temp = ((double)clock() - Game::Instance()->start) / CLOCKS_PER_SEC;
		Game::Instance()->inGameTime_Seconds = Game::Instance()->time - temp;
	}
	buffer2 += "Time: " + to_string((int)Game::Instance()->inGameTime_Seconds + (int)Game::Instance()->totalElapsed_Seconds);
	//cout << Game::Instance()->inGameTime_Seconds + Game::Instance()->totalElapsed_Seconds << endl;
	if (Game::Instance()->inGameTime_Seconds + Game::Instance()->totalElapsed_Seconds < 0.0f) { // Negative time means the time has run out!
		SDL_Delay(100);
		Game::Instance()->GetFSM()->ChangeState(new LoseState());
		//Game::Instance()->Quit();	/* Change this to trigger the lose state for Milestone 3*/
		cout << "Game Over!" << endl;

	}
	if (Game::Instance()->m_iNumEnemies == 0) {
		Game::Instance()->m_vSounds[WAV_Chunk::WIN_STATE]->Play();
		SDL_Delay(500);
		Game::Instance()->GetFSM()->ChangeState(new WinState());
		//Game::Instance()->Quit();	/* Change this to trigger the win state for Milestone 3*/
		cout << "You win!" << endl;
	}
	SDL_FreeSurface(TheTextureManager::Instance()->textSurf[1]);
	SDL_DestroyTexture(TheTextureManager::Instance()->textTexture[1]);
	SDL_FreeSurface(TheTextureManager::Instance()->textSurf[2]);
	SDL_DestroyTexture(TheTextureManager::Instance()->textTexture[2]);

	if (!Game::Instance()->m_bPauseRunning) {
		SDL_FreeSurface(TheTextureManager::Instance()->textSurf[0]);
		SDL_DestroyTexture(TheTextureManager::Instance()->textTexture[0]);
		TheTextureManager::Instance()->textSurf[0] = TTF_RenderText_Blended(TheTextureManager::Instance()->gemunuLibreBold, buffer2.c_str(), { 255, 0, 0 });
		TheTextureManager::Instance()->textTexture[0] = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), TheTextureManager::Instance()->textSurf[0]);
		TheTextureManager::Instance()->textDest[0] = { 512 - (TheTextureManager::Instance()->textSurf[0]->w / 2), 0, TheTextureManager::Instance()->textSurf[0]->w, TheTextureManager::Instance()->textSurf[0]->h };
		TheTextureManager::Instance()->blendRect[0].w = TheTextureManager::Instance()->textDest[0].w + 40;
		TheTextureManager::Instance()->textDest[0].x = TheTextureManager::Instance()->blendRect[0].x + (TheTextureManager::Instance()->blendRect[0].w / 2) - (TheTextureManager::Instance()->textDest[0].w / 2);
	}
	buffer = "Enemies: " + to_string(Game::Instance()->m_iNumEnemies);
	TheTextureManager::Instance()->textSurf[1] = TTF_RenderText_Blended(TheTextureManager::Instance()->gemunuLibreBold, buffer.c_str(), { 255, 0, 0 });
	TheTextureManager::Instance()->blendRect[1].w = TheTextureManager::Instance()->textSurf[1]->w + 40;
	TheTextureManager::Instance()->blendRect[1].x = TheTextureManager::Instance()->textDest[1].x - 20;
	TheTextureManager::Instance()->textTexture[1] = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), TheTextureManager::Instance()->textSurf[1]);
	TheTextureManager::Instance()->textDest[1] = { 768 - (TheTextureManager::Instance()->textSurf[1]->w / 2), 0, TheTextureManager::Instance()->textSurf[1]->w, TheTextureManager::Instance()->textSurf[1]->h };
	buffer2.clear();
	buffer = "Health: ";
	TheTextureManager::Instance()->textSurf[2] = TTF_RenderText_Blended(TheTextureManager::Instance()->gemunuLibreBold, buffer.c_str(), { 255, 0, 0 });
	TheTextureManager::Instance()->textDest[2] = { Game::Instance()->m_pPlayer->m_rHealthBarDst.x - TheTextureManager::Instance()->textSurf[2]->w, 3, TheTextureManager::Instance()->textSurf[2]->w, TheTextureManager::Instance()->textSurf[2]->h };
	TheTextureManager::Instance()->textTexture[2] = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), TheTextureManager::Instance()->textSurf[2]);
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			Game::Instance()->m_pTiles[row][col]->Render();
		}
	}
	for (GameObject* laser : Game::Instance()->m_vBullet) {
		laser->Render();
	}
	for (GameObject* object : Game::Instance()->m_vObject) {
		object->Render();
	}
	Game::Instance()->m_pPlayer->Render();
	for (int i = 0; i < m_vButton.size(); i++) {
		m_vButton[i]->Render();
	}
	for (int i = 0; i < 2; i++) {
		SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 125);
		SDL_RenderFillRect(Game::Instance()->GetRenderer(), &TheTextureManager::Instance()->blendRect[i]);
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	}
	for (int i = 0; i < 3; i++)
		SDL_RenderCopy(Game::Instance()->GetRenderer(), TheTextureManager::Instance()->textTexture[i], NULL, &TheTextureManager::Instance()->textDest[i]);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);

}

void GameState::Exit()
{
	Game::Instance()->m_vSounds[WAV_Chunk::CAR_HORN]->Hault();
	Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->Hault();
	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	m_vButton.shrink_to_fit();
	Game::Instance()->keepPlaying = true;
}

void GameState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void TitleState::Enter()
{
	if (!Game::Instance()->keepPlaying)
	{
		Game::Instance()->m_vSounds[WAV_Chunk::TITLE_MUSIC]->Hault();
		Game::Instance()->m_vSounds[WAV_Chunk::TITLE_MUSIC]->Play();
	}
	Game::Instance()->m_vSounds[WAV_Chunk::GAME_MUSIC]->Hault();
	
	
	m_vButton.push_back(new Button("Start Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 320, 384));
	m_vButton.push_back(new Button("How To Play", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 704, 384));
	m_vButton.push_back(new Button("Highscores", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 320, 384+128));
	m_vButton.push_back(new Button("Quit Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 704, 384+128));
	Game::Instance()->keepPlaying = false;
}

void TitleState::Update()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new GameState());
			break;
		case 1:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new InstructionState());
			break;
		case 2:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new HighScoreState());
			break;
		case 3:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->Quit();
			break;
		}

	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 196, 255, 255);
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Render();
	}
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->instructionTexture,
		m_rInstructionSrc, m_rInstructionDst);
}

void TitleState::Exit()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	m_vButton.shrink_to_fit();
	Game::Instance()->m_bMousePressed = false;
}

void TitleState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void InstructionState::Enter() // 128
{
	Game::Instance()->keepPlaying = true;
	m_vLabelTexts.push_back(new LabelText("Instruction Menu:", "Assets/Fonts/calibri.ttf", 50, { 0, 0, 0 }, 128, 128));
	m_vLabelTexts.push_back(new LabelText("Movement:", "Assets/Fonts/calibri.ttf", 40, { 0, 0, 0 }, 128, 128 + m_vLabelTexts.back()->m_rDst.h + 10));
	m_vLabelTexts.push_back(new LabelText("W - Move Forward", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h + 10));
	m_vLabelTexts.push_back(new LabelText("S - Move Backwards", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("A - Steer Left", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("D - Steer Right", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("Space Button - Fire", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("H - Honk Horn", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));

	m_vLabelTexts.push_back(new LabelText("How to play:", "Assets/Fonts/calibri.ttf", 40, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h + 10));
	m_vLabelTexts.push_back(new LabelText("Survive as long as possible against series of deadly enemy,", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("turrets. Watch out for the hazards. Use the geometry to", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));
	m_vLabelTexts.push_back(new LabelText("take cover", "Assets/Fonts/calibri.ttf", 30, { 0, 0, 0 }, 128, m_vLabelTexts.back()->m_rDst.y + m_vLabelTexts.back()->m_rDst.h));

	m_vButton.push_back(new Button("Back to Menu", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 704));
}

void InstructionState::Update()
{
	for (LabelText* text : m_vLabelTexts)
	{
		text->Update();
	}
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new TitleState());
			break;
		}

	}
}

void InstructionState::Render()
{
	for (LabelText* text : m_vLabelTexts)
	{
		text->Render();
	}
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Render();
	}
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 48, 96, 48, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rblendedRect);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 128, 128, 255, 255);
}

void InstructionState::Exit()
{
	for (int i = 0; i < m_vLabelTexts.size(); i++)
	{
		delete m_vLabelTexts[i];
		m_vLabelTexts.erase(m_vLabelTexts.begin() + i);
	}
	m_vLabelTexts.shrink_to_fit();
}

void InstructionState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void PauseState::Enter()
{
	Game::Instance()->paused_Start = clock();

	Game::Instance()->m_bPauseRunning = true;
	m_vButton.push_back(new Button("Resume Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 384));
	m_vButton.push_back(new Button("Main Menu", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 384 + (141.5)));
	m_vButton.push_back(new Button("Quit Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 384 + (2 * 141.5)));
	m_vLabelTexts.push_back(new LabelText("Game Paused..", "Assets/Fonts/calibri.ttf", 50, { 0, 0, 0 }, 512, 256));
	m_vLabelTexts.back()->PlaceInCenter();
}

void PauseState::Update()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ResumeState(new GameState());
			break;
		case 1:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new TitleState());
			break;
		case 2:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->Quit();
			break;
		}
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Update();
	}
}

void PauseState::Render()
{
	GameState::Instance()->Render();

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 255, 100);
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_ADD);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &blendedRect);

	for (int i = 0; i < m_vButton.size(); i++)
		m_vButton[i]->Render();
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 0, 0, 255);
	for (LabelText* text : m_vLabelTexts)
	{
		text->Render();
	}
}

void PauseState::Exit()
{
	Game::Instance()->totalElapsed_Seconds += ((double)clock() - Game::Instance()->paused_Start) / CLOCKS_PER_SEC;

	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	m_vButton.shrink_to_fit();
	Game::Instance()->m_bPauseRunning = false;
	Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->Hault();
}

void PauseState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void LoseState::Enter()
{
	PlayerStats::deaths++;
	Game::Instance()->wonGame = false;

	Game::Instance()->SaveData();
	Game::Instance()->accuracy = (Game::Instance()->shotsFired / Game::Instance()->shotsFired) * 100.0f;
	//cout << "Accuracy: " << Game::Instance()->accuracy << endl;

	Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->Hault();
	Game::Instance()->m_vSounds[WAV_Chunk::GAME_MUSIC]->Hault();
	
	std::string temp = "You Survived " + to_string((int)Game::Instance()->inGameTime_Seconds + (int)Game::Instance()->totalElapsed_Seconds) + " seconds";

	m_vLabelTexts.push_back(new LabelText("Game Over!", "Assets/Fonts/GemunuLibre-Bold.ttf", 90, { 0, 0, 0 }, 512, 256 - 25));
	m_vLabelTexts.back()->PlaceInCenter();

	m_vLabelTexts.push_back(new LabelText(temp.c_str(), "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 25));
	m_vLabelTexts.back()->PlaceInCenter();

	temp = "Enemy Kills: " + to_string(10 - Game::Instance()->m_iNumEnemies);

	m_vLabelTexts.push_back(new LabelText(temp.c_str(), "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 75));
	m_vLabelTexts.back()->PlaceInCenter();

	char buffer[50];
	sprintf_s(buffer, "Accuracy: %.1f%%", (Game::Instance()->shotsHit == 0 && Game::Instance()->shotsFired == 0 ? 0.0f : (Game::Instance()->shotsHit / Game::Instance()->shotsFired) * 100.0f));

	//temp = "Accur: " + to_string(10 - Game::Instance()->m_iNumEnemies);

	m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 125));
	m_vLabelTexts.back()->PlaceInCenter();

	m_vButton.push_back(new Button("Main Menu", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 512));
	m_vButton.push_back(new Button("Quit Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 512 + 141.5));
}

void LoseState::Update()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new TitleState());
			break;
		case 1:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->Quit();
			break;
		}
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Update();
	}
}

void LoseState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 125);
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Render();
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Render();
	}
}

void LoseState::Exit()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	for (int i = 0; i < m_vLabelTexts.size(); i++)
	{
		delete m_vLabelTexts[i];
		m_vLabelTexts.erase(m_vLabelTexts.begin() + i);
	}
	m_vButton.shrink_to_fit();
	m_vLabelTexts.shrink_to_fit();
}

void LoseState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void WinState::Enter()
{
	Game::Instance()->wonGame = true;
	Game::Instance()->SaveData();
	Game::Instance()->accuracy = (Game::Instance()->shotsFired / Game::Instance()->shotsFired) * 100.0f;
	//cout << "Accuracy: " << Game::Instance()->accuracy << endl;

	Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->Hault();
	Game::Instance()->m_vSounds[WAV_Chunk::GAME_MUSIC]->Hault();

	std::string temp = "You Survived " + to_string(120 - ((int)Game::Instance()->inGameTime_Seconds + (int)Game::Instance()->totalElapsed_Seconds)) + " seconds";

	m_vLabelTexts.push_back(new LabelText("You Win!", "Assets/Fonts/GemunuLibre-Bold.ttf", 90, { 0, 0, 0 }, 512, 256 - 25));
	m_vLabelTexts.back()->PlaceInCenter();

	m_vLabelTexts.push_back(new LabelText(temp.c_str(), "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 25));
	m_vLabelTexts.back()->PlaceInCenter();

	temp = "Enemy Kills: " + to_string(10 - Game::Instance()->m_iNumEnemies);

	m_vLabelTexts.push_back(new LabelText(temp.c_str(), "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 75));
	m_vLabelTexts.back()->PlaceInCenter();

	char buffer[50];
	sprintf_s(buffer, "Accuracy: %4.1f%%", (Game::Instance()->shotsHit == 0 && Game::Instance()->shotsFired == 0 ? 0.0f : (Game::Instance()->shotsHit / Game::Instance()->shotsFired) * 100.0f));

	//temp = "Accur: " + to_string(10 - Game::Instance()->m_iNumEnemies);

	m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 30, { 0, 0, 0 }, 512, 256 + 125));
	m_vLabelTexts.back()->PlaceInCenter();

	m_vButton.push_back(new Button("Main Menu", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 512));
	m_vButton.push_back(new Button("Quit Game", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 512 + 141.5));
}

void WinState::Update()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new TitleState());
			break;
		case 1:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->Quit();
			break;
		}
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Update();
	}
}

void WinState::Render()
{
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 0, 125);
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Render();
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Render();
	}

}

void WinState::Exit()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	for (int i = 0; i < m_vLabelTexts.size(); i++)
	{
		delete m_vLabelTexts[i];
		m_vLabelTexts.erase(m_vLabelTexts.begin() + i);
	}
	m_vButton.shrink_to_fit();
	m_vLabelTexts.shrink_to_fit();
}

void WinState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}

void HighScoreState::Enter()
{
	char buffer[50];
	Game::Instance()->keepPlaying = true;
	m_vLabelTexts.push_back(new LabelText("Player Stats:", "Assets/Fonts/GemunuLibre-Bold.ttf", 90, { 0, 0, 0 }, 512, 64));
	m_vLabelTexts.back()->PlaceInCenter();
	m_vLabelTexts.push_back(new LabelText("Best Times:", "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 256, 192));
	m_vLabelTexts.back()->PlaceInCenter();
	m_vLabelTexts.push_back(new LabelText("Best Accuracy:", "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 704, 192));
	m_vLabelTexts.back()->PlaceInCenter();
	sprintf_s(buffer, "Kills: %d", PlayerStats::kills);
	m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 124, 542));
	m_vLabelTexts.back()->m_rDst.x = m_vLabelTexts[1]->m_rDst.x + m_vLabelTexts[1]->m_rDst.w / 2;
	m_vLabelTexts.back()->PlaceInCenter();
	sprintf_s(buffer, "Deaths: %d", PlayerStats::deaths);
	m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 704, 542));
	m_vLabelTexts.back()->m_rDst.x = m_vLabelTexts[2]->m_rDst.x + m_vLabelTexts[2]->m_rDst.w / 2;
	m_vLabelTexts.back()->PlaceInCenter();

	m_vButton.push_back(new Button("Main Menu", "Assets/Fonts/calibri.ttf", 30, { 255, 255, 255 }, 512, 640));
	
	for (int i = 0; i < (PlayerStats::m_vHighestTimes.size() > 5 ? 5 : PlayerStats::m_vHighestTimes.size()); i++)
	{
		sprintf_s(buffer, "%d. %.1f seconds", i + 1, PlayerStats::m_vHighestTimes[i]);
		m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 256, 256 + (i * 50)));
		m_vLabelTexts.back()->m_rDst.x = m_vLabelTexts[1]->m_rDst.x;
	}

	for (int i = 0; i < (PlayerStats::m_vAccuracy.size() > 5 ? 5 : PlayerStats::m_vAccuracy.size()); i++)
	{
		sprintf_s(buffer, "%d. %.1f%%", i + 1, PlayerStats::m_vAccuracy[i]);
		m_vLabelTexts.push_back(new LabelText(buffer, "Assets/Fonts/GemunuLibre-Bold.ttf", 50, { 0, 0, 0 }, 704, 256 + (i * 50)));
		m_vLabelTexts.back()->m_rDst.x = m_vLabelTexts[2]->m_rDst.x;
	}
}

void HighScoreState::Update()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Update();
		switch (i)
		{
		case 0:
			if (m_vButton[i]->GetState() == CLICKED)
				Game::Instance()->GetFSM()->ChangeState(new TitleState());
			break;
		}
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Update();
	}
}

void HighScoreState::Render()
{
	
	for (int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i]->Render();
	}
	for (LabelText* text : m_vLabelTexts)
	{
		text->Render();
	}
	//SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 0, 0, 255);
	//SDL_RenderDrawLine(Game::Instance()->GetRenderer(), 256, 0, 256, 1024);
	//SDL_RenderDrawLine(Game::Instance()->GetRenderer(), 704, 0, 704, 1024);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0, 196, 255, 255);
}

void HighScoreState::Exit()
{
	for (int i = 0; i < m_vButton.size(); i++)
	{
		delete m_vButton[i];
		m_vButton.erase(m_vButton.begin() + i);
	}
	for (int i = 0; i < m_vLabelTexts.size(); i++)
	{
		delete m_vLabelTexts[i];
		m_vLabelTexts.erase(m_vLabelTexts.begin() + i);
	}
	m_vButton.shrink_to_fit();
	m_vLabelTexts.shrink_to_fit();
}

void HighScoreState::HandleEvents()
{
	int x, y;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::Instance()->Quit();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&x, &y);
			Game::Instance()->SetMousePosition(x, y);
			cout << x << " " << y << endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				Game::Instance()->m_bMousePressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			Game::Instance()->m_bMousePressed = false;
			break;
		}
	}
}
