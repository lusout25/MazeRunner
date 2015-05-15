#pragma once
#include <vector>
#include <math.h>
#include "Math.h"
#include <glm\glm.hpp>


namespace GameEngine3D {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void Update();

		glm::vec3 getCameraPosition() { return _cameraPosition; };
		glm::vec3 getLookAtPosition() { return _lookAt; };
		glm::mat4 getMVPMatrix() { return _mvpMatrix; };

		void setCameraPosition(glm::vec3& newPosition) { _cameraPosition = newPosition; _needsMatrixUpdate = true; };
		void setLookAtPosition(glm::vec3& newPosition) { _lookAt = newPosition; _needsMatrixUpdate = true; };

	private:
		bool _needsMatrixUpdate;

		glm::mat4 _viewMatrix;
		glm::mat4 _modelMatrix;
		glm::mat4 _projMatrix;
		glm::mat4 _mvpMatrix;

		glm::vec3 _cameraPosition;
		glm::vec3 _lookAt;
	};

}