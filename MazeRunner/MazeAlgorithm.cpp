#include "MazeAlgorithm.h"

MazeAlgorithm::MazeAlgorithm(int Rows, int Cols)
{
	int j;

	_mazeRows = Rows;
	_mazeCols = Cols;
	_goalFound = false;

	_numWalls = 0;
	_color = vec4(.297, .063, .063, 1);
	_outlineColor = vec4(0, 0, 0, 1);
	_trailColor = vec4(1, .843, 0, 1);
	_goalColor = vec4(1, .6, 0, 1);
	_mazeNodes = new Node*[_mazeRows];

	for (j = 0; j < _mazeRows; ++j)
	{
		_mazeNodes[j] = new Node[_mazeCols];
	}
	srand((unsigned int)time(NULL));

	_mapBoundary = new AABB(Point(Cols / 2.0f, Rows / 2.0f), Point(Cols / 2.0f + 2, Rows / 2.0f + 2));
	_quadTree = new QuadTree<AABB>(*_mapBoundary);
}

MazeAlgorithm::~MazeAlgorithm()
{
	int j;

	for (j = 0; j < _mazeCols; ++j)
	{
		delete[] _mazeNodes[j];
	}

	delete[] _mazeNodes;
}

/***********************************************************
	Handles weighting for maze walls
***********************************************************/
void MazeAlgorithm::generateMazeWeights(void)
{
	int i, j;

	for (i = 0; i < _mazeRows; ++i)
	{
		for (j = 0; j < _mazeRows; ++j)
		{
			_mazeNodes[i][j].x = i;
			_mazeNodes[i][j].y = j;
			_mazeNodes[i][j].weight = (rand() % 9) + 1;
			_mazeNodes[i][j].open = true;
			_mazeNodes[i][j].isGoal = false;
			_mazeNodes[i][j].inList = false;
			_mazeNodes[i][j].inPathToGoal = false;
			_mazeNodes[i][j].parent = NULL;
		}
	}
}

/***********************************************************
	Generate maze nodes
***********************************************************/
void MazeAlgorithm::generateMaze(void)
{
	_frontierNodes.push_back(_mazeNodes[0][0]); //starting node
	markClosedNode(&_mazeNodes[0][0]);

	while (!_frontierNodes.empty())
	{
		findNextNodeToAdd();
	}
}

/***********************************************************
	Support function for adding nodes
***********************************************************/
void MazeAlgorithm::findNextNodeToAdd(void)
{
	Node temp = { -1, -1, -1, false };

	//Loop through Nodes in the frontier
	_it = _frontierNodes.begin();

	while (_it != _frontierNodes.end())
	{
		//Check to see if this node has any values to add.
		if (validFrontierNode(*_it))
		{
			++_it;
		}
		//remove this node
		else
		{
			_it = _frontierNodes.erase(_it);
		}
	}

	//Loop through Nodes in the possible next selection
	for (_pon = _possibleOpenNodes.begin(); _pon != _possibleOpenNodes.end(); ++_pon)
	{
		if (temp.weight == -1)
		{
			temp = *_pon;
		}
		else
		{
			if ((*_pon).weight < temp.weight)
			{
				temp = *_pon;
			}
		}
	}

	if (temp.weight != -1)
	{
		_frontierNodes.push_back(_mazeNodes[temp.x][temp.y]);
		markClosedNode(&_mazeNodes[temp.x][temp.y]);
		_goalNode = _mazeNodes[temp.x][temp.y];
	}

	_possibleOpenNodes.erase(_possibleOpenNodes.begin(), _possibleOpenNodes.end());
}

