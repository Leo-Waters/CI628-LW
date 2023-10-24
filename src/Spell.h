#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Macros.h"
#include "Camera.h"

class Spell
{
public:
	SDL_Texture* SpellTextureFire;
	SDL_Texture* SpellTextureIce;
	bool Type = true;
	Spell(int _X, int _Y);
	~Spell();
	
	void NetworkUpdate(std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

private:
	int X, Y;
	SDL_Rect* RenderPosition;
};

