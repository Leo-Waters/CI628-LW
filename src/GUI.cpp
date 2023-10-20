#include "GUI.h"


GUI::GUI(SDL_Renderer* _renderer)
{
	Background = nullptr;
	renderer = _renderer;
	Buttons = new std::vector<Button>();
	Text = new std::vector<UI_Text>();
	Imgs = new std::vector<Image>();;
	Sliders = new std::vector<UI_Slider>();;
}

GUI::~GUI()
{
	delete Buttons;
	delete Text;
	delete Imgs;
	delete Sliders;
}

void GUI::UpdateInputs(SDL_Event* events)
{
	for (Button& button : *Buttons) {
		button.CheckHover(events->button.x, events->button.y);
	}
	if (events->type == SDL_MOUSEBUTTONDOWN)
	{
		for (Button& button : *Buttons) {
			button.CheckClick(events->button.x, events->button.y);
		}
	}

}

void GUI::RenderUI()
{
	if (Background != nullptr) {
		SDL_Rect Location;
		Location.x = 0;
		Location.y = 0;
		Location.w =WindowWidth;
		Location.h = WindowHeight;
		SDL_RenderCopy(renderer, Background, NULL, &Location);
	}

	for (Image& Img : *Imgs) {
		Img.Draw(renderer);
	}

	for (Button& button : *Buttons) {
		button.Draw(renderer);
	}
	for (UI_Text& txt : *Text) {
		txt.Draw(renderer);
	}
	for (UI_Slider& txt : *Sliders) {
		txt.Draw(renderer);
	}

}
