#pragma once

#include "Particle.h"

class Spring
{
public:
	Particle* p1 = nullptr;
	Particle* p2 = nullptr;
	float ks = 40.0f;
	float kd = 0.5f;
	float restingLength = 1.0f;

public:
	Spring()
	{

	}

	Spring(Particle* p1, Particle* p2, float restDist)
	{
		Spring::p1 = p1;
		Spring::p2 = p2;
		restingLength = restDist;
	}

	void applySpringForce()
	{
		// Calculate distance between the two particles
		glm::vec3 dist = p2->getPos() - p1->getPos();
		float length = glm::length(dist);
		// Normalize the distance for direciton
		glm::vec3 nDist = glm::normalize(dist);

		// Get the velocity along the direction
		float v1 = glm::dot(p1->velocity, nDist);
		float v2 = glm::dot(p2->velocity, nDist);

		// Calculate the force of the spring
		float fSD = -ks * (restingLength - length) - kd * (v1 - v2);
		glm::vec3 force = fSD * nDist;

		p1->applyForce(force);
		p2->applyForce(-force);
	}
};