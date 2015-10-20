#include "Wall.h"

namespace GameEngine3D
{

	Wall::Wall() : _vbo(0)
	{
		_pointData.reserve(NUM_VERTICES_WALL * NUM_3D_VERTEX);
		_colorData.reserve(NUM_VERTICES_WALL * NUM_RGBA_COLOR);
	}


	Wall::~Wall()
	{

	}

	void Wall::init()
	{
	}

	/***********************************************************
		Place wall centered at x y z
	***********************************************************/
	void Wall::placeCube(float x, float y, float z)
	{
		float farX, farY, farZ, nearX, nearY, nearZ;

		farX = x + WALL_EDGE_LENGTH / 2;
		farY = y + WALL_EDGE_LENGTH / 2;
		farZ = z + WALL_EDGE_LENGTH / 2;

		nearX = x - WALL_EDGE_LENGTH / 2;
		nearY = y - WALL_EDGE_LENGTH / 2;
		nearZ = z - WALL_EDGE_LENGTH / 2;

		//set collision boundary box
		_collisionBox = AABB(Point(x, z), Point(WALL_EDGE_LENGTH / 2, WALL_EDGE_LENGTH / 2));
		
		//triangle 1 - back face
		_normals[0] = 0;
		_normals[1] = 0;
		_normals[2] = 1;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);

		//triangle 2 - back face
		_normals[3] = 0;
		_normals[4] = 0;
		_normals[5] = 1;
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);

		//triangle 3 - front face
		_normals[6] = 0;
		_normals[7] = 0;
		_normals[8] = -1;
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);

		//triangle 4 - front face
		_normals[9] = 0;
		_normals[10] = 0;
		_normals[11] = -1;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);

		//triangle 5 - right face
		_normals[12] = 1;
		_normals[13] = 0;
		_normals[14] = 0;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);

		//triangle 6 - right face
		_normals[15] = 1;
		_normals[16] = 0;
		_normals[17] = 0;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
		
		//triangle 7 - left side
		_normals[18] = -1;
		_normals[19] = 0;
		_normals[20] = 0;
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);

		//triangle 8 - left side
		_normals[21] = -1;
		_normals[22] = 0;
		_normals[23] = 0;
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);

		//triangle 9 - top side
		_normals[24] = 0;
		_normals[25] = 1;
		_normals[26] = 0;
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);

		//triangle 10 - top side
		_normals[27] = 0;
		_normals[28] = 1;
		_normals[29] = 0;
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(farX);
		_pointData.push_back(farY);
		_pointData.push_back(nearZ);
		_pointData.push_back(nearX);
		_pointData.push_back(farY);
		_pointData.push_back(farZ);

		//triangle 11 - bottom side
		_normals[30] = 0;
		_normals[31] = -1;
		_normals[32] = 0;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);

		//triangle 12 - bottom side
		_normals[33] = 0;
		_normals[34] = -1;
		_normals[35] = 0;
		_pointData.push_back(farX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(farZ);
		_pointData.push_back(nearX);
		_pointData.push_back(nearY);
		_pointData.push_back(nearZ);
	}

	/***********************************************************
		Draw and render Wall to screen
	***********************************************************/
	void Wall::draw()
	{
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _pointData.size() * sizeof(float), &_pointData.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_WALL);
	}

	/***********************************************************
	Draw and render Wall Outline to screen
	***********************************************************/
	void Wall::drawWallOutline()
	{
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _pointData.size() * sizeof(float), &_pointData.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glVertexAttribPointer(0, NUM_3D_VERTEX, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_LINE_LOOP, 0, NUM_VERTICES_WALL);
	}
}