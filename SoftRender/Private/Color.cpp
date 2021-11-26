#include "Color.h"

Color::Color(float cr, float cg, float cb, float ca)
: r(cr), g(cg), b(cb), a(ca)
{

}


Color::~Color()
{
}

Color Color::operator + (const Color& right) const
{
	Color returnValue(r + right.r, g + right.g, b + right.b, a + right.a);
	return returnValue;
}

Color Color::operator - (const Color& right) const
{
	Color returnValue(r - right.r, g - right.g, b - right.b, a - right.a);
	return returnValue;
}

Color Color::operator * (const Color& right) const
{
	Color returnValue(r * right.r, g * right.g, b * right.b, a * right.a);
	return returnValue;
}

Color Color::operator * (float value) const
{
	Color returnValue(r * value, g * value, b * value, a * value);
	return returnValue;
}

Color Color::operator / (float value) const
{
	float rhvalue = 1.0f / value;
	Color returnValue(r * rhvalue, g * rhvalue, b * rhvalue, a * rhvalue);
	return returnValue;
}

Color Color::Lerp(const Color& left, const Color& right, float t)
{
	return left + (right - left) * t;
}

Color Color::white = Color(1, 1, 1, 1);

