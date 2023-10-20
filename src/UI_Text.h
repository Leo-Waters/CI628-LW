#pragma once
/// <summary>
/// Text UI
/// by leo waters 17/03/2022
/// displays text on the screen 
/// 
/// Modified for CI516 to support multiline Text
/// 20/11/2022
/// </summary>
/// 
#include "SDL.h" 
#include "SDL_ttf.h"
#include "Constants.h"
#include <vector>
class UI_Text
{
private:
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* surface = NULL;
	
	Anchor anchor;
	int x , y;
	int texW = 0;
	int texH = 0;

	std::vector<SDL_Texture*> Textures;
	std::vector <SDL_Rect*> rects;

public:
	UI_Text();
	UI_Text(Anchor _anchor, int _x, int _y, int fontSize, const char* text, SDL_Color color, SDL_Renderer* rend);
	~UI_Text();
	TTF_Font* Font;
	void SetText(const char* text, SDL_Renderer* rend);
	void Draw(SDL_Renderer* rend);
};

