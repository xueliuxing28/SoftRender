#include "Shader.h"
using namespace std;
void PhongShader::VS(Vertex & v0, Vertex & v1, Vertex & v2)
{
	v2f.v0 = v0;
	v2f.v1 = v1;
	v2f.v2 = v2;
	Vertex *v = &v0;
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			v = &v0;
			v2f.lp0 = v0.pos;
			v2f.lp0= v2f.m.MultiplyVector3(v2f.lp0);
			v2f.lp0= v2f.lightV.MultiplyVector3(v2f.lp0);
			v2f.lp0 = v2f.lightP.MultiplyVector3(v2f.lp0);
			break;
		case 1:
			v = &v1;
			v2f.lp1 = v1.pos;
			//v2f.lp1.Print();
			v2f.lp1 = v2f.m.MultiplyVector3(v2f.lp1);
			v2f.lp1 = v2f.lightV.MultiplyVector3(v2f.lp1);
			v2f.lp1 = v2f.lightP.MultiplyVector3(v2f.lp1);
			break;
		case 2:
			v = &v2;
			v2f.lp2 = v2.pos;
			v2f.lp2 = v2f.m.MultiplyVector3(v2f.lp2);
			v2f.lp2 = v2f.lightV.MultiplyVector3(v2f.lp2);
			v2f.lp2 = v2f.lightP.MultiplyVector3(v2f.lp2);

			break;
		}
	
//		Vector3f posLightSpace;
		//mvp变换
		v->pos = v2f.m.MultiplyVector3(v->pos);

		//std::cout << depth << std::endl;
		//posLightSpace.Print();
		//待优化

		Matrix nm = v2f.m;
		//去掉平移.不考虑变形缩放的情况下使用，正常情况下使用逆的转置
		nm.value[0][3] = 0;
		nm.value[1][3] = 0;
		nm.value[2][3] = 0;
		v->normal = nm.MultiplyVector3(v->normal).Normalize();
		//v->normal.Print();

		//可选着色

		float diffuse = 0, specular = 0;
		float ambient = 0.1;
		for (auto light : v2f.dirlights)
		{
			Vector3f l = light.GetDirection().Normalize();
			//l.Print();
			diffuse += max(0, l.Dot(v->normal))*light.intensity;

			Vector3f h = ((v2f.cameraPos - v->pos).Normalize() +l).Normalize();
			//h.Normalize();

			specular += pow(max(0, v->normal.Dot(h)), 1)*light.intensity;
			//cout << diffuse <<" "<< specular << endl;  Color::white+
			// v.color + specular + diffuse;
			//diffuse+ specular超过1会出现渲染错误
		}
		//点光源
// 		for (auto light : v2f.pointlights)
// 		{
// 			Vector3f l = (light.position-v->pos).Normalize();
// 			diffuse += max(0, l.Dot(v->normal))*light.intensity*min(1, 1 / (light.position - v->pos).Magnitude());
// 
// 			Vector3f h = ((v2f.cameraPos - v->pos).Normalize() + (light.position-v->pos).Normalize()).Normalize();
// 			//h.Normalize();
// 
// 			specular += pow(max(0, v->normal.Dot(h)), 1)*light.intensity*min(1, 1 / (light.position - v->pos).Magnitude());
// 			//float k = (specular + diffuse) *light.intensity*min(1, 1 / (light.position - v.pos).Magnitude());
// 			//cout << k << endl;
// 			//cout << diffuse <<" "<< specular << endl;  Color::white+
// 			// v.color + specular + diffuse;
// 				//diffuse+ specular超过1会出现渲染错误
// 			//v.color =v.color*(min(1, k));// Color(specular + diffuse, specular + diffuse, specular + diffuse, 1);
// 				//Color(diffuse+specular, diffuse + specular, diffuse + specular, 1);
// 			//cout << v.color.r << " "<<v.color.g << " " << v.color.b << " " << endl;
// 		}
		
		v->color = v->color*(min(1, specular + diffuse+ambient));// v.color *(min(1, specular + diffuse + ambient));
		//std::cout << min(1, specular + diffuse) << std:: endl;
		v->pos = v2f.v.MultiplyVector3(v->pos);
		//  	v2f.v.Print();
		
		v->pos = v2f.p.MultiplyVector3(v->pos);
		//	std::cout << v.pos.w << std::endl;
		v->pos.standardization();
		//v->pos.Print();
		//v->pos.Print();
		// 	v2f.p.Print();
		//std::cout << v.pos.w << std::endl;
	}
	//cout << endl;
	if (v1.pos.y < v0.pos.y)
	{
		std::swap(v2f.lp0, v2f.lp1);
		std::swap(v1, v0);
		//cout << 1;
	}

	if (v2.pos.y < v0.pos.y)
	{
		std::swap(v2f.lp0, v2f.lp2);
		std::swap(v2, v0);
//		std::cout << 2;
	}

	if (v2.pos.y < v1.pos.y)
	{
		std::swap(v2f.lp1, v2f.lp2);
		std::swap(v2, v1);
		//cout << 3;
	}
