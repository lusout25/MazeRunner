#pragma once
#include "MazeAlgorithm.h"
#include <GameEngine3D\SimpleObjLoader.h>
#include <GameEngine3D\Window.h>
#include <GameEngine3D\ShaderProgram.h>
#include <GameEngine3D\Camera.h>
#include <GameEngine3D\InputManager.h>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\Camera2D.h>
#include "Player.h"
#include <GameEngine3D\QuadTree.h>

using namespace GameEngine3D;
using namespace glm;
using namespace std;

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

	Window _window;
	ShaderProgram _shaderProgram;
	Camera _camera;
	Camera2D _hud;
	InputManager _inputManager;
	SimpleObjLoader _androidObj;

	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	JumpState _jumpState;
	ShaderState _shaderState;

	Wall _walls;
	Player _player;

	MazeAlgorithm _maze = MazeAlgorithm(MAZE_HEIGHT, MAZE_LENGTH);
	QuadTree<AABB>* _quadTree;
};

