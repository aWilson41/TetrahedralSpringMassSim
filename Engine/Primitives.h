#pragma once
#include "Engine\Polygon.h"

class Plane : public Poly
{
public:
	Plane()
	{
		// Defines a 1x1x1 plane
		data = std::vector<VertexData>(6);
		data[0].pos = glm::vec3(-0.5f, 0.0f, -0.5f);
		data[1].pos = glm::vec3(0.5f, 0.0f, -0.5f);
		data[2].pos = glm::vec3(-0.5f, 0.0f, 0.5f);

		data[3].pos = glm::vec3(-0.5f, 0.0f, 0.5f);
		data[4].pos = glm::vec3(0.5f, 0.0f, -0.5f);
		data[5].pos = glm::vec3(0.5f, 0.0f, 0.5f);
		data[0].normal = data[1].normal = 
			data[2].normal = data[3].normal = 
			data[4].normal = data[5].normal = glm::vec3(0.0f, 1.0f, 0.0f);
		setVertexBuffer(data);
	}
};