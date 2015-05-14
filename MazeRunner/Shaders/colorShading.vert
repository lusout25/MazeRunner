#version 400

in vec4 vertexPosition;
in vec4 vertexColor;

out vec4 fragmentColor;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vertexPosition;
	fragmentColor = vertexColor;
}