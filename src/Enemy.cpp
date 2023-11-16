#include "Enemy.h"
#include "Macros.h"


Enemy::Enemy(int _ID, int _X, int _Y) : ID(_ID),X(_X),Y(_Y), LastX(_X), LastY(_Y),PredictedX(_X),PredictedY(_Y), RenderPosition(new SDL_Rect{_X,_Y,30,30})
{
	//DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	FireType = TextureManager::GetTexture("Enemy.png");
	IceType = TextureManager::GetTexture("Enemy2.png");
}

Enemy::~Enemy()
{
	delete RenderPosition;
}



void Enemy::NetworkUpdate(std::vector<std::string>& args)
{
	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
	LastX = X;
	LastY = Y;
	int newX = stoi(args.at(1));
	int newY = stoi(args.at(2));

	//to far position needs to  sync
	if (abs(LastX - newX) > 200) {
		LastX = newX;

	}
	if (abs(LastY - newY) > 200) {
		LastY = newY;
	}
	
	PredictedX = newX + (newX - LastX);
	PredictedY = newY + (newY - LastY);

	Health = stof(args.at(3));
	IsFireType = args.at(4) == "true";
	LastMessageTime = SDL_GetTicks();
}

void Enemy::Render(SDL_Renderer* renderer)
{
	double time = (SDL_GetTicks() - LastMessageTime) / 1000;

	
	X = lerpClamped(LastX, PredictedX, time);
	Y = lerpClamped(LastY, PredictedY, time);
	RenderPosition->x = X - Camera::x;
	RenderPosition->y = Y - Camera::y;
	if (IsFireType) {
		SDL_RenderCopy(renderer, FireType, NULL, RenderPosition);
	}
	else
	{
		SDL_RenderCopy(renderer, IceType, NULL, RenderPosition);
	}
	
}

float Enemy::lerpClamped(float a, float b, float f)
{
	float lerp = a * (1.0 - f) + (b * f);

	if (lerp < a) {
		lerp = a;
	}
	if (lerp > b) {
		lerp = b;
	}
	return lerp;
}

int Enemy::GetPosX()
{
	return int(X);
}

int Enemy::GetPosY()
{
	return  int(Y);
}
