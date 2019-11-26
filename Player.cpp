#include "Player.h"
#include "Game.h"
#include "BGTile.h"
Vector2 Camera::m_gCameraPos = Vector2(512.0f, 384.0f);
/*======================= Milestone 1 ============================*//*
 > Player can move freely in the level while staying inside the arena
																	*/

/*======================= Milestone 2 ============================*//*
> Player can shoot and do damage to the turrets
																	*/
/*======================= Milestone 3 ============================*//*
> Player does damage to the turrets health, and rather decreases a
  health bar that each of them have
> Player can now collide with hazards
																	*/
void Player::Update()
{
	std::stringstream ss;
	ss << "Speed: " << std::setprecision(2) << m_dSpeed << " px/frame";
	string temp = ss.str();
	for (LabelText* m_vText : m_vTexts)
	{
		m_vText->ChangeText(temp.c_str());
		m_vText->Update();
	}
	AnimatedSprite_Update(m_rSrc, m_bisAlive);
	if (m_dHealth <= 0.0f)
	{
		if (m_bAnimate)
		{
			start_Death = clock();
		}
		m_bAnimate = true;
	}
	if (m_dHealth <= 0.0f)
	{
		float temp = ((double)clock() - start_Death) / CLOCKS_PER_SEC;
		if (temp >= 2.0f)
		{
			SDL_Delay(100);
			Game::Instance()->GetFSM()->ChangeState(new LoseState());
		}
	}
	
	counter++;
	if (Game::Instance()->KeyDown(SDL_SCANCODE_W)) {
		if (m_dSpeed <= 5.2f)
			m_dSpeed += 0.4f;
	}
	else {
		if (m_dSpeed >= 0.4f)
			m_dSpeed -= 0.4f;
	}
	if (Game::Instance()->KeyDown(SDL_SCANCODE_S)) {
		if (m_dSpeed >= -5.2f)
			m_dSpeed -= 0.4f;
	}
	else {
		if (m_dSpeed <= -0.4f)
			m_dSpeed += 0.4f;
	}
	if (Game::Instance()->KeyDown(SDL_SCANCODE_A)) {
		m_dAngle -= 6.0f;
	}

	if (Game::Instance()->KeyDown(SDL_SCANCODE_D)) {
		m_dAngle += 6.0f;
	}
	Game::Instance()->timeSinceHonk = ((double)clock() - Game::Instance()->timeSinceHonk_Start) / CLOCKS_PER_SEC;

	if (Game::Instance()->KeyDown(SDL_SCANCODE_H) && Game::Instance()->timeSinceHonk >= 0.5f)
	{
		Game::Instance()->m_vSounds[WAV_Chunk::CAR_HORN]->Play();
		Game::Instance()->timeSinceHonk_Start = clock();
	}

	//cout << Game::Instance()->timeSinceHonk << endl;
	if ((Game::Instance()->m_bMousePressed || Game::Instance()->KeyDown(SDL_SCANCODE_SPACE)) && counter >= 8)
	{
		Game::Instance()->m_vSounds[WAV_Chunk::FIRE]->Play();
		counter = 0;
		Game::Instance()->shotsFired++;

		Game::Instance()->m_vBullet.push_back(new Bullet(m_rDst.x + (m_rDst.w / 2) - (BULLET_WIDTH / 2),
			m_rDst.y + (m_rDst.h / 2) - (BULLET_HEIGHT / 2),
			TargetActor::NONE));

	}
	//cout << Game::Instance()->shotsFired << " " << Game::Instance()->shotsHit << endl;
	Camera::MoveX(-m_dSpeed * VectorMath::DeltaX(m_dAngle));
	Camera::MoveY(-m_dSpeed * VectorMath::DeltaY(m_dAngle));
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			switch (Game::Instance()->m_pTiles[row][col]->GetTexture())
			{
			case BRICK:
				if (SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[row][col]->m_rDst))
				{
					if (m_dSpeed >= 0.1f)
					{
						Camera::MoveX(5.2f * VectorMath::DeltaX(m_dAngle));
						Camera::MoveY(5.2f * VectorMath::DeltaY(m_dAngle));
					}
					else
					{
						Camera::MoveX(-5.2f * VectorMath::DeltaX(m_dAngle));
						Camera::MoveY(-5.2f * VectorMath::DeltaY(m_dAngle));
					}
				}

				break;
			case LAVA:
				if (SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[row][col]->m_rDst))
				{
					m_dHealth -= 2.0f;	//	1 * 60FPS = 120% damage per second (a little under a second to live)
					m_dSpeed /= 2.0f;

					if ((int)m_dHealth % 20 == 0)	//	Play the damage sound every 10 frames.
						Game::Instance()->m_vSounds[WAV_Chunk::CAR_STANDARD_DAMAGE]->Play();
				}
				break;
			default:
				break;
			}

			/*======================= Milestone 3 ============================*///	--> Trigger the lose state
			/*else if (Game::Instance()->m_pTiles[row][col].tileType == Game::Instance()->m_pTiles[row][col].BRICK &&
				SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[row][col].m_rDst))
			{
				cout << "Detecting collision with spike. Damage not implemented yet . . ." << endl;

			}*/
			/*======================= Milestone 3 ============================*///
			/*else if (Game::Instance()->m_pTiles[row][col].tileType == Game::Instance()->m_pTiles[row][col].LAVA &&
				SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[row][col].m_rDst))
			{
				cout << "Colliding with lava tile..." << endl;

			}*/
			/*======================= Milestone 3 ============================*/
			/*else if (Game::Instance()->m_pTiles[row][col].tileType == Game::Instance()->m_pTiles[row][col].BUSH &&
				SDL_HasIntersection(&m_rDst, &Game::Instance()->m_pTiles[row][col].m_rDst))
			{
				> Have the bush get destroyed as an animation effect
			}*/

		}
	}
	for (GameObject* object : Game::Instance()->m_vObject)
	{
		if (object->GetType() == ROCK || object->GetType() == BUSH || object->GetType() == SPIKE)
		{
			SDL_Rect temp = object->m_rDst;
			if (object->GetType() != SPIKE)
			{
				temp.w = 24;
				temp.h = 24;
			}
			else
			{
				temp.w = 16;
				temp.h = 16;
			}
			
			temp.x = object->m_rDst.x + (object->m_rDst.w / 2) - (temp.w / 2);
			temp.y = object->m_rDst.y + (object->m_rDst.h / 2) - (temp.h / 2);
			if (SDL_HasIntersection(&m_rDst, &temp))
			{
				//cout << Game::Instance()->m_bCanGo << endl;
				//Game::Instance()->m_bCanGo = false;
				if (m_dSpeed >= 0.1f)
				{
					Camera::MoveX(5.2f * VectorMath::DeltaX(m_dAngle));
					Camera::MoveY(5.2f * VectorMath::DeltaY(m_dAngle));
				}
				else
				{
					Camera::MoveX(-5.2f * VectorMath::DeltaX(m_dAngle));
					Camera::MoveY(-5.2f * VectorMath::DeltaY(m_dAngle));
				}

			}
		}
		if (object->GetType() == SPIKE)
		{
			//SDL_Rect temp = object->m_rDst;
			//temp.w = 64;
			//temp.h = 64;
			//temp.x = object->m_rDst.x + (object->m_rDst.w / 2) - (temp.w / 2);
			//temp.y = object->m_rDst.y + (object->m_rDst.h / 2) - (temp.h / 2);
			if (SDL_HasIntersection(&m_rDst, &object->m_rDst))
			{
				if ((int)m_dHealth % 10 == 0) // Every 15 frames, play the damage effect.
				{
					Game::Instance()->m_vSounds[WAV_Chunk::CAR_STANDARD_DAMAGE]->Play();
				}
				m_dHealth -= 1.0f; // 1.5 * 60 = 6% damage per second
			}
		}
	}
	/* Keep the player from leaving the arena if he happens to get out */
	if (Camera::GetPosition().x < 36.0)
	{
		Camera::SetX(36);
	}
	if (Camera::GetPosition().y < 35.8)
	{
		Camera::SetY(35.9);
	}
	if (Camera::GetPosition().x > 924.3)
	{
		Camera::SetX(924.2);
	}
	if (Camera::GetPosition().y > 1435.4)
	{
		Camera::SetY(1435.4);
	}
	if (m_dSpeed >= 0.0f)			// 30-70% volume depending on the players speed. 5.2 pixels per frame is the max speed
		Mix_Volume(Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->channelNumber, 20 + (m_dSpeed / 5.2f) * 20);
	else												
		Mix_Volume(Game::Instance()->m_vSounds[WAV_Chunk::CAR_IDLE]->channelNumber, 20 + (-m_dSpeed / 5.2f) * 20);
}

