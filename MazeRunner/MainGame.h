#pragma once

#include <GameEngine3D\Window.h>
#include <GameEngine3D\ShaderProgram.h>
#include <GameEngine3D\Camera.h>

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
	void draw();

	GameEngine3D::Window _window;
	GameEngine3D::ShaderProgram _shaderProgram;
	GameEngine3D::Camera _camera;
	int _screenWidth;
	int _screenHeight;
	float rotation_angle;
	GameState _gameState;

};

