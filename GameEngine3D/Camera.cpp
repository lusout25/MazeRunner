#pragma once
#include "Camera.h"
#include "GL\glew.h"
#include <glm\gtx\transform.hpp>

namespace GameEngine3D {

	Camera::Camera() : _needsMatrixUpdate(true), 
		_cameraPosition(vec3(0, .25, 0)), 
		_lookAtDir(_lookAtDir = normalize(vec3(1 , 0, 1)))
	{

	}

	Camera::~Camera()
	{
	}

	/***********************************************************
		Updates the camera matrix based off the player position and lookAt direction
	***********************************************************/
	void Camera::Update()
	{
		mat4 viewMatrix, modelMatrix;

		if (_needsMatrixUpdate)
		{
			//field of view in degrees, aspect ratio, display range min, display range max
			mat4 projMatrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);  

			//rotation it is looking at {0,1,0} is head up {0,-1,0} is upside down.
			viewMatrix = lookAt(_cameraPosition - vec3(_lookAtDir.x, 0, _lookAtDir.z), _cameraPosition + _lookAtDir, vec3(0, 1, 0)); 
			modelMatrix = mat4(1.0f); //identity matrix, model will be at the origin
			_modelViewMatrix = viewMatrix * modelMatrix;
			_mvpMatrix = projMatrix * _modelViewMatrix;

			_needsMatrixUpdate = false;
		}

	}
}