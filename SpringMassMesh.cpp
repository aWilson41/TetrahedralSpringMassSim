#include "SpringMassMesh.h"

#include <QOpenGLBuffer>

void SpringMassMesh::update(GLfloat dt, GLfloat g)
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

	// Update the buffer (Only holds boundary points to draw the mesh)
	vertexBuffer->bind();
	void* bufferData = vertexBuffer->map(QOpenGLBuffer::WriteOnly);
	memcpy(bufferData, data.data(), data.size() * sizeof(VertexData));
	vertexBuffer->unmap();
	vertexBuffer->release();
}