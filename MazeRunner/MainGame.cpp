#include "MainGame.h"
#include <GameEngine3D\GameEngine3D.h>
#include <glm\glm.hpp>

MainGame::MainGame()
{
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	GameEngine3D::init();
	_window.create("Maze Runner", _screenWidth, _screenHeight);

	initShaders();
}

void MainGame::initShaders()
{
	_shaderProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop()
{
	float points[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,

		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.5f,

		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.5f
	};

	float colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	SDL_Event Event;

	while (_gameState != GameState::EXIT)
	{

		GLuint vbo = 0;
		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, (27+36) * sizeof(float), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 27 * sizeof(float), points);
		glBufferSubData(GL_ARRAY_BUFFER, 27 * sizeof(float), 36 * sizeof(float), colors);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) (27 * sizeof(float)));

		processInput();

		//update the camera model-view-projection matrix
		_camera.Update();

		draw();
	}

}

void MainGame::processInput()
{
	SDL_Event input;

	const float CAMERA_SPEED = .001f;
	
	//set input state
	while (SDL_PollEvent(&input))
	{
		switch (input.type)
		{

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			_inputManager.pressKey(input.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(input.key.keysym.sym);
			break;
		}
	}

	glm::vec3& cameraPosition = _camera.getCameraPosition();
	glm::vec3& lookAtPosition = _camera.getLookAtPosition();

	//do input actions
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		//move camera forward
		cameraPosition += glm::vec3(0, 0, -CAMERA_SPEED);
	}

	if (_inputManager.isKeyPressed(SDLK_a))
	{
		//strafe left
		cameraPosition += glm::vec3(-CAMERA_SPEED, 0, 0);
		lookAtPosition += glm::vec3(-CAMERA_SPEED, 0, 0);

	}

	if (_inputManager.isKeyPressed(SDLK_s))
	{
		//move camera backward
		cameraPosition += glm::vec3(0, 0, CAMERA_SPEED);
	}

	if (_inputManager.isKeyPressed(SDLK_d))
	{
		//strafe right
		cameraPosition += glm::vec3(CAMERA_SPEED, 0, 0);
		lookAtPosition += glm::vec3(CAMERA_SPEED, 0, 0);
	}

	_camera.setCameraPosition(cameraPosition);
	_camera.setLookAtPosition(lookAtPosition);

}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();

	//locate the location of "MVP" in the shader
	GLint mvpLocation = _shaderProgram.getUniformLocation("MVP");

	//pass the camera matrix to the shader
	glm::mat4 cameraMatrix = _camera.getMVPMatrix();
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	glDrawArrays(GL_TRIANGLES, 0, 9);

	_shaderProgram.unuse();

	_window.swapBuffer();
}