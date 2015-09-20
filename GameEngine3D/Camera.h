#pragma once
#include <vector>
#include <math.h>
#include <glm\glm.hpp>

using namespace glm;

namespace GameEngine3D {

	class Camera
	{
	public:
		Camera();
		~Camera();

		void Update();

		vec3 getCameraPosition() { return _cameraPosition; };
		vec3 getLookAtDirection() { return _lookAtDir; };
		mat4 getMVPMatrix() { return _mvpMatrix; };
		mat4 getModelView() { return _modelViewMatrix; };

		void setCameraPosition(vec3& newPosition) { _cameraPosition = newPosition; _needsMatrixUpdate = true; };
		void setLookAtDirection(vec3& newDirection) { _lookAtDir = newDirection; _needsMatrixUpdate = true; };

	private:
		bool _needsMatrixUpdate;

		mat4 _mvpMatrix;
		mat4 _modelViewMatrix;

		vec3 _cameraPosition;
		vec3 _lookAtDir;
	};

}