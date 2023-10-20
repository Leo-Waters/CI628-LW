#include "UI_Text.h"
#include <string>
#include <math.h>
UI_Text::UI_Text() {

}
UI_Text::UI_Text(Anchor _anchor, int _x, int _y,int fontSize, const char* text, SDL_Color _color, SDL_Renderer* rend)
{
	//scale with window
	fontSize = (int)(((float)fontSize / 1920)* WindowWidth);
	x = ((float)_x / 1920) * WindowWidth;
	y = ((float)_y / 1080) * WindowHeight;

	color = _color;
	Font =  TTF_OpenFont("Assets/Fonts/arial.ttf", fontSize);
	anchor = _anchor;


	SetText(text,rend);
}

UI_Text::~UI_Text()
{
	//TTF_CloseFont(Font);
}

void UI_Text::SetText(const char* text, SDL_Renderer* rend)
{
	//get all lines from text input and put into seperate strings
	std::vector<std::string> Lines= std::vector<std::string>();
	Lines.push_back(std::string());
	int line = 0;
	for (int i = 0; text[i]; i++) {
		if (text[i] == '\n') {
			Lines.push_back(std::string());
			line++;
		}
		else {
			Lines.at(line) += text[i];
		}
	}

	int ln = 0;
	for (SDL_Texture* texture : Textures)// clearing old textures and rects
	{
		SDL_DestroyTexture(texture);
		delete rects.at(ln);
		ln++;
	}

	Textures.clear();
	rects.clear();

	//create textures for each line of inputted text
	for (int i = 0; i < line+1; i++)
	{
		surface = TTF_RenderText_Solid(Font, Lines.at(i).c_str(), color);
		Textures.push_back(SDL_CreateTextureFromSurface(rend, surface));
		SDL_FreeSurface(surface);
	}

	// create rects for drawing each line texture based on positions size and scale and offset based on previouse line
	ln = 0;
	int yLineOffset = 0;
	for (SDL_Texture* texture : Textures)// clearing old textures
	{

		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

		int yStart = 0;
		if (anchor == Anchor::bottomCenter || anchor == Anchor::bottomLeft || anchor == Anchor::bottomRight) {
			yStart = WindowHeight;
		}
		if (anchor == Anchor::midCenter || anchor == Anchor::midLeft || anchor == Anchor::midRight) {
			yStart = WindowHeightOffset;
		}
		int xStart = 0;
		if (anchor == Anchor::bottomCenter || anchor == Anchor::midCenter || anchor == Anchor::topCenter) {
			xStart = WindowWidthOffset;
		}
		if (anchor == Anchor::midRight || anchor == Anchor::bottomRight || anchor == Anchor::topRight) {
			xStart = WindowWidth;
		}

		rects.push_back(new SDL_Rect());
		rects.at(ln)->x = xStart + x - texW / 2;
		rects.at(ln)->y = (yLineOffset+yStart) + y - texH / 2;
		rects.at(ln)->w = texW;
		rects.at(ln)->h = texH;
		yLineOffset += texH;//offset hight from last line
		ln++;
	}

}

void UI_Text::Draw(SDL_Renderer* rend)
{
	//draw lines
	int ln = 0;
	for (SDL_Texture* texture : Textures)
	{
		SDL_RenderCopy(rend, texture, NULL, rects.at(ln));
		ln++;
	}
}
