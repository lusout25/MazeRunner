#include "MainGame.h"
#include <GameEngine3D\GameEngine3D.h>
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>

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


	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	
	initShaders();

	//Trap mouse within window
	//If SDL_SetRelativeMouseMode fails exit game
	if (SDL_SetRelativeMouseMode(SDL_TRUE))
	{
		_gameState = GameState::EXIT;
	}

	//Generate Maze
	mazeAlgor.generateMazeWeights();
	mazeAlgor.generateMaze();
	mazeAlgor.printMaze();
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
	while (_gameState != GameState::EXIT)
	{
		processInput();

		//update the camera model-view-projection matrix
		_camera.Update();

		draw();
	}
}

void MainGame::processInput()
{
	SDL_Event input;

	bool needsUpdate = false;

	const float CAMERA_SPEED = .002f;
	const float ROTATE_SPEED = .002f;
	
	glm::vec3& cameraPosition = _camera.getCameraPosition();
	glm::vec3& lookAtDirection = _camera.getLookAtDirection();
	
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

		case SDL_MOUSEMOTION:
			_inputManager.updateMouseCoordinates(input.motion.xrel);
		    break;
		}
	}

	//input actions
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		//move camera forward
		cameraPosition += CAMERA_SPEED * lookAtDirection;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_a))
	{
		//strafe left
		cameraPosition += -CAMERA_SPEED * glm::cross(lookAtDirection, glm::vec3(lookAtDirection.x, 1, lookAtDirection.z));
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_s))
	{
		//move camera backward
		cameraPosition += -CAMERA_SPEED * lookAtDirection;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_d))
	{
		//strafe right
		cameraPosition += CAMERA_SPEED * glm::cross(lookAtDirection, glm::vec3(lookAtDirection.x, 1, lookAtDirection.z));
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_q) || _inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		_gameState = GameState::EXIT;
	}

	if (_inputManager.getMouseCoordinates())
	{
		lookAtDirection = glm::rotateY(lookAtDirection, -ROTATE_SPEED*((float)_inputManager.getMouseCoordinates()));
		_inputManager.updateMouseCoordinates(0);
		needsUpdate = true;
	}

	if (needsUpdate)
	{
		_camera.setCameraPosition(cameraPosition);
		_camera.setLookAtDirection(lookAtDirection);
	}
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

	mazeAlgor.drawMaze();

	_shaderProgram.unuse();

	_window.swapBuffer();
}