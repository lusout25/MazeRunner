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
		glm::mat4 getCameraMatrix() { return _mvpMatrix; };

	private:
		glm::mat4 _viewMatrix;
		glm::mat4 _modelMatrix;
		glm::mat4 _projMatrix;
		glm::mat4 _mvpMatrix;
	};

}