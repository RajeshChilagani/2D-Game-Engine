#pragma once
#include<iostream>
#define MATH_API Engine::Math
namespace Engine
{
	namespace Math
	{
		class Vector3
		{
		public:
			Vector3(void) {
				m_x = 0;
				m_y = 0;
				m_z = 0;
			};
			Vector3(int i_x, int i_y,int i_z);
			Vector3(float i_x, float i_y,float i_z);
			Vector3(const Vector3& i_Vector3);
			//accessors
			float x(void) const;
			float y(void) const;
			float z(void) const;
			void x(float i_x);
			void y(float i_y);
			void z(float i_z);

			// modifying operators
			Vector3 & operator+=(const Vector3 & i_rhs);

			Vector3 & operator-=(const Vector3 & i_rhs);

			Vector3 & operator*=(const Vector3 & i_rhs);
			Vector3 & operator*=(int i_val);
			Vector3 & operator*=(float i_val);

			Vector3 & operator/=(const Vector3 & i_rhs);
			Vector3 & operator/=(int i_val);
			Vector3 & operator/=(float i_val);

			Vector3& operator-(void);

			~Vector3();

			float Length() const;
			float LengthSq() const;

			void Normalize();
			Vector3 GetNormalized() const;
			static const Vector3 zero;
			friend inline std::ostream& operator<<(std::ostream& stream, const Vector3& vector)
			{
				stream << "(" << vector.x() << "," << vector.y() << ")";
				return stream;
			}
		private:
			float m_x, m_y,m_z;

		};

		Vector3 operator+(const Vector3 & i_lhs, const Vector3 & i_rhs);
		Vector3 operator+(const Vector3 & i_lhs, float i_rhs);

		Vector3 operator-(const Vector3 & i_lhs, const Vector3 & i_rhs);
		Vector3 operator-(const Vector3 & i_lhs, float i_rhs);

		Vector3 operator*(const Vector3 & i_lhs, const Vector3 & i_rhs);
		Vector3 operator*(const Vector3 & i_lhs, int i_rhs);
		Vector3 operator*(const Vector3 & i_lhs, float i_rhs);
		Vector3 operator*(float i_lhs, const Vector3 & i_rhs);

		Vector3 operator/(const Vector3 & i_lhs, const Vector3 & i_rhs);
		Vector3 operator/(const Vector3 & i_lhs, int i_rhs);
		Vector3 operator/(const Vector3 & i_lhs, float i_rhs);

		bool operator==(const Vector3 & i_lhs, const Vector3 & i_rhs);
		bool operator!=(const Vector3 & i_lhs, const Vector3 & i_rhs);

		inline float dot(const Vector3 & i_lhs, const Vector3 & i_rhs);


	}//Math
}//Engine
#include "Vector3_inl.h"