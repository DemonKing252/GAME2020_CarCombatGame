#pragma once
#ifndef __OBJECT__
#define __OBJECT__
#include "GameObject.h"
class Rock :
	public GameObject
{
public:
	void Update();
	void Render();
	Rock(int, int);
	~Rock();
};
class Bush
	: public GameObject
{
public:
	void Render();
	void Update();
	Bush(int, int, int, int, SDL_Rect);
	~Bush();
};
#endif // !__OBJECT__