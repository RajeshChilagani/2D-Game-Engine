#pragma once
using namespace Engine;
using namespace Math;
inline Point2D::Point2D(float i_x, float i_y):m_x(i_x),m_y(i_y){}
inline Point2D::Point2D(int i_x, int i_y) : m_x(static_cast<float>(i_x)), m_y(static_cast<float>(i_y)) {}
inline Point2D::Point2D(const Point2D& i_Point2D) : m_x(i_Point2D.x()), m_y(i_Point2D.y()) {}
inline float Point2D::x(void) const
{
	return m_x;
}
inline float Point2D::y(void) const
{
	return m_y;
}
inline void Point2D::x(float i_x)
{
	m_x = i_x;
}
inline void Point2D::y(float i_y)
{
	m_y = i_y;
}
inline Point2D & Point2D::operator+=(const Point2D & i_rhs)
{
	m_x += i_rhs.m_x;
	m_y += i_rhs.m_y;

	return *this;
}
inline Point2D & Point2D::operator-=(const Point2D & i_rhs)
{
	m_x -= i_rhs.m_x;
	m_y -= i_rhs.m_y;

	return *this;
}
inline Point2D & Point2D::operator*=(const Point2D & i_rhs)
{
	m_x *= i_rhs.m_x;
	m_y *= i_rhs.m_y;

	return *this;
}
inline Point2D & Point2D:: operator*=(float i_val)
{
	m_x *= i_val;
	m_y *= i_val;
	return *this;
}
inline Point2D & Point2D:: operator*=(int i_val)
{
	m_x *= static_cast<float>(i_val);
	m_y *= static_cast<float>(i_val);
	return *this;
}
//Test this 
inline Point2D & Point2D::operator/=(const Point2D & i_rhs)
{
	m_x /= i_rhs.m_x;
	m_y /= i_rhs.m_y;

	return *this;
}
inline Point2D & Point2D:: operator/=(float i_val)
{
	m_x /= i_val;
	m_y /= i_val;
	return *this;
}
inline Point2D & Point2D:: operator/=(int i_val)
{
	m_x /= static_cast<float>(i_val);
	m_y /= static_cast<float>(i_val);
	return *this;
}
inline Point2D& Point2D::operator-(void)
{
	m_x = -m_x;
	m_y = -m_y;
	return *this;
}
//Standalone

inline Point2D Math::operator+(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return Point2D(i_lhs.x()+i_rhs.x(),i_lhs.y()+i_rhs.y());
}
//
inline Point2D Math::operator-(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return Point2D(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y());
}
//
inline Point2D Math::operator*(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return Point2D(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y());
}
inline Point2D Math::operator*(const Point2D & i_lhs, float i_rhs)
{
	return Point2D(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs);
}
inline Point2D Math::operator*(const Point2D & i_lhs, int i_rhs)
{
	return Point2D(static_cast<float>(i_lhs.x() * i_rhs), static_cast<float>(i_lhs.y() * i_rhs));
}
//
inline Point2D Math::operator/(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return Point2D(i_lhs.x() / i_rhs.x(), i_lhs.y() / i_rhs.y());
}
inline Point2D Math::operator/(const Point2D & i_lhs, float i_rhs)
{
	return Point2D(i_lhs.x() / i_rhs, i_lhs.y() / i_rhs);
}
inline Point2D Math::operator/(const Point2D & i_lhs, int i_rhs)
{
	return Point2D(static_cast<float>(i_lhs.x() / i_rhs), static_cast<float>(i_lhs.y() / i_rhs));
}
inline bool Math::operator==(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return (i_lhs.x()==i_rhs.x()&&i_lhs.y()==i_rhs.y());
}
inline bool Math::operator!=(const Point2D & i_lhs, const Point2D & i_rhs)
{
	return !Math::operator==(i_lhs, i_rhs);
}



