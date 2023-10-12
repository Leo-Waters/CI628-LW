#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Macros.h"
class Player
{
public:
	SDL_Texture* PlayerTexture;
	Player(int _ID, int _X, int _Y);
	~Player();
	void PossessPlayer();
	void NetworkUpdate(std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);
private:
	int ID;
	bool HasOwner = false;
	bool IsLocalPlayer = false;
	SDL_Rect* Position;
};

