#pragma once
#include <GL\glew.h>
#include <GameEngine3D\QuadTree.h>

class Player
{
	static const int NUM_VERTICES = 36;

public:
	Player();
	~Player();

	void placeCube(float x, float y, float z); //center coordinates

	void init();
	void draw();
	void render();

	GameEngine3D::AABB getCollisionBoundary() { return _collisionBoundary; }

private:
	float _points[3 * NUM_VERTICES];
	float _colors[4 * NUM_VERTICES];

	GameEngine3D::AABB _collisionBoundary;

	GLuint _vbo;
	GLuint _vao;
};

