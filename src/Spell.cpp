#include "Spell.h"
#include "Macros.h"


Spell::Spell(int _X, int _Y) : X(_X),Y(_Y), RenderPosition(new SDL_Rect{_X,_Y,20,20})
{
	//DEBUG("Created Player with ID: " << _ID << " at pos x" << _X << " y" << _Y);
	SpellTextureIce = TextureManager::GetTexture("IceSpell.png");
	SpellTextureFire = TextureManager::GetTexture("FireSpell.png");
	particleSystem = new ParticleSystem(10000,0.001f,3);
}

Spell::~Spell()
{
	delete RenderPosition;
	delete particleSystem;
}


void Spell::Update(float tpf)
{
	if (Active) {
		X += vX* tpf;
		Y += vY* tpf;
		
	}
	particleSystem->Update(tpf, X+10, Y+10, vX, vY, Active);
}
void Spell::NetworkUpdate(string cmd, std::vector<std::string>& args)
{
	if (cmd == "SPELL_START") {
		Active = true;
		Type = args.at(1)=="true";
		AudioManager::PlaySoundEffect("fire.wav", 20);
		

		X = stof(args.at(2));
		Y = stof(args.at(3));
		vX = stof(args.at(4));
		vY = stof(args.at(5));

		//simulate netowrk delay in seconds--- could be improved via getting the network ping time
		double estimatedDelay = 0.4;
		X += vX * estimatedDelay;
		Y += vY * estimatedDelay;

		particleSystem->Reset();
	}
	else if(cmd=="SPELL_FINISH")
	{
		Active = false;
	}

	//DEBUG("Recived PLAYER"+ args.at(0)+"   X: " + args.at(1) + " Y: " + args.at(2) + " State: " + args.at(3));

}
void Spell::Render(SDL_Renderer* renderer)
{
	
	particleSystem->Render(renderer,Type?FirePraticleColour:IcePraticleColour);
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

