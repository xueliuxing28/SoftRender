#include "Renderer.h"
#include "Matrix.h"
#include <iostream>
using namespace std;
Renderer::Renderer(HDC hdc, int screenWidth, int screenHeight,Camera *cam)
{
	screenHDC = hdc;
	deviceWidth = screenWidth;
	deviceHeight = screenHeight;
	camera = cam;
	

	tex = new Texture();
	tex->LoadTexture("gezi.bmp");
}


Renderer::~Renderer()
{
}


void Renderer::DrawByIndex(Mesh * m, IShader *shader, DepthBuffer *zbuffer)
{
	
	for (int i = 0; i <m->indexBuffer.size(); i = i + 4)
	{
		Vertex p1;
		p1.pos =m->positionBuffer[m->indexBuffer[i].x-1];
		p1.uv = m->uvBuffer[m->indexBuffer[i].y-1];
		p1.normal = m->normalBuffer[m->indexBuffer[i].z-1];
		Vertex p2;
		p2.pos = m->positionBuffer[m->indexBuffer[i+1].x-1];
		p2.uv = m->uvBuffer[m->indexBuffer[i+1].y-1];
		p2.normal = m->normalBuffer[m->indexBuffer[i+1].z-1];
		Vertex p3;
		p3.pos = m->positionBuffer[m->indexBuffer[i+2].x-1];
		p3.uv = m->uvBuffer[m->indexBuffer[i+2].y-1];
		p3.normal = m->normalBuffer[m->indexBuffer[i+2].z-1];

		Vertex p4;
		p4.pos = m->positionBuffer[m->indexBuffer[i + 3].x - 1];
		p4.uv = m->uvBuffer[m->indexBuffer[i + 3].y - 1];
		p4.normal = m->normalBuffer[m->indexBuffer[i + 3].z - 1];

//		Vertex p2 = m->vertexBuffer[m->indexBuffer[i + 1]];
//		Vertex p3 = m->vertexBuffer[m->indexBuffer[i + 2]];
		//DrawPrimitive(p1, p2, p3, m->transform.objectToWorld);

		DrawPrimitive(p1, p2, p3, shader,zbuffer);
		DrawPrimitive(p1, p3, p4 ,shader,zbuffer);
	}
}

void Renderer::DrawByArray(Mesh * m, IShader *shader, DepthBuffer *zbuffer)
{
	for (int i = 0; i < m->vertexBuffer.size(); i = i + 3)
	{
		Vertex p1 = m->vertexBuffer[i];
		Vertex p2 = m->vertexBuffer[i + 1];
		Vertex p3 = m->vertexBuffer[i + 2];
		
		//DrawPrimitive(p1, p2, p3, m->transform.objectToWorld);
		DrawPrimitive(p1, p2, p3,shader,zbuffer);
	}
}

void Renderer::DrawMesh(Mesh * m, IShader *shader, DepthBuffer *zbuffer)
{
	if (m->indexBuffer.size() > 0)
		DrawByIndex(m, shader,zbuffer);
	else
		DrawByArray(m,shader,zbuffer);
}

//顶点需要进行值传递，因为有重复
void Renderer::DrawPrimitive(Vertex v1, Vertex v2, Vertex v3, IShader *shader, DepthBuffer *zbuffer)
{

	shader->VS(v1,v2,v3);

	//进行CVV简单剔除判断
	// 	if (SimpleCVVCullCheck(v1) && SimpleCVVCullCheck(v2) && SimpleCVVCullCheck(v3))
	// 		return;

		//透视除法，视口映射，数据准备（全部改为1/z）
	PrepareRasterization(v1,zbuffer);
	PrepareRasterization(v2,zbuffer);
	PrepareRasterization(v3,zbuffer);
	//三角形三个点重合明显存在问题，直接不渲染,或者两个
	if (((int)v1.pos.y == (int)v2.pos.y && abs(v2.pos.y - v3.pos.y) <= 1) ||
		((int)v3.pos.y == (int)v2.pos.y && abs(v2.pos.y - v1.pos.y) <= 1) ||
		((int)v1.pos.y == (int)v3.pos.y && abs(v2.pos.y - v3.pos.y) <= 1))
		return;
// 	v1.Print();
// 	v2.Print();
// 	v3.Print();
// 	cout << endl;
	//cout << v1.pos.x << " " << v1.pos.y << " " << v1.pos.z << " " << endl;
	RasterizeTrangle(v1, v2, v3,shader,zbuffer);
}

