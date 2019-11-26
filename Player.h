#pragma once
#ifndef __PLAYER__
#define __PLAYER__
#include "SDL_image.h"
#include "GameObject.h"
#include "Camera.h"
#include "LabelText.h"
#include <ctime>
#include <sstream>
#include <vector>
#include <iomanip>
class Player :
	public GameObject
{
private:
	std::vector <LabelText*> m_vTexts;
	int counter = 400;
	time_t timeSinceStart, start;
	double m_dSpeed = 0.0f;
public:

	void Update();
	void Render();
	Player();
	~Player();
};
#endif // !__PLAYER__