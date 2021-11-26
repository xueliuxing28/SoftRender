/*************************************
 * \file Texture.h
 * \date 2021/08/28 16:23
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/
#pragma once
#include "Color.h"

class Color;
class Texture
{
private:
	int width;
	int height;
	Color textureData[1024][1024];
public:
	Texture();
	~Texture();

	void LoadTexture(const char* path);
	Color Sample(float u, float v);
	float Clamp(float min, float max, float value);
};

