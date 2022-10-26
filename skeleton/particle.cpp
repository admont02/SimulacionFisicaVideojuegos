
#include "particle.hpp"


Particle::Particle(ParticleType Pt, Vector3 p, Vector3 v, Vector3 acc, float m, float d)
{
	_type = Pt;
	setPosition(p);
	setVelocity(v);
	setAcceleration(acc);
	setMass(m);
	setDamping(d);
	dir = GetCamera()->getDir();
	if(_type==FIREBALL)
		establishParticle(GetCamera()->getEye(), {0,10.0f,0});
	//setPosition({ GetCamera()->getEye()});
	setAlive(true);
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
	pos = pose.p;
	vel += (a*mass) * t;
	vel *= powf(damping, t);

	if (_remaining_time > 0) _remaining_time -= t;
	else _alive = false;
}
void Particle::establishParticle(Vector3 P,Vector3 V) {
	switch (_type)
	{
	case FIREBALL:
		pose = physx::PxTransform(P);
		setMass(10.0f);
		setVelocity(V);
		setAcceleration({ 0.0f,-0.6f,0.0f });
		setDamping(0.9f);
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, { 0,0.5,0.5,0.3 });
		RegisterRenderItem(renderItem);
		break;
	default:
		break;
		
	}
}
Particle* Particle::clone() const
{
	auto cloneP = new Particle(_type,pos,vel,a,mass,damping);
	cloneP->establishParticle(pos,vel);
	return cloneP;

}