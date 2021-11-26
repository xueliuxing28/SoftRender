#pragma once
#include "Transform.h"
class Camera
{
public:
// 	Vector3 position;
// 	Vector3 rotation;
	Transform transform;
	Matrix v, p;
public:
	Camera(Transform t) : transform(t){}
	Camera() {}
	Matrix LookAt(const Vector3f& eyePos, const Vector3f& lookat, const Vector3f& upAxis);
/*	Matrix MyWorldToCamera(const Vector3& eyePos, const Vector3& lookPos, const Vector3& upAxis);*/
	Matrix Perspective(float fov, float aspect, float nearPanel, float farPanel);
	Matrix Perspective(float l, float r, float n, float f, float t, float b);
	Matrix Orthographic(float l, float r, float n, float f, float t, float b);
};

