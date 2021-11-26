#pragma once
#include "MyMath.h"
#include"Vertex.h"
#include "Uniform.h"
struct IShader
{
public:
	virtual void VS(Vertex & v0,Vertex & v1,Vertex & v2)=0;
	virtual bool FS(Vertex & v, Vector3f g)=0;
};

struct PhongShader:public IShader
{
public:
	PhongShader(PhongVert v2f, PhongFrag frag) :v2f(v2f), frag(frag) {}
	~PhongShader() {}
	// 通过 IShader 继承
	virtual void VS(Vertex & v0, Vertex & v1, Vertex & v2) override;
	virtual bool FS(Vertex & v,Vector3f g) override;
	float CalcuteShadow(Vector3f posLightSpace, double bias);
public:
	PhongVert v2f;
	PhongFrag frag;
};


class ShadowShader :public IShader
{
public:
	ShadowShader() {}
	~ShadowShader() {}
	// 通过 IShader 继承
	virtual void VS(Vertex & v0, Vertex & v1, Vertex & v2) override;

	virtual bool FS(Vertex & v, Vector3f g) override;
	ShadowVert v2f;
};





