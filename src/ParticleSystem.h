#pragma once
#include "SDL.h"
#include "Macros.h"
#include "Camera.h"
class Particle {
public:
	//initalize the particle positions, velocity and acceleration
	void Init(float lifeTime, int posx, int posy, double velX, double velY, double xInc,double yInc);
	void Update(float tpf);//update particle
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
	ParticleSystem(int _ParticleAmount,float _spawnRate,float _lifeTime); //initalize particle system
	~ParticleSystem();
	void Reset(); //reset particles
	void Update(float tpf, int x, int y,int vX, int vY, bool CreateNewParticles); //update particles
	void Render(SDL_Renderer* renderer, SDL_Colour color); // render particles
private:
	int RandomRange(int min, int max); //used to randomise particle velocity and direction
	Particle* Particles; //pointer to array of particle objects
	int ParticleAmount; //amount of particles allocated
	float spawnRate; //rate in mili seconds that the particles spawn
	float TimeTillSpawn=0; //remaining time to spawn next particle
	float lifeTime; //life time for each particle before being recycled
};

