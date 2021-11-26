

#include "Color.h"
#include "Matrix.h"
#pragma once
class Vertex
{
public:
	Vector3f pos;
	Vector3f normal;
	Color color;
	Vector2 uv;
	//float u, v;
	Vertex() {};
	Vertex(const Vector3f& p, const Color& c, Vector2 uv);
	Vertex(const Vector3f& p, const Color& c, const Vector3f &normal, Vector2 uv);
	~Vertex();

	void LerpVertexData(Vertex& left, const Vertex& right, float t);
	void Print();
	Vertex &operator * (const Matrix & m);
	static float LerpFloat(float v1, float v2, float t){ return v1 + (v2 - v1) * t;}
};

