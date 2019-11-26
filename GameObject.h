#pragma once
#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__
#include "SDL.h"
#include "Vector2.h"
#include "Sprite.h"

enum ObjectType { ROCK, BUSH, SPIKE, MINE, ENEMY };
enum TargetActor { NONE, PLAYER };

class GameObject : public Sprite
{
private:
	Vector2 position;	// Actual Position in world space
protected:
	ObjectType m_type;
	TargetActor m_TargetActor;
public:
	TargetActor GetTargetActor();
	void SetTargetActor(TargetActor);

	GameObject();
	~GameObject();

	float GetAbsolutePositionX();
	float GetAbsolutePositionY();
	bool m_bisAlive;

	SDL_Rect m_rHealthBarDst;
	SDL_Rect m_rHealthBackBarDst;
	double m_dHealth, m_dAngle = 0;

	void SetPosition(Vector2);
	Vector2 GetPosition();

	ObjectType GetType();
	void SetType(ObjectType);


	void SetX(double x);
	void SetY(double y);

	virtual void Update() = 0;
	virtual void Render() = 0;
};
#endif // __GAMEOBJECT__