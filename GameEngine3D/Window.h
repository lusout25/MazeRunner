#pragma once
#include <string>
#include <SDL/SDL.h>
#include "Error.h"
#include <GL/glew.h>

using namespace std;

namespace GameEngine3D {

	class Window
	{
	public:
		Window();
		~Window();

		int create(string windowName, int screenWidth, int screenHeight);
		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }
		void swapBuffer();

	private:
		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;
	};

}