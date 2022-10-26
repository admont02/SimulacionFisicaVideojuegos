#include "particle.hpp"
#include "particleGenerator.hpp"
#ifndef __GAUSSIAN_PARTICLE_GENERATOR_H__
#define __GAUSSIAN_PARTICLE_GENERATOR_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include <random>


class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 std_dev_vel;
	Vector3 std_dev_pos;
	double std_dev_t;
	//distribuciones velocidad
	std::normal_distribution<double>_x_pos_Distr;
	std::normal_distribution<double>_y_pos_Distr;
	std::normal_distribution<double>_z_pos_Distr;
	//distribucion posicion
	std::normal_distribution<double>_x_vel_Distr;
	std::normal_distribution<double>_y_vel_Distr;
	std::normal_distribution<double>_z_vel_Distr;
	//0-1
	std::normal_distribution<double> _normal_distr{ 0,1 };
	//rd
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	//time
	std::uniform_real_distribution<> _time_Distr;



public:
	GaussianParticleGenerator(std::string name, ParticleType pT, int numPart, Vector3 vel,
		Vector3 pos,Vector3 dev_vel,Vector3 dev_pos,double time);
	~GaussianParticleGenerator(){};

    std::list<Particle*>generateParticles() override;



};
#endif