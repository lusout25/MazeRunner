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
		glm::vec3 getLookAtDirection() { return _lookAtDir; };
		glm::mat4 getMVPMatrix() { return _mvpMatrix; };
		glm::mat4 getModelView() { return _modelViewMatrix; };

		void setCameraPosition(glm::vec3& newPosition) { _cameraPosition = newPosition; _needsMatrixUpdate = true; };
		void setLookAtDirection(glm::vec3& newDirection) { _lookAtDir = newDirection; _needsMatrixUpdate = true; };

	private:
		bool _needsMatrixUpdate;

		glm::mat4 _mvpMatrix;
		glm::mat4 _modelViewMatrix;

		glm::vec3 _cameraPosition;
		glm::vec3 _lookAtDir;
	};

}