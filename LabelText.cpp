#include "LabelText.h"
#include "Game.h"


LabelText::LabelText(const char* text, const char* fileName, int size, SDL_Color color, int x, int y)
{
	this->color = color;
	this->m_cText = text;
	m_pFont = TTF_OpenFont(fileName, size);
	m_pSurface = TTF_RenderText_Blended(m_pFont, text, color);
	m_pTexture = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurface);
	m_rDst.w = m_pSurface->w;
	m_rDst.h = m_pSurface->h;
	/*m_rDst.x = x - (m_pSurface->w / 2);
	m_rDst.y = y - (m_pSurface->h / 2);*/
	m_rDst.x = x;
	m_rDst.y = y;

}


LabelText::~LabelText()
{
	TTF_CloseFont(m_pFont);
	SDL_FreeSurface(m_pSurface);
	SDL_DestroyTexture(m_pTexture);
}

void LabelText::Render()
{
	SDL_RenderCopy(Game::Instance()->GetRenderer(), m_pTexture, NULL, &m_rDst);
}

void LabelText::PlaceInCenter()
{
	m_rDst.x = m_rDst.x - (m_rDst.w / 2);
	m_rDst.y = m_rDst.y - (m_rDst.h / 2);
}

void LabelText::Update()
{
}

void LabelText::ChangeText(const char *p_cText)
{
	SDL_FreeSurface(m_pSurface);
	SDL_DestroyTexture(m_pTexture);
	this->m_cText = p_cText;
	m_pSurface = TTF_RenderText_Blended(m_pFont, m_cText, color);
	m_pTexture = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), m_pSurface);
	m_rDst.w = m_pSurface->w;
	m_rDst.h = m_pSurface->h;
}
