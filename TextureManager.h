#pragma once
#ifndef __TEXTUREMANAGER__
#define __TEXTUREMANAGER__
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <unordered_map>
class TextureManager
{
public:
	SDL_Rect blendRect[2] = { { 512 - 45, 0, 90, 30 }, { 768 - 45, 0, 90, 30 } };
	SDL_Rect textDest[3] = { { 0, 0, 0, 0 }, {0, 0, 0, 0} };
	SDL_Surface* textSurf[3];
	SDL_Texture* textTexture[3];
	TTF_Font* calibriRegular;
	TTF_Font* gemunuLibreBold;

	SDL_Texture* tileTexture[5];
	SDL_Texture* laserTexture;
	SDL_Texture* tankTexture;
	SDL_Texture* spikeTexture;
	SDL_Texture* rockTexture;
	SDL_Texture* natureTexture;
	SDL_Texture* enemyTurret;
	SDL_Texture* mineTexture;
	SDL_Texture* buttonTexture;
	SDL_Texture* explosionSprite;
	SDL_Texture* instructionTexture;

	static TextureManager* Instance()
	{
		static TextureManager* instance = new TextureManager;
		return instance;
	}
	SDL_Texture* Load(const char*);

	void Draw(SDL_Texture*, SDL_Rect, SDL_Rect, double, SDL_RendererFlip);
	void Draw(SDL_Texture*, SDL_Rect, SDL_Rect, double);
	void Draw(SDL_Texture*, SDL_Rect, SDL_Rect);
	void Draw(SDL_Texture*, SDL_Rect);
	TextureManager();
	~TextureManager();
};
typedef TextureManager TheTextureManager;
#endif // !__TEXTUREMANAGER__