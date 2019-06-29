#pragma once
#include "Headerz.h"



namespace Denjine{

const float MAX_FPS = 60.0f;
static	const int	NUM_SAMPLES = 10;

// Class designated to time and control our program's execution
class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	void calculateFPS();	// calculateFPS(); there's no init function because our FPS performs on a frame-by-frame basis
	void limitFPS();		// delay SDL to fit out max
	float getFPS() { return _FPS; }

private:
	float			_startFrameTicks;
	float			_FPS;
	float			_frameTime;
};
}
