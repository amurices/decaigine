#include "Profiler.h"

namespace Denjine
{

Profiler::Profiler()
{
}


Profiler::~Profiler()
{
}


void Profiler::beginFrame()
{
	_startTime = SDL_GetTicks();
}

float Profiler::endFrame()
{
	return SDL_GetTicks() - _startTime;
}

void Profiler::beginTimingSubsystem()
{
	_ind = SDL_GetTicks();
}

float Profiler::endTimingSubsystem()
{
	return SDL_GetTicks() - _ind;
}
}