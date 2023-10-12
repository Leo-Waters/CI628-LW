#include "Player.h"
#include "Macros.h"
Player::Player(int _ID, int _X, int _Y) : ID(_ID), Position(new SDL_Rect{ _X,_Y,40,40 })
{
	DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	PlayerTexture = TextureManager::GetTexture("Player.png");
}

Player::~Player()
{
	delete Position;
}

void Player::PossessPlayer()
{
}

void Player::NetworkUpdate(std::vector<std::string>& args)
{
	DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
	Position->x = stoi(args.at(1));
	Position->y = stoi(args.at(2));
	if (args.at(3) == "IDLE") {
		HasOwner = false;
	}
	else
	{
		HasOwner = true;
	}
}

void Player::Render(SDL_Renderer* renderer)
{

	SDL_RenderCopy(renderer, PlayerTexture, NULL, Position);
}
