#include "Matrix4.h"
#include "MathUtilitiesh.h"
#include <math.h>
namespace Engine
{
	namespace Math
	{
		void Matrix4::SetDiagonal(float i_Val)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					M[i][j] = i == j ? i_Val : 0;
				}
			}
		}
		void Matrix4::Transpose()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					M[i][j] = M[j][i];
				}
			}
		}
		Matrix4 Matrix4::GetTranspose()
		{
			Matrix4 transpose;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					transpose.M[i][j] = M[j][i];
				}
			}
			return transpose;
		}
		void Matrix4::Invert()
		{
			Matrix4 Temp = *this;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					M[i][j] = Temp.M[j][i];
				}
			}
			M[0][3] = -M[0][0] * Temp.M[0][3] + -M[0][1] * Temp.M[1][3] + -M[0][2] * Temp.M[2][3];
			M[1][3] = -M[1][0] * Temp.M[0][3] + -M[1][1] * Temp.M[1][3] + -M[1][2] * Temp.M[2][3];
			M[2][3] = -M[2][0] * Temp.M[0][3] + -M[2][1] * Temp.M[1][3] + -M[2][2] * Temp.M[2][3];
		}
		Matrix4 Matrix4::GetInverse()
		{
			Matrix4 inverse;
			inverse = *this;
			inverse.Invert();
			return inverse;
		}
		//Static
		Matrix4 Matrix4::CreateXRotation(float i_RotationDegrees)
		{
			Matrix4 rotationX;
			rotationX.SetIdentity();
			rotationX.M[1][1] = cos(DegreeToRadians(i_RotationDegrees));
			rotationX.M[1][2] = -sin(DegreeToRadians(i_RotationDegrees));
			rotationX.M[2][1] = sin(DegreeToRadians(i_RotationDegrees));
			rotationX.M[2][2] = cos(DegreeToRadians(i_RotationDegrees));
			return rotationX;
		}
		Matrix4 Matrix4::CreateYRotation(float i_RotationDegrees)
		{
			Matrix4 rotationY;
			rotationY.SetIdentity();
			rotationY.M[0][0] = cos(DegreeToRadians(i_RotationDegrees));
			rotationY.M[0][2] = -sin(DegreeToRadians(i_RotationDegrees));
			rotationY.M[2][0] = sin(DegreeToRadians(i_RotationDegrees));
			rotationY.M[2][2] = cos(DegreeToRadians(i_RotationDegrees));
			return rotationY;
		}
		Matrix4 Matrix4::CreateZRotation(float i_RotationDegrees)
		{
			Matrix4 rotationZ;
			rotationZ.SetIdentity();
			rotationZ.M[0][0] = cosf(DegreeToRadians(i_RotationDegrees));
			rotationZ.M[0][1] = -sinf(DegreeToRadians(i_RotationDegrees));
			rotationZ.M[1][0] = sinf(DegreeToRadians(i_RotationDegrees));
			rotationZ.M[1][1] = cosf(DegreeToRadians(i_RotationDegrees));
			return rotationZ;
		}
		Matrix4 Matrix4::CreateTranslation(float i_TransX, float i_TransY, float i_TransZ)
		{
			Matrix4 traslationMatrix;
			traslationMatrix.SetIdentity();
			traslationMatrix.M[0][3] = i_TransX;
			traslationMatrix.M[1][3] = i_TransY;
			traslationMatrix.M[2][3] = i_TransZ;
			return traslationMatrix;
		}
		Matrix4 Matrix4::CreateTranslation(const Vector3& i_Translation)
		{
			return CreateTranslation(i_Translation.x(), i_Translation.y(), i_Translation.z());
		}
		Matrix4 Matrix4::CreateScale(float i_ScaleX, float i_ScaleY, float i_ScaleZ)
		{
			Matrix4 scaleMatrix;
			scaleMatrix.SetIdentity();
			scaleMatrix.M[0][0] = i_ScaleX;
			scaleMatrix.M[1][1] = i_ScaleY;
			scaleMatrix.M[2][2] = i_ScaleZ;
			return scaleMatrix;
		}
		//friends
		Matrix4 operator+(const Matrix4& i_Lhs, const Matrix4& i_Rhs)
		{
			Matrix4 result;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.M[i][j] = i_Lhs.M[i][j] + i_Rhs.M[i][j];
				}
			}
			return result;
		}
		Matrix4 operator-(const Matrix4& i_Lhs, const Matrix4& i_Rhs)
		{
			Matrix4 result;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.M[i][j] = i_Lhs.M[i][j] - i_Rhs.M[i][j];
				}
			}
			return result;
		}
		Matrix4 operator*(const Matrix4& i_Lhs, const Matrix4& i_Rhs)
		{
			Matrix4 result;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					float sum = 0;
					for (int k = 0; k < 4; k++)
					{
						sum += i_Lhs.M[i][k] * i_Rhs.M[k][j];
					}
					result.M[i][j] = sum;
				}
			}
			return result;
		}
		Vector4 operator*(const Matrix4& i_Lhs, const Vector4& i_Rhs)
		{
			float x = i_Lhs.M[0][0] * i_Rhs.x() + i_Lhs.M[0][1] * i_Rhs.y() + i_Lhs.M[0][2] * i_Rhs.z() + i_Lhs.M[0][3] * i_Rhs.w();
			float y = i_Lhs.M[1][0] * i_Rhs.x() + i_Lhs.M[1][1] * i_Rhs.y() + i_Lhs.M[1][2] * i_Rhs.z() + i_Lhs.M[1][3] * i_Rhs.w();
			float z = i_Lhs.M[2][0] * i_Rhs.x() + i_Lhs.M[2][1] * i_Rhs.y() + i_Lhs.M[2][2] * i_Rhs.z() + i_Lhs.M[2][3] * i_Rhs.w();
			float w = i_Lhs.M[3][0] * i_Rhs.x() + i_Lhs.M[3][1] * i_Rhs.y() + i_Lhs.M[3][2] * i_Rhs.z() + i_Lhs.M[3][3] * i_Rhs.w();
			return Vector4(x, y, z, w);
		}
		Vector4 operator*(const Vector4& i_Lhs, const Matrix4& i_Rhs)
		{
			float x = i_Lhs.x() * i_Rhs.M[0][0] + i_Lhs.y() * i_Rhs.M[1][0] + i_Lhs.z() * i_Rhs.M[2][0] + i_Lhs.w() * i_Rhs.M[3][0];
			float y = i_Lhs.x() * i_Rhs.M[0][1] + i_Lhs.y() * i_Rhs.M[1][1] + i_Lhs.z() * i_Rhs.M[2][1] + i_Lhs.w() * i_Rhs.M[3][1];
			float z = i_Lhs.x() * i_Rhs.M[0][2] + i_Lhs.y() * i_Rhs.M[1][2] + i_Lhs.z() * i_Rhs.M[2][2] + i_Lhs.w() * i_Rhs.M[3][2];
			float w = i_Lhs.x() * i_Rhs.M[0][3] + i_Lhs.y() * i_Rhs.M[1][3] + i_Lhs.z() * i_Rhs.M[2][3] + i_Lhs.w() * i_Rhs.M[3][3];
			return Vector4(x, y, z, w);
		}
	}
}