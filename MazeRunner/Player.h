/***********************************************************
	User-controlled player object.
***********************************************************/

#pragma once
#include <GL\glew.h>
#include <GameEngine3D\QuadTree.h>
#include <GameEngine3D\Shared.h>
#include "Shared.h"

using namespace GameEngine3D;
using namespace glm;

class Player
{

public:
	Player();
	~Player();

	void placeCube(float x, float y, float z); //center coordinates for wall

	void init();
	void draw();
	void drawPlayerOutline();
	vec4 getColor() { return _color; };
	vec4 getOutlineColor() { return _outlineColor; };

	AABB getCollisionBoundary() { return _collisionBoundary; }

private:
	float _points[NUM_3D_VERTEX * NUM_VERTICES_WALL];
	float _colors[NUM_RGBA_COLOR * NUM_VERTICES_WALL];
	vec4 _color;
	vec4 _outlineColor;
	AABB _collisionBoundary;

	GLuint _vbo;
};

