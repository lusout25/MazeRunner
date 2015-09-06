#pragma once

#include "MazeAlgorithm.h"
#include <GameEngine3D\SimpleObjLoader.h>
#include <GameEngine3D\Window.h>
#include <GameEngine3D\ShaderProgram.h>
#include <GameEngine3D\Camera.h>
#include <GameEngine3D\InputManager.h>
#include <GameEngine3D\Triangle.h>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\Camera2D.h>
#include "Player.h"
#include <GameEngine3D\QuadTree.h>

#define MAZE_HEIGHT 20
#define MAZE_LENGTH 20

enum class GameState { PLAY, EXIT };
enum class JumpState { NONE, UP, DOWN };
enum class ShaderState { COLOR, LIGHTING };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders(ShaderState ss);
	void gameLoop();
	void processInput();
	void draw();
	void drawHUD();

	GameEngine3D::Window _window;
	GameEngine3D::ShaderProgram _shaderProgram;
	GameEngine3D::Camera _camera;
	GameEngine3D::Camera2D _hud;
	GameEngine3D::InputManager _inputManager;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	JumpState _jumpState;
	ShaderState _shaderState;

	GameEngine3D::Wall _walls;
	Player _player;
	SimpleObjLoader _androidObj;

	MazeAlgorithm mazeAlgor = MazeAlgorithm(MAZE_HEIGHT, MAZE_LENGTH);
	GameEngine3D::QuadTree<GameEngine3D::AABB>* _quadTree;

};

