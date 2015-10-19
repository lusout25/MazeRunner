/***********************************************************
	Main logic for MazeRunner.
***********************************************************/

#pragma once
#include <GameEngine3D\SimpleObjLoader.h>
#include <GameEngine3D\Window.h>
#include <GameEngine3D\ShaderProgram.h>
#include <GameEngine3D\Camera.h>
#include <GameEngine3D\InputManager.h>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\Camera2D.h>
#include <GameEngine3D\QuadTree.h>
#include <GameEngine3D\Sprite.h>
#include <glm\glm.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "MazeAlgorithm.h"
#include "Player.h"
#include "Shared.h"

using namespace GameEngine3D;
using namespace glm;
using namespace std;

enum class GameState { PLAY, EXIT };
enum class JumpState { NONE, UP, DOWN };
enum class ShaderState { COLOR, LIGHTING, TEXTURE };
enum class ProjectionState { ORTHOGRAPHIC, PERSPECTIVE };

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
	void switchTextureOn(bool turnOn, GLint textureFlagUniformLocation, GLint samplerUniformLocation = 0);
	void setShaderColor(vec4& color, GLint colorUniformLocation);
	void setShaderProjection(ProjectionState view, GLint projUniformLocation);

	Window _window;
	ShaderProgram _shaderProgram;
	Camera _camera;
	Camera2D _hud;
	InputManager _inputManager;
	SimpleObjLoader _androidObj;

	int _screenWidth, _screenHeight;
	bool _showTrail;

	GameState _gameState;
	JumpState _jumpState;
	ShaderState _shaderState;

	Player _player;
	Sprite _grassTexture;

	MazeAlgorithm _maze = MazeAlgorithm(MAZE_ROWS, MAZE_COLUMNS);
	QuadTree<AABB>* _quadTree; //data structure for spacial collision
};

