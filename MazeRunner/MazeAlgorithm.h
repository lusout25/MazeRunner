#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <GameEngine3D\Wall.h>
#include <GameEngine3D\QuadTree.h>

class MazeAlgorithm
{
public:
	MazeAlgorithm(int Rows, int Cols);
	~MazeAlgorithm(void);

	void generateMazeWeights(void);
	void generateMaze(void);
	void printMaze(void);
	void drawMaze(void);

	GameEngine3D::QuadTree<GameEngine3D::AABB>* getQuadTree() { return _quadTree; };
	std::list<GameEngine3D::Wall> getAllWalls() { return walls; };

private:
	enum DIRECTION { UP, DOWN, LEFT, RIGHT };

	struct Node
	{
		int weight;
		int x;
		int y;
		bool open;
	};

	int MazeRows;
	int MazeCols;
	Node GoalNode;
	Node **MazeNodes;
	std::list<Node> frontierNodes, possibleOpenNodes;
	std::list<Node>::iterator it, pon;
	std::list<GameEngine3D::Wall>::iterator wallIt;
	std::list<GameEngine3D::Wall> walls;
	GameEngine3D::QuadTree<GameEngine3D::AABB>* _quadTree;
	GameEngine3D::AABB* _mapBoundary;

	void findNextNodeToAdd(void);
	bool validFrontierNode(Node NodeToLookat);
	bool validNextNode(Node NodeToLookat, DIRECTION dir);
	void markClosedNode(Node* closeNode);

};