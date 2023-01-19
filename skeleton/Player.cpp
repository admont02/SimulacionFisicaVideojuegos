#include "Player.h"

AuxiliarPlayer::AuxiliarPlayer():Particle()
{
	vel = { 0,0,0 };
	mass = 5.0;
	damping = 0.5;
	a = { 0,0,0 };
	inverse_mass = 1 / mass;
	pose = physx::PxTransform({ 0,2,0 });
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, { 0.5, 0.8, 0.7, 1.0 });
	setAlive(true);
	auto cameraPos = pose.p - Vector3({ 0, -15, -10 });
	isOnSlinky = false;
//	GetCamera()->mDir = pose.p - cameraPos;
	clearForce();

}

AuxiliarPlayer::~AuxiliarPlayer()
{
	DeregisterRenderItem(renderItem);
}

void AuxiliarPlayer::update(double t)
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

	//moveCamera();

	clearForce();

}

void AuxiliarPlayer::move(char c)
{
	switch (c)
	{
		
	case 'W': setVelocity({ GetCamera()->getDir().x*30,0, GetCamera()->getDir().z * 30 });
		break;

	default:
		break;
	}
}

void AuxiliarPlayer::moveCamera()
{
	auto auxDir = GetCamera()->getDir();
	auxDir.y = 0;
	auxDir.normalize();
	auto cameraPos = pose.p /*- Vector3({ 0, -35, -15 })*/;
	cameraPos.y +=30;
	//cameraPos
	//GetCamera()->mDir = pose.p - cameraPos;
	cameraPos -= auxDir*20;
	GetCamera()->mEye = cameraPos ;


	/*Vector3 dir = mDir; dir.y = 0; dir.normalize();
	PxVec3 relativePos = golfBall->getGlobalPose().p;
	relativePos.y += foco - foco / 3;
	relativePos -= dir * foco;
	mEye = relativePos;*/
}


