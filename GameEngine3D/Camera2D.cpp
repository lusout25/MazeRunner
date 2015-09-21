#include "Camera2D.h"

namespace GameEngine3D
{

	Camera2D::Camera2D() : _position(1.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(1024),
		_screenHeight(768),
		_vbo(0),
		_color(vec4(.5, 1, .5, 1))
	{
		
	}

	Camera2D::~Camera2D()
	{
	}

	/***********************************************************
		Initialize camera to screen dimensions
	***********************************************************/
	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	/***********************************************************
		Update the camera properties
	***********************************************************/
	void Camera2D::update()
	{
		if (_needsMatrixUpdate)
		{
			vec3 camTranslation(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = translate(_orthoMatrix, camTranslation);

			_needsMatrixUpdate = false;
		}
	}

	/***********************************************************
		Render camera to screen
	***********************************************************/
	void Camera2D::draw()
	{

		float _points[] = { _screenWidth / 2 - 10, _screenHeight / 2, 0, _screenWidth / 2, _screenHeight / 2 + 10, 0, _screenWidth / 2 + 10, _screenHeight / 2, 0 }; //small triangle

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_points), _points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}