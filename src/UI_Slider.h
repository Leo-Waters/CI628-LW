#pragma once
#include "TextureManager.h"
#include "Constants.h"
#include "SDL.h"
/// <summary>
/// Slider class
/// leo waters 20 11 2022
/// displays a scalabled Image graphic on screen to represent data such as health
/// </summary>
class UI_Slider
{
private:
	int x, y, w, h;
	Anchor anchor;
	SDL_Texture* Graphic;
	SDL_Rect GetRect();
public:
	//slider value from 0-100
	int Value;
	UI_Slider(Anchor _anchor, int _x, int _y, int _w, int _h, const char* graphic);

	void Draw(SDL_Renderer* renderer);
};
