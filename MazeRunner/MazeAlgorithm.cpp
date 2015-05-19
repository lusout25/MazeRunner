#include "MazeAlgorithm.h"

MazeAlgorithm::MazeAlgorithm(int Rows, int Cols)
{
	MazeRows = Rows;
	MazeCols = Cols;

	MazeNodes = new Node*[MazeRows];

	for (int j = 0; j < MazeRows; ++j)
	{
		MazeNodes[j] = new Node[MazeCols];
	}
	srand((unsigned int)time(NULL));
}

MazeAlgorithm::~MazeAlgorithm()
{
	for (int j = 0; j < MazeCols; ++j)
	{
		delete[] MazeNodes[j];
	}

	delete[] MazeNodes;
}

void MazeAlgorithm::generateMazeWeights(void)
{
	for (int i = 0; i < MazeRows; ++i)
	{
		for (int j = 0; j < MazeRows; ++j)
		{
			MazeNodes[i][j].x = i;
			MazeNodes[i][j].y = j;
			MazeNodes[i][j].weight = (rand() % 9) + 1;
			MazeNodes[i][j].open = true;
		}
	}
}

void MazeAlgorithm::generateMaze(void)
{
	//Add starting node to list
	frontierNodes.push_back(MazeNodes[0][0]);
	markClosedNode(&MazeNodes[0][0]);

	while (!frontierNodes.empty())
	{
		findNextNodeToAdd();
	}
}

void MazeAlgorithm::findNextNodeToAdd(void)
{
	Node temp = { -1, -1, -1, false };

	//Loop through Nodes in the frontier
	it = frontierNodes.begin();

	while (it != frontierNodes.end())
	{
		//Check to see if this node has any values to add.
		if (validFrontierNode(*it))
		{
			++it;
		}
		//remove this node
		else
		{
			it = frontierNodes.erase(it);
		}
	}

	//Loop through Nodes in the possible next selection
	for (pon = possibleOpenNodes.begin(); pon != possibleOpenNodes.end(); ++pon)
	{
		if (temp.weight == -1)
		{
			temp = *pon;
		}
		else
		{
			if ((*pon).weight < temp.weight)
			{
				temp = *pon;
			}
		}
	}

	if (temp.weight != -1)
	{
		frontierNodes.push_back(MazeNodes[temp.x][temp.y]);
		markClosedNode(&MazeNodes[temp.x][temp.y]);
		GoalNode = MazeNodes[temp.x][temp.y];
	}

	possibleOpenNodes.erase(possibleOpenNodes.begin(), possibleOpenNodes.end());
}

//check to see if the adjacent nodes are available
bool MazeAlgorithm::validFrontierNode(Node NodeToLookat)
{
	bool validLower = false;
	bool validUpper = false;
	bool validRight = false;
	bool validLeft = false;

	//check upper node
	if (NodeToLookat.y + 1 < MazeRows)
	{
		if (MazeNodes[NodeToLookat.x][NodeToLookat.y + 1].open)
		{
			validUpper = validNextNode(MazeNodes[NodeToLookat.x][NodeToLookat.y + 1], LEFT);
			if (validUpper)
			{
				possibleOpenNodes.push_back(MazeNodes[NodeToLookat.x][NodeToLookat.y + 1]);
			}
		}
	}

	//check lower node
	if (NodeToLookat.y - 1 >= 0)
	{
		if (MazeNodes[NodeToLookat.x][NodeToLookat.y - 1].open)
		{
			validLower = validNextNode(MazeNodes[NodeToLookat.x][NodeToLookat.y - 1], RIGHT);
			if (validLower)
			{
				possibleOpenNodes.push_back(MazeNodes[NodeToLookat.x][NodeToLookat.y - 1]);
			}
		}
	}

	//check right node
	if (NodeToLookat.x + 1 < MazeCols)
	{
		if (MazeNodes[NodeToLookat.x + 1][NodeToLookat.y].open)
		{
			validRight = validNextNode(MazeNodes[NodeToLookat.x + 1][NodeToLookat.y], DOWN);
			if (validRight)
			{
				possibleOpenNodes.push_back(MazeNodes[NodeToLookat.x + 1][NodeToLookat.y]);
			}
		}
	}

	//check left node
	if (NodeToLookat.x - 1 >= 0)
	{
		if (MazeNodes[NodeToLookat.x - 1][NodeToLookat.y].open)
		{
			validRight = validNextNode(MazeNodes[NodeToLookat.x - 1][NodeToLookat.y], UP);
			if (validRight)
			{
				possibleOpenNodes.push_back(MazeNodes[NodeToLookat.x - 1][NodeToLookat.y]);
			}
		}
	}

	return validLower || validUpper || validRight || validLeft;
}

//Make sure the next node doesnt connect to any other open nodes
bool MazeAlgorithm::validNextNode(Node NodeToLookat, DIRECTION dir)
{
	bool validLower = false;
	bool validUpper = false;
	bool validRight = false;
	bool validLeft = false;

	//check upper node
	if (NodeToLookat.y + 1 < MazeRows)
	{
		validUpper = MazeNodes[NodeToLookat.x][NodeToLookat.y + 1].open;
	}
	else
	{
		validUpper = true;
	}

	//check lower node
	if (NodeToLookat.y - 1 >= 0)
	{
		validLower = MazeNodes[NodeToLookat.x][NodeToLookat.y - 1].open;
	}
	else
	{
		validLower = true;
	}


	//check right node
	if (NodeToLookat.x + 1 < MazeCols)
	{
		validRight = MazeNodes[NodeToLookat.x + 1][NodeToLookat.y].open;
	}
	else
	{
		validRight = true;
	}

	//check left node
	if (NodeToLookat.x - 1 >= 0)
	{
		validLeft = MazeNodes[NodeToLookat.x - 1][NodeToLookat.y].open;
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

void MazeAlgorithm::markClosedNode(Node *closeNode)
{
	(*closeNode).open = false;
	(*closeNode).weight = 0;
}

void MazeAlgorithm::printMaze(void)
{
	for (int i = 0; i < MazeRows; ++i)
	{
		for (int j = 0; j < MazeRows; ++j)
		{
			if (MazeNodes[i][j].weight)
			{
				std::cout << '\xDB';
			}
			else if (i == 0 && j == 0)
			{
				std::cout << 'S';
			}
			else if (i == GoalNode.x && j == GoalNode.y)
			{
				std::cout << 'G';
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

void MazeAlgorithm::drawMaze(void)
{
	GameEngine3D::Wall wallyWorld;

	for (int i = 0; i < MazeRows; ++i)
	{
		for (int j = 0; j < MazeRows; ++j)
		{
			if (MazeNodes[i][j].weight)
			{
				wallyWorld = GameEngine3D::Wall();
				wallyWorld.placeCube(i, 0, j);
				wallyWorld.draw();
				wallyWorld.render();
			}
		}
		std::cout << std::endl;
	}
}