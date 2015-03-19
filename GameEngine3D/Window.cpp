#include "Window.h"
#include "Error.h"
#include <GL/glew.h>

namespace GameEngine3D
{
	Window::Window()
	{

	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight)
	{


		_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (_window == nullptr)
		{
			fatalError("Failed to create a window.");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_window);
		if (glContext == nullptr)
		{
			fatalError("Failed to create opengl context.");
		}

		GLenum error = glewInit();
		if (GLEW_OK != error)
		{
			fatalError("Glew initialization failed.");
		}

		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		
		//Vertical synch
		SDL_GL_SetSwapInterval(0);

		//enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_window);
	}

}