void Renderer::RasterizeTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader, DepthBuffer *zbuffer)
{
// 	v0.Print();
// 	v1.Print();
// 	v2.Print();
// 	cout << endl;
	int ty0 = v0.pos.y;
	int ty1 = v1.pos.y;
	int ty2 = v2.pos.y;
	if (ty0 == ty1)	//上三角形
	{
//		cout << 11 << endl;
		DrawTopFlatTrangle(v0, v1, v2,shader,zbuffer,Vertex(),-1);
	}
	else if (ty1 == ty2) //下三角形
	{

		DrawBottomFlatTrangle(v0, v1, v2,shader,zbuffer,Vertex(),-1);
		//std::cout << v0.pos.z<<" "<<v1.pos.z <<" "<<v2.pos.z<< std::endl;
	}
	else//拆分为一个平顶三角形和一个平底三角形
	{
		//中心点为直线(x0, y0)，(x2, y2)上取y1的点
		float x3 = (v1.pos.y - v0.pos.y) * (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y) + v0.pos.x;
		float y3 = v1.pos.y;
		float t = (y3 - v0.pos.y) / (v2.pos.y - v0.pos.y);

		Vertex v3(Vector3f(x3, y3, 0), Color(0, 0, 0, 0), Vector2(0,0));
		v3.LerpVertexData(v0, v2, t);
		DrawBottomFlatTrangle(v0, v1, v3,shader,zbuffer,v2,3);
		DrawTopFlatTrangle(v3, v1, v2,shader,zbuffer,v0,1);
	}
}

