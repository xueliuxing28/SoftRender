#pragma once

#include "Matrix.h"
class DirectionLight
{
public:
	float intensity;
	//逐顶点或逐像素光照着色
	bool forVertex;
	DirectionLight() { intensity = 1;  forVertex = true; }

	DirectionLight(const Vector3f &dir, const Vector3f &pos,float Intensity=1,bool forV=true)
	{ 
		direction = dir; 
		position = pos;
	    intensity = Intensity; 
		forVertex = forV;
	}
	~DirectionLight() {}
	//取反
	Vector3f GetDirection()
	{
		return Vector3f(0,0,0)-direction;
	}
	Matrix LookAt(const Vector3f& upAxis);
	
private:
	Vector3f direction;
	Vector3f position;
};

class PointLight
{
public:
	PointLight() { intensity = 1.0; }
	PointLight(const Vector3f&pos, float inten) :position(pos), intensity(inten) {}
	~PointLight() {}
	Vector3f position;
	float intensity;

};