/***********************************************************
	Validate nodes.  Check to see if adjacent nodes are available.
***********************************************************/
bool MazeAlgorithm::validFrontierNode(Node NodeToLookat)
{
	bool validLower = false;
	bool validUpper = false;
	bool validRight = false;
	bool validLeft = false;

	//check upper node
	if (NodeToLookat.y + 1 < _mazeRows)
	{
		if (_mazeNodes[NodeToLookat.x][NodeToLookat.y + 1].open)
		{
			validUpper = validNextNode(_mazeNodes[NodeToLookat.x][NodeToLookat.y + 1], LEFT);
			if (validUpper)
			{
				_possibleOpenNodes.push_back(_mazeNodes[NodeToLookat.x][NodeToLookat.y + 1]);
			}
		}
	}

	//check lower node
	if (NodeToLookat.y - 1 >= 0)
	{
		if (_mazeNodes[NodeToLookat.x][NodeToLookat.y - 1].open)
		{
			validLower = validNextNode(_mazeNodes[NodeToLookat.x][NodeToLookat.y - 1], RIGHT);
			if (validLower)
			{
				_possibleOpenNodes.push_back(_mazeNodes[NodeToLookat.x][NodeToLookat.y - 1]);
			}
		}
	}

	//check right node
	if (NodeToLookat.x + 1 < _mazeCols)
	{
		if (_mazeNodes[NodeToLookat.x + 1][NodeToLookat.y].open)
		{
			validRight = validNextNode(_mazeNodes[NodeToLookat.x + 1][NodeToLookat.y], DOWN);
			if (validRight)
			{
				_possibleOpenNodes.push_back(_mazeNodes[NodeToLookat.x + 1][NodeToLookat.y]);
			}
		}
	}

	//check left node
	if (NodeToLookat.x - 1 >= 0)
	{
		if (_mazeNodes[NodeToLookat.x - 1][NodeToLookat.y].open)
		{
			validRight = validNextNode(_mazeNodes[NodeToLookat.x - 1][NodeToLookat.y], UP);
			if (validRight)
			{
				_possibleOpenNodes.push_back(_mazeNodes[NodeToLookat.x - 1][NodeToLookat.y]);
			}
		}
	}

	return validLower || validUpper || validRight || validLeft;
}

/***********************************************************
	Validate next node. Make sure the next node doesnt connect to any other open nodes. 
***********************************************************/
bool MazeAlgorithm::validNextNode(Node NodeToLookat, DIRECTION dir)
{
	bool validLower = false;
	bool validUpper = false;
	bool validRight = false;
	bool validLeft = false;

	//check upper node
	if (NodeToLookat.y + 1 < _mazeRows)
	{
		validUpper = _mazeNodes[NodeToLookat.x][NodeToLookat.y + 1].open;
	}
	else
	{
		validUpper = true;
	}

	//check lower node
	if (NodeToLookat.y - 1 >= 0)
	{
		validLower = _mazeNodes[NodeToLookat.x][NodeToLookat.y - 1].open;
	}
	else
	{
		validLower = true;
	}


	//check right node
	if (NodeToLookat.x + 1 < _mazeCols)
	{
		validRight = _mazeNodes[NodeToLookat.x + 1][NodeToLookat.y].open;
	}
	else
	{
		validRight = true;
	}

	//check left node
	if (NodeToLookat.x - 1 >= 0)
	{
		validLeft = _mazeNodes[NodeToLookat.x - 1][NodeToLookat.y].open;
	}
	else
	{
		validLeft = true;
	}

	switch (dir)
	{
	case UP:
		validRight = true;
		break;
	case DOWN:
		validLeft = true;
		break;
	case LEFT:
		validLower = true;
		break;
	case RIGHT:
		validUpper = true;
		break;
	}

	return validLower && validUpper && validRight && validLeft;
}

/***********************************************************
	Close maze nodes.
	***********************************************************/
void MazeAlgorithm::markClosedNode(Node *closeNode)
{
	(*closeNode).open = false;
	(*closeNode).weight = 0;
}

/***********************************************************
	Print maze.
	***********************************************************/
