#include "Object.h"
#include "Game.h"


void Rock::Update()
{
}

void Rock::Render()
{
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->rockTexture, m_rDst);
}

Rock::Rock(int x, int y)
{
	m_type = ROCK;
	m_rDst = { x * 32, y * 32, 32, 32 };
	SetPosition(Vector2(x * 32, y * 32));
}


Rock::~Rock()
{
}
/*======================= Milestone 2 ============================*//*
 > Bushes will get destroyed when run over
																	*/
Bush::Bush(int x, int y, int w, int h, SDL_Rect src)
{
	m_type = BUSH;
	m_rDst = { x * 32, y * 32, w, h };
	SetPosition(Vector2(x * 32, y * 32));
	this->m_rSrc = src;
}
Bush::~Bush()
{
}
void Bush::Render()
{
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->natureTexture,
		m_rSrc, m_rDst);
}
void Bush::Update()
{

}