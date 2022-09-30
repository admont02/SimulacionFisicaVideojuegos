
#include "particle.hpp"


Particle::Particle(ParticleType Pt)
{
	type = Pt;
	dir = GetCamera()->getDir();
	establishParticle();
	setPosition({ GetCamera()->getEye()});
    
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(mass)), &pose, { 1, 0.5, 0, 1 });
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
	vel += (a*mass) * t;
	vel *= powf(damping, t);
}
void Particle::establishParticle() {
	switch (type)
	{
	case FIREBALL:
		setMass(1.0f);
		setVelocity(10.0f);
		setAcceleration({ 0.0f,-0.6f,0.0f });
		setDamping(0.9f);
		break;
	default:
		break;
		
	}
}