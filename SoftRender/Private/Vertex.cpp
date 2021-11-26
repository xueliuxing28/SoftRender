#include "Vertex.h"
#include "Vector3.h"
Vertex::Vertex(const Vector3f& p, const Color& c,  Vector2 uv)
	: pos(p), color(c), uv(uv)
{

}

Vertex::Vertex(const Vector3f & p, const Color & c, const Vector3f & normal, Vector2 uv) : pos(p), color(c), normal(normal),uv(uv)
{
}

Vertex::~Vertex()
{

}

void Vertex::LerpVertexData(Vertex& left, const Vertex& right, float t)
{
	pos.z = LerpFloat(left.pos.z, right.pos.z, t);
	color = Color::Lerp(left.color, right.color, t);
	uv.x = LerpFloat(left.uv.x, right.uv.x, t);
	uv.y = LerpFloat(left.uv.y, right.uv.y, t);
//	std::cout << u << " " << v << " " << t << std::endl;
	normal=left.normal+(right.normal-left.normal)* t;
}

void Vertex::Print()
{
	std::cout << "Vector3f: " << pos.x << " " << pos.y << " " << pos.z;
	std::cout << " Color: "  << color.r << " " << color.g << " " << color.b <<std::endl;
}

Vertex& Vertex::operator*(const Matrix & m) 
{
	pos.x = pos.x * m.value[0][0] + pos.y * m.value[0][1] + pos.z * m.value[0][2] + pos.w * m.value[0][3];
	pos.y = pos.x * m.value[1][0] + pos.y * m.value[1][1] + pos.z * m.value[1][2] + pos.w * m.value[1][3];
	pos.z = pos.x * m.value[2][0] + pos.y * m.value[2][1] + pos.z * m.value[2][2] + pos.w * m.value[2][3];
	pos.w = pos.x * m.value[3][0] + pos.y * m.value[3][1] + pos.z * m.value[3][2] + pos.w * m.value[3][3];

	//注意，只有旋转加平移的能直接使用MVP矩阵，变形需要计算矩阵的逆的转置
	normal.x = normal.x * m.value[0][0] + normal.y * m.value[0][1] + normal.z * m.value[0][2] + normal.w * m.value[0][3];
	normal.y = normal.x * m.value[1][0] + normal.y * m.value[1][1] + normal.z * m.value[1][2] + normal.w * m.value[1][3];
	normal.z = normal.x * m.value[2][0] + normal.y * m.value[2][1] + normal.z * m.value[2][2] + normal.w * m.value[2][3];
	normal.w = normal.x * m.value[3][0] + normal.y * m.value[3][1] + normal.z * m.value[3][2] + normal.w * m.value[3][3];
	
	return *this;
}
