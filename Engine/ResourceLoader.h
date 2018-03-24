#pragma once
#include "Engine\MathHelper.h"
#include "Engine\Polygon.h"

#include <iostream>
#include <fstream>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <qdir.h>
#include <qmessagebox.h>

namespace resourceLoader
{
	// Loads a scene from files found in a directory (no scene graph/object implemented yet)
	/*static void loadDirScene(QString path)
	{
		QDir dir(path);
		QFileInfoList list = dir.entryInfoList();

		for (int i = 0; i < list.size(); i++)
		{
			QFileInfo fileInfo = list[i];
			if (fileInfo.completeSuffix() == "obj")
				loadPolygon(fileInfo.path().toStdString());
		}
	}*/

	static Poly* loadPolygon(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* aiscene = importer.ReadFile(path, aiProcess_Triangulate/* | aiProcess_JoinIdenticalVertices*/);
		if (aiscene == nullptr)
			return nullptr;
		if (!aiscene->HasMeshes())
			return nullptr;
		aiMesh* aimesh = aiscene->mMeshes[0];

		// This function only loads in a single mesh from a scene
		Poly* poly = new Poly();

		// Get the vertices
		std::vector<VertexData> vertexData = std::vector<VertexData>(aimesh->mNumVertices);
		std::vector<GLuint> indexData = std::vector<GLuint>(aimesh->mNumVertices);
		for (UINT i = 0; i < aimesh->mNumVertices; i++)
		{
			vertexData[i].pos.x = aimesh->mVertices[i].x;
			vertexData[i].pos.y = aimesh->mVertices[i].y - 5.0f;
			vertexData[i].pos.z = aimesh->mVertices[i].z;

			vertexData[i].normal.x = aimesh->mNormals[i].x;
			vertexData[i].normal.y = aimesh->mNormals[i].y;
			vertexData[i].normal.z = aimesh->mNormals[i].z;
			indexData[i] = i;
		}
		poly->setVertexBuffer(vertexData);
		poly->setIndexBuffer(indexData);

		// Get the faces connectivity information (not used for rendering but stored)
		/*std::vector<FaceData> faceData = std::vector<FaceData>(aimesh->mNumFaces);
		for (UINT i = 0; i < aimesh->mNumFaces; i++)
		{
			UINT numIndices = aimesh->mFaces[i].mNumIndices;
			faceData[i].vertices = std::vector<VertexData*>(numIndices);
			for (UINT j = 0; j < numIndices; j++)
			{
				faceData[i].vertices[j] = &vertexData[aimesh->mFaces[i].mIndices[j]];
			}
		}*/

		return poly;
	}

	// Loads in file of the format
	static SpringMassMesh* loadSpringMesh(std::string path)
	{
		std::ifstream input;
		input.open(path);
		if (input.fail())
			return nullptr;

		SpringMassMesh* poly = new SpringMassMesh();
		unsigned int count = 0;
		input >> count;
		std::vector<VertexData> vertexData = std::vector<VertexData>(count);
		for (unsigned int i = 0; i < count; i++)
		{
			VertexData vData;
			input >> vData.pos.x >> vData.pos.y >> vData.pos.z;
			vertexData[i] = vData;
		}

		input >> count;
		std::vector<GLuint> indexData = std::vector<GLuint>(count * 2);
		for (unsigned int i = 0; i < count * 2; i += 2)
		{
			input >> indexData[i] >> indexData[i + 1];
		}

		input.close();

		poly->setVertexBuffer(vertexData);
		poly->setIndexBuffer(indexData);
		poly->setupSpringMesh();
		return poly;
	}

	static void addEdge(int i1, int i2, std::vector<GLuint>& indices)
	{
		for (int i = 0; i < indices.size(); i += 2)
		{
			int j1 = indices[i];
			int j2 = indices[i + 1];
			if ((j1 == i1 && j2 == i2) || (j2 == i1 && j1 == i2))
				return;
		}
		indices.push_back(i1);
		indices.push_back(i2);
	}
	static SpringMassMesh* loadTetgenMesh(std::string path, float scale = 1.0f)
	{
		// Read the vertices
		std::ifstream input;
		input.open(path + ".node");
		if (input.fail())
			return nullptr;

		SpringMassMesh* poly = new SpringMassMesh();
		int trash = -1;
		unsigned int count = 0;
		input >> count >> trash >> trash >> trash;
		std::vector<VertexData> vertexData = std::vector<VertexData>(count);
		for (unsigned int i = 0; i < count; i++)
		{
			VertexData vData;
			input >> trash >> vData.pos.x >> vData.pos.y >> vData.pos.z;
			vData.pos *= scale;
			vertexData[i] = vData;
		}
		input.close();

		// Read the edges
		input.open(path + ".ele");
		if (input.fail())
			return nullptr;

		input >> count >> trash >> trash;
		std::vector<GLuint> indexData;
		for (unsigned int i = 0; i < count; i++)
		{
			int j = i * 12;
			int index[4];
			input >> trash >> index[0] >> index[1] >> index[2] >> index[3];
			index[0]--;
			index[1]--;
			index[2]--;
			index[3]--;

			addEdge(index[0], index[1], indexData);
			addEdge(index[0], index[2], indexData);
			addEdge(index[0], index[3], indexData);
			addEdge(index[1], index[2], indexData);
			addEdge(index[1], index[3], indexData);
			addEdge(index[2], index[3], indexData);
		}
		input.close();

		poly->setVertexBuffer(vertexData);
		poly->setIndexBuffer(indexData);
		poly->setupSpringMesh();
		return poly;
	}
}