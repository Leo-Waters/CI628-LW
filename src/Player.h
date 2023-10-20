#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Macros.h"
#include "Camera.h"

class Player
{
public:
	SDL_Texture* PlayerTexture;
	Player(int _ID, int _X, int _Y);
	~Player();
	
	void NetworkUpdate(std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

	int GetPosX();
	int GetPosY();
	int ID;
	float Health;
	bool HasOwner = false;
	bool IsLocalPlayer = false;
private:
	int X, Y;
	SDL_Rect* RenderPosition;
};

