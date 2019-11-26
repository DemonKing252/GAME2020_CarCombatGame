#include "Bullet.h"
#include "Game.h"
/*======================= Milestone 3 ============================*//*
 > Collision with brick walls/obstacles etc...
																	*/
/* This bullet class will be used for all enemy and player bullets */

// Red bullets:
// m_rSrc = { 214, 316, 256-214, 394-316 };

// Blue bullets:
// m_rSrc = {};

// int xpos, int ypos, int index
Bullet::Bullet(int x, int y, TargetActor p_gTargetActor)
{
	
	SetPosition(Vector2(x, y));
	m_rDst = { x, y, BULLET_WIDTH, BULLET_HEIGHT };
	m_dAngle = Game::Instance()->m_pPlayer->m_dAngle;
	
	if (p_gTargetActor == NONE)
		m_rSrc = { 307, 41, 349 - 307, 127 - 41 };
	else if (p_gTargetActor == PLAYER)
		m_rSrc = { 213, 316, 256 - 213, 396 - 316 };
	
	m_bisAlive = true;
	this->m_TargetActor = p_gTargetActor;
}


Bullet::~Bullet()
{
}

void Bullet::Render()
{
	TheTextureManager::Instance()->Draw(m_bAnimate ? TheTextureManager::Instance()->explosionSprite : TheTextureManager::Instance()->laserTexture,
		m_rSrc, m_rDst, m_dAngle);
}

void Bullet::Update()
{
	if (m_bAnimate) {
		AnimatedSprite_Update(m_rSrc, m_bisAlive);
	}
	if (!m_bAnimate) {
		Vector2 newPosition;
		newPosition.x = GetPosition().x + 16.0f * VectorMath::DeltaX(m_dAngle);
		newPosition.y = GetPosition().y + 16.0f * VectorMath::DeltaY(m_dAngle);
		SetPosition(newPosition);

	}
	for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
		if (SDL_HasIntersection(&m_rDst, &Game::Instance()->m_vObject[i]->m_rDst) && GetTargetActor() == TargetActor::NONE) {
			switch (Game::Instance()->m_vObject[i]->GetType()) {
			case ROCK:
				m_bAnimate = true;
				Game::Instance()->m_vObject[i]->m_dHealth -= 12.5f; // 12.5% damage (float precision)
				break;
			case ENEMY:
				Game::Instance()->shotsHit++;
				Game::Instance()->accuracy = (Game::Instance()->shotsHit / Game::Instance()->shotsFired) * 100;
				m_bisAlive = false;
				Game::Instance()->m_vObject[i]->m_dHealth -= 12.5f;	// 12.5% damage (float precision)
				break;
			}
		}
	}

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[i][j]->m_rDst)) {
				switch (Game::Instance()->m_pTiles[i][j]->GetTexture()) {
				case BRICK:
					m_bAnimate = true;
					break;
				}
			}
		}
	}
	if (SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pPlayer->m_rDst) && GetTargetActor() == TargetActor::PLAYER) {
		if (!m_bAnimate) {
			Game::Instance()->m_vSounds[WAV_Chunk::EXPLOSION]->Play();
			Game::Instance()->m_pPlayer->m_dHealth -= 7.5f;
		}
		
		
		m_bAnimate = true;
	}
	Game::Instance()->m_vObject.shrink_to_fit();
}
