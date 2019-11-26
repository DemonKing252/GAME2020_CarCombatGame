#pragma once
#ifndef __TURRET__
#define __TURRET__
#include "GameObject.h"
#include <ctime>

class Turret :
	public GameObject
{
public:
	clock_t shootNext_T, start_T;
	double m_dDistanceFromTarget = 0;
	void Update();
	void Render();
	Turret(SDL_Rect, float, float);
	~Turret();
};

#endif // !__TURRET__