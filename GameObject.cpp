#include "GameObject.h"
#include "Game.h"


TargetActor GameObject::GetTargetActor()
{
	return m_TargetActor;
}

void GameObject::SetTargetActor(TargetActor tActor)
{
	this->m_TargetActor = tActor;
}

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

float GameObject::GetAbsolutePositionX()
{
	return position.x + m_rDst.w / 2;
}

float GameObject::GetAbsolutePositionY()
{
	return position.y + m_rDst.h / 2;
}

void GameObject::SetPosition(Vector2 p_gPosition)
{
	this->position = p_gPosition;
	this->m_rDst.x = p_gPosition.x;
	this->m_rDst.y = p_gPosition.y;
}

Vector2 GameObject::GetPosition()
{
	return position;
}

ObjectType GameObject::GetType()
{
	return m_type;
}

void GameObject::SetType(ObjectType type)
{
	this->m_type = type;
}

void GameObject::SetX(double x)
{
	position.x += x;
	m_rDst.x = position.x;
	m_rHealthBackBarDst.x = position.x;
	m_rHealthBarDst.x = position.x;
}

void GameObject::SetY(double y)
{
	position.y += y;
	m_rDst.y = position.y;
	m_rHealthBackBarDst.y = position.y;
	m_rHealthBarDst.y = position.y;
}
