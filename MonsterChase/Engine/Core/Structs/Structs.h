#pragma once
#include "Core/Math/Point2D.h"
namespace Engine
{
	struct Rect
	{
		unsigned int w, h;
		float x, y;
	};
	struct AABB
	{
		Math::Point2D Center;
		Math::Point2D Extents;
	};
}
