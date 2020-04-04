#pragma once
#include "Core/Helpers/Helpers.h"
namespace Engine
{
	namespace Math
	{
		inline Vector3::Vector3(float i_x, float i_y, float i_z) :m_x(i_x), m_y(i_y),m_z(i_z) {}
		inline Vector3::Vector3(int i_x, int i_y,int i_z) : m_x(static_cast<float>(i_x)), m_y(static_cast<float>(i_y)), m_z(static_cast<float>(i_z)) {}
		inline Vector3::Vector3(const Vector3& i_Vector3) : m_x(i_Vector3.x()), m_y(i_Vector3.y()), m_z(i_Vector3.z()) {}
		inline float Vector3::x(void) const
		{
			return m_x;
		}
		inline float Vector3::y(void) const
		{
			return m_y;
		}
		inline float Vector3::z(void) const
		{
			return m_z;
		}
		inline void Vector3::x(float i_x)
		{
			assert(!Helpers::IsNaN(i_x));
			m_x = i_x;
		}
		inline void Vector3::y(float i_y)
		{
			assert(!Helpers::IsNaN(i_y));
			m_y = i_y;
		}
		inline void Vector3::z(float i_z)
		{
			assert(!Helpers::IsNaN(i_z));
			m_z = i_z;
		}
		inline Vector3 & Vector3::operator+=(const Vector3 & i_rhs)
		{
			m_x += i_rhs.m_x;
			m_y += i_rhs.m_y;
			m_z += i_rhs.m_z;

			return *this;
		}
		inline Vector3 & Vector3::operator-=(const Vector3 & i_rhs)
		{
			m_x -= i_rhs.m_x;
			m_y -= i_rhs.m_y;
			m_z -= i_rhs.m_z;
			return *this;
		}
		inline Vector3 & Vector3::operator*=(const Vector3 & i_rhs)
		{
			m_x *= i_rhs.m_x;
			m_y *= i_rhs.m_y;
			m_z *= i_rhs.m_z;
			return *this;
		}
		inline Vector3 & Vector3:: operator*=(float i_val)
		{
			m_x *= i_val;
			m_y *= i_val;
			m_z *= i_val;
			return *this;
		}
		inline Vector3 & Vector3:: operator*=(int i_val)
		{
			m_x *= static_cast<float>(i_val);
			m_y *= static_cast<float>(i_val);
			m_z *= static_cast<float>(i_val);
			return *this;
		}
		//Test this 
		inline Vector3 & Vector3::operator/=(const Vector3 & i_rhs)
		{
			m_x /= i_rhs.m_x;
			m_y /= i_rhs.m_y;
			m_z /= i_rhs.m_z;
			return *this;
		}
		inline Vector3 & Vector3:: operator/=(float i_val)
		{
			m_x /= i_val;
			m_y /= i_val;
			m_z /= i_val;
			return *this;
		}
		inline Vector3 & Vector3:: operator/=(int i_val)
		{
			m_x /= static_cast<float>(i_val);
			m_y /= static_cast<float>(i_val);
			m_z /= static_cast<float>(i_val);
			return *this;
		}
		inline Vector3& Vector3::operator-(void)
		{
			m_x = -m_x;
			m_y = -m_y;
			m_z = -m_z;
			return *this;
		}
		//Standalone

		inline Vector3 Math::operator+(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return Vector3(i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y(), i_lhs.z() + i_rhs.z());
		}
		inline Vector3 Math::operator+(const Vector3 & i_lhs, float i_rhs)
		{
			return Vector3(i_lhs.x() + i_rhs, i_lhs.y() + i_rhs, i_lhs.z() + i_rhs);
		}
		//+
		inline Vector3 Math::operator-(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return Vector3(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y(), i_lhs.z() - i_rhs.z());
		}
		inline Vector3 Math::operator-(const Vector3 & i_lhs, float i_rhs)
		{
			return Vector3(i_lhs.x() - i_rhs, i_lhs.y() - i_rhs, i_lhs.z() - i_rhs);
		}
		//-
		inline Vector3 Math::operator*(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return Vector3(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y(), i_lhs.z() * i_rhs.z());
		}
		inline Vector3 Math::operator*(const Vector3 & i_lhs, float i_rhs)
		{
			return Vector3(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs, i_lhs.z() * i_rhs);
		}
		inline Vector3 Math::operator*(float i_lhs, const Vector3 & i_rhs)
		{
			return Vector3(i_lhs * i_rhs.x(), i_lhs * i_rhs.y(), i_lhs * i_rhs.z());
		}
		inline Vector3 Math::operator*(const Vector3 & i_lhs, int i_rhs)
		{
			return Vector3(static_cast<float>(i_lhs.x() * i_rhs), static_cast<float>(i_lhs.y() * i_rhs), static_cast<float>(i_lhs.z() * i_rhs));
		}
		//
		inline Vector3 Math::operator/(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return Vector3(i_lhs.x() / i_rhs.x(), i_lhs.y() / i_rhs.y(), i_lhs.z() / i_rhs.z());
		}
		inline Vector3 Math::operator/(const Vector3 & i_lhs, float i_rhs)
		{
			return Vector3(i_lhs.x() / i_rhs, i_lhs.y() / i_rhs, i_lhs.z() / i_rhs);
		}
		inline Vector3 Math::operator/(const Vector3 & i_lhs, int i_rhs)
		{
			return Vector3(static_cast<float>(i_lhs.x() / i_rhs), static_cast<float>(i_lhs.y() / i_rhs), static_cast<float>(i_lhs.z() / i_rhs));
		}
		inline bool Math::operator==(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return (i_lhs.x() == i_rhs.x() && i_lhs.y() == i_rhs.y() && i_lhs.z() == i_rhs.z());
		}
		inline bool Math::operator!=(const Vector3 & i_lhs, const Vector3 & i_rhs)
		{
			return !Math::operator==(i_lhs, i_rhs);
		}
	}
}
