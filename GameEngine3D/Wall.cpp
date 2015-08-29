#include "Wall.h"

namespace GameEngine3D
{

	Wall::Wall()
	{
		_vbo = 0;
		_vao = 0;
	}


	Wall::~Wall()
	{

	}

	void Wall::init()
	{

	}

	void Wall::placeCube(float x, float y, float z)
	{
		const float EDGE_LENGTH = 1.0f;
		
		float farX = x + EDGE_LENGTH / 2;
		float farY = y + EDGE_LENGTH / 2;
		float farZ = z + EDGE_LENGTH / 2;

		float nearX = x - EDGE_LENGTH / 2;
		float nearY = y - EDGE_LENGTH / 2;
		float nearZ = z - EDGE_LENGTH / 2;

		//set collision corners
		_collisionCorners[0] = nearX;
		_collisionCorners[1] = nearZ;
		_collisionCorners[2] = farX;
		_collisionCorners[3] = farZ;

		//set collision boundary box
		_collisionBox = GameEngine3D::AABB(GameEngine3D::Point(x, z), GameEngine3D::Point(EDGE_LENGTH / 2, EDGE_LENGTH / 2));
		
		//triangle 1 - back face
		_points[0] = farX;
		_points[1] = nearY;
		_points[2] = nearZ;
		_points[3] = farX;
		_points[4] = farY;
		_points[5] = nearZ;
		_points[6] = nearX;
		_points[7] = farY;
		_points[8] = nearZ;

		//triangle 2 - back face
		_points[9] = nearX;
		_points[10] = nearY;
		_points[11] = nearZ;
		_points[12] = farX;
		_points[13] = nearY;
		_points[14] = nearZ;
		_points[15] = nearX;
		_points[16] = farY;
		_points[17] = nearZ;

		//triangle 3 - front face
		_points[18] = farX;
		_points[19] = farY ;
		_points[20] = farZ;
		_points[21] = farX;
		_points[22] = nearY;
		_points[23] = farZ;
		_points[24] = nearX;
		_points[25] = farY;
		_points[26] = farZ;

		//triangle 4 - front face
		_points[27] = farX;
		_points[28] = nearY;
		_points[29] = farZ;
		_points[30] = nearX;
		_points[31] = nearY;
		_points[32] = farZ;
		_points[33] = nearX;
		_points[34] = farY;
		_points[35] = farZ;

		//triangle 5 - right face
		_points[36] = farX;
		_points[37] = nearY;
		_points[38] = farZ;
		_points[39] = farX;
		_points[40] = farY;
		_points[41] = farZ;
		_points[42] = farX;
		_points[43] = farY;
		_points[44] = nearZ;

		//triangle 6 - right face
		_points[45] = farX;
		_points[46] = nearY;
		_points[47] = farZ;
		_points[48] = farX;
		_points[49] = farY;
		_points[50] = nearZ;
		_points[51] = farX;
		_points[52] = nearY;
		_points[53] = nearZ;
		
		//triangle 7 - left side
		_points[54] = nearX;
		_points[55] = farY;
		_points[56] = farZ;
		_points[57] = nearX;
		_points[58] = nearY;
		_points[59] = farZ;
		_points[60] = nearX;
		_points[61] = farY;
		_points[62] = nearZ;

		//triangle 8 - left side
		_points[63] = nearX;
		_points[64] = farY;
		_points[65] = nearZ;
		_points[66] = nearX;
		_points[67] = nearY;
		_points[68] = farZ;
		_points[69] = nearX;
		_points[70] = nearY;
		_points[71] = nearZ;

		//triangle 9 - top side
		_points[72] = farX;
		_points[73] = farY;
		_points[74] = nearZ;
		_points[75] = farX;
		_points[76] = farY;
		_points[77] = farZ;
		_points[78] = nearX;
		_points[79] = farY;
		_points[80] = farZ;

		//triangle 10 - top side
		_points[81] = nearX;
		_points[82] = farY;
		_points[83] = nearZ;
		_points[84] = farX; 
		_points[85] = farY;
		_points[86] = nearZ;
		_points[87] = nearX;
		_points[88] = farY;
		_points[89] = farZ;

		//triangle 11 - bottom side
		_points[90] = farX;
		_points[91] = nearY;
		_points[92] = farZ; 
		_points[93] = farX;
		_points[94] = nearY;
		_points[95] = nearZ;
		_points[96] = nearX;
		_points[97] = nearY;
		_points[98] = farZ;

		//triangle 12 - bottom side
		_points[99] = farX;
		_points[100] = nearY;
		_points[101] = nearZ;
		_points[102] = nearX;
		_points[103] = nearY;
		_points[104] = nearZ;
		_points[105] = nearX;
		_points[106] = nearY;
		_points[107] = farZ;

		//set all sides purple
		for (int i = 0; i < NUM_VERTICES; i++)
		{
			_colors[4 * i] = 1;
			_colors[4 * i + 1] = 0;
			_colors[4 * i + 2] = 1;
			_colors[4 * i + 3] = 1;
		}
	}

	void Wall::draw()
	{
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(_points) + sizeof(_colors), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_points), _points);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(_points), sizeof(_colors), _colors);

		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(_points)));
	}

	void Wall::render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}