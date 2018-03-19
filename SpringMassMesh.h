#pragma once

#include "Engine\MathHelper.h"
#include "Engine\Polygon.h"
#include "Particle.h"
#include "Spring.h"

class SpringMassMesh : public Poly
{
public:
	void setupSprings()
	{
		// Generate edge list (there could easily be duplicate edges)

		// To do this we need a doubly conected structure (ie: Each face object references is vertex object children
		// and each vertex object references its face object)
	}

	void update(GLfloat dt, GLfloat g)
	{
		for (UINT i = 0; i < springs.size(); i++)
		{
			springs[i].applySpringForce();
		}

		for (UINT i = 0; i < springs.size(); i++)
		{
			particles[i].applyForce(glm::vec3(0.0f, g, 0.0f));
			particles[i].integrate(dt);
		}
	}

protected:
	std::vector<glm::vec3> orgPositions;
	std::vector<Particle> particles;
	std::vector<Spring> springs;
};