void MazeAlgorithm::printMaze(void)
{
	Wall* wallyWorld;
	int i, j;

	for (i = 0; i < _mazeRows; ++i)
	{
		for (j = 0; j < _mazeRows; ++j)
		{
			if (_mazeNodes[i][j].weight)
			{
				cout << '\xDB';
				wallyWorld = new Wall();
				wallyWorld->placeCube((float)i, 0, (float)j);
				_walls.push_back(*wallyWorld);

				delete wallyWorld;
			}
			else if (i == 0 && j == 0)
			{
				cout << 'S';
			}
			else if (i == _goalNode.x && j == _goalNode.y)
			{
				createGoalBox();
				cout << 'G';
			}
			else if (_mazeNodes[i][j].inPathToGoal)
			{
				cout << "*";
			}
			else
			{
				cout << " ";
			}
		}
		cout << endl;
	}

	addOutsideWalls();
	storeCollisionData();
	storeWallPoints();
}

/***********************************************************
Solve the maze with A*
***********************************************************/
void MazeAlgorithm::solveMaze(int x, int y)
{
	resetSolveMaze();

	if (_goalNode.x == x && _goalNode.y == y) //we're already at the goal
	{
		return;
	}

	//make sure we don't calculate from a position outside the maze nodes knowledge
	if (x < 0) 
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}

	_mazeNodes[_goalNode.x][_goalNode.y].isGoal = true;
	_trail.clear();
	_closedList.clear();
	_openList.clear();

	//Add starting node to closed list
	_mazeNodes[x][y].f = calculateF(_mazeNodes[x][y]);
	_mazeNodes[x][y].inList = true;
	_closedList.push_back(_mazeNodes[x][y]);
	addNodesToOpenList(_mazeNodes[x][y]);

	//Loop while openList is not empty or goal not found
	while (!_openList.empty())
	{
		Node temp;

		//Find the node in the list with the lowest cost (F)
		_openList.sort([](const Node &a, const Node &b){return a.f < b.f; });
		temp = _openList.front();
		_openList.pop_front();
		_closedList.push_back(_mazeNodes[temp.x][temp.y]);

		if (_mazeNodes[temp.x][temp.y].isGoal)
		{
			_goalFound = true;
			traceBackToGoal(&_mazeNodes[temp.x][temp.y]);
			break;
		}
		else
		{
			addNodesToOpenList(_mazeNodes[temp.x][temp.y]);
		}
	}
}

/***********************************************************
Reset variables so we can solve the maze from another starting point
***********************************************************/
void MazeAlgorithm::resetSolveMaze(void)
{
	_openList.clear();
	_closedList.clear();

	for (int i = 0; i < _mazeRows; ++i)
	{
		for (int j = 0; j < _mazeRows; ++j)
		{
			_mazeNodes[i][j].inList = false;
			_mazeNodes[i][j].inPathToGoal = false;
			_mazeNodes[i][j].parent = NULL;
		}
	}
}

/***********************************************************
Add nodes to open list
***********************************************************/
void MazeAlgorithm::addNodesToOpenList(Node n)
{
	//Right
	if (n.x + 1 < _mazeCols)
	{
		if (!_mazeNodes[n.x + 1][n.y].weight && !_mazeNodes[n.x + 1][n.y].inList)
		{
			_mazeNodes[n.x + 1][n.y].f = calculateF(_mazeNodes[n.x + 1][n.y]);
			_mazeNodes[n.x + 1][n.y].inList = true;
			_mazeNodes[n.x + 1][n.y].parent = &_mazeNodes[n.x][n.y];
			_openList.push_back(_mazeNodes[n.x + 1][n.y]);
		}
	}

	//Left
	if (n.x - 1 >= 0)
	{
		if (!_mazeNodes[n.x - 1][n.y].weight && !_mazeNodes[n.x - 1][n.y].inList)
		{
			_mazeNodes[n.x - 1][n.y].f = calculateF(_mazeNodes[n.x - 1][n.y]);
			_mazeNodes[n.x - 1][n.y].inList = true;
			_mazeNodes[n.x - 1][n.y].parent = &_mazeNodes[n.x][n.y];
			_openList.push_back(_mazeNodes[n.x - 1][n.y]);
		}
	}

	//Down
	if (n.y + 1 < _mazeRows)
	{
		if (!_mazeNodes[n.x][n.y + 1].weight && !_mazeNodes[n.x][n.y + 1].inList)
		{
			_mazeNodes[n.x][n.y + 1].f = calculateF(_mazeNodes[n.x][n.y + 1]);
			_mazeNodes[n.x][n.y + 1].inList = true;
			_mazeNodes[n.x][n.y + 1].parent = &_mazeNodes[n.x][n.y];
			_openList.push_back(_mazeNodes[n.x][n.y + 1]);
		}
	}

	//Up
	if (n.y - 1 >= 0)
	{
		if (!_mazeNodes[n.x][n.y - 1].weight && !_mazeNodes[n.x][n.y - 1].inList)
		{
			_mazeNodes[n.x][n.y - 1].f = calculateF(_mazeNodes[n.x][n.y - 1]);
			_mazeNodes[n.x][n.y - 1].inList = true;
			_mazeNodes[n.x][n.y - 1].parent = &_mazeNodes[n.x][n.y];
			_openList.push_back(_mazeNodes[n.x][n.y - 1]);
		}
	}
}

