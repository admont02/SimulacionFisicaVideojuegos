
#include "particle.hpp"


Particle::Particle(Vector3 Pos, Vector3 Vel)
{
	vel = Vel;
	pose = physx::PxTransform(Pos);
	
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1)), &pose, { 1, 1, 0.5, 1 });
	RegisterRenderItem(renderItem);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	//if (inverse_mass <= 0.0f) return;
	pose.p += vel * t;
	/*v += a * t;
	v *= powf(damping, t);*/
}
