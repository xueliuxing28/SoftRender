#include "Mesh.h"
#include <iostream>
Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}



// Mesh* Mesh::CreateCube()
// {
// 	Mesh* mesh = new Mesh();
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Color(1, 0, 0, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Color(1, 0, 0, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Color(1, 0, 0, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Color(1, 0, 0, 1));
// 	mesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Color(1, 0, 0, 1));
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Color(1, 0, 0, 1));
// 	//Ç°
// 	mesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Color(0, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Color(0, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Color(0, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Color(0, 1, 0, 1));
// 	mesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, Color(0, 1, 0, 1));
// 	mesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Color(0, 1, 0, 1));
// 	//×ó
// 	mesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
// 	mesh->AddVertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
// 	mesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
// 	mesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
// 	//ÓÒ
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Color(0, 0, 1, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Color(0, 0, 1, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Color(0, 0, 1, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Color(0, 0, 1, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Color(0, 0, 1, 1));
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Color(0, 0, 1, 1));
// 	//ÏÂ
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Color(1, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, Color(1, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Color(1, 1, 0, 1));
// 	mesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Color(1, 1, 0, 1));
// 	mesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Color(1, 1, 0, 1));
// 	mesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Color(1, 1, 0, 1));
// 	//ÉÏ
// 	mesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f);
// 	mesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
// 	mesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f);
// 	mesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
// 	mesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f);
// 	return mesh;
// }

void Mesh::AddVertexData(const Vector3f  pos, float u, float v, const Color color /* = Color::white */)
{
	Vertex p(pos, color, Vector2(u,v));
	vertexBuffer.push_back(p);
}

void Mesh::AddVertexData(float posx, float posy, float posz, float u, float v, const Color color /* = Color::white */)
{
	AddVertexData(Vector3f(posx, posy, posz), u, v, color);
}

void Mesh::AddVertexData(float posx, float posy, float posz, float u, float v, Vector3f  nor, const Color  color)
{
	Vertex p(Vector3f(posx, posy, posz), color, nor,Vector2(u,v));
	vertexBuffer.push_back(p);
}