/***********************************************************
Calculate distance
***********************************************************/
int MazeAlgorithm::calculateF(Node n)
{
	return abs(n.x - _goalNode.x) + abs(n.y - _goalNode.y);
}

/***********************************************************
Trace the solution trail
***********************************************************/
void MazeAlgorithm::traceBackToGoal(Node *n)
{
	(*n).inPathToGoal = true;
	if ((*n).parent)
	{
		_trail.insert(_trail.begin(), (*n).y);
		_trail.insert(_trail.begin(), 0);
		_trail.insert(_trail.begin(), (*n).x);

		traceBackToGoal((*n).parent);
	}
}

/***********************************************************
Add outside boundary to wall list
***********************************************************/
void MazeAlgorithm::addOutsideWalls()
{
	int x,y;
	Wall* wallyWorld;

	//start space
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 0, 1);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 1, 1);
	_walls.push_back(*wallyWorld);
	
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 0, 0);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 1, 0);
	_walls.push_back(*wallyWorld);

	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 0, -1);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 1, -1);
	_walls.push_back(*wallyWorld);

	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 0, -2);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(-2, 1, -2);
	_walls.push_back(*wallyWorld);

	wallyWorld = new Wall();
	wallyWorld->placeCube(-1, 0, -2);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(-1, 1, -2);
	_walls.push_back(*wallyWorld);

	wallyWorld = new Wall();
	wallyWorld->placeCube(0, 0, -2);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(0, 1, -2);
	_walls.push_back(*wallyWorld);

	wallyWorld = new Wall();
	wallyWorld->placeCube(1, 0, -1);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(1, 1, -1);
	_walls.push_back(*wallyWorld);

	//left side hole
	wallyWorld = new Wall();
	wallyWorld->placeCube(_mazeCols, 0, 0);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(_mazeCols, 1, 0);
	_walls.push_back(*wallyWorld);

	//right side hole
	wallyWorld = new Wall();
	wallyWorld->placeCube(0, 0, _mazeRows);
	_walls.push_back(*wallyWorld);
	wallyWorld = new Wall();
	wallyWorld->placeCube(0, 1, _mazeRows);
	_walls.push_back(*wallyWorld);

	//bottom and top sides
	for (y = 1; y < _mazeRows+1; y++)
	{
		//side 1
		wallyWorld = new Wall();
		wallyWorld->placeCube(-1, 0, y);
		_walls.push_back(*wallyWorld);
		wallyWorld = new Wall();
		wallyWorld->placeCube(-1, 1, y);
		_walls.push_back(*wallyWorld);
		
		//side 2
		wallyWorld = new Wall();
		wallyWorld->placeCube(_mazeCols, 0, y);
		_walls.push_back(*wallyWorld);
		wallyWorld = new Wall();
		wallyWorld->placeCube(_mazeCols, 1, y);
		_walls.push_back(*wallyWorld);
	}

	//left and right sides
	for (x = 1; x < _mazeCols + 1; x++)
	{
		//side 1
		wallyWorld = new Wall();
		wallyWorld->placeCube(x, 0, -1);
		_walls.push_back(*wallyWorld);
		wallyWorld = new Wall();
		wallyWorld->placeCube(x, 1, -1);
		_walls.push_back(*wallyWorld);

		//side 2
		wallyWorld = new Wall();
		wallyWorld->placeCube(x, 0, _mazeRows);
		_walls.push_back(*wallyWorld);
		wallyWorld = new Wall();
		wallyWorld->placeCube(x, 1, _mazeRows);
		_walls.push_back(*wallyWorld);
	}
}


