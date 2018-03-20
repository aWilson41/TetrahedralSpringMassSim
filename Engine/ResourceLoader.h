#pragma once
#include "Engine\MathHelper.h"
#include "Engine\Polygon.h"

#include <iostream>
#include <fstream>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <qdir.h>

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
		for (UINT i = 0; i < aimesh->mNumVertices; i++)
		{
			vertexData[i].pos.x = aimesh->mVertices[i].x;
			vertexData[i].pos.y = aimesh->mVertices[i].y - 5.0f;
			vertexData[i].pos.z = aimesh->mVertices[i].z;

			vertexData[i].normal.x = aimesh->mNormals[i].x;
			vertexData[i].normal.y = aimesh->mNormals[i].y;
			vertexData[i].normal.z = aimesh->mNormals[i].z;
		}
		poly->setVertexBuffer(vertexData);

		// Get the faces connectivity information (not used for rendering but stored)
		std::vector<FaceData> faceData = std::vector<FaceData>(aimesh->mNumFaces);
		for (UINT i = 0; i < aimesh->mNumFaces; i++)
		{
			UINT numIndices = aimesh->mFaces[i].mNumIndices;
			faceData[i].vertices = std::vector<VertexData*>(numIndices);
			for (UINT j = 0; j < numIndices; j++)
			{
				faceData[i].vertices[j] = &vertexData[aimesh->mFaces[i].mIndices[j]];
			}
		}

		return poly;
	}

	// Loads in file of the format
	static SpringMassMesh* loadSpringMesh(std::string path)
	{
		std::ifstream input;
		input.open(path);

		SpringMassMesh* poly = new SpringMassMesh();
		int numPts = -1;
		int trash = -1;
		input >> numPts;
		std::vector<VertexData> vertexData = std::vector<VertexData>(numPts);
		for (int i = 0; i < numPts; i++)
		{
			VertexData vData;
			input >> trash >> vData.pos.x >> vData.pos.y >> vData.pos.z;
			vertexData[i] = vData;
		}
		poly->setVertexBuffer(vertexData);

		input.close();
		return poly;
	}
}