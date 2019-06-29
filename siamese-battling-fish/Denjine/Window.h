#include "Headerz.h"
#include "Errors.h"
namespace Denjine{

	// Flags to set the window to the developer's liking
	enum WindowFlags{ INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();
		int init(std::string windowName, int windowWidth, int windowHeight, unsigned int flags);
		void swapBuffers();
		int getWidth()	{ return _width; }
		int getHeight()	{ return _height; }
	private:
		SDL_Window* _sdlWindowPtr;
		int _width, _height;
	};

}