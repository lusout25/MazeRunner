#pragma once
#include <GL/glew.h>
#include "Texture.h"
#include "ImageLoader.h"
#include <glm\glm.hpp>
#include <string>
#include <vector>

using namespace glm;
using namespace std;

namespace GameEngine3D {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(vec3 bottomleft, vec3 topLeft, vec3 topRight, vec3 bottomRight, std::string texturePath, float xMultiple, float yMultiple);

		void draw();

	private:
		float _x;
		float _y;
		float _z;
		float _width;
		float _height;
		GLuint _vboID;
		Texture _texture;
		vector<float> _points;
		vector<float> _uv;
	};

}