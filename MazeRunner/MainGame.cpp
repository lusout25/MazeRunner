#include "MainGame.h"
#include <GameEngine3D\GameEngine3D.h>

MainGame::MainGame() : _screenWidth(SCREEN_WIDTH_PIXELS),
_screenHeight(SCREEN_HEIGHT_PIXELS),
	_gameState(GameState::PLAY)
{

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
	init();
	_window.create(GAME_TITLE, _screenWidth, _screenHeight);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//Trap mouse within window
	//If SDL_SetRelativeMouseMode fails exit game
	if (SDL_SetRelativeMouseMode(SDL_TRUE))
	{
		_gameState = GameState::EXIT;
	}

	_player.placeCube(0, 0, 0);

	_hud.init(_screenWidth, _screenHeight);
	_hud.setPosition(vec2(_screenWidth / 2, _screenHeight / 2));

	//Generate Maze
	_maze.generateMazeWeights();
	_maze.generateMaze();
	_maze.printMaze();

	//Load Object
	//_androidObj = SimpleObjLoader();
	//_androidObj.loadObject("..\\MazeRunner\\ObjectModels\\Jigglypuff.obj");

	//get collision data structure
	_quadTree = _maze.getQuadTree();
}

void MainGame::initShaders(ShaderState ss)
{
	string vertFilePath, fragFilePath;
	string* attributeList;
	int attributeCount = 0, i;

	if (ss == ShaderState::COLOR)
	{
		vertFilePath = "Shaders/colorShading.vert";
		fragFilePath = "Shaders/colorShading.frag";
		attributeCount = 2;
		attributeList = new string[attributeCount];
		attributeList[0] = "vertexPosition";
		attributeList[1] = "vertexColor";
	}
	else if (ss == ShaderState::LIGHTING)
	{
		vertFilePath = "Shaders/lightingShader.vert";
		fragFilePath = "Shaders/lightingShader.frag";
		attributeCount = 3;
		attributeList = new string[attributeCount];
		attributeList[0] = "inPosition";
		attributeList[1] = "inColor";
		attributeList[2] = "inNormal";
	}
	else
	{
		attributeList = new string[attributeCount];
	}

	_shaderProgram.compileShaders(vertFilePath, fragFilePath );
	for (i = 0; i < attributeCount; i++)
	{
		_shaderProgram.addAttribute(attributeList[i]);
	}
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();

		//update the camera model-view-projection matrix
		_camera.Update();
		_hud.update();

		draw();
	}
}

