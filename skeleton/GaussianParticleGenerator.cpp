#include "particleGenerator.hpp"
#include "GaussianParticleGenerator.hpp"


GaussianParticleGenerator::GaussianParticleGenerator(std::string name,ParticleType pT, int numPart, Vector3 vel, Vector3 pos,
	Vector3 dev_vel, Vector3 dev_pos, double time) /*: ParticleGenerator(name, numPart, vel, pos)*/
{
	turnOn();
	_model = new Particle();

	if (pT == FIREBALL)
		_model->establishParticle(pos,vel);

	_num_particles = numPart;
	_mean_vel = vel;
	_mean_pos = pos;
	std_dev_vel = dev_vel;
	std_dev_pos = dev_pos;
	std_dev_t = time;
	//pos distr
	_x_pos_Distr = std::normal_distribution<>(_mean_pos.x, std_dev_pos.x);
	_y_pos_Distr = std::normal_distribution<>(_mean_pos.y, std_dev_pos.y);
	_z_pos_Distr = std::normal_distribution<>(_mean_pos.z, std_dev_pos.z);
	//vel distr
	_x_vel_Distr = std::normal_distribution<>(_mean_vel.x, std_dev_vel.x);
	_y_vel_Distr = std::normal_distribution<>(_mean_vel.y, std_dev_vel.y);
	_z_vel_Distr = std::normal_distribution<>(_mean_vel.z, std_dev_vel.z);
	//time distr
	_time_Distr = std::uniform_real_distribution<>(.0, std_dev_t);
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	std::list<Particle*> l = std::list<Particle*>();
	if (_active) {
		for (int i = 0; i < _num_particles; i++)
		{
			/*if(_normal_distr(gen))*/
			auto* p = _model->clone();
			p->setPosition(_model->getPosition()+ Vector3((float)_x_pos_Distr(gen), (float)_y_pos_Distr(gen), (float)_z_pos_Distr(gen)));
			p->setNewVelocity(_model->getVelocity() + Vector3((float)_x_vel_Distr(gen),(float)_y_vel_Distr(gen),(float)_z_vel_Distr(gen)));
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			p->setColor2(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
			p->setLifeTime(_time_Distr(gen));
			l.push_back(p);
		}
	}


	return l;
}
