#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Macros.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "AudioManager.h"
class Spell
{
public:
	SDL_Texture* SpellTextureFire;
	SDL_Texture* SpellTextureIce;
	bool Type = true;
	bool Active = false;
	Spell(int _X, int _Y);
	~Spell();
	
	void Update(float tpf);
	void NetworkUpdate(string cmd, std::vector<std::string>& args);
	void Render(SDL_Renderer* renderer);

private:
	ParticleSystem* particleSystem;
	float X, Y;
	float vX, vY;
	SDL_Rect* RenderPosition;
	SDL_Colour IcePraticleColour{ 0, 0, 255, 255};
	SDL_Colour FirePraticleColour{ 255, 0, 0, 255 };
};

