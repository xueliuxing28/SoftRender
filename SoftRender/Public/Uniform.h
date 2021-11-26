#pragma once
#include <vector>
#include "Light.h"
#include "Vector3.h"
#include "Texture.h"
#include "Matrix.h"
#include "Buffer.h"
struct PhongVert
{
	std::vector<DirectionLight> dirlights;
	std::vector<PointLight> pointlights;
	Vector3f cameraPos;
	Vector3f lp0,lp1,lp2;
	Matrix m, v, p;
	Vertex v0, v1, v2;
	Matrix lightV, lightP;
};
struct PhongFrag
{
	PhongFrag(Texture *baseTex, DepthBuffer *depthBuffer) :baseTex(baseTex), depthBuffer(depthBuffer) {}
	~PhongFrag() {
		//delete baseTex, depthBuffer;
	}
	Texture *baseTex;
	DepthBuffer *depthBuffer;
};

struct ShadowVert
{
	Matrix lightV,lightP;
	Matrix m;
};