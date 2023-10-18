#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Macros.h"
#include "Camera.h"

class Enemy
{
public:
	SDL_Texture* PlayerTexture;
	Enemy(int _ID, int _X, int _Y);
	~Enemy();
	
	void NetworkUpdate(std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

	int GetPosX();
	int GetPosY();
	int ID;
private:
	int X, Y;
	SDL_Rect* RenderPosition;
};

