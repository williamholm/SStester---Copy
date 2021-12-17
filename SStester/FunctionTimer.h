#pragma once
#include <chrono>
#include <functional>
#include <iostream>
namespace Testing
{

	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> mStartTime;
	public:

		Timer() = default;
		~Timer() = default;

		void startTimer() { mStartTime = std::chrono::steady_clock::now(); }

		void printTime(const std::string& message) noexcept
		{
			long long timeTaken = timeElapsed();
			std::cout << message.c_str() << timeTaken << " micro seconds";
		}
		//returns time since startTimer called in micro seconds.
		long long timeElapsed() noexcept
		{ return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - mStartTime).count(); }
	};

	template<typename Return, typename... Ts>
	uint64_t timeFunction(const std::function<Return(Ts...)> & func, Ts... params)
	{
		Timer timer;
		timer.startTimer();
		func(params...);
		return timer.timeElapsed();
	};
	//should return time taken for function called functionCalls times
	template<typename Return, typename... Ts>
	uint64_t timeFunction(int functionCalls, const std::function<Return(Ts...)> & func, Ts &&... params )
	{
		Timer timer;
		timer.startTimer();
		
		for (int i = 0; i < functionCalls; i++)
		{
			func(std::forward<Ts>(params)...);
		}
		return timer.timeElapsed();

	}
}