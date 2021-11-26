/*************************************
 * \file Mesh.h
 * \date 2021/08/23 10:50
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/

#pragma once
#include "Vertex.h"
#include "Transform.h"
#include <vector>

class Mesh
{
private:
	//Matrix objectToWorld;
	//void DrawElement(ApcDevice* device);
	//void DrawArray(ApcDevice* device);
public:
	Transform transform;
	std::vector<Vertex> vertexBuffer;
	std::vector<Vector3f> positionBuffer;
	std::vector<Vector3f> normalBuffer;
	std::vector<Vector2> uvBuffer;
	std::vector<Vector3i> indexBuffer;
	Mesh();
	~Mesh();
	Transform GetTransform() { return transform; }
	int GetIndexBufferLength() { return indexBuffer.size(); }
	void SetTransform(Transform &t) { transform = t; }
	Matrix GetObjectToWorld() { return transform.objectToWorld; }
	void SetObjectToWorld(const Matrix &m) { transform.objectToWorld = m; }

	//static Mesh* CreateCube();

	void AddVertexData(const Vector3f  pos, float u, float v, const Color color = Color::white);
	void AddVertexData(float posx, float posy, float posz, float u, float v, const Color color = Color::white);
	void AddVertexData(float posx, float posy, float posz, float u, float v, Vector3f nor,const Color color = Color::white);
	//void DrawMesh(ApcDevice* device);

};