void MainGame::processInput()
{
	SDL_Event input;

	bool needsUpdate = false;
	
	vec3& cameraPosition = _camera.getCameraPosition();
	vec3& lookAtDirection = _camera.getLookAtDirection();
	vec3 temp;

	float collisionX = 0, collisionY = 0;
	AABB wallBoundary, playerBoundary, searchBoundary;
	vector<Data<AABB>> res;
	uint i;
	
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
			_inputManager.updateMouseCoordinates(input.motion.xrel, input.motion.yrel);
		    break;
		}
	}

	//input actions
	if (_inputManager.isKeyPressed(SDLK_w))
	{
		//move camera forward
		cameraPosition.x += CAMERA_SPEED_STRAFE * lookAtDirection.x;
		cameraPosition.z += CAMERA_SPEED_STRAFE * lookAtDirection.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_a))
	{
		//strafe left
		temp = -CAMERA_SPEED_STRAFE * cross(lookAtDirection, vec3(lookAtDirection.x, 1, lookAtDirection.z));
		cameraPosition.x += temp.x;
		cameraPosition.z += temp.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_s))
	{
		//move camera backward
		cameraPosition.x += -CAMERA_SPEED_STRAFE * lookAtDirection.x;
		cameraPosition.z += -CAMERA_SPEED_STRAFE * lookAtDirection.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_d))
	{
		//strafe right
		temp = CAMERA_SPEED_STRAFE * cross(lookAtDirection, vec3(lookAtDirection.x, 1, lookAtDirection.z));
		cameraPosition.x += temp.x;
		cameraPosition.z += temp.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_SPACE))
	{
		//Lebrons Jumping ability
		if (_jumpState == JumpState::NONE)
		{
			_jumpState = JumpState::UP;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_q) || _inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		_gameState = GameState::EXIT;
	}

	if (_inputManager.getMouseXCoordinates() || _inputManager.getMouseYCoordinates())
	{
		lookAtDirection = rotateY(lookAtDirection, -CAMERA_SPEED_STRAFE * ((float)_inputManager.getMouseXCoordinates()));
		lookAtDirection.y += -CAMERA_SPEED_ROTATE * (float)_inputManager.getMouseYCoordinates();
		_inputManager.updateMouseCoordinates(0, 0);
		needsUpdate = true;
	}

	if (_jumpState != JumpState::NONE)
	{
		if (_jumpState == JumpState::UP)
		{
			cameraPosition.y += PLAYER_JUMP_SPEED;
			if (cameraPosition.y > 1.0f)
			{
				_jumpState = JumpState::DOWN;
			}
		}
		else
		{
			cameraPosition.y -= PLAYER_JUMP_SPEED;
			if (cameraPosition.y <= .25)
			{
				_jumpState = JumpState::NONE;
			}
		}
		needsUpdate = true;
		_camera.setCameraPosition(cameraPosition);
	}

	if (needsUpdate)
	{
		_camera.setCameraPosition(cameraPosition);
		_camera.setLookAtDirection(lookAtDirection);
		_player.placeCube(cameraPosition.x, cameraPosition.y - 0.25, cameraPosition.z);

		//test collision stuffy stuff
		playerBoundary = _player.getCollisionBoundary();
		searchBoundary = playerBoundary;
		searchBoundary.halfSize = { COLLISION_SEARCH_RADIUS, COLLISION_SEARCH_RADIUS };

		res = _quadTree->queryRange(searchBoundary);
		for (i = 0; i < res.size(); i++)
		{
			if (playerBoundary.intersects(res[i].box, collisionX, collisionY))
			{
				cameraPosition.x -= collisionX;
				cameraPosition.z -= collisionY;

				_player.placeCube(cameraPosition.x, cameraPosition.y - 0.25, cameraPosition.z);
				_camera.setCameraPosition(cameraPosition);
			}
		}
	}
}

void MainGame::draw()
{
	GLint mvpLocation, colorLocation;
	mat4 cameraMatrix, projMatrix, mvp;
	vec4 color;

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	initShaders(ShaderState::COLOR);

	_shaderProgram.use();

	//locate the location of "MVP" in the shader
	mvpLocation = _shaderProgram.getUniformLocation("MVP");
	colorLocation = _shaderProgram.getUniformLocation("COLOR");

	//pass the camera matrix to the shader
	cameraMatrix = _camera.getMVPMatrix();
	color = _maze.getColor();

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	glUniform4fv(colorLocation, 1, &(color[0]));

	_maze.drawMaze();

	color = _player.getColor();
	glUniform4fv(colorLocation, 1, &(color[0]));

	_player.draw();
	_player.render();

	projMatrix = _hud.getCameraMatrix();
	mvp = projMatrix;
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(mvp[0][0]));

	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

	color = _hud.getColor();
	glUniform4fv(colorLocation, 1, &(color[0]));

	_hud.draw();

	_shaderProgram.unuse();

	//***************
	// Lighting 
	//***************
	/*initShaders(ShaderState::LIGHTING);
	_shaderProgram.use();

	//locate the location of "MVP" in the shader
	mvpLocation = _shaderProgram.getUniformLocation("MVP");

	//pass the camera matrix to the shader
	cameraMatrix = _camera.getMVPMatrix();
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint normalMatLoc = _shaderProgram.getUniformLocation("normalMatrix");

	mat4 normalMatrix = transpose(inverse(_camera.getModelView()));
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, &(normalMatrix[0][0]));

	_androidObj.render();
	_shaderProgram.unuse();*/

	_window.swapBuffer();
}