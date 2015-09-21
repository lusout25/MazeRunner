/***********************************************************
	3D model objects
***********************************************************/

#pragma once
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <GL\glew.h>
#include "Shared.h"

using namespace glm;
using namespace std;

namespace GameEngine3D
{

	class SimpleObjLoader
	{

	public:
		SimpleObjLoader();
		~SimpleObjLoader();

		bool loadObject(char* filePath);
		void render(void);

	private:
		vector< unsigned int > _vertexIndices, _uvIndices, _normalIndices;
		vector< vec3 > _vertices;
		vector< vec4 > _colors;
		vector< vec2 > _uvs;
		vector< vec3 > _normals;
		int _numFaces;

		GLuint _vbo;
	};

}