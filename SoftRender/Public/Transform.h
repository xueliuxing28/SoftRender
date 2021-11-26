/*************************************
 * \file Transform.h
 * \date 2021/08/23 10:45
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/
#pragma once
#include "Matrix.h"
#include "MyMath.h"
//TODO
class Transform
{
public:
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
	Matrix objectToWorld;
	Matrix Translate(const Vector3f &v);
	Matrix RotateX(float angle);
	Matrix RotateY(float angle);
	Matrix RotateZ(float angle);
	Matrix Rotate(const Vector3f& rotAngle);
	Matrix Scale(const Vector3f &s);
	Transform(Vector3f pos, Vector3f rot, Vector3f s) :position(pos), rotation(rot), scale(s) {}
	Transform() { objectToWorld.Identity(); }
};

