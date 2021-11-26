#include "MyMath.h"

float clamp(float x, float mi, float ma)
{
	if (x < mi) x = mi;
	if (x > ma) x = ma;
	return x;
}



Vector3f centerOfGravity(Vector3f v1, Vector3f v2, Vector3f v3, Vector2 p)
{
	if ((-(v1.x - v2.x)*(v3.y - v2.y) + (v1.y - v2.y)*(v3.x - v2.x))==0)
		return Vector3f(1, 0, 0);
	if (-(v2.x - v3.x)*(v1.y - v3.y) + (v2.y - v3.y)*(v1.x - v3.x)==0)
		return Vector3f(1, 0, 0);
	float alpha = (-(p.x - v2.x)*(v3.y - v2.y) + (p.y - v2.y)*(v3.x - v2.x)) / (-(v1.x - v2.x)*(v3.y - v2.y) + (v1.y - v2.y)*(v3.x - v2.x));
	float beta = (-(p.x - v3.x)*(v1.y - v3.y) + (p.y - v3.y)*(v1.x - v3.x)) / (-(v2.x - v3.x)*(v1.y - v3.y) + (v2.y - v3.y)*(v1.x - v3.x));
	float gamma = 1 - alpha - beta;
	return Vector3f(alpha, beta, gamma);
}
