#include "Camera.h"
#include "Macros.h"
#include <iostream>
int Camera::x = 0;
int Camera::y = 0;

int Camera::StartPanx, Camera::EndPanx;
int Camera::StartPany, Camera::EndPany;

int Camera::Max_x, Camera::Max_y;
int Camera::Min_x, Camera::Min_y;

bool Camera::StartedPan = false;
void Camera::BoundryCorrection()
{
	if (x < Min_x) {
		x = Min_x;
	}
	if (y < Min_y) {
		y = Min_y;
	}

	if (x > Max_x) {
		x = Max_x;
	}
	if (y > Max_y) {
		y = Max_y;
	}

}
void Camera::Update(SDL_Event& events)
{
	//DEBUG("camera X"<< x<<" Y"<<y);
	if (events.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&StartPanx, &StartPany);
		StartedPan = true;
	}
	if (StartedPan) {
		SDL_GetMouseState(&EndPanx, &EndPany);
		x += StartPanx - EndPanx;
		y += StartPany - EndPany;
		SDL_GetMouseState(&StartPanx, &StartPany);
	}
	if (events.type == SDL_MOUSEBUTTONUP) {
			StartedPan = false;
	}



	BoundryCorrection();
}

