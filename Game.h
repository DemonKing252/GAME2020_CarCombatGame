#pragma once
#ifndef __GAME__
#define __GAME__
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "TextureManager.h"
#include "FSM.h"
#include <iostream>
#include <vector>
#include "PlayerStats.h"
#include <ctime>
#include <string>
#include <cassert>
#include <fstream>
#include <cstdlib>
#include "XMLStuff/tinyxml2.h"
#define FPS 60

using namespace std;
using namespace tinyxml2;

class Game
{
protected:
	int flag = 1;
	const Uint8* m_iKeystates;
	Uint32 frameStart, frameTime, delayTime = 1000.0f / FPS;
	SDL_Renderer* m_pRenderer;
	SDL_Window* m_pWindow;
	bool m_bRunning;
	SDL_Point m_mousePosition;
	FSM* m_pFSM;
public:
	float accuracy;
	float shotsFired, shotsHit;

	float timeSinceHonk_Start;
	float timeSinceHonk = 0.0f;

	bool wonGame = false;

	bool keepPlaying = false;
	std::vector <AudioManager*> m_vSounds;
	float paused_Start, totalElapsed_Seconds = 0, totalElapsed_Minutes = 0;
	bool m_bPauseRunning = false;
	float time, inGameTime_Seconds, inGameTime_Minutes, start;
	float displaceX, displaceY;
	FSM* GetFSM();
	bool m_bMousePressed;
	void SetMousePosition(int, int);
	SDL_Point GetMousePosition();

	BGTile* m_pTiles[ROWS][COLS];
	int m_iNumEnemies = 0;
	/* Move all of these to the gamestate */
	GameObject* m_pPlayer;
	vector<GameObject*> m_vObject;	/*	Vector for all of the different objects on screen that are not part of the geometry.
										Handled using abstract classes for efficient use of code
									*/
	vector <GameObject*> m_vBullet; /* Vector for all of the different bullets in the game */
	void Quit();
	bool Run(const char* title, int xpos, int ypos, int width, int height, int flags);
	SDL_Renderer* GetRenderer();
	static Game* Instance()
	{
		static Game* instance = new Game;
		return instance;
	}
	void LoadData();
	void SaveData();

	void Load();
	bool Running();
	bool KeyDown(SDL_Scancode c);
	void Render();
	void Update();
	void HandleEvents();
	void Clean();
};
#endif // !__GAME__
