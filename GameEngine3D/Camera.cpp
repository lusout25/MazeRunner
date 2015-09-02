#pragma once
#include "Camera.h"
#include "GL\glew.h"
#include <glm\gtx\transform.hpp>

namespace GameEngine3D {

	Camera::Camera()
	{
		_needsMatrixUpdate = true;
		_cameraPosition = glm::vec3(0, .25, 1);
		_lookAtDir = glm::vec3(0, 0, -1);  //begin looking down z-axis
	}


	Camera::~Camera()
	{
	}


	void Camera::Update()
	{

		if (_needsMatrixUpdate)
		{
			//field of view in degrees, aspect ratio, display range min, display range max
			glm::mat4 projMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);  

			//Camera point in world space, point the camera is looking at, 
			//rotation it is looking at {0,1,0} is head up {0,-1,0} is upside down.
			glm::mat4 viewMatrix = glm::lookAt(_cameraPosition - glm::vec3(_lookAtDir.x,0,_lookAtDir.z), _cameraPosition + _lookAtDir, glm::vec3(0, 1, 0)); 
			glm::mat4 modelMatrix = glm::mat4(1.0f); //identity matrix, model will be at the origin
			_mvpMatrix = projMatrix * viewMatrix * modelMatrix;

			_needsMatrixUpdate = false;
		}

	}
}