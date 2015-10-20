#include "MainGame.h"
#include <GameEngine3D\GameEngine3D.h>

MainGame::MainGame() : _screenWidth(SCREEN_WIDTH_PIXELS),
_screenHeight(SCREEN_HEIGHT_PIXELS),
_gameState(GameState::PLAY),
_showTrail(false)
{
}

MainGame::~MainGame()
{
}

/***********************************************************
Entry function to game logic.
***********************************************************/
void MainGame::run()
{
	initSystems();
	gameLoop();
}

/***********************************************************
Initialize openGL variables and initial state of the game.
Functions here will be ran only once.
***********************************************************/
void MainGame::initSystems()
{
	init();
	_window.create(GAME_TITLE, _screenWidth, _screenHeight);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	if (SDL_SetRelativeMouseMode(SDL_TRUE))	//Trap mouse within window
	{
		_gameState = GameState::EXIT;
	}

	_grassTexture.init(vec3(-2, -.5, -2), vec3(MAZE_ROWS, -.5, -2), vec3(MAZE_ROWS, -.5, MAZE_COLUMNS), vec3(-2, -.5, MAZE_COLUMNS), "Textures/grass.png", MAZE_ROWS + 2, MAZE_COLUMNS + 2);
	_player.placeCube(0, 0, 0);

	_hud.init(_screenWidth, _screenHeight);
	_hud.setPosition(vec2(_screenWidth / 2, _screenHeight / 2));

	//Generate maze using Prim's algorithm
	_maze.generateMazeWeights();
	_maze.generateMaze();
	_maze.solveMaze(0, 0);
	_maze.printMaze();

	//Load object from 3d model
	//_androidObj = SimpleObjLoader();
	//_androidObj.loadObject("..\\MazeRunner\\ObjectModels\\Jigglypuff.obj");

	_quadTree = _maze.getQuadTree();
}

/***********************************************************
Compile and link vertex and fragment shaders
***********************************************************/
void MainGame::initShaders(ShaderState ss)
{
	string vertFilePath, fragFilePath;
	string* attributeList;
	int attributeCount = 0, i;

	if (ss == ShaderState::COLOR)
	{
		vertFilePath = "Shaders/colorShading.vert";
		fragFilePath = "Shaders/colorShading.frag";
		attributeCount = 1;
		attributeList = new string[attributeCount];
		attributeList[0] = "vertexPosition";
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
	else if (ss == ShaderState::TEXTURE)
	{
		vertFilePath = "Shaders/textureShader.vert";
		fragFilePath = "Shaders/textureShader.frag";
		attributeCount = 2;
		attributeList = new string[attributeCount];
		attributeList[0] = "vertexPosition";
		attributeList[1] = "vertexUV";
	}
	else
	{
		attributeList = new string[attributeCount];
	}

	_shaderProgram.compileShaders(vertFilePath, fragFilePath);
	for (i = 0; i < attributeCount; i++)
	{
		_shaderProgram.addAttribute(attributeList[i]);
	}
	_shaderProgram.linkShaders();
}

/***********************************************************
Game loop.  This will call appropirate functions to handle
input, update cameras, and render a frame to screen
***********************************************************/
void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		_camera.Update();
		_hud.update();

		draw();
	}
}

