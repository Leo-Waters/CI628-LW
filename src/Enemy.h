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
	SDL_Texture* FireType;
	SDL_Texture* IceType;
	Enemy(int _ID, int _X, int _Y);
	~Enemy();
	
	void NetworkUpdate(std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

	int GetPosX();
	int GetPosY();
	int ID;
	float Health;
private:
	bool IsFireType;
	int X, Y;

	double LastMessageTime;
	int LastX, LastY;
	int PredictedX, PredictedY;

	SDL_Rect* RenderPosition;
	float lerpClamped(float a, float b, float f);
};

