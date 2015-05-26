#include "Camera2D.h"

namespace GameEngine3D
{

	Camera2D::Camera2D() : _position(1.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(1024),
		_screenHeight(768),
		_vao(0),
		_vbo(0)
	{
	}


	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::update()
	{
		if (_needsMatrixUpdate)
		{
			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			_needsMatrixUpdate = false;
		}
	}

	void Camera2D::draw()
	{

		float _points[] = { _screenWidth / 2 - 10, _screenHeight / 2, 0, _screenWidth / 2, _screenHeight / 2 + 10, 0, _screenWidth / 2 + 10, _screenHeight / 2, 0 };
		float _colors[] = { 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 };

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(_points) + sizeof(_colors), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_points), _points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_points), sizeof(_colors), _colors);

		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(_points)));

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}