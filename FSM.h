#pragma once
#ifndef __FSM__
#define __FSM__
#include "SDL.h"
#include "SDL_ttf.h"
#include "Player.h"
#include "Object.h"
#include "Hazard.h"
#include "Turret.h"
#include "AudioManager.h"
#include <string>
#include "Bullet.h"
#include "BGTile.h"
#include "LabelText.h"
#include "Button.h"
#include "AudioManager.h"
#include <vector>
#include "PlayerStats.h"
#define ROWS 48
#define COLS 32
class Game;
class State
{
protected:
	std::vector <Button*> m_vButton;
	std::vector <LabelText*> m_vLabelTexts;
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void HandleEvents() = 0;
};
class InstructionState : public State
{
	static InstructionState* Instance()
	{
		InstructionState* instance = new InstructionState();
		return instance;
	}
	SDL_Rect m_rblendedRect = { 512 - (768 / 2), 384 - (512 / 2), 768, 512 };
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class TitleState : public State
{
public:
	SDL_Rect m_rInstructionDst = { 512 - (260 / 2) - (260 / 2), 64, 516, 260 };
	SDL_Rect m_rInstructionSrc = { 0, 0, 516, 260 };
	static TitleState* Instance()
	{
		TitleState* instance = new TitleState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class GameState : public State
{
public:
	std::string buffer, buffer2;
	static GameState* Instance()
	{
		GameState* instance = new GameState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class PauseState : public State
{
public:
	SDL_Rect blendedRect = { 128, 128, 768, 604 };
	static PauseState* Instance()
	{
		static PauseState* instance = new PauseState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class LoseState : public State
{
public:
	static LoseState* Instance()
	{
		static LoseState* instance = new LoseState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class WinState : public State
{
public:
	static WinState* Instance()
	{
		static WinState* instance = new WinState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class HighScoreState : public State
{
	static HighScoreState* Instance()
	{
		static HighScoreState* instance = new HighScoreState();
		return instance;
	}
	void Enter();
	void Update();
	void Render();
	void Exit();
	void HandleEvents();
};
class FSM
{
public:
	State* m_pState;
	
	void ChangeState(State* changeTo);
	void SaveState(State* changeTo);
	void ResumeState(State* changeTo);
	
	FSM();
	~FSM();
	void Render();
	void Update();
	void HandleEvents();
};
#endif // !__FSM__
