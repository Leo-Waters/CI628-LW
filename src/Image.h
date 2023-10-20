#pragma once
#include "TextureManager.h"
#include "Constants.h"
#include "SDL.h"
/// <summary>
/// Image class
/// leo waters 17 03 2022
/// displays a Image graphic on screen
/// </summary>
class Image
{
private:
	int x, y, w, h;
	Anchor anchor;
	SDL_Texture* Graphic;
	SDL_Rect GetRect();
public:

	Image(Anchor _anchor, int _x, int _y, int _w, int _h, const char* button_graphic);

	void Draw(SDL_Renderer* renderer);
};
