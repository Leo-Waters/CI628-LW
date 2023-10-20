#include "Image.h"
SDL_Rect Image::GetRect()
{
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


	SDL_Rect Location;
	Location.x = xStart + x - w / 2;
	Location.y = yStart + y - h / 2;
	Location.w = w;
	Location.h = h;
	return Location;
}

Image::Image(Anchor _anchor, int _x, int _y, int _w, int _h, const char* button_graphic)
{
	anchor = _anchor; 
	//scale to screen resoluion
	x = ((float)_x / 1920) * WindowWidth;
	y = ((float)_y / 1080) * WindowHeight;
	w = ((float)_w / 1920) * WindowWidth;
	h = ((float)_h / 1080) * WindowHeight;
	Graphic = TextureManager::GetTexture(button_graphic);
}

void Image::Draw(SDL_Renderer* renderer)
{
	SDL_Rect Location = GetRect();

	SDL_RenderCopy(renderer, Graphic, NULL, &Location);
}
