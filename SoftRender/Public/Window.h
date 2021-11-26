#pragma once
#include <windows.h>
// LRESULT OnEvent1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
// {
// 	switch (msg)
// 	{
// 	}
// 	return DefWindowProc(hwnd, msg, wParam, lParam);
// }

class Window
{
public:
	HWND window;
	int windowWidth;
	int windowHeight;
	HDC hdc;
	HDC screenHDC;
	Window(int w, int h, const char* name);
	~Window();

};

