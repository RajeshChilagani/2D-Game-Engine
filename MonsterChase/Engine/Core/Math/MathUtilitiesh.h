#pragma once
namespace Engine
{
	namespace Math
	{
		inline float DegreeToRadians(float i_Degree)
		{
			float pi = 3.14159265f;
			return static_cast<float>(i_Degree * pi / 180.0);
		}
	}
}
