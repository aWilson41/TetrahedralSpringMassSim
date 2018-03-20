#include "Polygon.h"
#include "Material.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

void Poly::setVertexBuffer(std::vector<VertexData> data)
{
	Poly::data = data;
	vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	//vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	vertexBuffer->create();

	// Transfer vertex data to VBO 0
	vertexBuffer->bind();
	vertexBuffer->allocate(data.data(), sizeof(VertexData) * static_cast<int>(data.size()));
	vertexBuffer->release();
}

void Poly::setShaderProgram(QOpenGLShaderProgram* program)
{
	Poly::program = program;
	posAttributeLocation = program->attributeLocation("inPos");
	normalAttributeLocation = program->attributeLocation("inNormal");
}

void Poly::draw(glm::mat4 viewProj)
{
	glUniformMatrix4fv(program->uniformLocation("mvp_matrix"), 1, GL_FALSE, &(viewProj * world)[0][0]);
	glUniform3f(program->uniformLocation("mat.diffuseColor"), mat->getDiffuse().r, mat->getDiffuse().g, mat->getDiffuse().b);
	glUniform3f(program->uniformLocation("mat.ambientColor"), mat->getAmbient().r, mat->getAmbient().g, mat->getAmbient().b);

	// Tell OpenGL which VBOs to use
	vertexBuffer->bind();

	quintptr offset = 0;

	// Tell the pipeline where to find the vertex data
	program->setAttributeBuffer(posAttributeLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
	program->enableAttributeArray(posAttributeLocation);

	offset += sizeof(glm::vec3);

	// Tell the pipeline where to find the normal data
	program->setAttributeBuffer(normalAttributeLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
	program->enableAttributeArray(normalAttributeLocation);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(data.size()));

	program->disableAttributeArray(posAttributeLocation);
	program->disableAttributeArray(normalAttributeLocation);
	vertexBuffer->release();
}