// 	v1.Print();
// 	v2.Print();
// 	v3.Print();
}

bool PhongShader::FS(Vertex & v, Vector3f g)
{
	//g.Print();
	v.color = v.color *frag.baseTex->Sample(v.uv.x, v.uv.y);
// 	Vector3f fragPos=v2f.v0.pos*g.x+v2f.v1.pos*g.y+v2f.v2.pos*g.z;
 	Vector3f normal = v2f.v0.normal*g.x + v2f.v1.normal*g.y + v2f.v2.normal*g.z;

	Vector3f posLightSpace = v2f.lp0*g.x + v2f.lp1*g.y + v2f.lp2*g.z;
// 	v2f.lp0.Print();
// 	v2f.lp1.Print();
// 	v2f.lp2.Print();
// 	g.Print();
// 	cout << endl;
	//posLightSpace.Print();

// 	v2f.lp0.Print();
// 	cout << " "<<endl;
//v.Print();
//	attrib.uv = vary.uv[0] * bc[0] + vary.uv[1] * bc[1] + vary.uv[2] * bc[2];
	float bias = 0.005;
	if (v2f.dirlights.size() > 0)
	{
	 bias = max(0.02 * (1.0 - abs(Vector3f::Dot(normal.Normalize(), v2f.dirlights[0].GetDirection().Normalize()))), 0.005);
	//cout << bias<<" ";
	}
	//float bias = max(0.05 * (1.0 - Vector3f::Dot(normal, v2f.lights[0].GetDirection())), 0.005);
	float depth = CalcuteShadow(posLightSpace, bias);
 //	v.pos.Print();
// 	cout << endl;
// 	float d=frag.depthBuffer->Sample(v.pos.y, v.pos.x);
	//if(depth)
	v.color = v.color*(1-depth);


	//std::cout <<depth<< std::endl;
	//v.Print();
// 	if (frag.depthBuffer->Sample(v.uv.x, v.uv.y) == 0)
// 		v.color = Color::white*0.1f;
 //   std::cout << v.pos.x << " " << v.pos.y << std::endl;
// 	v.color = //Color(1, 0, 0, 1);
// 		v.color*frag.depthBuffer->Sample(v.pos.y, v.pos.x);
	return v.color.a > 0;
}


//bias可以通过角度计算
float PhongShader::CalcuteShadow(Vector3f posLightSpace, double bias)
{
	float reciprocalW = 1.0f / posLightSpace.w;
	//加0.5做之后的四舍五入
	posLightSpace.x = (posLightSpace.x * reciprocalW + 1.0f) * 0.5f * (frag.depthBuffer->width - 1)+0.5;
	posLightSpace.y = (posLightSpace.y * reciprocalW+1.0f) * 0.5f * (frag.depthBuffer->height - 1)+0.5;

	float depth = (posLightSpace.z + 1.0) / 2.0;
	

	//PCF
 	float shadow = 0.0;
// 	for (int x = -10; x <= 10; ++x)
// 	{
// 		for (int y = -10; y <= 10; ++y)
// 		{
// 			float pcfDepth = frag.depthBuffer->Sample(posLightSpace.y+y, posLightSpace.x+x);
// 			shadow += depth - bias > pcfDepth ? 1.0 : 0.0;
// 		}
// 	}
// 	shadow /= 441;
// 	return shadow;

	//普通阴影
	float closestDepth = frag.depthBuffer->Sample(posLightSpace.y, posLightSpace.x);
	shadow = depth - bias > closestDepth ? 1 : 0;


	//	std::cout <<depth <<" "<<closestDepth<< std::endl;
	//std::cout << posLightSpace.y << " " << posLightSpace.x << std::endl;
// 	if (shadow)
// 	{
// 		std::cout << depth << " " << closestDepth << std::endl;
// 		posLightSpace.Print();
// 	}
     return shadow;
}

void ShadowShader::VS(Vertex & v0, Vertex & v1, Vertex & v2)
{
	Vertex *v=&v1;
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			v = &v0;
			break;
		case 1:
			v = &v1;
			break;
		case 2:
			v = &v2;
			break;
		}
		//MVP矩阵
		v->pos = v2f.m.MultiplyVector3(v->pos);
		v->pos = v2f.lightV.MultiplyVector3(v->pos);
		// 	v2f.v.Print();
		// 	v.pos.Print();
		v->pos = v2f.lightP.MultiplyVector3(v->pos);
		v->pos.standardization();
		//v.pos.z = (v.pos.z + 1.0f) / 2.0f;
//		v->pos.Print();
	}
	if (v1.pos.y < v0.pos.y)
	{
		std::swap(v1, v0);
		//cout << 1;
	}

	if (v2.pos.y < v0.pos.y)
	{
		std::swap(v2, v0);
		//		std::cout << 2;
	}

	if (v2.pos.y < v1.pos.y)
	{
		std::swap(v2, v1);
		//cout << 3;
	}
}

bool ShadowShader::FS(Vertex & v, Vector3f g)
{
	//啥都不做
//	v.pos.Print();
	return false;
}