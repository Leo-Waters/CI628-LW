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



void Player::NetworkUpdate(std::string cmd,std::vector<std::string>& args)
{
	if (cmd == "PLAYER_DATA") {
		//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
		X = stoi(args.at(1));
		Y = stoi(args.at(2));
		HasOwner = args.at(3) != "IDLE";

		Health = stof(args.at(4));
		angle = stof(args.at(5));
		Kills = stoi(args.at(6));
	}
	else if(cmd == "PLAYER_STAT")
	{
		HasOwner = args.at(1) != "IDLE";

		Health = stof(args.at(2));
		
		Kills = stoi(args.at(3));
	}
	else if (cmd == "PLAYER_POS")
	{
		X = stoi(args.at(1));
		Y = stoi(args.at(2));
		angle = stof(args.at(3));
	}


}

void Player::Render(SDL_Renderer* renderer)
{
	if (!IsDead()) {
		RenderPosition->x = X - Camera::x;
		RenderPosition->y = Y - Camera::y;
		SDL_RenderCopyEx(renderer, PlayerTexture, NULL, RenderPosition, angle, NULL, SDL_FLIP_NONE);
	}
	
}

string Player::GetState()
{

	string state = "";

	if (HasOwner) {
		if (IsLocalPlayer) {
			state = "Your Player";
		}else{
			state = "Other Player";
		}
	}
	else
	{
		state = "Idle Player";
	}

	if (Health <= 0) {
		state += " | Is Dead";
	}
	else {
		state += " | Is Alive, Health:" + std::to_string((int)Health);
	}

	state += " | KILLS : " + std::to_string(Kills) + " | ";

	return state;
}

int Player::GetPosX()
{
	return int(X);
}

int Player::GetPosY()
{
	return  int(Y);
}

float Player::GetAngle()
{
	return angle;
}

bool Player::IsDead()
{
	return Health<=0;
}
