#pragma once
#ifndef __HAZARD__
#define __HAZARD__
#include "GameObject.h"
class Spike :
	public GameObject
{
public:
	void Update();
	void Render();
	Spike(int, int);
	~Spike();
};
class Mine :
	public GameObject
{
public:
	void Update();
	void Render();
	Mine(int, int);
	~Mine();
};
#endif // !__HAZARD__