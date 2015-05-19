#pragma once

#include "MazeAlgorithm.h"
#include <GameEngine3D\Window.h>
#include <GameEngine3D\ShaderProgram.h>
#include <GameEngine3D\Camera.h>
#include <GameEngine3D\InputManager.h>
#include <GameEngine3D\Triangle.h>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\Camera2D.h>

#define MAZE_HEIGHT 5
#define MAZE_LENGTH 5

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void draw();

	GameEngine3D::Window _window;
	GameEngine3D::ShaderProgram _shaderProgram;
	GameEngine3D::Camera _camera;
	GameEngine3D::Camera2D _hud;
	GameEngine3D::InputManager _inputManager;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	GameEngine3D::Wall _walls;

	MazeAlgorithm mazeAlgor = MazeAlgorithm(MAZE_HEIGHT, MAZE_LENGTH);

};

