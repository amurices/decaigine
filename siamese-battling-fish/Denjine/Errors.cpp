#include "Errors.h"

namespace Denjine{

	// Prints out an error message and exits the game, shutting down SDL
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}
}