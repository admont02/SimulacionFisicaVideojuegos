#include "particleGenerator.hpp"
#include "GaussianParticleGenerator.hpp"

GaussianParticleGenerator::GaussianParticleGenerator(std::string name, int numPart, Vector3 vel, Vector3 pos) : ParticleGenerator(name,numPart,vel,pos)
{
	_model = new Particle();

}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{

	return std::list<Particle*>();
}
