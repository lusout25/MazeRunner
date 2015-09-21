#include "MazeAlgorithm.h"

MazeAlgorithm::MazeAlgorithm(int Rows, int Cols)
{
	int j;

	_mazeRows = Rows;
	_mazeCols = Cols;

	_numWalls = 0;
	_color = vec4(1, 0, 1, 1);
	_mazeNodes = new Node*[_mazeRows];

	for (j = 0; j < _mazeRows; ++j)
	{
		_mazeNodes[j] = new Node[_mazeCols];
	}
	srand((unsigned int)time(NULL));

	_mapBoundary = new AABB(Point(Cols / 2.0f, Rows / 2.0f), Point(Cols / 2.0f, Rows / 2.0f));
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
	AABB tempBox;
	Point p;
	Data<AABB> data;
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

				//store collision data
				tempBox = wallyWorld->getCollisionBox();
				p = { tempBox.center.x, tempBox.center.y };

				data = { p, wallyWorld->getCollisionBox() };
				_quadTree->insert(data);

				delete wallyWorld;
			}
			else if (i == 0 && j == 0)
			{
				cout << 'S';
			}
			else if (i == _goalNode.x && j == _goalNode.y)
			{
				cout << 'G';
			}
			else
			{
				cout << " ";
			}
		}
		cout << endl;
	}
}

/***********************************************************
	Render all walls in the maze.
***********************************************************/
void MazeAlgorithm::drawMaze(void)
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

	if (_vbo == 0)
	{
		glGenBuffers(1, &_vbo);
	}
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(float), &_points.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_WALL * _numWalls);
}