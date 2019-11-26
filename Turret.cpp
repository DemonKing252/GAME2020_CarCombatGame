#include "Turret.h"
#include "Game.h"
/*======================= Milestone 1 ============================*//*
 > 10 Turrets are placed on the map
 > No AI behaviour implemented yet

																	*/

/*======================= Milestone 2 ============================*//*
 > Turrets fire bullets
 > Turrets look at the player
 > Damage to the player is not implemented yet
																    */

/*======================= Milestone 3 ============================*//*
 > There is more than one variety not just one. They all deal different 
 amounts of damage
 > Damage from the turrets will decrease the players health which will
 be added when health bars are implemented
																	*/

/*======================= Milestone 4 ============================*//*
> Nothing new except that when the player dies it triggers the lose state
																	*/
void Turret::Update()
{
	double playerX = Game::Instance()->m_pPlayer->GetAbsolutePositionX();
	double playerY = Game::Instance()->m_pPlayer->GetAbsolutePositionY();
	m_dAngle = VectorMath::Angle(GetPosition().x + (m_rDst.w / 2), GetPosition().y + (m_rDst.h / 2), playerX, playerY);//(atan2(playerY - (m_y + (m_rDst.h / 2)), playerX - (m_x + m_rDst.w / 2))) * 180 / M_PI + 90;

	if (((double)clock() - start_T) / CLOCKS_PER_SEC >= shootNext_T)
	{
		// 4, 9, 14, 19, 24, 29
		shootNext_T = (5 * (rand() % 3)) + 4;
		start_T = clock();
		Game::Instance()->m_vBullet.push_back(new Bullet(m_rDst.x + (m_rDst.w / 2) - (BULLET_WIDTH / 2),
			m_rDst.y + (m_rDst.h / 2) - (BULLET_HEIGHT / 2),
			TargetActor::PLAYER));
		Game::Instance()->m_vSounds[WAV_Chunk::FIRE]->Play();
		Game::Instance()->m_vBullet.back()->m_dAngle = m_dAngle;
	}

	if (m_bAnimate) {
		AnimatedSprite_Update(m_rSrc, m_bisAlive);
	}

	m_dDistanceFromTarget = VectorMath::Magnitude(GetPosition().x + (m_rDst.w / 2), GetPosition().y + (m_rDst.h / 2), playerX, playerY);//sqrt(pow(playerX - (m_x + (m_rDst.w / 2)), 2) + (pow(playerY - (m_y + (m_rDst.h / 2)), 2)));
	if (m_dHealth <= 0.0)
	{
		if (!m_bAnimate)
			PlayerStats::kills++;
		Game::Instance()->m_vSounds[WAV_Chunk::EXPLOSION]->Play();
		AnimatedSprite_Init();
		m_rHealthBackBarDst = { 0 };
		m_rHealthBarDst = { 0 };
	}
	if (!m_bisAlive)
	{
		m_bisAlive = false;
	}
}

void Turret::Render()
{
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_NONE);
	if (m_dHealth >= 0.0f)
		m_rHealthBarDst.w = (int)((m_dHealth / 100) * m_rDst.w);

	TheTextureManager::Instance()->Draw(m_bAnimate ? TheTextureManager::Instance()->explosionSprite : TheTextureManager::Instance()->enemyTurret,
		m_rSrc, m_rDst, (m_bAnimate ? 0 : m_dAngle + 90));

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 48, 96, 48, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rHealthBackBarDst);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), (Uint8)(255 - (255 * (m_dHealth / 100))), (Uint8)(255 * (m_dHealth / 100)), 0, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rHealthBarDst);
	
	
}

Turret::Turret(SDL_Rect p_rSrc, float x, float y)
{
	shootNext_T = (5 * (rand() % 3)) + 4;
	start_T = clock();
	SetType(ENEMY);
	m_rHealthBackBarDst = { (int)(x * 32) - 9, (int)(y * 32) - 9, 194/3, 6 };
	m_rHealthBarDst = { (int)(x * 32), (int)(y * 32) - 9, 194 / 3, 6 };
	m_dHealth = 100;
	this->m_rSrc = p_rSrc;
	m_rDst = { static_cast<int>(x * 32), static_cast<int>(y * 32), 194/3, 129/3 };
	SetPosition(Vector2(x * 32, y * 32));
	m_bisAlive = true;

}


Turret::~Turret()
{
}
