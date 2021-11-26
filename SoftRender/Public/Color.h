/*************************************
 * \file Color.h
 * \date 2021/08/28 16:14
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/
#pragma once
class Color
{
public:
	float r, g, b, a;
	Color(float cr, float cg, float cb, float ca);
	Color() {};
	~Color();

	Color operator + (const Color& right) const;
	Color operator+(const float c)
	{
		return Color(r + c, g + c, b + c, a);
	}
	Color operator - (const Color& right) const;
	Color operator * (const Color& right) const;
	Color operator * (float value) const;
	Color operator / (float value) const;
	static Color Lerp(const Color& left, const Color& right, float t);
	static Color white;
};

