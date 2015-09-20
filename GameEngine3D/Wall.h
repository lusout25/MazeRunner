#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>
#include "QuadTree.h"
#include "Shared.h"

using namespace std;

namespace GameEngine3D
{

	class Wall
	{
	
	public:
		Wall();
		~Wall();

		void placeCube(float x, float y, float z); //center coordinates
		
		AABB getCollisionBox() { return _collisionBox; };
		vector<float> getWallVertices() { return _pointData; };
		vector<float> getWallColors() { return _colorData; };
		
		void init();
		void draw();
		void render();

	private:
		vector<float> _pointData;
		vector<float> _colorData;

		float _normals[NUM_VERTICES_WALL];

		AABB _collisionBox;

		GLuint _vbo;
	};

}