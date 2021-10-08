#ifndef TIMER_H
#define TIMER_H
#include"Core.h"

namespace CoreSystems
{

	class Timer
	{
	public:
		Timer();
		~Timer();

		void CalculateFPS();
		void Update();

		float GetTotalTime() const { return totalTime; }
		float GetDeltaTime() const { return deltaTime; }
		float GetFPS() const { return fps; }


		//Private Members
	private:
		float deltaTime;
		float secsPerCnts;
		_int64 prevCount;
		float totalTime;
		float fps;
		float totalFrames;
		float oneSecTimer;

	};
	extern Timer* timer;
};
#endif // !TIMER_H
