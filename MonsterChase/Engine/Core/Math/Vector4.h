#pragma once
#include "Vector3.h"
#include<iostream>
#define MATH_API Engine::Math
namespace Engine
{
	namespace Math
	{
		class Vector4
		{
		public:
			Vector4(void) {
				m_x = 0;
				m_y = 0;
				m_z = 0;
				m_w = 0;
			};
			Vector4(int i_x, int i_y, int i_z,int i_w);
			Vector4(float i_x, float i_y, float i_z, float i_w);
			Vector4(Vector3& i_Vec, float i_w);
			Vector4(const Vector4& i_Vector4);
			//accessors
			float x(void) const;
			float y(void) const;
			float z(void) const;
			float w(void) const;
			void x(float i_x);
			void y(float i_y);
			void z(float i_z);
			void w(float i_w);

			// modifying operators
			Vector4 & operator+=(const Vector4 & i_rhs);

			Vector4 & operator-=(const Vector4 & i_rhs);

			Vector4 & operator*=(const Vector4 & i_rhs);
			Vector4 & operator*=(int i_val);
			Vector4 & operator*=(float i_val);

			Vector4 & operator/=(const Vector4 & i_rhs);
			Vector4 & operator/=(int i_val);
			Vector4 & operator/=(float i_val);

			Vector4& operator-(void);

			~Vector4();

			static const Vector4 zero;
			friend inline std::ostream& operator<<(std::ostream& stream, const Vector4& vector)
			{
				stream << "(" << vector.m_x << "," << vector.m_y << "," <<vector.m_z<<","<<vector.m_w<< ")";
				return stream;
			}
		private:
			float m_x, m_y, m_z,m_w;

		};

		Vector4 operator+(const Vector4 & i_lhs, const Vector4 & i_rhs);
		Vector4 operator+(const Vector4 & i_lhs, float i_rhs);

		Vector4 operator-(const Vector4 & i_lhs, const Vector4 & i_rhs);
		Vector4 operator-(const Vector4 & i_lhs, float i_rhs);

		Vector4 operator*(const Vector4 & i_lhs, const Vector4 & i_rhs);
		Vector4 operator*(const Vector4 & i_lhs, int i_rhs);
		Vector4 operator*(const Vector4 & i_lhs, float i_rhs);
		Vector4 operator*(float i_lhs, const Vector4 & i_rhs);

		Vector4 operator/(const Vector4 & i_lhs, const Vector4 & i_rhs);
		Vector4 operator/(const Vector4 & i_lhs, int i_rhs);
		Vector4 operator/(const Vector4 & i_lhs, float i_rhs);

		bool operator==(const Vector4 & i_lhs, const Vector4 & i_rhs);
		bool operator!=(const Vector4 & i_lhs, const Vector4 & i_rhs);

		inline int dot(const Vector4 & i_lhs, const Vector4 & i_rhs);


	}//Math
}//Engine
#include "Vector4_inl.h"