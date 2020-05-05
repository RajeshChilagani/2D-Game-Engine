#pragma once
#include <cmath>
#include <assert.h>
#include <float.h>
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <string>
//Logging System
#if _DEBUG
#define TRACE_ERROR(s){std::stringstream os;os<<"\nError:";os<<s;os<<std::endl;OutputDebugStringA( os.str().c_str() );}
#define TRACE_INFO(s){std::stringstream os;os<<"\nINFO:";os<<s;os<<std::endl;OutputDebugStringA( os.str().c_str() );}
#else
#define TRACE_ERROR(s) NULL
#define TRACE_INFO(s) NULL
#endif // _DEBUG
namespace Engine
{
	namespace Helpers
	{
		inline bool IsNaN(float i_value)
		{
			volatile float val = i_value;
			return val != val;//val != val is true if val is a NaN
		}
		inline bool AreAboutEqual(float i_Lhs, float i_Rhs, float i_epsilon = FLT_EPSILON)
		{
			assert(!IsNaN(i_Lhs) && !IsNaN(i_Rhs) && !IsNaN(i_epsilon));
			return std::fabs(i_Rhs - i_Lhs) <= i_epsilon;
		}
		inline bool AreEqual_Rel(float i_Lhs, float i_Rhs, float i_Difference=FLT_EPSILON)
		{
			assert(!IsNaN(i_Lhs) && !IsNaN(i_Rhs) && !IsNaN(i_Difference));
			if (i_Lhs == i_Rhs)
				return true;
			float largest = i_Lhs > i_Rhs ? i_Lhs : i_Rhs;
			float relDiff=static_cast<float>(fabs(i_Lhs-i_Rhs)/largest);
			return relDiff <= i_Difference;
		}
		inline bool IsEqualToZero(float i_Value)
		{
			return AreAboutEqual(i_Value,0.0f);
		}
		inline void FloatingPtUnitTest()
		{
			float sum = 0.0f;
			for (int i = 0; i < 10; ++i)
			{
				sum += 0.1f;
			}
			if ((sum-1.0f) == 0.0f)
			{
				TRACE_INFO("IS Zero without helper");
			}
			else
			{
				if(IsEqualToZero((sum - 1.0f)))
					TRACE_INFO("Near Zero with helper");
			}
			if (sum == 1.0f)
			{
				TRACE_INFO("ISEqual without helper");
			}
			else
			{
				if(AreAboutEqual(sum,1.0f))
					TRACE_INFO("AreAboutEqual");
				if (AreEqual_Rel(sum, 1.0f))
					TRACE_INFO("AreEqaul_Rel");
			}
			float f1 = 67329.234f,f2= 67329.242f;
			if (AreAboutEqual(f1, f2))
			{
				TRACE_INFO("AreAboutEqual");
			}
			else if (AreEqual_Rel(f1, f2))
				TRACE_INFO("AreEqualRel");
		}

	}
	
}
