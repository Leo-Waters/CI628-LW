#pragma once
#include <SDL.h>
#include<SDL_image.h>

/*
Modified code From year 1 CI411 and for year 2 CI516 now for CI628
Leo Waters 
Represents the players viewing position

modified to add focus on tile funcuality and improved zoom acuracy

*/
static class Camera
{

private:
	static int StartPanx, EndPanx,
			   StartPany, EndPany;
	static bool StartedPan;

	static void BoundryCorrection();
public:
	static int Max_x, Max_y;
	static int Min_x, Min_y;
	static int x, y;
	static float scale;
	static void Update(SDL_Event& events);
};