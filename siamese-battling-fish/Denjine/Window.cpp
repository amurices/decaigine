#include "Window.h"


namespace Denjine{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}


	int Window::init(std::string windowName, int windowWidth, int windowHeight, unsigned int flags)
	{
		_height = windowHeight;
		_width = windowWidth;

		Uint32 insideflags = SDL_WINDOW_OPENGL;

		if (flags & INVISIBLE)
		{
			insideflags |= SDL_WINDOW_HIDDEN;
		}

		if (flags & BORDERLESS)
		{
			insideflags |= SDL_WINDOW_BORDERLESS;
		}

		if (flags & FULLSCREEN)
		{
			insideflags |= SDL_WINDOW_FULLSCREEN;
		}

		// Create an OpenGL window
		_sdlWindowPtr = SDL_CreateWindow(windowName.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			windowWidth, windowHeight, insideflags);
		if (_sdlWindowPtr == nullptr)
		{
			fatalError("Window is still nullptr; could not create it");
		}

		// Create an OpenGL context for SDL
		SDL_GLContext GLcontext = SDL_GL_CreateContext(_sdlWindowPtr);
		if (GLcontext == nullptr)
		{
			fatalError("GLcontext is nullptr; could not create it");
		}

		// Set up Glew
		GLenum error = glewInit();
		if (error != GLEW_OK)
		{
			fatalError("Glew was not initialized");
		}

		// Clear screen to dark blue
		glClearColor(0.0f, 0.0f, 0.15f, 1.0f);

		// Turn on VSYNC
		SDL_GL_SetSwapInterval(1);

		// Enable OpenGL blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}


	void Window::swapBuffers()
	{
		SDL_GL_SwapWindow(_sdlWindowPtr);
	}
}