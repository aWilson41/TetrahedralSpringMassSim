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
		springs[i].kd = 1.0f;
		springs[i].ks = 3.0f;
		Particle* p1 = &particles[indexData[i * 2]];
		Particle* p2 = &particles[indexData[i * 2 + 1]];
		springs[i].p1 = p1;
		springs[i].p2 = p2;
		glm::vec3 diff = p2->getPos() - p1->getPos();
		springs[i].restingLength = diff.length();
	}
}

void SpringMassMesh::update(GLfloat dt, GLfloat g)
{
	for (UINT i = 0; i < springs.size(); i++)
	{
		springs[i].applySpringForce();
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