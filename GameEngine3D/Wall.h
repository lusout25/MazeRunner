#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>

namespace GameEngine3D
{

	class Wall
	{
	
	static const int NUM_VERTICES = 36;
	
	public:
		Wall();
		~Wall();

		void placeCube(float x, float y, float z); //center coordinates
		float* getCollisionCoords() { return _collisionCorners; };
		void init();
		void draw();
		void render();

	private:
		float _points[3 * NUM_VERTICES];
		float _colors[4 * NUM_VERTICES];
		float _collisionCorners[4];

		GLuint _vbo;
		GLuint _vao;
	};

}