void Player::Render()
{
	for (LabelText* text : m_vTexts)
		text->Render();
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_NONE);
	m_rHealthBarDst.w = (int)((m_dHealth / 100) * 256);
	/* Note: + 90 degrees because the sprite is facing left when it should be left, this is a good solution */
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 48, 96, 48, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rHealthBackBarDst);
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), (Uint8)(255 - (255 * (m_dHealth / 100))), (Uint8)(255 * (m_dHealth / 100)), 0, 255);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), &m_rHealthBarDst);
	
	TheTextureManager::Instance()->Draw((m_bAnimate ? TheTextureManager::Instance()->explosionSprite : TheTextureManager::Instance()->tankTexture),
		m_rSrc, m_rDst, m_dAngle);
}

Player::Player()
{
	std::stringstream ss;
	ss << "Speed: " << std::setprecision(1) << m_dSpeed << " px/s";
	string temp = ss.str();
	m_vTexts.push_back(new LabelText(temp.c_str(), "Assets/Fonts/MotionControl.otf", 35, { 255, 0, 0 }, 30, 36));
	m_dHealth = 100;
	start = clock();
	m_bisAlive = true;
	m_rSrc = { 0, 0, 620, 691 };
	SetPosition(Vector2(512.0f, 384.0f));
	m_rDst = { 512, 384, 64, 64 };
	m_rHealthBackBarDst = { 115, 3, 256, 25 };
	m_rHealthBarDst = { 115, 3, 256, 25 };
}


Player::~Player()
{
	for (int i = 0; i < m_vTexts.size(); i++)
	{
		delete m_vTexts[i];
		m_vTexts.erase(m_vTexts.begin() + i);
	}
	m_vTexts.shrink_to_fit();
}
