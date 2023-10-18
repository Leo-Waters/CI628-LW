#include "Enemy.h"
#include "Macros.h"


Enemy::Enemy(int _ID, int _X, int _Y) : ID(_ID),X(_X),Y(_Y), RenderPosition(new SDL_Rect{_X,_Y,30,30})
{
	DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	PlayerTexture = TextureManager::GetTexture("Enemy.png");
}

Enemy::~Enemy()
{
	delete RenderPosition;
}



void Enemy::NetworkUpdate(std::vector<std::string>& args)
{
	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
	X = stoi(args.at(1));
	Y = stoi(args.at(2));
}

void Enemy::Render(SDL_Renderer* renderer)
{
	RenderPosition->x = X - Camera::x;
	RenderPosition->y = Y - Camera::y;
	SDL_RenderCopy(renderer, PlayerTexture, NULL, RenderPosition);
}

int Enemy::GetPosX()
{
	return int(X);
}

int Enemy::GetPosY()
{
	return  int(Y);
}
