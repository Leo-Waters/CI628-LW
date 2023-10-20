#pragma once
#include <functional>
#include "TextureManager.h"
#include "Constants.h"
#include "SDL.h"
/// <summary>
/// button class
/// leo waters 17 03 2022
/// displays a graphic on screen, shows a hover graphic when mouse is over it and calls a function when the user clicks on the button
/// </summary>
class Button
{
private:
	int x, y, w, h;
	Anchor anchor;
	SDL_Texture* Graphic;
	SDL_Texture* Hover_Graphic;
	bool Hover = false;
	SDL_Rect GetRect();
public:
	std::function<void(std::string)> onClick;
	std::string ButtonName;
	Button(Anchor _anchor,int _x,int _y,int _w,int _h,const char* button_graphic, const char* hover_graphic);


	void CheckHover(int mouse_x, int mouse_y);
	void CheckClick(int mouse_x, int mouse_y);

	void Draw(SDL_Renderer* renderer);
};

