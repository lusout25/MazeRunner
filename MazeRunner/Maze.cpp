#include "Maze.h"


Maze::Maze()
{
}

Maze::~Maze()
{
}

void Maze::init()
{
	addCube(0, 0, 0);
	addCube(0, 0, -2);
	addCube(0, 0, -4);
	addCube(0, 0, -6);
}

void Maze::addCube(float x, float y, float z)
{
	Wall* wall = new Wall();

	wall->placeCube(x, y, z);
	_walls.push_back(wall);
}

void Maze::renderWalls()
{
	for (int i = 0; i < _walls.size(); i++)
	{
		_walls[i]->draw();
		_walls[i]->render();
	}
}