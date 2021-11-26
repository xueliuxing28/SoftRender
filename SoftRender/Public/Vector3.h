
#pragma once
#include <iostream>
#include <windows.h>


template<class T>
class Vector3
{
public:
	T x, y, z, w;

	Vector3<T>(T fx, T fy, T fz);
	Vector3<T>(T fx, T fy, T fz,T fw);
	Vector3<T>()
	{
		x = y = z = 0;
	    w = 1;
	}
	 Vector3<T> operator + (const Vector3<T>& right) const;
	 Vector3<T> operator - (const Vector3<T>& right) const;
	 Vector3<T> operator * (float value) const;
	 Vector3<T> operator / (float value) const;
	 float SquareMagnitude();
	 float Magnitude();
	 Vector3<T>& Normalize();

	float static Dot(const Vector3<T>& left, const Vector3<T>& right);
	float  Dot(const Vector3<T>& right);
	Vector3<T> static Cross(const Vector3<T>& left, const Vector3<T>& right);
    Vector3<T> static Lerp(const Vector3<T>& left, const Vector3<T>& right, float t);
	void standardization()
	{
		if (w == 0)
		{
			//std::cout << "error w==0" << std::endl;
			return;
		}			
		x /= w;
		y /= w;
		z /= w;
		w = 1;
	}
	void Print();
};
using Vector3f= Vector3<float>;
using Vector3i = Vector3<int>;


template<class T>
Vector3<T>::Vector3(T fx, T fy, T fz)
{
	x = fx;
	y = fy;
	z = fz;
	w = 1;
}
template<class T>
Vector3<T>::Vector3(T fx, T fy, T fz, T fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}
template<class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& right)const
{
	Vector3<T> returnValue(x + right.x, y + right.y, z + right.z);
	return returnValue;
}
template<class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& right)const
{
	Vector3<T> returnValue(x - right.x, y - right.y, z - right.z);
	return returnValue;
}
template<class T>
Vector3<T> Vector3<T>::operator*(float value)const
{
	Vector3<T> returnValue(x * value, y * value, z * value);
	return returnValue;
}
template<class T>
Vector3<T> Vector3<T>::operator/(float value)const
{
	Vector3<T> returnValue(x / value, y / value, z / value);
	return returnValue;
}
template<class T>
float Vector3<T>::SquareMagnitude()
{
	return Dot(*this, *this);
}
template<class T>
float Vector3<T>::Magnitude()
{
	return sqrt(SquareMagnitude());
}
template<class T>
Vector3<T>& Vector3<T>::Normalize()
{
	*this = *this / Magnitude();
	return *this;
}
template<class T>
float Vector3<T>::Dot(const Vector3<T>& left, const Vector3<T>& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}
template<class T>
float Vector3<T>::Dot(const Vector3<T> & right)
{
	return x * right.x + y * right.y + z * right.z;
}
template<class T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& left, const Vector3<T>& right)
{
	float valueX = left.y * right.z - left.z * right.y;
	float valueY = left.z * right.x - left.x * right.z;
	float valueZ = left.x * right.y - left.y * right.x;
	Vector3<T> returnValue(valueX, valueY, valueZ);
	return returnValue;
}
template<class T>
Vector3<T> Vector3<T>::Lerp(const Vector3<T>& left, const Vector3<T>& right, float t)
{
	return left + (right - left) * t;
}
template<class T>
void Vector3<T>::Print()
{
	std::cout << "Vector3<T> x: " << x << " y: " << y << " z: " << z << std::endl;
}


class Vector2
{
public:
	Vector2() { x = y = 0; }
	Vector2(float fx, float fy) {
		x = fx;
		y = fy;
	}

	~Vector2() {}
	Vector2 operator + (const Vector2& right) const
	{
		return Vector2(x + right.x, y + right.y);
	}
	Vector2 operator - (const Vector2& right) const
	{
		return Vector2(x- right.x, y - right.y);
	}
	Vector2 operator * (float value) const
	{
		return Vector2(x *value, y*value);
	}
	Vector2 operator / (float value) const
	{
		return Vector2(x /value, y/value);
	}
public:
	float x, y;
};