/***********************************************************
Handles keyboard and mouse events
***********************************************************/
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

	//set input event state
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
	if (_inputManager.isKeyPressed(SDLK_w)) //move forward
	{
		cameraPosition.x += CAMERA_SPEED_STRAFE * lookAtDirection.x;
		cameraPosition.z += CAMERA_SPEED_STRAFE * lookAtDirection.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_a)) //strafe left
	{
		temp = -CAMERA_SPEED_STRAFE * cross(lookAtDirection, vec3(lookAtDirection.x, 1, lookAtDirection.z));
		cameraPosition.x += temp.x;
		cameraPosition.z += temp.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_s)) //move backward
	{
		cameraPosition.x += -CAMERA_SPEED_STRAFE * lookAtDirection.x;
		cameraPosition.z += -CAMERA_SPEED_STRAFE * lookAtDirection.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_d)) //strafe right
	{
		temp = CAMERA_SPEED_STRAFE * cross(lookAtDirection, vec3(lookAtDirection.x, 1, lookAtDirection.z));
		cameraPosition.x += temp.x;
		cameraPosition.z += temp.z;
		needsUpdate = true;
	}

	if (_inputManager.isKeyPressed(SDLK_SPACE)) //jump
	{
		if (_jumpState == JumpState::NONE)
		{
			_jumpState = JumpState::UP;
		}
	}

	if (_inputManager.isKeyPressed(SDLK_q) || _inputManager.isKeyPressed(SDLK_ESCAPE)) //exit
	{
		_gameState = GameState::EXIT;
	}

	if (_inputManager.isKeyPressed(SDLK_v)) //show trail to goal
	{
		//_maze.solveMaze((int)cameraPosition.x, (int)cameraPosition.z); //TODO_ REMOVE COMMENT
		_showTrail = true;
	}
	else
	{
		_showTrail = false;
	}

	if (_inputManager.getMouseXCoordinates() || _inputManager.getMouseYCoordinates())  //move mouse
	{
		lookAtDirection = rotateY(lookAtDirection, -CAMERA_SPEED_STRAFE * ((float)_inputManager.getMouseXCoordinates()));
		lookAtDirection.y += -CAMERA_SPEED_ROTATE * (float)_inputManager.getMouseYCoordinates();
		_inputManager.updateMouseCoordinates(0, 0);
		needsUpdate = true;
	}

	if (_jumpState != JumpState::NONE)
	{
		if (_jumpState == JumpState::UP) //jumping upward
		{
			cameraPosition.y += PLAYER_JUMP_SPEED;
			if (cameraPosition.y > 1.0f)
			{
				_jumpState = JumpState::DOWN;
			}
		}
		else  //falling downward
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

	if (needsUpdate) //something has changed based off input
	{
		_camera.setCameraPosition(cameraPosition);
		_camera.setLookAtDirection(lookAtDirection);
		_player.placeCube(cameraPosition.x, cameraPosition.y - 0.25f, cameraPosition.z);

		//handle collision
		playerBoundary = _player.getCollisionBoundary();
		searchBoundary = playerBoundary;
		searchBoundary.halfSize = { COLLISION_SEARCH_RADIUS, COLLISION_SEARCH_RADIUS };

		res = _quadTree->queryRange(searchBoundary); //get walls within search radius
		for (i = 0; i < res.size(); i++)
		{
			if (playerBoundary.intersects(res[i].box, collisionX, collisionY))
			{
				cameraPosition.x -= collisionX;
				cameraPosition.z -= collisionY;

				_player.placeCube(cameraPosition.x, cameraPosition.y - 0.25f, cameraPosition.z);
				_camera.setCameraPosition(cameraPosition);
			}
		}
	}
}

/***********************************************************
Draw and render game objects
***********************************************************/
void MainGame::draw()
{
	GLint projLocation, colorLocation, samplerLocation,textureFlagLocation;

	initShaders(ShaderState::TEXTURE);

	_shaderProgram.use();

	//get location for uniform variables
	projLocation = _shaderProgram.getUniformLocation("MVP");
	colorLocation = _shaderProgram.getUniformLocation("COLOR");
	samplerLocation = _shaderProgram.getUniformLocation("SAMPLER");
	textureFlagLocation = _shaderProgram.getUniformLocation("USETEXTURE");

	//draw maze
	setShaderProjection(ProjectionState::PERSPECTIVE, projLocation);
	setShaderColor(_maze.getColor(), colorLocation);
	_maze.drawMaze();

	//draw ground texture
	switchTextureOn(true, textureFlagLocation, samplerLocation);
	setShaderColor(vec4(1, 1, 1, 1), colorLocation);
	_grassTexture.draw();

	//draw wireframe of maze
	switchTextureOn(false, textureFlagLocation);
	glLineWidth(3.0f);
	setShaderColor(_maze.getWireFrameColor(), colorLocation);
	_maze.drawMazeWireFrame();

	//draw trail to goal
	//if (_showTrail) //TODO: REMOVE COMMENT
	{
		setShaderColor(_maze.getTrailColor(), colorLocation);
		glLineWidth(7.0f);
		_maze.drawTrail();
	}

	//draw player
	setShaderColor(_player.getColor(), colorLocation);
	_player.draw();

	//draw player outline
	glLineWidth(5.0f);
	setShaderColor(_player.getOutlineColor(), colorLocation);
	_player.drawPlayerOutline();

	//draw hud
	setShaderProjection(ProjectionState::ORTHOGRAPHIC, projLocation);
	setShaderColor(_hud.getColor(), colorLocation);
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

void MainGame::switchTextureOn(bool turnOn, GLint textureFlagUniformLocation, GLint samplerUniformLocation)
{
	if (turnOn)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerUniformLocation, 0);
		glUniform1i(textureFlagUniformLocation, 1);
	}
	else
	{
		glUniform1i(textureFlagUniformLocation, 0);
	}
}

void MainGame::setShaderColor(vec4& color, GLint colorUniformLocation)
{
	glUniform4fv(colorUniformLocation, 1, &(color[0]));
}

void MainGame::setShaderProjection(ProjectionState view, GLint projUniformLocation)
{
	if (view == ProjectionState::ORTHOGRAPHIC)
	{
		glDisable(GL_CULL_FACE);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, &(_hud.getCameraMatrix()[0][0]));
	}
	else if (view == ProjectionState::PERSPECTIVE)
	{
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, &(_camera.getMVPMatrix()[0][0]));
	}
}