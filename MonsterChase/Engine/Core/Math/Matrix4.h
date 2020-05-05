#pragma once
#include "Vector4.h"
#include "Vector3.h"
namespace Engine
{
	namespace Math
	{
		class Matrix4
		{
		public:
			Matrix4() {}
			Matrix4(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4); //think and maybe change the implementation to columns
			void SetDiagonal(float i_Val = 1);
			void SetIdentity();
			void Invert();
			void Transpose();
			Vector4 Col(unsigned int i_ColIndex) const;
			Matrix4 GetInverse();
			Matrix4 GetTranspose();
			//operator
			Matrix4& operator+=(const Matrix4 & i_rhs);
			Matrix4& operator-=(const Matrix4 & i_rhs);
			Matrix4& operator*=(const Matrix4 & i_rhs);
			friend Matrix4 operator+(const Matrix4& i_Lhs, const Matrix4& i_Rhs);
			friend Matrix4 operator-(const Matrix4& i_Lhs, const Matrix4& i_Rhs);
			friend Matrix4 operator*(const Matrix4& i_Lhs, const Matrix4& i_Rhs);
			friend Vector4 operator*(const Matrix4& i_Lhs, const Vector4& i_Rhs);
			friend Vector4 operator*(const Vector4& i_Lhs, const Matrix4& i_Rhs);
			//static
			static Matrix4 CreateIdentity();
			static Matrix4 CreateXRotation(float i_RotationDegrees);
			static Matrix4 CreateYRotation(float i_RotationDegrees);
			static Matrix4 CreateZRotation(float i_RotationDegrees);
			static Matrix4 CreateTranslation(float i_TransX, float i_TransY, float i_TransZ);
			static Matrix4 CreateTranslation(const Vector3& i_Trans);
			static Matrix4 CreateScale(float i_ScaleX, float i_ScaleY, float i_ScaleZ);
		private:
			float M[4][4];
		};


	}
}
#include "Matrix4_inl.h"