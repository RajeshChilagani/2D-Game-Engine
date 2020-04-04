#pragma once
#include<iostream>
namespace Engine
{
	namespace Math
	{
		class Point2D
		{
		public:
			Point2D(void) {
				m_x = 0;
				m_y = 0;
			};
			Point2D(int i_x, int i_y);
			Point2D(float i_x, float i_y);
			Point2D(const Point2D& i_Point2D);
			//accessors
			float x(void) const;
			float y(void) const;
			void x(float i_x);
			void y(float i_y);

			// modifying operators
			Point2D & operator+=(const Point2D & i_rhs);

			Point2D & operator-=(const Point2D & i_rhs);

			Point2D & operator*=(const Point2D & i_rhs);
			Point2D & operator*=(int i_val);
			Point2D & operator*=(float i_val);

			Point2D & operator/=(const Point2D & i_rhs);
			Point2D & operator/=(int i_val);
			Point2D & operator/=(float i_val);

			Point2D& operator-(void);
			
			~Point2D();
			
			static const Point2D zero;
			friend inline std::ostream& operator<<(std::ostream& stream, const Point2D& vector)
			{
					stream << "(" << vector.x() << "," << vector.y() << ")";
					return stream;
			}
		private:
			float m_x, m_y;
			
		};
		
		Point2D operator+(const Point2D & i_lhs, const Point2D & i_rhs);

		Point2D operator-(const Point2D & i_lhs, const Point2D & i_rhs);
		
		Point2D operator*(const Point2D & i_lhs, const Point2D & i_rhs);
		Point2D operator*(const Point2D & i_lhs, int i_rhs);
		Point2D operator*(const Point2D & i_lhs, float i_rhs);

		Point2D operator/(const Point2D & i_lhs, const Point2D & i_rhs);
		Point2D operator/(const Point2D & i_lhs, int i_rhs);
		Point2D operator/(const Point2D & i_lhs, float i_rhs);

		bool operator==(const Point2D & i_lhs, const Point2D & i_rhs);
		bool operator!=(const Point2D & i_lhs, const Point2D & i_rhs);

		inline int dot(const Point2D & i_lhs, const Point2D & i_rhs);


	}//Math
}//Engine
#include "Point2D_inl.h"

