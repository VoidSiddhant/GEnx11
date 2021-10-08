#include "Timer.h"

namespace CoreSystems
{
	extern Timer* timer = nullptr;
	Timer::Timer()
	{
		totalFrames = 0.0f;
		deltaTime = 0.0f;
		secsPerCnts = 0.0f;
		totalTime = 0.0f;
		oneSecTimer = 0.0f;
		fps = 0.0f;
		//Get Initial Counts per sec
		_int64 cntsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
		QueryPerformanceCounter((LARGE_INTEGER*)&prevCount);
		//Seconds Per Count
		secsPerCnts = 1.0f / cntsPerSec;
	}

	void Timer::Update()
	{
		//Check Time For till current Frame
		_int64 currCount;
		QueryPerformanceCounter((LARGE_INTEGER*)&currCount);
		//Get Time difference(deltaTime) between current frame and previous frame
		deltaTime = (currCount - prevCount)*secsPerCnts;
		//Directx SDK mentions that if the processor goes into a power saver mode or if processor switches to another....then deltaTime can go to negative values
		//If that is the case Set Value to 0
		if (deltaTime < 0.0f)
		{
			deltaTime = 0.0f;
		}
		//Set Counts for Next Frame
		prevCount = currCount;
		//Calculate Total Time Elapsed Since Engine Start
		totalTime += deltaTime;

	}

	void Timer::CalculateFPS()
	{
		totalFrames++;
		oneSecTimer += deltaTime;
		if (oneSecTimer >= 1.0f)
		{
			fps = totalFrames / oneSecTimer;
			oneSecTimer = 0.0f;
			totalFrames = 0.0f;
		}

	}

	Timer::~Timer()
	{

	}
};