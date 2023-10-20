#include "UI_Slider.h"

SDL_Rect UI_Slider::GetRect()
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

	if (anchor == Anchor::topCenter) {
		Location.x = xStart + x - w / 2;
		Location.y = yStart + y - h / 2;
		Location.w = (w / 100) * Value;

		Location.x = (xStart + x) - (((w / 100) * Value)/2);
		Location.h = h;
	}
	else
	{
		Location.x = xStart + x - w / 2;
		Location.y = yStart + y - h / 2;
		Location.w = (w / 100) * Value;
		Location.h = h;
	}

	return Location;
}

UI_Slider::UI_Slider(Anchor _anchor, int _x, int _y, int _w, int _h, const char* graphic)
{
	Value = 100;
	anchor = _anchor;
	x = ((float)_x / 1920) * WindowWidth;
	y = ((float)_y / 1080) * WindowHeight;
	w = ((float)_w / 1920) * WindowWidth;
	h = ((float)_h / 1080) * WindowHeight;
	Graphic = TextureManager::GetTexture(graphic);
}

void UI_Slider::Draw(SDL_Renderer* renderer)
{
	SDL_Rect Location = GetRect();

	SDL_RenderCopy(renderer, Graphic, NULL, &Location);
}
