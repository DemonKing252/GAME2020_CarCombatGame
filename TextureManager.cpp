#include "TextureManager.h"
#include "Game.h"


SDL_Texture* TextureManager::Load(const char *file)
{
	SDL_Surface* pSurf = IMG_Load(file);
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Game::Instance()->GetRenderer(), pSurf);
	
	assert(pTexture != 0);

	SDL_FreeSurface(pSurf);

	return pTexture;
}

void TextureManager::Draw(SDL_Texture* pTexture, SDL_Rect p_rSrc, SDL_Rect p_rDst, double p_dAngle, SDL_RendererFlip p_fFlip)
{
	SDL_RenderCopyEx(Game::Instance()->GetRenderer(), pTexture, &p_rSrc, &p_rDst, p_dAngle, NULL, p_fFlip);
}

void TextureManager::Draw(SDL_Texture* pTexture, SDL_Rect p_rSrc, SDL_Rect p_rDst, double p_dAngle)
{
	SDL_RenderCopyEx(Game::Instance()->GetRenderer(), pTexture, &p_rSrc, &p_rDst, p_dAngle, NULL, SDL_FLIP_NONE);
}

void TextureManager::Draw(SDL_Texture* pTexture, SDL_Rect p_rSrc, SDL_Rect p_rDst)
{
	SDL_RenderCopy(Game::Instance()->GetRenderer(), pTexture, &p_rSrc, &p_rDst);
}

void TextureManager::Draw(SDL_Texture* pTexture, SDL_Rect p_rDst)
{
	SDL_RenderCopy(Game::Instance()->GetRenderer(), pTexture, NULL, &p_rDst);
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}