void Renderer::DrawTopFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader, DepthBuffer *zbuffer, Vertex v3, int swapIndex)
{
	float x0 = v0.pos.x;
	float y0 = v0.pos.y;
	float x1 = v1.pos.x;
	float y1 = v1.pos.y;
	float x2 = v2.pos.x;
	float y2 = v2.pos.y;
	for (float y = y0; y <= y2; y++)
	{
		float t = (y - y0) / (y2 - y0);
		//用int，不然会有断线
		int xl = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		Vertex vl(Vector3f(xl, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vl.LerpVertexData(v0, v2, t);
		//std::cout << vl.pos.z << std::endl;
		int xr = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		Vertex vr(Vector3f(xr, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vr.LerpVertexData(v1, v2, t);
		//std::cout << vl.pos.z << std::endl;
		//cout << " " << vl.pos.z << " " << vr.pos.z << " " << t << endl;
		switch (swapIndex)
		{
		case -1:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		case 1:
			DrawLine(vl, vr, shader, zbuffer, v3.pos, v1.pos, v2.pos);
			break;
		case 2:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v3.pos, v2.pos);
			break;
		case 3:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v3.pos);
			break;
		default:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		}
		
	}
}

void Renderer::DrawBottomFlatTrangle(Vertex v0, Vertex v1, Vertex v2, IShader *shader,DepthBuffer *zbuffer, Vertex v3, int swapIndex)
{
	float x0 = v0.pos.x;
	float y0 = v0.pos.y;
	float x1 = v1.pos.x;
	float y1 = v1.pos.y;
	float x2 = v2.pos.x;
	float y2 = v2.pos.y;
	for (float y = y0; y <= y1; y++)
	{
		float t = (y - y0) / (y2 - y0);

		int xl = ((y - y1) * (x0 - x1) / (y0 - y1) + x1);
		Vertex vl(Vector3f(xl, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vl.LerpVertexData(v0, v1, t);
		//std::cout << vl.pos.z << std::endl;
		//cout << y0 << " " << v1.pos.z << " " << v0.pos.z << " " << t << " " << vl.pos.z << endl;
		int xr = ((y - y2) * (x0 - x2) / (y0 - y2) + x2);
		Vertex vr(Vector3f(xr, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		vr.LerpVertexData(v0, v2, t);
		//std::cout << vr.pos.z << std::endl;
		//cout << y0 << " " << v2.pos.z << " " << v0.pos.z << " " << t << " " << vr.pos.z << endl;
		//cout << " " << vl.pos.z << " " << vr.pos.z << " " << t << endl;
		switch (swapIndex)
		{
		case -1:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		case 1:
			DrawLine(vl, vr, shader, zbuffer, v3.pos, v1.pos, v2.pos);
			break;
		case 2:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v3.pos, v2.pos);
			break;
		case 3:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v3.pos);
			break;
		default:
			DrawLine(vl, vr, shader, zbuffer, v0.pos, v1.pos, v2.pos);
			break;
		}
	}
}


//y值永远相等的线
void Renderer::DrawLine(Vertex v0, Vertex v1, IShader *shader,DepthBuffer *zbuffer, Vector3f p0, Vector3f p1, Vector3f p2)
{

	//std::cout << v0.color.r ;
	float x0 = v0.pos.x;
	float x1 = v1.pos.x;
	float y0 = v0.pos.y;
	float y1 = v1.pos.y;
	//只考虑x方向扫描线即可
	int dx = x1 - x0;
//	int dy = y1 - y0;
// 	if(dy!=0)
// 	cout << dy;
	int stepx = 1;
//	int stepy = 1;

	if (dx < 0)
	{
		stepx = -1;
		dx = -dx;
	}


	int x = x0;
	int y = y0;
//	cout << " " << v0.pos.z << " " << v1.pos.z << " " << endl;
//	int errorValue = dy2 - dx;
	Vertex frag;
	if (x1 == x0)
	{
		if (ZTestAndWrite(x, y, (v0.pos.z+1)/2.0,zbuffer))
		{
			//cout << x << endl;
// 			Color c = v0.color;
// 			c = c * tex->Sample(v0.uv.x , v0.uv.y );
// 			DrawPixel(x, y, v0.color);
	//c = c * tex->Sample(u, v);
// 			 	p0.Print();
//  	            p1.Print();
//  	            p2.Print();
// 			std::cout << x << " " << y << std::endl;
//  	            cout << endl;
			Vector3f g = centerOfGravity(p0, p1, p2, Vector2(x, y));
			if (shader->FS(v0,g))
			{
				DrawPixel(x, y, v0.color);
			}
		}
// 		else
// 		{
// 			cout << "errpr" << endl;
// 		}
		return;
	}
	for (int i = 0; i <= dx; i++)
	{
		float s = (x - x0) / (x1 - x0);
		//透视矫正,https://zhuanlan.zhihu.com/p/144331875
		float t = s * v0.pos.z / (s* v0.pos.z + (1 - s)* v1.pos.z);
		float z = Vertex::LerpFloat(v0.pos.z, v1.pos.z, t);
		z = (z + 1) / 2.0;
//		Vector3f n = Vector3f::Lerp(v0.normal, v1.normal, t);
//		Vector3f v = Vector3f::Lerp(v0.pos, v1.pos, t);
//		float realz = 1.0f / z;
		//cout << " " << v0.pos.z << " " << v1.pos.z << " " << z << endl;
		//std::cout <<  t<< std::endl;
		if (ZTestAndWrite(x, y, z,zbuffer))
		{
			//std::cout << v0.pos.z << " " << v1.pos.z << " "  << std::endl;
			//std::cout << z << std::endl;
			//待优化
 			Color c = Color::Lerp(v0.color, v1.color, t);
			float u = Vertex::LerpFloat(v0.uv.x, v1.uv.x,t);
			float v = Vertex::LerpFloat(v0.uv.y, v1.uv.y,t);
			frag.pos = Vector3f(x, y, z);
			frag.color = c;
			frag.uv = Vector2(u, v);
// 			float reciprocalW = 1.0f / p1.w;
// 			p1.x = (p1.x * reciprocalW + 1.0f) * 0.5f * (deviceWidth - 1);
// 			p1.y = (1 - p1.y * reciprocalW) * 0.5f * (deviceHeight - 1);
// 			p0.Print();
// 			p1.Print();
// 			p2.Print();
// 			cout << endl;
			Vector3f g = centerOfGravity(p0, p1, p2, Vector2(x, y));
			//c = c * tex->Sample(u, v);
			if (shader->FS(frag,g))
			{
				//std::cout << z << " " << c.r << " " << std::endl;
				DrawPixel(x, y, frag.color);
			}
			
		}
// 		else
// 		{
// 			std::cout << x << " " <<y << " " <<z<< std::endl;
// 		}
		x += stepx;
	}
}

bool Renderer::ZTestAndWrite(int x, int y, float depth,DepthBuffer *zbuffer)
{

	if (x >= 0 && x < zbuffer->width && y >= 0 && y < zbuffer->height)
	{
		if (zbuffer->depthBuffer[y][x] >= depth)
		{
			//cout << depth << endl;
			zbuffer->depthBuffer[y][x] = depth;
			return true;
		}
	}
	return false;
}

inline void Renderer::DrawPixel(int x, int y, const Color& color)
{
	//cout << color.r << " " << color.g << " " << color.b << " " << endl;
	//color值超过1会自动进行tonemapping
	SetPixel(screenHDC, x, y, RGB(255 * color.r, 255 * color.g, 255 * color.b));
}




void Renderer::Clear(DepthBuffer *zbuffer)
{
	BitBlt(screenHDC, 0, 0, deviceWidth, deviceHeight, NULL, NULL, NULL, BLACKNESS);
	//ClearZ
	for (int i = 0; i < zbuffer->height; i++)
	{
		for (int j = 0; j < zbuffer->width; j++)
		{
			zbuffer->depthBuffer[i][j] = 1;
		}
	}
}




inline void Renderer::PrepareRasterization(Vertex& vertex,Buffer *buffer)
{

	float reciprocalW = 1.0f / vertex.pos.w;
	//最后加0.5是为了后面取证做四舍五入
	vertex.pos.x = (vertex.pos.x * reciprocalW + 1.0f) * 0.5f * (buffer->width-1)+0.5;
	vertex.pos.y = (vertex.pos.y * reciprocalW+1.0f) * 0.5f * (buffer->height-1)+0.5;

}

inline bool Renderer::SimpleCVVCullCheck(const Vertex& vertex)
{
	float w = vertex.pos.w;
	if (vertex.pos.x < -w || vertex.pos.x > w)
		return true;
	if (vertex.pos.y < -w || vertex.pos.y > w)
		return true;
	if (vertex.pos.z < 0.0f || vertex.pos.z > w)
		return true;
	return false;
}



void Renderer::DrawPixel(int x, int y)
{
	//std::cout << x << " " << y << std::endl;
	SetPixel(screenHDC, x, y, RGB(255, 255, 0));
}


void Renderer::DrawLine(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int stepx = 1;
	int stepy = 1;

	if (dx < 0)
	{
		stepx = -1;
		dx = -dx;
	}

	if (dy < 0)
	{
		stepy = -1;
		dy = -dy;
	}

	int dy2 = dy << 1;
	int dx2 = dx << 1;

	//斜率k = dy / dx
	//以斜率小于1为例，x轴方向每单位都应该绘制一个像素，x累加即可，而y需要判断。
	//误差项errorValue = errorValue + k，一旦k > 1，errorValue = errorValue - 1，保证0 < errorValue < 1
	//errorValue > 0.5时，距离y + 1点较近，因而y++，否则y不变。
	/*float errorValue = 0;
	for (int x = x0, y = y0; x <= x1; x++)
	{
	DrawPixel(x, y);
	errorValue += (float)dy / dx;
	if (errorValue > 0.5)
	{
	errorValue = errorValue - 1;
	y++;
	}
	}*/

	int x = x0;
	int y = y0;
	int errorValue;

	//改为整数计算，去掉除法
	if (dy < dx)
	{
		errorValue = dy2 - dx;
		for (int i = 0; i <= dx; i++)
		{
			DrawPixel(x, y);
			x += stepx;
			errorValue += dy2;
			if (errorValue >= 0)
			{
				errorValue -= dx2;
				y += stepy;
			}
		}
	}
	else
	{
		errorValue = dx2 - dy;
		for (int i = 0; i <= dy; i++)
		{
			DrawPixel(x, y);
			y += stepy;
			errorValue += dx2;
			if (errorValue >= 0)
			{
				errorValue -= dy2;
				x += stepx;
			}
		}
	}
}


void Renderer::DrawTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	//按照y进行排序，使y0 < y1 < y2
	if (y1 < y0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (y2 < y0)
	{
		std::swap(x0, x2);
		std::swap(y0, y2);
	}
	if (y2 < y1)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	if (y0 == y1)	//平顶三角形
	{
		DrawTopFlatTrangle(x0, y0, x1, y1, x2, y2);
	}
	else if (y1 == y2) //平底三角形
	{
		DrawBottomFlatTrangle(x0, y0, x1, y1, x2, y2);
	}
	else			//拆分为一个平顶三角形和一个平底三角形
	{
		//中心点为直线(x0, y0)，(x2, y2)上取y1的点
		int x3 = (y1 - y0) * (x2 - x0) / (y2 - y0) + x0;
		int y3 = y1;

		//进行x排序，此处约定x2较小
		if (x1 > x3)
		{
			std::swap(x1, x3);
			std::swap(y1, y3);
		}
		DrawBottomFlatTrangle(x0, y0, x1, y1, x3, y3);
		DrawTopFlatTrangle(x1, y1, x3, y3, x2, y2);
	}
}

void Renderer::DrawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	for (int y = y0; y <= y1; y++)
	{
		int xl = (y - y1) * (x0 - x1) / (y0 - y1) + x1;
		int xr = (y - y2) * (x0 - x2) / (y0 - y2) + x2;
		DrawLine(xl, y, xr, y);
	}
}

void Renderer::DrawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	for (int y = y0; y <= y2; y++)
	{
		int xl = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		int xr = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		DrawLine(xl, y, xr, y);
	}
}

void Renderer::DrawTrangle3D(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, const Matrix& mvp)
{
	Vector3f vt1 = mvp.MultiplyVector3(v1);
	Vector3f vt2 = mvp.MultiplyVector3(v2);
	Vector3f vt3 = mvp.MultiplyVector3(v3);

	Vector3f vs1 = GetScreenCoord(vt1);
	Vector3f vs2 = GetScreenCoord(vt2);
	Vector3f vs3 = GetScreenCoord(vt3);

	DrawTrangle(vs1.x, vs1.y, vs2.x, vs2.y, vs3.x, vs3.y);
}

Vector3f Renderer::GetScreenCoord(const Vector3f& v)
{
	float reciprocalW = 1.0f / v.w;
	float x = (v.x * reciprocalW + 1.0f) * 0.5f * deviceWidth;
	float y = (1.0f - v.y * reciprocalW) * 0.5f * deviceHeight;
	float z = 1 / v.z;
	return Vector3f(x, y, z);
}
