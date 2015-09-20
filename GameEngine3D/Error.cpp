#include "Error.h"
#include <iostream>
#include <SDL/SDL.h>

namespace GameEngine3D {

	void fatalError(string errorString)
	{
		int tmp;

		cout << errorString << endl;
		cout << "Enter any key to quit...";
		cin >> tmp;

		SDL_Quit();
		exit(1);
	}

}