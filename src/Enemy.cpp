#include "Enemy.h"
#include "Macros.h"


Enemy::Enemy(int _ID, int _X, int _Y) : ID(_ID),X(_X),Y(_Y), RenderPosition(new SDL_Rect{_X,_Y,30,30})
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
	X = stoi(args.at(1));
	Y = stoi(args.at(2));
	Health = stof(args.at(3));
	IsFireType = args.at(4) == "true";
}

void Enemy::Render(SDL_Renderer* renderer)
{
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

int Enemy::GetPosX()
{
	return int(X);
}

int Enemy::GetPosY()
{
	return  int(Y);
}
