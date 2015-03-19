#pragma once

#include <string>
#include <SDL/SDL.h>

namespace GameEngine3D {

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight);
		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }
		void swapBuffer();

	private:
		SDL_Window* _window;
		int _screenWidth;
		int _screenHeight;
	};

}