#include "Player.h"
#include "Macros.h"


Player::Player(int _ID, int _X, int _Y) : ID(_ID),X(_X),Y(_Y), LastX(_X), LastY(_Y), PredictedX(_X), PredictedY(_Y), RenderPosition(new SDL_Rect{_X,_Y,40,40})
{
	//DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	PlayerTexture = TextureManager::GetTexture("Player.png");
}

Player::~Player()
{
	delete RenderPosition;
}



void Player::PositionPrediction(int newX, int newY)
{
	LastX = X;
	LastY = Y;

	//to far position needs to  sync
	if (abs(LastX - newX) > 100) {
		LastX = newX;

	}
	if (abs(LastY - newY) > 100) {
		LastY = newY;
	}

	PredictedX = newX + (newX - LastX);
	PredictedY = newY + (newY - LastY);

	LastMessageTime = SDL_GetTicks();
}

void Player::NetworkUpdate(std::string cmd,std::vector<std::string>& args)
{
	if (cmd == "PLAYER_DATA") {
		//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
		PositionPrediction(stoi(args.at(1)), stoi(args.at(2)));

		LastMessageTime = SDL_GetTicks();
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
		PositionPrediction(stoi(args.at(1)), stoi(args.at(2)));

		angle = stof(args.at(3));
	}


}
float Player::lerpClamped(float a, float b, float f)
{
	if (f > 1) {
		f = 1;
	}
	if (f < 0) {
		f = 0;
	}

	float lerp = a * (1.0 - f) + (b * f);
	return lerp;
}

void Player::Render(SDL_Renderer* renderer)
{
	if (!IsDead()) {

		double time = (SDL_GetTicks() - LastMessageTime) / 1000;

		X = lerpClamped(LastX, PredictedX, time);
		Y = lerpClamped(LastY, PredictedY, time);
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
