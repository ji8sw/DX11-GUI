#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point m_StartTime;
public:
	void Start()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	double GetElapsedMilliseconds() const
	{
		auto EndTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> Elapsed = EndTime - m_StartTime;
		return Elapsed.count();
	}
};