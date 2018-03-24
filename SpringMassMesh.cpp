#include "SpringMassMesh.h"

#include <QOpenGLBuffer>

void SpringMassMesh::setupSpringMesh()
{
	particles = std::vector<Particle>(data.size());
	for (UINT i = 0; i < data.size(); i++)
	{
		particles[i].pos = &data[i].pos;
	}

	// Add springs for every edge
	UINT springCount = static_cast<UINT>(indexData.size()) / 2;
	springs = std::vector<Spring>(springCount);
	for (UINT i = 0; i < springCount; i++)
	{
		Particle* p1 = &particles[indexData[i * 2]];
		Particle* p2 = &particles[indexData[i * 2 + 1]];
		springs[i].p1 = p1;
		springs[i].p2 = p2;
		float length = glm::distance(p2->getPos(), p1->getPos());
		springs[i].restingLength = length;
		springs[i].kd = 0.9f;
		springs[i].ks = 350.0f;// 0.5f * length;
	}
}

void SpringMassMesh::update(GLfloat dt, GLfloat g)
{
	for (UINT i = 0; i < springs.size(); i++)
	{
		springs[i].applySpringForce();
	}

	// If hit's plane
	glm::vec3 n = glm::vec3(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		// Resolve it to -30 if less than -30
		if (particles[i].getPos()[1] < -80.0f)
		{
			particles[i].getPos()[1] = -80.0f;

			// Remove the velocity in the direction of the plane
			float l = glm::dot(n, particles[i].velocity);
			if (l < 0.0f)
				particles[i].velocity -= l * n;
			l = glm::dot(n, particles[i].force);
			if (l < 0.0f)
				particles[i].force -= l * n;
		}
	}

	for (UINT i = 0; i < particles.size(); i++)
	{
		particles[i].applyForce(glm::vec3(0.0f, g, 0.0f));
		particles[i].integrate(dt);
	}

	// Update the buffer (Only holds boundary points to draw the mesh)
	vertexBuffer->bind();
	void* bufferData = vertexBuffer->map(QOpenGLBuffer::WriteOnly);
	memcpy(bufferData, data.data(), data.size() * sizeof(VertexData));
	vertexBuffer->unmap();
	vertexBuffer->release();
}