#include "BGTile.h"
#include "Game.h"


void BGTile::SetTexture(Texture text)
{
	m_texture = text;
	switch (text)
	{
	case BRICK:
		id = "brickWall";
		break;
	case GRASS:
		id = "tile";
		break;
	case WATER:
		id = "water";
		break;
	case LAVA:
		id = "lava";
		break;
	case GRAVEL:
		id = "gravel";
		break;
	}
}

Texture BGTile::GetTexture()
{
	return m_texture;
}

void BGTile::SetTile(int x, int y)
{
	m_rDst = { y * 32, x * 32, 32, 32 };
	SetPosition(Vector2(y * 32, x * 32));
}



void BGTile::Update()
{
}

void BGTile::Render()
{
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->tileTexture[GetTexture()], m_rDst);
}

