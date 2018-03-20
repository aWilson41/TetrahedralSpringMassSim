#pragma once
#include "Engine\MathHelper.h"
#include "Engine\Polygon.h"
#include "Spring.h"
#include "Particle.h"

class SpringMassMesh : public Poly
{
public:
	void setupSprings()
	{
		// Generate edge list (there could easily be duplicate edges)

		// To do this we need connectivity information (faceData)
	}

	void update(GLfloat dt, GLfloat g);

protected:
	std::vector<glm::vec3> orgPositions;
	std::vector<Particle> particles;
	std::vector<Spring> springs;
};