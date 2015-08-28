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
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	int NumFaces;
};