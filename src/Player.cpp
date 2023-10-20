#include "Player.h"
#include "Macros.h"


Player::Player(int _ID, int _X, int _Y) : ID(_ID),X(_X),Y(_Y), RenderPosition(new SDL_Rect{_X,_Y,40,40})
{
	//DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	PlayerTexture = TextureManager::GetTexture("Player.png");
}

Player::~Player()
{
	delete RenderPosition;
}



void Player::NetworkUpdate(std::vector<std::string>& args)
{
	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
	X = stoi(args.at(1));
	Y = stoi(args.at(2));
	HasOwner = args.at(3) != "IDLE";
	
	Health = stof(args.at(4));
}

void Player::Render(SDL_Renderer* renderer)
{
	RenderPosition->x = X - Camera::x;
	RenderPosition->y = Y - Camera::y;
	SDL_RenderCopy(renderer, PlayerTexture, NULL, RenderPosition);
}

int Player::GetPosX()
{
	return int(X);
}

int Player::GetPosY()
{
	return  int(Y);
}
