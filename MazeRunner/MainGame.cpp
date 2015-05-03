#include "MainGame.h"
#include <GameEngine3D\GameEngine3D.h>

MainGame::MainGame()
{
	_screenWidth = 1024;
	_screenHeight = 768;
	rotation_angle = 0.0f;
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MainGame::initShaders()
{
	_shaderProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop()
{
	float points[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	while (_gameState != GameState::EXIT)
	{
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		draw();
	}

}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(rotation_angle, 0.0f, 1.0f, 0.0f);

	//_shaderProgram.use();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	//_shaderProgram.unuse();

	for (int i = 0; i < 100000000; ++i )
	{}

	rotation_angle = rotation_angle + .05f;

	_window.swapBuffer();
}