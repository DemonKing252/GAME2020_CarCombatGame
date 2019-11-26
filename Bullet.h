#pragma once
#ifndef __BULLET__
#define __BULLET__
#include "GameObject.h"
#define BULLET_WIDTH (int)(42 / 2 * 0.9)
#define BULLET_HEIGHT (int)(86 / 2 * 0.9)
class Bullet :
	public GameObject
{
public:
	Bullet(int, int, TargetActor);
	~Bullet();
	void Render();
	void Update();
};
#endif // !__BULLET__