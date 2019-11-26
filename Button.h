#pragma once
#ifndef __BUTTON__
#define __BUTTON__
#include "SDL.h"
#include "SDL_ttf.h"
#include "LabelText.h"
enum ButtonState { NEUTRAL, HIGHLIGHTED, CLICKED };
class Button
{
private:
	ButtonState m_State;
public:
	LabelText* m_pLabelText;
	ButtonState GetState();
	void SetButtonState(ButtonState);
	SDL_Rect m_rButtonSrc[2] = { { 0, 0, 668, 446 / 2}, {0, 226, 668, 446 / 2} };
	SDL_Rect m_rButtonDst = { 512 - (668 / 4), 384 - (223 / 4), 668 / 2, 223 / 2 };

	Button(const char*, const char*, int, SDL_Color, int, int);
	~Button();
	void Render();
	void Update();
};
#endif // !__BUTTON__