
#include "particle.hpp"


Particle::Particle(ParticleType Pt, Vector3 p, Vector3 v, Vector3 acc, float m, float d)
{
	_type = Pt;
	setPosition(p);
	setVelocity(v);
	setAcceleration(acc);
	setMass(m);
	inverse_mass = 1 / mass;
	setDamping(d);
	dir = GetCamera()->getDir();
	if (_type == FIREBALL)
		establishParticle(GetCamera()->getEye(), { 0,10.0f,0 });
	//setPosition({ GetCamera()->getEye()});
	setAlive(true);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(mass)), &pose, { 1, 0.5, 0, 1 });
	//RegisterRenderItem(renderItem);
	clearForce();
}
Particle::Particle(ParticleType Pt, Vector3 p, Vector3 v, Vector3 acc, float m, float d, double t, Vector4 col) {
	_type = Pt;
	setPosition(p);
	vel = v;
	a = acc;
	mass = m;
	inverse_mass = 1 / mass;
	damping = d;
	_remaining_time = t;
	color = col;
	_alive = true;
	if (_type == BOX)
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(1.0f,1.0f,1.0f)), &pose, color);
	else if(_type==PLANE)
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(50.0f, .5f, 50.0f)), &pose, color);
	else
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(2.0)), &pose, color);
	//RegisterRenderItem(renderItem);
	clearForce();
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	if (inverse_mass <= 0.0f) return;
	pose.p += vel * t;
	pos = pose.p;
	//vel += (a*mass) * t;

	Vector3 totalAcceleration = a;
	totalAcceleration += force * inverse_mass;
	// Update linear velocity
	vel += totalAcceleration * t;
	vel *= powf(damping, t);



	clearForce();

	if (_remaining_time > 0) _remaining_time -= t;
	else _alive = false;


}
void Particle::establishParticle(Vector3 P, Vector3 V) {
	switch (_type)
	{
	case FIREBALL:
		pose = physx::PxTransform(P);
		setMass(10.0f);
		setVelocity(V);
		setAcceleration({ 0.0f,-0.6f,0.0f });
		setDamping(0.9f);
		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, { 0,0.5,0.5,0.3 });
		//RegisterRenderItem(renderItem);
		break;
	default:
		break;

	}
}
Particle* Particle::clone() const
{
	auto cloneP = new Particle(_type, pos, vel, a, mass, damping, _remaining_time, color);
	return cloneP;

}

void Particle::redimension(physx::PxTransform* newPos, double newMass, Vector4 newCol)
{
	DeregisterRenderItem(renderItem);
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(newMass)), newPos, newCol);
}
