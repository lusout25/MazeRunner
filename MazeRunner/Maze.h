#pragma once

#include <GameEngine3D\Wall.h>
#include <vector>

using namespace GameEngine3D;

class Maze
{
public:
	Maze();
	~Maze();

	void init();
	void addCube(float x, float y, float z);
	void renderWalls();

private:
	std::vector<Wall*> _walls;
};

