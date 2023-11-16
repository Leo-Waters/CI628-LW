#pragma once
#include "SDL.h"
#include "Macros.h"
#include "Camera.h"
class Particle {
public:
	void Init(float lifeTime, int posx, int posy, double velX, double velY, double xInc,double yInc);
	void Update(float tpf);
	float lifetime=0;//0 = dead

	//position
	int x, y;
	//velocity
	double xV,yV;
	//accelleration
	double xA, yA;
	
};

class ParticleSystem
{
public:
	ParticleSystem(int _ParticleAmount,float _spawnRate,float _lifeTime);
	~ParticleSystem();
	void Reset();
	void Update(float tpf, int x, int y,int vX, int vY, bool CreateNewParticles);
	void Render(SDL_Renderer* renderer, SDL_Colour color);
private:
	int RandomRange(int min, int max);
	Particle* Particles;
	int ParticleAmount;
	float spawnRate;
	float TimeTillSpawn=0;
	float lifeTime;
};

