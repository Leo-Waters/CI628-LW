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



void Spell::Update(float tpf)
{
	if (Active) {
		X += vX;
		Y += vY;
		
	}
}

void Spell::NetworkUpdate(string cmd, std::vector<std::string>& args)
{
	if (cmd == "SPELL_START") {
		Active = true;
		Type = args.at(1)=="true";
		X = stof(args.at(2));
		Y = stof(args.at(3));
		vX = stof(args.at(4));
		vY = stof(args.at(5));
	}
	else if(cmd=="SPELL_FINISH")
	{
		Active = false;
	}

	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));

}

void Spell::Render(SDL_Renderer* renderer)
{
	if (Active) {
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
	
}

