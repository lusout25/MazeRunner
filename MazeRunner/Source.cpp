#include <iostream>
#include <SDL\SDL.h>
#include <GL\glew.h>

int main(int, char**){
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Doh!");
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Quit();
	return 0;
}