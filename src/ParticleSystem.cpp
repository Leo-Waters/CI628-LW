#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int _ParticleAmount, float _spawnRate, float _lifeTime)
{
	Particles = new Particle[_ParticleAmount];
	ParticleAmount= _ParticleAmount;
	spawnRate = _spawnRate;
	lifeTime = _lifeTime;
	TimeTillSpawn = 0;

}

ParticleSystem::~ParticleSystem()
{
	delete[]Particles;
}

void ParticleSystem::Update(float tpf, int x, int y,int vX,int vY,bool CreateNewParticles)
{
	if (CreateNewParticles) {
		TimeTillSpawn -= tpf;
	}
	
	for (size_t i = 0; i < ParticleAmount; i++)
	{

		//apply velocities to existin particles
		if (Particles[i].lifetime >= 0.0f) {
			Particles[i].Update(tpf);
		}
		//spawn new particles
		else if (CreateNewParticles && TimeTillSpawn < 0.0f) {
				TimeTillSpawn += spawnRate;
				Particles[i].Init(lifeTime, RandomRange(x - 5, x + 5), RandomRange(y - 5, y + 5),RandomRange(-10,10), RandomRange(-10, 10), RandomRange(1, 4), RandomRange(1, 4));
			}

	}
}

void ParticleSystem::Render(SDL_Renderer* renderer)
{

	for (size_t i = 0; i < ParticleAmount; i++)
	{
		if (Particles[i].lifetime > 0.0f) {
			
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			
			SDL_RenderDrawPoint(renderer, Particles[i].x - Camera::x, Particles[i].y - Camera::y);
		}
		
	}

}

int ParticleSystem::RandomRange(int min, int max)
{
	return (int)(rand() % (max - min)) + min;
}

void Particle::Init(float _lifeTime, int posx, int posy, double velX, double velY, double xInc, double yInc)
{
	lifetime = _lifeTime;
	x = posx;
	y = posy;
	xV = velX;
	yV = velY;
	xA = xInc;
	yA = yInc;
}

void Particle::Update(float tpf)
{
	xV += xA * tpf;
	yV += yA * tpf;
	x += xV*tpf;
	y += yV*tpf;
	lifetime -= tpf;
}
