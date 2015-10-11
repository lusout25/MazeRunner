/***********************************************************
	Maze generation.
***********************************************************/

#pragma once
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\QuadTree.h>

using namespace GameEngine3D;
using namespace glm;
using namespace std;

class MazeAlgorithm
{
public:
	MazeAlgorithm(int Rows, int Cols);
	~MazeAlgorithm(void);

	void generateMazeWeights(void);
	void generateMaze(void);
	void printMaze(void);
	void drawMaze(void);
	void drawMazeWireFrame();
	
	vec4 getColor() { return _color; };
	vec4 getWireFrameColor() { return _outlineColor; };
	QuadTree<AABB>* getQuadTree() { return _quadTree; };
	list<Wall> getAllWalls() { return _walls; };

private:
	enum DIRECTION { UP, DOWN, LEFT, RIGHT };

	struct Node
	{
		int weight;
		int x;
		int y;
		bool open;
	};

	GLuint _vbo;
	int _mazeRows, _mazeCols, _numWalls;
	vector<float> _points, _colors;
	vec4 _color;
	vec4 _outlineColor;

	Node _goalNode;
	Node **_mazeNodes;
	list<Node> _frontierNodes, _possibleOpenNodes;
	list<Node>::iterator _it, _pon;
	
	list<Wall>::iterator _wallIt;
	list<Wall> _walls;
	QuadTree<AABB>* _quadTree;
	AABB* _mapBoundary;

	void findNextNodeToAdd(void);
	void markClosedNode(Node* closeNode);
	bool validFrontierNode(Node NodeToLookat);
	bool validNextNode(Node NodeToLookat, DIRECTION dir);
	void addOutsideWalls();
	void storeCollisionData();
};