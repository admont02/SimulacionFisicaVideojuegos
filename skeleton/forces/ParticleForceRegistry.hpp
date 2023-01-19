#pragma once
#include "ForceGenerator.hpp"
#include <map>
#include "../Particle.h"

typedef std::pair<ForceGenerator*, Particle*> FRPair;


class ParticleForceRegistry: public std::multimap<ForceGenerator*,Particle*>{
public:
	void updateForce(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	void addRegistry(ForceGenerator* fg, Particle* p) {
		this->insert(FRPair(fg, p));
	}

	void deleteParticleRegistry(Particle* p) {
		auto it = begin();
		while (it != end()) {
			if (it->second != p) it++;
			else it = this->erase(it);
		}
	}
};
typedef std::pair<ForceGenerator*, physx::PxRigidDynamic*> SRPair;


class SolidForceRegistry : public std::multimap<ForceGenerator*, physx::PxRigidDynamic*> {
public:
	void updateForce(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	void addRegistry(ForceGenerator* fg, physx::PxRigidDynamic* p) {
		this->insert(SRPair(fg, p));
	}
	void deleteParticleRegistry(physx::PxRigidDynamic* p) {
		auto it = begin();
		while (it != end()) {
			if (it->second != p) it++;
			else it = this->erase(it);
		}
	}
};
