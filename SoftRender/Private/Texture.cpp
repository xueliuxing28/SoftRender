#include "Texture.h"
#include <windows.h>


Texture::Texture()
{
	width = 512;
	height = 512;
}


Texture::~Texture()
{
}

void Texture::LoadTexture(const char* path)
{
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);
	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			COLORREF color = GetPixel(hdc, i, j);
			int r = color % 256;
			int g = (color >> 8) % 256;
			int b = (color >> 16) % 256;
			Color c((float)r / 256, (float)g / 256, (float)b / 256, 1);
			textureData[i][j] = c;
		}
	}
}

Color Texture::Sample(float u, float v)
{
	u = Clamp(0, 1.0f, u);
	v = Clamp(0, 1.0f, v);
	int intu = width * u;
	int intv = height * v;
	return textureData[intu][intv];
}

float Texture::Clamp(float min, float max, float value)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;
	return value;
}
