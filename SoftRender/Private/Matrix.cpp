#include "Matrix.h"


Matrix::Matrix()
{
	Identity();
}

Matrix::~Matrix()
{
}

Matrix Matrix::operator + (const Matrix& right) const
{
	Matrix m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.value[i][j] = this->value[i][j] + right.value[i][j];
		}
	}
	return m;
}

Matrix Matrix::operator - (const Matrix& right) const
{
	Matrix m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.value[i][j] = this->value[i][j] - right.value[i][j];
		}
	}
	return m;
}

Matrix Matrix::operator * (const Matrix& right) const
{
	Matrix m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.value[i][j] = 0;//temp
			for (int k = 0; k < 4; k++)
			{
				m.value[i][j] += this->value[i][k] * right.value[k][j];
			}
		}
	}
	return m;
}

Matrix Matrix::operator * (float k) const
{
	Matrix m;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.value[i][j] = this->value[i][j] * k;
		}
	}
	return m;
}

Vector3f Matrix::MultiplyVector3(const Vector3f& v) const
{
	float x = v.x * value[0][0] + v.y * value[0][1] + v.z * value[0][2] + v.w * value[0][3];
	float y = v.x * value[1][0] + v.y * value[1][1] + v.z * value[1][2] + v.w * value[1][3];
	float z = v.x * value[2][0] + v.y * value[2][1] + v.z * value[2][2] + v.w * value[2][3];
	float w = v.x * value[3][0] + v.y * value[3][1] + v.z * value[3][2] + v.w * value[3][3];
	Vector3f returnValue(x, y, z);
	returnValue.w = w;
	return returnValue;
}



void Matrix::Identity()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				value[i][j] = 1;
			else
				value[i][j] = 0;
		}
	}
}


void Matrix::Print()
{
	std::cout << "-----------------Matrix Begin--------------" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << "[" << value[i][j] << "]   ";
		}
		std::cout << std::endl;
	}
	std::cout << "-----------------Matrix End----------------" << std::endl;
}


