#ifndef _TIMER_H
#define _TIMER_H
#include <chrono>

class Timer {
public:
	Timer(){}
	~Timer(){}

	void start() {
		m_start = std::chrono::high_resolution_clock::now();
	}

	double getCurrentTime()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - m_start);
		return time_span.count();
	}

	double stop()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - m_start);
		m_start = end;
		return time_span.count();
	}

private:
	std::chrono::high_resolution_clock::time_point m_start;

};


#endif
