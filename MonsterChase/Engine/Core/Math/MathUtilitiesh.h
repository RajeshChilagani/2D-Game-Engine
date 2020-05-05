#pragma once
namespace Engine
{
	namespace Math
	{
		const float pi = 3.14159265f;
		inline float DegreeToRadians(float i_Degree)
		{
			return static_cast<float>(i_Degree * pi / 180.0);
		}
		inline float RadiansToDegree(float i_Rad)
		{
			return static_cast<float>(i_Rad*180/pi);
		}
	}
}
