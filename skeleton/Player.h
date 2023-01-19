#pragma once
#include "Particle.h"

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"


class AuxiliarPlayer: public Particle {
protected:
	float velFactor = 5.0f;
	
public:
	
	AuxiliarPlayer();
	~AuxiliarPlayer();

	void update(double t);


	void move(char c);
	void moveCamera();
	//posicion que guarda el jugador cuando va a unirse al slinky
	Vector3 savedPos;
	bool isOnSlinky;

	
};
