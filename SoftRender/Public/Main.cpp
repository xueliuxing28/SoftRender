/*************************************
 * \file Main.cpp
 * \date 2021/08/23 10:42
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/


#include "Renderer.h"
#include "ObjFileReader.h"
#include "Window.h"
#include "Input.h"
#include <iostream>
#include <cmath>
#pragma comment( lib,"winmm.lib" )
using namespace std;

static const int windowWidth = 500;
static const int windowHeight = 500;

Renderer* device = NULL;
Camera *camera =new Camera(Transform (Vector3f(0, 0, 100), Vector3f(0, 0, 0), Vector3f(0, 0, 0)));
Mesh* currentMesh = new Mesh();
Mesh *m = new Mesh();
Mesh *plane = new Mesh();
Vector3f moveVector,rotateVector;
PhongShader *phongShader;
PhongShader *planeShader;
ShadowShader *depthShader = new ShadowShader();
DepthBuffer *depthBuffer = new DepthBuffer(windowWidth*2,windowHeight*2);
DepthBuffer *zBuffer = new DepthBuffer(windowWidth, windowHeight);
//Matrix oP=camera->Perspective(30, 1, 1, 100);
Matrix M;
void UpdateInput(Mesh *m)
{

	if (IS_KEY_DOWN('A'))
	{
		moveVector.x -= 0.01f;
	}
	if (IS_KEY_DOWN('D'))
	{
		moveVector.x += 0.01f;
	}
	if (IS_KEY_DOWN('W'))
	{
		moveVector.y += 0.01f;
	}
	if (IS_KEY_DOWN('S'))
	{
		moveVector.y -= 0.01f;
	}
	if (IS_KEY_DOWN('E'))
	{
		moveVector.z -= 0.1f;
	}
	if (IS_KEY_DOWN('Q'))
	{
		moveVector.z += 0.1f;
	}
	if (IS_KEY_DOWN('I'))
	{
		rotateVector.y += 0.1f;
	}
	if (IS_KEY_DOWN('K'))
	{
		rotateVector.y -= 0.1f;
	}
	if (IS_KEY_DOWN('L'))
	{
		rotateVector.x += 0.5f;
	}
	if (IS_KEY_DOWN('J'))
	{
		rotateVector.x -= 0.5f;
	}
	if (IS_KEY_DOWN('U'))
	{
		rotateVector.z -= 0.1f;
	}
	if (IS_KEY_DOWN('O'))
	{
		rotateVector.z += 0.1f;
	}
//	rotateVector.Print();
	Matrix s= m->GetTransform().Scale(Vector3f(1,1,1));
	Matrix r = m->GetTransform().Rotate(rotateVector);//Vector3f(2,0,11)Vector3f(298,32.9,77.59)Vector3f(-141, 83.2993, -14.5)Vector3f(13.5, 2.1, 0)
	Matrix t=m->GetTransform().Translate(moveVector);//Vector3f(-1,0,0)
	//注意矩阵乘法顺序！！
//	moveVector.Print();
	M = t*r*s;
	//M.Print();
//	rotateVector.Print();
	phongShader->v2f.m = M;
	depthShader->v2f.m = M;
//	m->SetObjectToWorld(M);
}
void Update(Window *w);
void DoRender(Window *w);
void ShowFPS(Window *w);
void CreateCube();
void CreatePlane();
int main()
{

	Window *w = new Window(windowWidth, windowHeight, "Test");
	device = new Renderer(w->screenHDC, windowWidth, windowHeight,camera);
	//规定相机永远往-z方向看，这决定zbuffer初始化为最大值还是最小值（最大值，因为深度值为负数）
	//,视口的范围应该是0-负无穷，相反，如果往z轴方向看，视口的范围应该是0-正无穷
	Matrix cameraV = camera->LookAt(camera->transform.position, Vector3f(0, 0, -1), Vector3f(0, 1, 0));
	//这里远近平面的值相对于物体变换到相机坐标系的位置，范围从相机位置-1到-120，此时物体的位置在-100左右，近平面越靠近-100，深度值越趋近于1，，相反越趋近于-1
	//Matrix cameraP = camera->Perspective(0.1, 0.1, -1, -120, 0.1, -0.1);
	//Matrix cameraP = camera->Perspective(90, 1, 1, 10);
	Matrix cameraP = camera->Orthographic(-10, 10, 0, -120, 10, -10);
	Matrix lightP = camera->Orthographic(-10, 10, 0, -120, 10, -10);
	DirectionLight light(Vector3f(0.2, 0.2, -1),Vector3f(0, 0, 100));
	PointLight poingt(Vector3f(5, 5, -5), 2);
	Texture *gezi=new Texture();
	gezi->LoadTexture("gezi.bmp");
	phongShader = new PhongShader(PhongVert(), PhongFrag(gezi, depthBuffer));
	phongShader->v2f.cameraPos = camera->transform.position;
	phongShader->v2f.dirlights.push_back(light);
	phongShader->v2f.pointlights.push_back(poingt);
	phongShader->v2f.v = cameraV;
	phongShader->v2f.p = cameraP;
	phongShader->v2f.lightV = light.LookAt(Vector3f(0, 1, 0));
	phongShader->v2f.lightP = lightP;
// 	phongShader->frag.baseTex = gezi;
// 	phongShader->frag.depthBuffer = depthBuffer;


	depthShader->v2f.lightV = light.LookAt(Vector3f(0, 1, 0));
	depthShader->v2f.lightP = lightP;

	planeShader = new PhongShader(PhongVert(), PhongFrag(gezi, depthBuffer));;
	planeShader->v2f.cameraPos = camera->transform.position;
	planeShader->v2f.dirlights.push_back(light);
	planeShader->v2f.pointlights.push_back(poingt);
	planeShader->v2f.v = cameraV;
	planeShader->v2f.p = lightP;
	planeShader->v2f.lightV =  light.LookAt(Vector3f(0, 1, 0));
	planeShader->v2f.lightP = lightP;
	//planeShader->frag.depthBuffer = depthBuffer;
	//device->InitDevice(w->screenHDC, windowWidth, windowHeight);
	//CreateRenderDevice();
	ReadObjFile("cube.obj",m);
	CreateCube();
	CreatePlane();
	
	//Matrix lightV = light.LookAt(Vector3f(0, 1, 0));
	//Matrix lightP = camera->Perspective(-1, 1, -100, 0, 1, -1);
	device->directionlights.push_back(light);
	device->pointlights.push_back(poingt);
	Update(w);
	//DoRender();
	system("pause");
	return 0;
}




void Update(Window *w)
{
	
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		UpdateInput(currentMesh);
	
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			DoRender(w);
			ShowFPS(w);
		}
	}
} 

void DoRender(Window *w)
{
	device->Clear(depthBuffer);
	device->Clear(zBuffer);
	//Vector3f v1(0.0f, 1.0f, 0.0f);
	//Vector3f v2(-1.0f, -1.0f, 0.0f);
	//Vector3f v3(1.0f, -1.0f, 0.0f);
	//Matrix mvp = device->GenMVPMatrix();
	//device->DrawTrangle3D(v1, v2, v3, mvp);
	//device->DrawMesh(plane);
	//phongShader->v2f.m->Print();
	device->DrawMesh(currentMesh, depthShader, depthBuffer);
	device->DrawMesh(plane, depthShader, depthBuffer);
	device->DrawMesh(currentMesh,phongShader,zBuffer);
	device->DrawMesh(plane, planeShader, zBuffer);

// 	for (int i = 0; i < 500; i++)
//  	{
//  		for (int j = 0; j < 500; j++)
//  		{
//  			cout << planeShader->frag.depthBuffer->depthBuffer[i][j] << " ";
//  		}
//  	}
//device->DrawLine(0, 0, 600, 450);
//	device->DrawLine(0, 0, 300, 200);
// 	for (int i = 0; i < 800; i++)
// 	{
// 		device->DrawLine(0, 0, 800, i);
// 	}
	//device->DrawTrangle(800, 200, 400, 795, 800, 400);
//	device->DrawTrangle(0, 100, 0, 200, 100, 200);
//	device->DrawTrangle(0, 100, 100, 100, 100, 200);
//	device->DrawTrangle(0, 100, 0, 200, 100, 200);
	//双缓冲
	BitBlt(w->hdc, 0, 0, windowWidth, windowHeight, w->screenHDC, 0, 0, SRCCOPY);
}


void ShowFPS(Window *w)
{
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;
	static float  lastTime = 0.0f;

	frameCount++;
	currentTime = timeGetTime()*0.001f;

	if (currentTime - lastTime > 1.0f) 
	{
		fps = (float)frameCount / (currentTime - lastTime);  
		lastTime = currentTime; 
		frameCount = 0;
	}

	char strBuffer[20];
	sprintf_s(strBuffer, 20, "%0.3f", fps);
	TextOut(w->hdc, 0, 0, strBuffer, 6);
}

void CreateCube()
{
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0,0,-1),Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Vector3f(0, 0, -1),Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1),Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1),Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, -1),Color(1, 0, 0, 1));
	//前
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	//左
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(-1, 0, 0));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(-1, 0, 0));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0));
	//右
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0),Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	//下
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	//上
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 1, 0));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 1, 0));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 1, 0));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 1, 0));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 1, 0));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 1, 0));
}
void CreatePlane()
{
	plane->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	plane->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	plane->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	plane->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	plane->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	plane->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));

	Matrix M;
	Matrix s = m->GetTransform().Scale(Vector3f(5, 5,5));
	Matrix r = m->GetTransform().Rotate(Vector3f(0,0,0));//Vector3f(-142,0,0.9)
	Matrix t = m->GetTransform().Translate(Vector3f(0, 0, -5));
	//注意矩阵乘法顺序！！
	M = t * r*s;
	planeShader->v2f.m = M;
	plane->SetObjectToWorld(M);
}
