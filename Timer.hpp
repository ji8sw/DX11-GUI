#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point StartTime;
public:
	void Start()
	{
		StartTime = std::chrono::high_resolution_clock::now();
	}

	double GetElapsedMilliseconds() const
	{
		auto EndTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> Elapsed = EndTime - StartTime;
		return Elapsed.count();
	}
};