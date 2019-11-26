#include "Camera.h"
#include "Game.h"
/*======================= Milestone 1 ============================*//*
 > The player does not move but rather the geometry does. Nothing
 else will be changed in the future. The camera will always 
 work the same way.
																	*/
void Camera::SetPosition(Vector2 p_gNewPos)
{
	Set(p_gNewPos);
}
Vector2 Camera::GetPosition()
{
	return m_gCameraPos;
}
void Camera::MoveX(double x)
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Game::Instance()->m_pTiles[i][j]->SetX(x);
		}
	}
	for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
		Game::Instance()->m_vObject[i]->SetX(x);
	}
	for (int i = 0; i < Game::Instance()->m_vBullet.size(); i++) {
		Game::Instance()->m_vBullet[i]->SetX(x);
	}
	m_gCameraPos.x -= x;
	Game::Instance()->displaceX -= x;
}

void Camera::MoveY(double y)
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Game::Instance()->m_pTiles[i][j]->SetY(y);
		}
	}
	for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
		Game::Instance()->m_vObject[i]->SetY(y);
	}
	for (int i = 0; i < Game::Instance()->m_vBullet.size(); i++) {
		Game::Instance()->m_vBullet[i]->SetY(y);
	}
	Game::Instance()->displaceY -= y;
	m_gCameraPos.y -= y;
}

void Camera::Move(Vector2 p_gNewPos)
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Game::Instance()->m_pTiles[i][j]->SetX(p_gNewPos.x);
			Game::Instance()->m_pTiles[i][j]->SetY(p_gNewPos.y);
		}
	}
	for (int i = 0; i < Game::Instance()->m_vObject.size(); i++) {
		Game::Instance()->m_vObject[i]->SetX(p_gNewPos.x);
		Game::Instance()->m_vObject[i]->SetY(p_gNewPos.y);
	}
	for (int i = 0; i < Game::Instance()->m_vBullet.size(); i++) {
		Game::Instance()->m_vBullet[i]->SetX(p_gNewPos.x);
		Game::Instance()->m_vBullet[i]->SetY(p_gNewPos.y);
	}
	m_gCameraPos.x -= p_gNewPos.x;
	m_gCameraPos.y -= p_gNewPos.y;
	Game::Instance()->displaceX -= p_gNewPos.x;
	Game::Instance()->displaceY -= p_gNewPos.y;
}

void Camera::Set(Vector2 p_gNewPos)
{
	m_gCameraPos = p_gNewPos;
}

void Camera::SetX(double x)
{
	MoveX(m_gCameraPos.x - x);
	m_gCameraPos.x = x;
}

void Camera::SetY(double y)
{
	MoveY(m_gCameraPos.y - y);
	m_gCameraPos.y = y;
}
