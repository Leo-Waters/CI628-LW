#include "Button.h"
#include "AudioManager.h"

SDL_Rect Button::GetRect()
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

Button::Button(Anchor _anchor, int _x, int _y, int _w, int _h, const char* button_graphic, const char* hover_graphic)
{
	anchor = _anchor;
	//scale to screen resoluion
	x = ((float)_x / 1920) * WindowWidth;
	y = ((float)_y / 1080) * WindowHeight;
	w = ((float)_w / 1920) * WindowWidth;
	h = ((float)_h / 1080) * WindowHeight;
	Graphic = TextureManager::GetTexture(button_graphic);
	Hover_Graphic = TextureManager::GetTexture(hover_graphic);
}

void Button::Draw(SDL_Renderer* renderer)
{
	SDL_Rect Location = GetRect();

	SDL_RenderCopy(renderer, Graphic, NULL, &Location);
	if (Hover) {
		SDL_RenderCopy(renderer, Hover_Graphic, NULL, &Location);
	}
}


void Button::CheckHover(int mouse_x, int mouse_y) {

	SDL_Rect Location = GetRect();

	if (mouse_x > Location.x && mouse_x < Location.x + Location.w) {//within x bounding box

		if (mouse_y > Location.y && mouse_y < Location.y + Location.h) {//within x bounding box
			if (!Hover) {
				//not previously hovering
				AudioManager::PlaySoundEffect("ButtonHover.wav",MIX_MAX_VOLUME / 16);
			}
			Hover = true;
			return;
		}
	}
	Hover = false;
}
void Button::CheckClick(int mouse_x, int mouse_y) {
	SDL_Rect Location = GetRect();

	//std::cout << "\nMOUSE X " << mouse_x << " MOUSE Y " << mouse_y << " x " << Location.x << " y " << Location.y << " w " << Location.x+Location.w << " h " << Location.y+Location.h<<"\n";

	if (mouse_x > Location.x && mouse_x < Location.x + Location.w) {//within x bounding box
		 
		if (mouse_y > Location.y && mouse_y < Location.y + Location.h) {//within x bounding box

			if (onClick != nullptr) {
				onClick(ButtonName);
			}
			
		}
	}
}