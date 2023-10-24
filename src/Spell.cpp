#include "Spell.h"
#include "Macros.h"


Spell::Spell(int _X, int _Y) : X(_X),Y(_Y), RenderPosition(new SDL_Rect{_X,_Y,20,20})
{
	//DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	SpellTextureIce = TextureManager::GetTexture("IceSpell.png");
	SpellTextureFire = TextureManager::GetTexture("FireSpell.png");
}

Spell::~Spell()
{
	delete RenderPosition;
}



void Spell::NetworkUpdate(std::vector<std::string>& args)
{
	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));
	Type = stoi(args.at(1));
	X = stoi(args.at(2));
	Y = stoi(args.at(3));
}

void Spell::Render(SDL_Renderer* renderer)
{
	RenderPosition->x = X - Camera::x;
	RenderPosition->y = Y - Camera::y;
	if (Type) {
		SDL_RenderCopy(renderer, SpellTextureFire, NULL, RenderPosition);
	}
	else
	{
		SDL_RenderCopy(renderer, SpellTextureIce, NULL, RenderPosition);
	}
	
}

