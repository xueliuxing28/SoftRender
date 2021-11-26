/*************************************
 * \file Renderer.h
 * \date 2021/08/24 19:29
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/
#pragma once

#include "Vertex.h"
#include "Light.h"
#include "Texture.h"
#include "Camera.h"
#include"Shader.h"
#include "Mesh.h"
#include "Buffer.h"
using namespace std;
class Renderer
{
private:
	int deviceWidth;
	int deviceHeight;
	HDC screenHDC;
	Texture* tex;
	//float** zBuffer;
public:
	vector<DirectionLight> directionlights;
	vector<PointLight> pointlights;
	Camera *camera;
public:
	Renderer(HDC hdc, int screenWidth, int screenHeight, Camera *cam);
	~Renderer();
	

	//void ReleaseDevice();
	void Clear(DepthBuffer *zbuffer);

	void DrawByIndex(Mesh *m, IShader *shader,DepthBuffer *zbuffer);
	void DrawByArray(Mesh *m, IShader *shader,DepthBuffer *zbuffer);
	void DrawMesh(Mesh* m,IShader *shader,DepthBuffer *zbuffer);
	//void DrawPrimitive(Vertex v1, Vertex v2, Vertex v3, const Matrix& mvp, DepthBuffer *zbuffer);
	void DrawPrimitive(Vertex v0, Vertex v1, Vertex v2,IShader *shader, DepthBuffer *zbuffer);
	void RasterizeTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader, DepthBuffer *zbuffer);
	//swapIndex代表第几个点是插值出来的
	void DrawTopFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader, DepthBuffer *zbuffer,Vertex v3,int swapIndex);
	void DrawBottomFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader,DepthBuffer *zbuffer,Vertex v3, int swapIndex);
	void DrawLine(Vertex v0, Vertex v1, IShader *shader,DepthBuffer *zbuffer, Vector3f p0, Vector3f p1, Vector3f p2);
	void DrawPixel(int x, int y, const Color& color);
	
//	void VertexShader(Vertex &v);

	bool ZTestAndWrite(int x, int y, float depth,DepthBuffer *zbuffer);
	//简单CVV剔除，只考虑三顶点均不在的情况，未做边界三角形重新构建
	bool SimpleCVVCullCheck(const Vertex& vertex);
	//准备光栅化，透视投影除法，视口映射，三角形数据准备
	void PrepareRasterization(Vertex& vertex, Buffer *buffer);

	//以下为测试光栅化函数，单独的画线，画三角形，不带其他顶点数据的绘制
	void DrawTrangle3D(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Matrix& mvp);
	void DrawTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void DrawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void DrawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void DrawLine(int x0, int y0, int x1, int y1);
	void DrawPixel(int x, int y);
	Vector3f GetScreenCoord(const Vector3f& v);
	
};

