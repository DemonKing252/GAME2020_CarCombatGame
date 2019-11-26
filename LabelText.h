#pragma once
#ifndef __TTF_TEXT__
#define __TTF_TEXT__
#include "SDL.h"
#include "SDL_ttf.h"
class LabelText
{
public:
	const char* m_cText;
	SDL_Color color;
	SDL_Texture* m_pTexture;
	SDL_Surface* m_pSurface;
	SDL_Rect m_rDst;
	TTF_Font* m_pFont;
	LabelText(const char*, const char*, int, SDL_Color, int, int);
	~LabelText();
	void Render();
	void PlaceInCenter();
	void Update();
	void ChangeText(const char*);
};

#endif // !__TTF_TEXT__
