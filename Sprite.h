#pragma once
#ifndef __SPRITE__
#define __SPRITE__
#include "SDL.h"
#include <ctime>
class Sprite
{
private:
	int m_iFrameX,
		m_iFrameY;
protected:
	void AnimatedSprite_Update(SDL_Rect&, bool&);
	void AnimatedSprite_Init();
public:
	bool m_bAnimate = false;
	SDL_Rect m_rSrc;
	SDL_Rect m_rDst;
	float start_Death;
	Sprite();
	~Sprite();
};

#endif // !__SPRITE__