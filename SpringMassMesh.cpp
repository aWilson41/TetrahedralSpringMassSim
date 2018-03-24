#include "SpringMassMesh.h"

#include <QOpenGLBuffer>

void SpringMassMesh::setupSpringMesh()
{
	setupSpringMesh(indexData, indexCount);
}

void SpringMassMesh::setupSpringMesh(GLuint* indices, int size)
{
	particles = std::vector<Particle>(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		particles[i].pos = &vertexData[i].pos;
	}

	// Add springs for every edge
	int springCount = size / 2;
	springs = std::vector<Spring>(springCount);
	for (int i = 0; i < springCount; i++)
	{
		Particle* p1 = &particles[indices[i * 2]];
		Particle* p2 = &particles[indices[i * 2 + 1]];
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

	// Update the normals
	calculateNormals();
}

void SpringMassMesh::calculateNormals()
{
	// For every neighbor
	for (int i = 0; i < neighbors.size(); i++)
	{
		int numNeighborFaces = neighbors[i].size();
		glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);
		// For every face
		for (int j = 0; j < numNeighborFaces; j++)
		{
			glm::vec3 diff1 = neighbors[i][j]->v3->pos - neighbors[i][j]->v2->pos;
			glm::vec3 diff2 = neighbors[i][j]->v3->pos - neighbors[i][j]->v1->pos;
			sum += glm::normalize(glm::cross(diff1, diff2));
		}
		vertexData[i].normal = glm::normalize(sum / static_cast<float>(numNeighborFaces));
	}

	// Update the buffer (Only holds boundary points to draw the mesh)
	vertexBuffer->bind();
	void* bufferData = vertexBuffer->map(QOpenGLBuffer::WriteOnly);
	memcpy(bufferData, vertexData, vertexCount * sizeof(VertexData));
	vertexBuffer->unmap();
	vertexBuffer->release();
}