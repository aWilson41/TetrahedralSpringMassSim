#pragma once
#include "Engine\MathHelper.h"
#include "Engine\Polygon.h"
#include "Spring.h"
#include "Particle.h"

class SpringMassMesh : public Poly
{
public:
	void setupSpringMesh();

	void update(GLfloat dt, GLfloat g);

protected:
	std::vector<glm::vec3> orgPositions;
	std::vector<Particle> particles;
	std::vector<Spring> springs;
};