#pragma once
#include "Engine\MathHelper.h"

#include <QOpenGLFunctions>

class QOpenGLShaderProgram;
class QOpenGLBuffer;
class Material;

struct VertexData
{
	glm::vec3 pos;
	//glm::vec2 texCoords;
	glm::vec3 normal;
};

// Contains array of indices to vertices
struct FaceData
{
	std::vector<VertexData*> vertices;
};

class Poly : protected QOpenGLFunctions
{
public:
	Poly() { initializeOpenGLFunctions(); }

	void setVertexBuffer(std::vector<VertexData> data);

	void setMaterial(Material* mat) { Poly::mat = mat; }

	void setIndexBuffer(std::vector<int> data) { }

	// Each polygon gets a pointer to the shader it will use
	void setShaderProgram(QOpenGLShaderProgram* program);

	void draw(glm::mat4 viewProj);

public:
	glm::mat4 world = glm::mat4(1.0f);

public:
	QOpenGLShaderProgram* program;
	int posAttributeLocation = -1;
	int normalAttributeLocation = -1;

	QOpenGLBuffer* vertexBuffer;
	//QOpenGLBuffer* indexBuffer;
	// Contains per vertex data of the mesh
	std::vector<VertexData> data;
	std::vector<FaceData> faces;

public:
	Material* mat;
};