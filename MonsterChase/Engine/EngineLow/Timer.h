#pragma once
#include <Windows.h>
namespace Engine
{
	struct PCFrequency
	{
		LARGE_INTEGER Frequency;
		bool PerformanceCounterExists;
		PCFrequency() :PerformanceCounterExists(QueryPerformanceFrequency(&Frequency)) {}
	};
	class Timer
	{
	public:
		Timer(float i_time = 0) :m_Time(i_time)
		{}
		inline static Timer CalculateFrameTime()
		{
			static long double LastFrameStartTicks=0;
			static Timer timer(13.3f*0.001f);
			static LARGE_INTEGER Ticks;
			static PCFrequency pcFrequency;
			QueryPerformanceCounter(&Ticks);
			if (LastFrameStartTicks)
			{
				timer = static_cast<float>((Ticks.QuadPart- LastFrameStartTicks) / pcFrequency.Frequency.QuadPart);
			}
			LastFrameStartTicks = static_cast<long double>(Ticks.QuadPart);
			return timer;
		}
		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliSeconds() const { return m_Time*1000; }
	private:
		float m_Time;
	};
}
