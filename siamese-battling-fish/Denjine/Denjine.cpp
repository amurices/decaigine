#include "Headerz.h"
#include "Denjine.h"

namespace Denjine{

	int init()
	{
		// Set up SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Two buffers for a window
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}