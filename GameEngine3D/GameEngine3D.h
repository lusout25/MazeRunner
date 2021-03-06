/***********************************************************
	Setup for the Game Engine
***********************************************************/
#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace GameEngine3D {

	int init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}