#pragma once
#include <GL\glew.h>

namespace GameEngine3D
{

	class Triangle
	{
	public:
		Triangle();
		~Triangle();

		void init();
		void draw();
		void render();

	private:
		GLuint _vbo;
		GLuint _vao;

		float _points[27];
		float _colors[36];
	};

}