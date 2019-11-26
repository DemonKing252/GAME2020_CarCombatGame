#pragma once
#ifndef __BGTILE__
#define __BGTILE__
#include <fstream>
#include <string>
#include "GameObject.h"
enum Texture { GRASS, BRICK, GRAVEL, WATER, LAVA };
using namespace std;
class BGTile : public GameObject
{
private:
	Texture m_texture;
	string id;
public:
	void SetTexture(Texture);
	Texture GetTexture();
	void SetTile(int x, int y);
	void Update();
	void Render();
};
#endif // !__TILE__