/***********************************************************
	Constants for all MazeRunner files.
***********************************************************/

#include <string>

using namespace std;

#ifndef MAZERUNNER_SHARED_H
#define MAZERUNNER_SHARED_H

	const int SCREEN_HEIGHT_PIXELS = 768;
	const int  SCREEN_WIDTH_PIXELS = 1024;
	const string GAME_TITLE = "Maze Runner";

	const float CAMERA_SPEED_STRAFE = .008f;
	const float CAMERA_SPEED_ROTATE = .001f;
	const float PLAYER_JUMP_SPEED = .005f;

	const float PLAYER_EDGE_LENGTH = .25f;
	const float COLLISION_SEARCH_RADIUS = 1.0f;

	const int MAZE_ROWS = 20;
	const int MAZE_COLUMNS = 20;

	const float GOAL_HEIGHT_EDGE_LENGTH = 0.1f;
	const float GOAL_CENTER_Y_COORD = -0.5f;

#endif