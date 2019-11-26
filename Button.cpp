#include "Button.h"
#include "Game.h"


ButtonState Button::GetState()
{
	return m_State;
}

void Button::SetButtonState(ButtonState p_State)
{
	this->m_State = p_State;
}

Button::Button(const char* p_cText, const char* p_cFile, int p_iSize, SDL_Color p_cColor, int p_iX, int p_iY)
{
	m_pLabelText = new LabelText(p_cText, p_cFile, p_iSize, p_cColor, p_iX, p_iY);
	m_rButtonDst.x = p_iX - (m_rButtonDst.w / 2);
	m_rButtonDst.y = p_iY - (m_rButtonDst.h / 2);
	m_pLabelText->m_rDst.x = m_rButtonDst.x + (m_rButtonDst.w / 2) - (m_pLabelText->m_pSurface->w / 2);
	m_pLabelText->m_rDst.y = m_rButtonDst.y + (m_rButtonDst.h / 2) - (m_pLabelText->m_pSurface->h / 2);
}


Button::~Button()
{
	delete m_pLabelText;
	m_pLabelText = nullptr;
}

void Button::Render()
{
	TheTextureManager::Instance()->Draw(TheTextureManager::Instance()->buttonTexture, m_rButtonSrc[m_State], m_rButtonDst);
	
	m_pLabelText->Render();
}

void Button::Update()
{
	if (SDL_PointInRect(&Game::Instance()->GetMousePosition(), &m_rButtonDst)) {
		SetButtonState(ButtonState::HIGHLIGHTED);
		if (Game::Instance()->m_bMousePressed) {
			SetButtonState(ButtonState::CLICKED);
		}
	}
	else {
		SetButtonState(ButtonState::NEUTRAL);
	}
}
