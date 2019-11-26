#include "Sprite.h"



void Sprite::AnimatedSprite_Update(SDL_Rect &p_rSrc, bool &p_risAlive)
{
	if (m_bAnimate)
	{
		m_iFrameX++;
		if (m_iFrameX == 3)
		{
			m_iFrameX = 0;
			m_iFrameY++;
			if (m_iFrameY == 3)
			{
				p_risAlive = false;
			}
		}
		p_rSrc = { m_iFrameX * 128, m_iFrameY * 128, 128, 128 };
	}
}

void Sprite::AnimatedSprite_Init()
{
	m_bAnimate = true;
}

Sprite::Sprite()
{
	m_iFrameX = 0;
	m_iFrameY = 0;
}


Sprite::~Sprite()
{
}
