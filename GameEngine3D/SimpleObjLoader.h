#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <GL\glew.h>

class SimpleObjLoader
{
public:
	SimpleObjLoader();
	~SimpleObjLoader();

	bool loadObject(char* filePath);
	void render(void);

private:
	std::vector< unsigned int > _vertexIndices, _uvIndices, _normalIndices;
	std::vector< glm::vec3 > _vertices;
	std::vector< glm::vec4 > _colors;
	std::vector< glm::vec2 > _uvs;
	std::vector< glm::vec3 > _normals;
	int _numFaces;

	GLuint _vbo;
	GLuint _vao;
};