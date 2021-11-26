#include "Light.h"

Matrix DirectionLight::LookAt(const Vector3f& upAxis)
{
	Vector3f lookDir = direction;
	//lookDir.Print();
	lookDir.Normalize();

	Vector3f rightDir = Vector3f::Cross(upAxis, lookDir);
	rightDir.Normalize();

	Vector3f upDir = Vector3f::Cross(lookDir, rightDir);
	upDir.Normalize();
	Matrix m;
	m.value[0][0] = rightDir.x;  m.value[1][0] = upDir.x;  m.value[2][0] = lookDir.x;  m.value[3][0] = 0;
	m.value[0][1] = rightDir.y;	 m.value[1][1] = upDir.y;  m.value[2][1] = lookDir.y;  m.value[3][1] = 0;
	m.value[0][2] = rightDir.z;  m.value[1][2] = upDir.z;  m.value[2][2] = lookDir.z;  m.value[3][2] = 0;
	m.value[0][3] = -position.x;		 m.value[1][3] = -position.y;   m.value[2][3] = -position.z;	   m.value[3][3] = 1;
	
	//m.Print();
	return m;
}
