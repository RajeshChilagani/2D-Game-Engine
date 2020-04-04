#pragma once
#include "Core/Helpers/Helpers.h"
namespace Engine
{
	namespace Math
	{
		inline Vector4::Vector4(float i_x, float i_y, float i_z, float i_w) :m_x(i_x), m_y(i_y), m_z(i_z),m_w(i_w) {}
		inline Vector4::Vector4(int i_x, int i_y, int i_z, int i_w) : m_x(static_cast<float>(i_x)), m_y(static_cast<float>(i_y)), m_z(static_cast<float>(i_z)),m_w(static_cast<float>(i_w)) {}
		inline Vector4::Vector4(Vector3& i_Vec, float i_w) : m_x(i_Vec.x()), m_y(i_Vec.y()),m_z(i_Vec.z()),m_w(i_w){}
		inline Vector4::Vector4(const Vector4& i_Vector4) : m_x(i_Vector4.m_x), m_y(i_Vector4.m_y), m_z(i_Vector4.m_z), m_w(i_Vector4.m_w) {}
		inline float Vector4::x(void) const
		{
			return m_x;
		}
		inline float Vector4::y(void) const
		{
			return m_y;
		}
		inline float Vector4::z(void) const
		{
			return m_z;
		}
		inline float Vector4::w(void) const
		{
			return m_w;
		}
		inline void Vector4::x(float i_x)
		{
			assert(!Helpers::IsNaN(i_x));
			m_x = i_x;
		}
		inline void Vector4::y(float i_y)
		{
			assert(!Helpers::IsNaN(i_y));
			m_y = i_y;
		}
		inline void Vector4::z(float i_z)
		{
			assert(!Helpers::IsNaN(i_z));
			m_z = i_z;
		}
		inline void Vector4::w(float i_w)
		{
			assert(!Helpers::IsNaN(i_w));
			m_w = i_w;
		}
		inline Vector4 & Vector4::operator+=(const Vector4 & i_rhs)
		{
			m_x += i_rhs.m_x;
			m_y += i_rhs.m_y;
			m_z += i_rhs.m_z;
			m_w += i_rhs.m_w;

			return *this;
		}
		inline Vector4 & Vector4::operator-=(const Vector4 & i_rhs)
		{
			m_x -= i_rhs.m_x;
			m_y -= i_rhs.m_y;
			m_z -= i_rhs.m_z;
			m_w -= i_rhs.m_w;
			return *this;
		}
		inline Vector4 & Vector4::operator*=(const Vector4 & i_rhs)
		{
			m_x *= i_rhs.m_x;
			m_y *= i_rhs.m_y;
			m_z *= i_rhs.m_z;
			m_w *= i_rhs.m_w;
			return *this;
		}
		inline Vector4 & Vector4:: operator*=(float i_val)
		{
			m_x *= i_val;
			m_y *= i_val;
			m_z *= i_val;
			m_w *= i_val;
			return *this;
		}
		inline Vector4 & Vector4:: operator*=(int i_val)
		{
			m_x *= static_cast<float>(i_val);
			m_y *= static_cast<float>(i_val);
			m_z *= static_cast<float>(i_val);
			m_w *= static_cast<float>(i_val);
			return *this;
		}
		//Test this 
		inline Vector4 & Vector4::operator/=(const Vector4 & i_rhs)
		{
			m_x /= i_rhs.m_x;
			m_y /= i_rhs.m_y;
			m_z /= i_rhs.m_z;
			m_w /= i_rhs.m_w;
			return *this;
		}
		inline Vector4 & Vector4:: operator/=(float i_val)
		{
			m_x /= i_val;
			m_y /= i_val;
			m_z /= i_val;
			m_w /= i_val;
			return *this;
		}
		inline Vector4 & Vector4:: operator/=(int i_val)
		{
			m_x /= static_cast<float>(i_val);
			m_y /= static_cast<float>(i_val);
			m_z /= static_cast<float>(i_val);
			m_w /= static_cast<float>(i_val);
			return *this;
		}
		inline Vector4& Vector4::operator-(void)
		{
			m_x = -m_x;
			m_y = -m_y;
			m_z = -m_z;
			m_w = -m_w;
			return *this;
		}
		//Standalone

		inline Vector4 Math::operator+(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return Vector4(i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y(), i_lhs.z() + i_rhs.z(), i_lhs.w() + i_rhs.w());
		}
		inline Vector4 Math::operator+(const Vector4 & i_lhs, float i_rhs)
		{
			return Vector4(i_lhs.x() + i_rhs, i_lhs.y() + i_rhs, i_lhs.z() + i_rhs, i_lhs.w() + i_rhs);
		}
		//+
		inline Vector4 Math::operator-(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return Vector4(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y(), i_lhs.z() - i_rhs.z(), i_lhs.w() - i_rhs.w());
		}
		inline Vector4 Math::operator-(const Vector4 & i_lhs, float i_rhs)
		{
			return Vector4(i_lhs.x() - i_rhs, i_lhs.y() - i_rhs, i_lhs.z() - i_rhs, i_lhs.w() - i_rhs);
		}
		//-
		inline Vector4 Math::operator*(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return Vector4(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y(), i_lhs.z() * i_rhs.z(), i_lhs.w() * i_rhs.w());
		}
		inline Vector4 Math::operator*(const Vector4 & i_lhs, float i_rhs)
		{
			return Vector4(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs, i_lhs.z() * i_rhs, i_lhs.w() * i_rhs);
		}
		inline Vector4 Math::operator*(float i_lhs, const Vector4 & i_rhs)
		{
			return Vector4(i_lhs * i_rhs.x(), i_lhs * i_rhs.y(), i_lhs * i_rhs.z(), i_lhs * i_rhs.w());
		}
		inline Vector4 Math::operator*(const Vector4 & i_lhs, int i_rhs)
		{
			return Vector4(static_cast<float>(i_lhs.x() * i_rhs), static_cast<float>(i_lhs.y() * i_rhs), static_cast<float>(i_lhs.z() * i_rhs), static_cast<float>(i_lhs.w() * i_rhs));
		}
		//
		inline Vector4 Math::operator/(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return Vector4(i_lhs.x() / i_rhs.x(), i_lhs.y() / i_rhs.y(), i_lhs.z() / i_rhs.z(), i_lhs.w() / i_rhs.w());
		}
		inline Vector4 Math::operator/(const Vector4 & i_lhs, float i_rhs)
		{
			return Vector4(i_lhs.x() / i_rhs, i_lhs.y() / i_rhs, i_lhs.z() / i_rhs, i_lhs.w() / i_rhs);
		}
		inline Vector4 Math::operator/(const Vector4 & i_lhs, int i_rhs)
		{
			return Vector4(static_cast<float>(i_lhs.x() / i_rhs), static_cast<float>(i_lhs.y() / i_rhs), static_cast<float>(i_lhs.z() / i_rhs), static_cast<float>(i_lhs.w() / i_rhs));
		}
		inline bool Math::operator==(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return (i_lhs.x() == i_rhs.x() && i_lhs.y() == i_rhs.y() && i_lhs.z() == i_rhs.z() && i_lhs.w() == i_rhs.w());
		}
		inline bool Math::operator!=(const Vector4 & i_lhs, const Vector4 & i_rhs)
		{
			return !Math::operator==(i_lhs, i_rhs);
		}
	}
}
