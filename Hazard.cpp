#include "Hazard.h"
#include "Game.h"
/*======================= Milestone 1 ============================*//*
 > Spikes are rendered and rotate on the screen, no damage yet														

																	*/

/*======================= Milestone 2 ============================*//*
 > Spikes can kill the player, will immediatly auto quit the game
																	*/

/*======================= Milestone 4 ============================*//*
 > Spikes will kill the player and trigger the lose state
																	*/
void Spike::Update()
{
	m_dAngle += 4.0f;
}

void Spike::Render()
{
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->spikeTexture,
		m_rSrc, m_rDst, m_dAngle);
}

Spike::Spike(int x, int y)
{
	m_rSrc = { 0, 0, 700, 700 };
	m_type = SPIKE;
	SetPosition(Vector2(x * 32, y * 32));
	m_rDst = { x * 32, y * 32, 64, 64 };
}


Spike::~Spike()
{
}

void Mine::Update()
{
	SDL_Rect temp = m_rDst;
	temp.w = 24;
	temp.h = 24;
	temp.x = m_rDst.x + (m_rDst.w / 2) - (temp.w / 2);
	temp.y = m_rDst.y + (m_rDst.h / 2) - (temp.h / 2);

	if (SDL_HasIntersection(&Game::Instance()->m_pPlayer->m_rDst, &m_rDst))
	{
		Game::Instance()->m_vSounds[WAV_Chunk::EXPLOSION]->Play();
		m_bAnimate = true;
		if (m_dHealth != 0.0f)
		{
			Game::Instance()->m_pPlayer->m_dHealth -= 55.0f;	// Player takes 55% damage
		}
		m_dHealth = 0.0f;
	}
	if (m_bAnimate) {
		AnimatedSprite_Update(m_rSrc, m_bisAlive);
	}
	if (m_dHealth <= 0.0)
	{
		AnimatedSprite_Init();
		m_rHealthBackBarDst = { 0 };
		m_rHealthBarDst = { 0 };
	}
}

void Mine::Render()
{
	TheTextureManager::Instance()->Draw(m_bAnimate ? TheTextureManager::Instance()->explosionSprite : TheTextureManager::Instance()->mineTexture, m_rSrc, m_rDst);
}

Mine::Mine(int x, int y)
{
	m_bisAlive = true;
	m_bAnimate = false;
	m_dHealth = 100.0f;
	m_rSrc = { 0, 0, 229, 220 };
	SetPosition(Vector2(x * 32, y * 32));
	m_rDst = { x * 32, y * 32, 64, 64 };
	m_type = MINE;
}

Mine::~Mine()
{
}
