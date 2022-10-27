#include "UniformParticleGenerator.hpp"
UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel)
{
	_model = new Particle();
	_pos = pos;
	_vel = vel;
	_num_particles = 5;
	_active = false;
	e= std::default_random_engine(rand());
}
std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> l;

	if (_model != nullptr) {
		for (int i = 0; i < _num_particles; i++)
		{
			auto p = _model->clone();
			p->setPosition(_model->getPosition() + Vector3(_pos.x * unf(e), _pos.y * unf(e), _pos.z * unf(e)));
			p->setVelocity(_model->getVelocity() + Vector3(_vel.x * unf(e), _vel.y * unf(e), _vel.z * unf(e)));
			l.push_back(p);
		}
	}
	return l;
}
