#include "Triangle.h"

namespace GameEngine3D
{

	Triangle::Triangle()
	{
		_vao = 0;
		_vbo = 0;
	}


	Triangle::~Triangle()
	{
	}

	void Triangle::init()
	{
		_points[0] = -0.5f;
		_points[1] = -0.5f;
		_points[2] = 0.0f;
		_points[3] = 0.5f;
		_points[4] = -0.5f;
		_points[5] = 0.0f;
		_points[6] = 0.0f;
		_points[7] = 0.5f;
		_points[8] = 0.0f;
		_points[9] = 0.0f;
		_points[10] = -0.5f;
		_points[11] = 0.5f;
		_points[12] = -0.5f;
		_points[13] = -0.5f;
		_points[14] = 0.0f;
		_points[15] = 0.0f;
		_points[16] = 0.5f;
		_points[17] = 0.0f;
		_points[18] = 0.0f;
		_points[19] = 0.5f;
		_points[20] = 0.0f;
		_points[21] = 0.5f;
		_points[22] = -0.5f;
		_points[23] = 0.0f;
		_points[24] = 0.0f;
		_points[25] = -0.5f;
		_points[26] = 0.5f;

		_colors[0] = 1.0f;
		_colors[1] = 0.0f;
		_colors[2] = 0.0f;
		_colors[3] = 1.0f;
		_colors[4] = 1.0f;
		_colors[5] = 0.0f;
		_colors[6] = 0.0f;
		_colors[7] = 1.0f;
		_colors[8] = 1.0f;
		_colors[9] = 0.0f;
		_colors[10] = 0.0f;
		_colors[11] = 1.0f;
		_colors[12] = 0.0f;
		_colors[13] = 1.0f;
		_colors[14] = 0.0f;
		_colors[15] = 1.0f;
		_colors[16] = 0.0f;
		_colors[17] = 1.0f;
		_colors[18] = 0.0f;
		_colors[19] = 1.0f;
		_colors[20] = 0.0f;
		_colors[21] = 1.0f;
		_colors[22] = 0.0f;
		_colors[23] = 1.0f;
		_colors[24] = 0.0f;
		_colors[25] = 0.0f;
		_colors[26] = 1.0f;
		_colors[27] = 1.0f;
		_colors[28] = 0.0f;
		_colors[29] = 0.0f;
		_colors[30] = 1.0f;
		_colors[31] = 1.0f;
		_colors[32] = 0.0f;
		_colors[33] = 0.0f;
		_colors[34] = 1.0f;
		_colors[35] = 1.0f;

	}

	void Triangle::draw()
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

	void Triangle::render()
	{
		glDrawArrays(GL_TRIANGLES, 0, 9);
	}

}