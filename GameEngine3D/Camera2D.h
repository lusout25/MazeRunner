/***********************************************************
	 Camera object for 2-D world space.
***********************************************************/

#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL\glew.h>
#include "Shared.h"

using namespace glm;

namespace GameEngine3D
{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();
		void draw();

		void setPosition(vec2& newPosition){ _position = newPosition; _needsMatrixUpdate = true; };

		mat4 getCameraMatrix() { return _cameraMatrix; };
		vec4 getColor() { return _color; };

	private:
		GLuint _vbo;

		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;

		vec2 _position;
		vec4 _color;
		mat4 _cameraMatrix;
		mat4 _orthoMatrix;

	};

}