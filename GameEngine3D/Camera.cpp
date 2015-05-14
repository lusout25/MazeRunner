#pragma once
#include "Camera.h"
#include "GL\glew.h"
#include <glm\gtx\transform.hpp>

namespace GameEngine3D {


#include <glm\gtx\transform.hpp>

	Camera::Camera()
	{

	}


	Camera::~Camera()
	{
	}


	void Camera::Update()
	{
		_projMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); //field of view in degrees, aspect ratio, display range min, display range max 
		_viewMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //Camera point in world space, point the camera is looking at, rotation it is looking at {0,1,0} is head up {0,-1,0} is upside down.
		_modelMatrix = glm::mat4(1.0f); //identity matrix, model will be at the origin
		_mvpMatrix = _projMatrix * _viewMatrix * _modelMatrix;
	}

}