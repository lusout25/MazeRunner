#include "SimpleObjLoader.h"

namespace GameEngine3D
{

	SimpleObjLoader::SimpleObjLoader()
	{

	}

	SimpleObjLoader::~SimpleObjLoader()
	{

	}

	bool SimpleObjLoader::loadObject(char* filePath)
	{
		vector< vec3 > temp_vertices;
		vector< vec2 > temp_uvs;
		vector< vec3 > temp_normals;
		_numFaces = 0;

		ifstream fp(filePath);

		if (!fp.is_open())
		{
			cout << "COULD NOT OPEN FILE!" << endl;
			return false;
		}

		while (!fp.eof())
		{
			string buf;
			fp >> buf;

			//Load the Vertices from file
			if (buf == "v")
			{
				vec3 tempVector3;
				fp >> tempVector3.x >> tempVector3.y >> tempVector3.z;
				temp_vertices.push_back(tempVector3);
			}
			//Load the Texture Coordinate
			else if (buf == "vt")
			{
				vec2 tempVector2;
				fp >> tempVector2.x >> tempVector2.y;
				temp_uvs.push_back(tempVector2);
			}
			//Load the Normals
			else if (buf == "vn")
			{
				vec3 tempVector3;
				fp >> tempVector3.x >> tempVector3.y >> tempVector3.z;
				temp_normals.push_back(tempVector3);
			}
			//Load the Faces. NOTE: EDIT THE .OBJ FILE TO REPLACE "/" WITH " "
			// BECAUSE I'M TOO LAZY TO SPLIT THE STRING
			// Another Note: F FOR THIS MODEL ONLY HAS VERTEX AND NORMAL WILL NEED TO MODIFY PARSER 
			// IF TEXTURE COORDS OR 'UV' IS WANTED
			else if (buf == "f")
			{
				unsigned int tempVertIndex[3], tempUvIndex[3], normalIndex[3];
				fp >> tempVertIndex[0] >> tempUvIndex[0] >> normalIndex[0] >>
					tempVertIndex[1] >> tempUvIndex[1] >> normalIndex[1] >>
					tempVertIndex[2] >> tempUvIndex[2] >> normalIndex[2];

				_vertexIndices.push_back(tempVertIndex[0]);
				_vertexIndices.push_back(tempVertIndex[1]);
				_vertexIndices.push_back(tempVertIndex[2]);
				_uvIndices.push_back(tempUvIndex[0]);
				_uvIndices.push_back(tempUvIndex[1]);
				_uvIndices.push_back(tempUvIndex[2]);
				_normalIndices.push_back(normalIndex[0]);
				_normalIndices.push_back(normalIndex[1]);
				_normalIndices.push_back(normalIndex[2]);

				++_numFaces;
			}
		}

		fp.close();

		//INDEX THE VERTICES AND STORE THEM IN ORDER FOR OPENGL
		for (uint i = 0; i < _vertexIndices.size(); i++)
		{
			_vertices.push_back(temp_vertices[_vertexIndices[i] - 1]);
			_colors.push_back(vec4(0, 1, 1, 1));
		}

		for (uint i = 0; i < _uvIndices.size(); i++)
		{
			_uvs.push_back(temp_uvs[_uvIndices[i] - 1]);
		}

		for (uint i = 0; i < _normalIndices.size(); i++)
		{
			_normals.push_back(temp_normals[_normalIndices[i] - 1]);
		}

		return true;

	}

	void SimpleObjLoader::render()
	{
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vec3) + _colors.size() * sizeof(vec4) + _normals.size() * sizeof(vec3), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(vec3), &_vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vec3), _colors.size() * sizeof(vec4), &_colors[0]);
		glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vec3) + _colors.size() * sizeof(vec4), _normals.size() * sizeof(vec4), &_normals[0]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(_vertices.size() * sizeof(vec3)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(_vertices.size() * sizeof(vec3) + _colors.size() * sizeof(vec4)));

		glDrawArrays(GL_TRIANGLES, 0, NUM_3D_VERTEX * _numFaces);
	}

}