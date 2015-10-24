#include "Sprite.h"


namespace GameEngine3D {

	Sprite::Sprite()
	{
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	/***********************************************************
	Initialize variables needed for the sprite
	***********************************************************/
	void Sprite::init(vec3 bottomleft, vec3 topLeft, vec3 topRight, vec3 bottomRight, std::string texturePath, float xMultiple, float yMultiple)
	{

		_texture = ImageLoader::loadPNG(texturePath);

		_points = { bottomleft.x, bottomleft.y, bottomleft.z, topLeft.x, topLeft.y, topLeft.z, topRight.x, topRight.y, topRight.z, topRight.x, topRight.y, topRight.z, bottomRight.x, bottomRight.y, bottomRight.z, bottomleft.x, bottomleft.y, bottomleft.z };
		_uv = { 0.0f, 0.0f, 0.0f, yMultiple, xMultiple, yMultiple, xMultiple, yMultiple, xMultiple, 0.0f, 0.0f, 0.0f };

	}

	/***********************************************************
	Draw sprite to screen
	***********************************************************/
	void Sprite::draw()
	{
		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		glBindTexture(GL_TEXTURE_2D, _texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(float) + _uv.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _points.size() * sizeof(float), &_points.front());
		glBufferSubData(GL_ARRAY_BUFFER, _points.size() * sizeof(float), _uv.size() * sizeof(float), &_uv.front());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//position attribute pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		//uv attribute pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(_points.size () * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}