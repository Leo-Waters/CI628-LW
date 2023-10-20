#pragma once
#include "UI_Slider.h"
#include "Button.h"
#include "UI_Text.h"
#include "Image.h"
#include "Constants.h"
#include <vector>
#include <SDL.h>
#include "TextureManager.h"

/// <summary>
/// GUI
/// by leo waters for CI411 17/03/2022
/// renders a graphical user interface to the screen consisting of images, buttons and text that can be used by the user
/// 
/// CI516 - added UI Sliders
/// 
/// </summary>

class GUI
{

public:
	std::vector<Button>* Buttons;
	std::vector<UI_Text>* Text;
	std::vector<Image>* Imgs;
	std::vector <UI_Slider> * Sliders;
	SDL_Texture* Background;
	SDL_Renderer* renderer;
	GUI(SDL_Renderer* _renderer);
	~GUI();
	void UpdateInputs(SDL_Event* events);
	void RenderUI();
};

