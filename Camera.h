#pragma once
#ifndef __CAMERA__
#define __CAMERA__
#include "Vector2.h"
class Camera
{
private:
	 static Vector2 m_gCameraPos;
public:
	static void SetPosition(Vector2);
	static Vector2 GetPosition();

	static void MoveX(double);
	static void MoveY(double);

	static void Move(Vector2);
	static void Set(Vector2);

	static void SetX(double);
	static void SetY(double);
};
#endif // !__CAMERA__