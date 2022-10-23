#include "particleGenerator.hpp"

ParticleGenerator::ParticleGenerator(std::string name, int numPart, Vector3 vel, Vector3 pos)
{
	_name = name;
	_num_particles = numPart;
	_mean_vel = vel;
	_mean_pos = pos;
}
