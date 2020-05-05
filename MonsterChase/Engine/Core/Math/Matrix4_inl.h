#pragma once
#include "Core/Helpers/Helpers.h"
namespace Engine
{
	namespace Math
	{
		inline void Matrix4::SetIdentity()
		{
			SetDiagonal();
		}
		inline Matrix4::Matrix4(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4) //think and maybe change the implementation to columns
		{
			M[0][0] = r1.x(); M[0][1] = r1.y();  M[0][2] = r1.z();  M[0][3] = r1.w();
			M[1][0] = r2.x(); M[1][1] = r2.y();  M[1][2] = r2.z();  M[1][3] = r2.w();
			M[2][0] = r3.x(); M[2][1] = r3.y();  M[2][2] = r3.z();  M[2][3] = r3.w();
			M[3][0] = r4.x(); M[3][1] = r4.y();  M[3][2] = r4.z();  M[3][3] = r4.w();
		}
		inline Matrix4& Matrix4::operator+=(const Matrix4 & i_Rhs)
		{
			*this = *this + i_Rhs;
			return *this;
		}
		inline Matrix4& Matrix4::operator-=(const Matrix4 & i_Rhs)
		{
			*this = *this - i_Rhs;
			return *this;
		}
		inline Matrix4& Matrix4::operator*=(const Matrix4& i_Rhs)
		{
			*this = *this*i_Rhs;
			return *this;
		}
		inline Vector4 Matrix4::Col(unsigned int i_ColIndex) const
		{
			assert(!Helpers::IsNaN(static_cast<float>(i_ColIndex)));
			switch(i_ColIndex)
			{
				case 0:
					return Vector4(M[0][0], M[1][0], M[2][0], M[3][0]);
				case 1:
					return Vector4(M[0][1], M[1][1], M[2][1], M[3][1]);
				case 2:
					return Vector4(M[0][2], M[1][2], M[2][2], M[3][2]);
				default:
					assert(false);
					return Vector4(NAN, NAN, NAN,NAN);
			}
		}
		//static
		inline Matrix4 Matrix4::CreateIdentity()
		{
			Matrix4 Temp;
			Temp.SetDiagonal();
			return Temp;
		}
	}
}
