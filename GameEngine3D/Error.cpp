#include "Error.h"
#include <iostream>
#include <SDL/SDL.h>

namespace GameEngine3D {

	void fatalError(std::string errorString)
	{
		int tmp;

		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		std::cin >> tmp;

		SDL_Quit();
		exit(1);
	}

}