/***********************************************************
Store collision data in quadtree
***********************************************************/
void MazeAlgorithm::storeCollisionData()
{
	AABB tempBox;
	Point p;
	Data<AABB> data;

	for (_wallIt = _walls.begin(); _wallIt != _walls.end(); ++_wallIt)
	{
		//store collision data
		tempBox = _wallIt->getCollisionBox();
		p = { tempBox.center.x, tempBox.center.y };

		data = { p, _wallIt->getCollisionBox() };
		_quadTree->insert(data);
	}
}

void MazeAlgorithm::storeWallPoints(void)
{
	vector<float> temp;

	if (_numWalls == 0) //only need to get wall coordinates once
	{
		for (_wallIt = _walls.begin(); _wallIt != _walls.end(); ++_wallIt)
		{
			temp = _wallIt->getWallVertices();
			_points.insert(_points.end(), temp.begin(), temp.end());

			_numWalls++;
		}
	}
}

/***********************************************************
	Render all walls in the maze.
***********************************************************/
void MazeAlgorithm::drawMaze(void)
{


	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(float), &_points.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_WALL * _numWalls);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/***********************************************************
Draw edge lines to walls
***********************************************************/
void MazeAlgorithm::drawMazeWireFrame(void)
{
	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(float), &_points.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

	for (int i = 0; i < _numWalls; i++)
	{
		glDrawArrays(GL_LINE_LOOP, i * NUM_VERTICES_WALL, NUM_VERTICES_WALL);
	}
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/***********************************************************
Draw trail from player location to goal
***********************************************************/
void MazeAlgorithm::drawTrail(void)
{
	if (_trail.size() == 0)
	{
		return;
	}

	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _trail.size() * sizeof(float), &_trail.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_LINE_STRIP, 0, _trail.size() / NUM_3D_VERTEX);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/***********************************************************
Draw goal space to screen
***********************************************************/
void MazeAlgorithm::drawGoal(void)
{
	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _goalBox.size() * sizeof(float), &_goalBox.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_WALL);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/***********************************************************
Create goal space
***********************************************************/
void MazeAlgorithm::createGoalBox(void)
{
	float farX, farY, farZ, nearX, nearY, nearZ;

	farX = _goalNode.x + WALL_EDGE_LENGTH / 2;
	farY = GOAL_CENTER_Y_COORD + GOAL_HEIGHT_EDGE_LENGTH / 2;
	farZ = _goalNode.y + WALL_EDGE_LENGTH / 2;

	nearX = _goalNode.x - WALL_EDGE_LENGTH / 2;
	nearY = GOAL_CENTER_Y_COORD - GOAL_HEIGHT_EDGE_LENGTH / 2;
	nearZ = _goalNode.y - WALL_EDGE_LENGTH / 2;

	//triangle 1 - back face
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);

	//triangle 2 - back face
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);

	//triangle 3 - front face
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);

	//triangle 4 - front face
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);

	//triangle 5 - right face
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);

	//triangle 6 - right face
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);

	//triangle 7 - left side
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);

	//triangle 8 - left side
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);

	//triangle 9 - top side
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);

	//triangle 10 - top side
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(farY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(farY);
	_goalBox.push_back(farZ);

	//triangle 11 - bottom side
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);

	//triangle 12 - bottom side
	_goalBox.push_back(farX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(farZ);
	_goalBox.push_back(nearX);
	_goalBox.push_back(nearY);
	_goalBox.push_back(nearZ);

}