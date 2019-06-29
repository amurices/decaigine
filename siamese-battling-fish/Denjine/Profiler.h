#pragma once
#include "Headerz.h"

namespace Denjine
{

class Profiler
{
public:
	Profiler();
	~Profiler();

	void beginFrame();	// start timing the frame
	float endFrame();	// return the total frame time

	void beginTimingSubsystem();	// start timing the subsystem
	float endTimingSubsystem();		// return the total subsystem time, place it appropriately

	// One float for the time of each subsystem that runs in real time
	float _startTime;
	float _inputTime;
	float _timerTime;
	float _cameraTime;
	float _graphicsTime;
	float _physicsTime;

	// one float to time individual subsystems
	float _ind;
};

}