#include "ForceGenerator.hpp"

WindForceGenerator::WindForceGenerator(const float k1, const float k2, Vector3 _vel): ParticleDragGenerator(k1,k2)
{
	vel = _vel;
}

void WindForceGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(particle->getInvMass()) < 1e-10) return;

	Vector3 v = vel-particle->getVelocity();
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;
	//Apply drag force
	/*std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;*/
	particle->addForce(dragF);
}
