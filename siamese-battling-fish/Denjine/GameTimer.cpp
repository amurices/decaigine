#include "GameTimer.h"
namespace Denjine{

	GameTimer::GameTimer()
	{

	}

	GameTimer::~GameTimer()
	{

	}


	void GameTimer::calculateFPS()
	{
		_startFrameTicks = SDL_GetTicks();
		static	float		frameTimes[NUM_SAMPLES];
		static	int			currentFrame = 0;
		static	float		previousTicks = _startFrameTicks;
		float		currentTicks = SDL_GetTicks();

		_frameTime = currentTicks - previousTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		previousTicks = currentTicks;

		int count;

		currentFrame++;

		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frameTimeAvg = 0.0f;
		for (int i = 0; i < count; i++)
		{
			frameTimeAvg += frameTimes[i];
		}

		frameTimeAvg /= count;

		if (frameTimeAvg > 0)
		{
			_FPS = 1000.0f / frameTimeAvg;
		}
		else
		{
			_FPS = 60.0f;
		}
	}


	void GameTimer::limitFPS()
	{
		float frameTicks = SDL_GetTicks() - _startFrameTicks;
		if ((1000.0f / MAX_FPS) > frameTicks)
		{
			SDL_Delay((1000.0f / MAX_FPS) - frameTicks);
		}
	}
}