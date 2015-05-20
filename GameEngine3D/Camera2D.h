#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL\glew.h>

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

		void setPosition(glm::vec2& newPosition){ _position = newPosition; _needsMatrixUpdate = true; };

		glm::mat4 getCameraMatrix() { return _cameraMatrix; };

	private:
		GLuint _vbo;
		GLuint _vao;

		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;

		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};

}