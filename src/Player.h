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
	
	void NetworkUpdate(std::string cmd,std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

	string GetState();

	int GetPosX();
	int GetPosY();
	float GetAngle();
	int ID=-1;
	float Health=0;
	int Kills=0;
	bool HasOwner = false;
	bool IsLocalPlayer = false;
	bool IsDead();
private:
	int X, Y;
	float angle;
	SDL_Rect* RenderPosition;
};

